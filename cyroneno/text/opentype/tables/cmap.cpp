// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cmap.h"

#include "utils/number.hpp"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool CMAP::parseTable(std::istream& s) {
        auto start_offset = utl::num_cast<uint32_t>(std::streamoff(s.tellg()));

        // 解析 Header
        CMAPHeader& header = header_;

        READ_STREAM_BE(header.version, 2);
        READ_STREAM_BE(header.table_num, 2);

        for (uint16_t i = 0; i < header.table_num; ++i) {
            EncodingRecord record;
            READ_STREAM_BE(record.platform_id, 2);
            READ_STREAM_BE(record.encoding_id, 2);
            READ_STREAM_BE(record.offset, 4);
            header.records.push_back(record);
        }

        /**
         * 挑选合适的 EncodingRecord 进行解析
         * 在 Windows 平台，支持 Unicode BMP（U+0000 - U+FFFF）的字体
         * 必须包含 platform ID 3，encoding ID 1.
         * 在 Windows 平台，支持 Unicode supplementary-plane（U+10000 - U+10FFFF）的字体
         * 必须包含 platform ID 3，encoding ID 10.
         */
        for (uint16_t i = 0; i < header.table_num; ++i) {
            if (header.records[i].platform_id == PID_Windows &&
                header.records[i].encoding_id == WEID_UnicodeFull)
            {
                if (!parseRecord(s, header.records[i], start_offset)) {
                    return false;
                }
                used_er_ = &header.records[i];
                return true;
            }
        }

        return true;
    }

    bool CMAP::findGlyphIndex(std::istream& s, uint32_t c, uint16_t* glyph_idx) {
        *glyph_idx = 0;
        return findInRecord(s, c, glyph_idx);
    }

    bool CMAP::parseRecord(
        std::istream& s, const EncodingRecord& record, uint32_t tab_start)
    {
        // 定位至子表开始位置
        int64_t cur_offset = s.tellg();
        s.seekg(tab_start + record.offset, std::ios::beg);

        // 先看看是什么格式
        format_ = 0;
        READ_STREAM_BE(format_, 2);
        s.seekg(tab_start + record.offset, std::ios::beg);

        switch (format_) {
        case 4:
            if (!parseFormat4(s, &sub_table4_)) {
                return false;
            }
            break;

        case 12:
            if (!parseFormat12(s, &sub_table12_)) {
                return false;
            }
            break;

        default:
            return false;
        }

        s.seekg(cur_offset, std::ios::beg);
        return true;
    }

    bool CMAP::parseFormat4(std::istream& s, Format4* out) {
        Format4& sub_tab = *out;

        READ_STREAM_BE(sub_tab.format, 2);
        READ_STREAM_BE(sub_tab.length, 2);
        READ_STREAM_BE(sub_tab.lang, 2);
        READ_STREAM_BE(sub_tab.seg_count_x2, 2);
        READ_STREAM_BE(sub_tab.search_range, 2);
        READ_STREAM_BE(sub_tab.entry_selector, 2);
        READ_STREAM_BE(sub_tab.range_shift, 2);

        auto seg_count = sub_tab.seg_count_x2 / 2;
        if (seg_count > 0) {
            sub_tab.end_code.reset(new uint16_t[seg_count], std::default_delete<uint16_t[]>());
            READ_STREAM(sub_tab.end_code[0], seg_count * 2);
            for (uint16_t i = 0; i < seg_count; ++i) {
                auto tmp = utl::fromToBE(sub_tab.end_code[i]);
                sub_tab.end_code[i] = tmp;
            }
        }

        READ_STREAM(sub_tab.reserved_pad, 2);

        if (seg_count > 0) {
            sub_tab.start_code.reset(new uint16_t[seg_count], std::default_delete<uint16_t[]>());
            READ_STREAM(sub_tab.start_code[0], seg_count * 2);
            for (uint16_t i = 0; i < seg_count; ++i) {
                auto tmp = utl::fromToBE(sub_tab.start_code[i]);
                sub_tab.start_code[i] = tmp;
            }
        }

        if (seg_count > 0) {
            sub_tab.id_delta.reset(new int16_t[seg_count], std::default_delete<int16_t[]>());
            READ_STREAM(sub_tab.id_delta[0], seg_count * 2);
            for (uint16_t i = 0; i < seg_count; ++i) {
                auto tmp = utl::fromToBE(sub_tab.id_delta[i]);
                sub_tab.id_delta[i] = tmp;
            }
        }

        if (seg_count > 0) {
            sub_tab.iro_off = s.tellg();
            sub_tab.id_range_offset.reset(new uint16_t[seg_count], std::default_delete<uint16_t[]>());
            READ_STREAM(sub_tab.id_range_offset[0], seg_count * 2);
            for (uint16_t i = 0; i < seg_count; ++i) {
                auto tmp = utl::fromToBE(sub_tab.id_range_offset[i]);
                sub_tab.id_range_offset[i] = tmp;
            }
        }

        return true;
    }

    bool CMAP::parseFormat12(std::istream& s, Format12* out) {
        Format12& sub_tab = *out;

        READ_STREAM_BE(sub_tab.format, 2);
        READ_STREAM(sub_tab.reserved, 2);
        READ_STREAM_BE(sub_tab.length, 4);
        READ_STREAM_BE(sub_tab.lang, 4);
        READ_STREAM_BE(sub_tab.group_num, 4);

        for (uint32_t i = 0; i < sub_tab.group_num; ++i) {
            SeqMapGroup group;
            READ_STREAM_BE(group.start_char_code, 4);
            READ_STREAM_BE(group.end_char_code, 4);
            READ_STREAM_BE(group.start_glyph_id, 4);
            sub_tab.groups.push_back(group);
        }
        return true;
    }

    bool CMAP::findInRecord(
        std::istream& s, uint32_t c, uint16_t* glyph_idx) const
    {
        switch (format_) {
        case 4:
            if (c > 0xFFFF) {
                *glyph_idx = 0;
                break;
            }
            if (!findInFormat4(s, c, glyph_idx)) {
                return false;
            }
            break;

        case 12:
            if (!findInFormat12(c, glyph_idx)) {
                return false;
            }
            break;

        default:
            return false;
        }
        return true;
    }

    bool CMAP::findInFormat4(std::istream& s, uint32_t c, uint16_t* glyph_idx) const {
        auto& sub_tab = sub_table4_;
        auto seg_count = sub_tab.seg_count_x2 / 2;

        // 搜索 endCode
        uint16_t i;
        bool hit = false;
        for (i = 0; i < seg_count; ++i) {
            if (sub_tab.end_code[i] == 0xFFFF) {
                break;
            }
            if (sub_tab.end_code[i] >= c && sub_tab.start_code[i] <= c) {
                hit = true;
                break;
            }
        }

        if (!hit) {
            *glyph_idx = 0;
            return true;
        }

        auto cur_iro = sub_tab.id_range_offset[i];
        if (cur_iro == 0) {
            *glyph_idx = c + sub_tab.id_delta[i];
        } else {
            uint16_t glyph_id = 0;
            auto idx = (cur_iro + 2 * (c - sub_tab.start_code[i]) + (sub_tab.iro_off + i * 2));
            SEEKG_STREAM(idx);
            READ_STREAM_BE(glyph_id, 2);
            if (glyph_id == 0) {
                *glyph_idx = 0;
            } else {
                *glyph_idx = glyph_id + sub_tab.id_delta[i];
            }
        }

        return true;
    }

    bool CMAP::findInFormat12(uint32_t c, uint16_t* glyph_idx) const {
        auto& sub_tab = sub_table12_;

        for (uint32_t i = 0; i < sub_tab.group_num; ++i) {
            auto& group = sub_tab.groups[i];
            if (group.start_char_code <= c && group.end_char_code >= c) {
                *glyph_idx = group.start_glyph_id + (c - group.start_char_code);
                return true;
            }
        }
        return false;
    }

}
}
