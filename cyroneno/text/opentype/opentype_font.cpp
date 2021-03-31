// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "opentype_font.h"

#include "utils/number.hpp"
#include "utils/stream_utils.h"

#include "tables/loca.h"


namespace {

    // 'OTTO'
    const uint32_t kSfntVerCFFData = 0x4F54544F;

    // TrueType
    const uint32_t kSfntVerTrueType = 0x00010000;

    // Apple Spec TrueType 'true'
    const uint32_t kSfntVerAppleTT1 = 0x74727565;

    // Apple Spec TrueType 'typ1'
    const uint32_t kSfntVerAppleTT2 = 0x74797031;

    // 'ttcf'
    const uint32_t kTTCTag = 0x74746366;

}

namespace cyro {
namespace otf {

    OpenTypeFont::OpenTypeFont()
        : ot_(), head_(), hori_header_(), profile_(), cmap_() {
    }

    bool OpenTypeFont::parse(std::istream& s) {
        uint32_t sfnt_ver;

        // 先看下是 TTC Header 还是 Offset Table
        READ_STREAM_BE(sfnt_ver, 4);
        s.seekg(0, std::ios::beg);

        if (sfnt_ver == kTTCTag) {
            // 解析 TTC
            return parseFontCollection(s);
        }
        if (sfnt_ver == kSfntVerCFFData ||
            sfnt_ver == kSfntVerTrueType ||
            sfnt_ver == kSfntVerAppleTT1 ||
            sfnt_ver == kSfntVerAppleTT2)
        {
            // 解析 Offset Table
            return parseOneFont(s);
        }

        return false;
    }

    bool OpenTypeFont::parseOneFont(std::istream& s) {
        // Offset Table
        READ_STREAM_BE(ot_.sfnt_ver, 4);
        READ_STREAM_BE(ot_.table_num, 2);
        READ_STREAM_BE(ot_.search_range, 2);
        READ_STREAM_BE(ot_.entry_selector, 2);
        READ_STREAM_BE(ot_.range_shift, 2);

        for (uint16_t i = 0; i < ot_.table_num; ++i) {
            TableRecordEntry tre;
            READ_STREAM(tre.tag[0], 4);
            READ_STREAM_BE(tre.check_sum, 4);
            READ_STREAM_BE(tre.offset, 4);
            READ_STREAM_BE(tre.length, 4);

            std::string tag_str(reinterpret_cast<char*>(tre.tag), 4);
            tr_[tag_str] = tre;
        }

        {
            auto it = tr_.find("head");
            if (it != tr_.end()) {
                s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                    return false;
                }
                s.seekg(it->second.offset, std::ios::beg);
                if (!HEAD::parseTable(s, &head_)) {
                    return false;
                }
            } else {
                return false;
            }
        }

        {
            auto it = tr_.find("maxp");
            if (it != tr_.end()) {
                s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                    return false;
                }
                s.seekg(it->second.offset, std::ios::beg);
                if (!MAXP::parseTable(s, &profile_)) {
                    return false;
                }
            } else {
                return false;
            }
        }

        {
            auto it = tr_.find("cvt ");
            if (it != tr_.end()) {
                s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                    return false;
                }
                s.seekg(it->second.offset, std::ios::beg);
                if (!CVT::parseTable(s, it->second.length, &cvt_)) {
                    return false;
                }
            }
        }

        {
            auto it = tr_.find("loca");
            if (it != tr_.end()) {
                s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                    return false;
                }
                s.seekg(it->second.offset, std::ios::beg);
                if (!LOCA::parseTable(s, head_, profile_, &loca_)) {
                    return false;
                }
            } else {
                return false;
            }
        }

        {
            auto it = tr_.find("hhea");
            if (it != tr_.end()) {
                s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                    return false;
                }
                s.seekg(it->second.offset, std::ios::beg);
                if (!HHEA::parseTable(s, &hori_header_)) {
                    return false;
                }
            }
        }

        {
            auto it = tr_.find("hmtx");
            if (it != tr_.end()) {
                s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                    return false;
                }
                s.seekg(it->second.offset, std::ios::beg);
                if (!HMTX::parseTable(s, hori_header_.hm_num, profile_.glyph_num, &hms_)) {
                    return false;
                }
            }
        }

        {
            auto it = tr_.find("cmap");
            if (it != tr_.end()) {
                s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                    return false;
                }
                s.seekg(it->second.offset, std::ios::beg);
                if (!cmap_.parseTable(s)) {
                    return false;
                }
            }
        }

        return true;
    }

    bool OpenTypeFont::parseGlyphIdForChar(std::istream& s, uint16_t c, uint16_t* glyph_idx) {
        if (!cmap_.findGlyphIndex(s, c, glyph_idx)) {
            return false;
        }
        return true;
    }

    bool OpenTypeFont::parseGlyph(std::istream& s, uint32_t index, GlyphInfo* glyph) {
        if (ot_.sfnt_ver != kSfntVerTrueType &&
            ot_.sfnt_ver != kSfntVerAppleTT1 &&
            ot_.sfnt_ver != kSfntVerAppleTT2)
        {
            return false;
        }

        if (!glyphs_.empty()) {
            if (glyphs_.size() > index) {
                *glyph = glyphs_[index];
                return true;
            }
            return false;
        }

        if (loca_.size() <= index + 1) {
            return false;
        }

        {
            auto it = tr_.find("glyf");
            if (it != tr_.end()) {
                /*s.seekg(it->second.offset, std::ios::beg);
                if (calcTableChecksum(0, it->second.length) != it->second.check_sum) {
                    return false;
                }*/
                s.seekg(it->second.offset + loca_[index], std::ios::beg);
                uint32_t prev_offset = it->second.offset + loca_[index];

                GlyphInfo glyph_info;
                glyph_info.glyph_idx = index;
                {
                    auto cur_len = loca_[index + 1] - loca_[index];
                    if (cur_len == 0) {
                        glyph_info.empty = true;
                        *glyph = std::move(glyph_info);
                        return true;
                    }
                    if (!GLYF::parseTable(s, &glyph_info.glyph_header, &glyph_info.glyph_table)) {
                        return false;
                    }
                    int64_t cur = s.tellg();
                    if (cur - prev_offset > cur_len) {
                        return false;
                    }

                    glyph_info.empty = false;
                    *glyph = std::move(glyph_info);
                }
            }
        }
        return true;
    }

    bool OpenTypeFont::parseAllGlyph(std::istream& s) {
        if (ot_.sfnt_ver == kSfntVerTrueType ||
            ot_.sfnt_ver == kSfntVerAppleTT1 ||
            ot_.sfnt_ver == kSfntVerAppleTT2)
        {
            glyphs_.clear();
            {
                auto it = tr_.find("glyf");
                if (it != tr_.end()) {
                    s.seekg(it->second.offset, std::ios::beg);
                    if (calcTableChecksum(s, 0, it->second.length) != it->second.check_sum) {
                        return false;
                    }
                    s.seekg(it->second.offset, std::ios::beg);

                    GlyphInfo glyph_info;
                    uint32_t prev_offset = it->second.offset;
                    for (uint16_t i = 0; i < profile_.glyph_num; ++i) {
                        glyph_info.glyph_idx = i;

                        auto cur_len = loca_[i + 1] - loca_[i];
                        if (cur_len == 0) {
                            glyph_info.empty = true;
                            glyphs_.push_back(std::move(glyph_info));
                            continue;
                        }
                        if (!GLYF::parseTable(s, &glyph_info.glyph_header, &glyph_info.glyph_table)) {
                            return false;
                        }
                        uint32_t cur = utl::num_cast<uint32_t>(std::streamoff(s.tellg()));
                        if (cur - prev_offset < cur_len) {
                            s.seekg(prev_offset + cur_len, std::ios::beg);
                            prev_offset += cur_len;
                        } else if (cur - prev_offset == cur_len) {
                            prev_offset = cur;
                        } else {
                            return false;
                        }

                        glyph_info.empty = false;
                        glyphs_.push_back(std::move(glyph_info));
                    }
                }
            }
        }
        return true;
    }

    bool OpenTypeFont::parseFontCollection(std::istream& s) {
        TTCHeader header;

        READ_STREAM(header.tag[0], 4);
        READ_STREAM_BE(header.major_ver, 2);
        READ_STREAM_BE(header.minor_ver, 2);
        READ_STREAM_BE(header.font_num, 4);

        if (header.font_num > 0) {
            header.offset_tables.reset(new uint32_t[header.font_num], std::default_delete<uint32_t[]>());
            READ_STREAM(header.offset_tables[0], header.font_num * 4);
            for (uint32_t i = 0; i < header.font_num; ++i) {
                auto tmp = header.offset_tables[i];
                header.offset_tables[i] = utl::fromToBE(tmp);
            }
        }

        if (header.major_ver == 2) {
            READ_STREAM_BE(header.dsig_tag, 4);
            READ_STREAM_BE(header.dsig_length, 4);
            READ_STREAM_BE(header.dsig_offset, 4);
        }

        for (uint32_t i = 0; i < header.font_num; ++i) {
            auto offset = header.offset_tables[i];
            s.seekg(offset, std::ios::beg);
            if (!parseOneFont(s)) {
                return false;
            }
            break;
        }

        return true;
    }

    uint32_t OpenTypeFont::calcTableChecksum(std::istream& s, uint32_t table, uint32_t length) {
        uint32_t sum = 0;
        uint32_t end = (length + 3) / 4;
        while (end-- > 0) {
            uint32_t tmp = 0;
            READ_STREAM_BE(tmp, 4);
            sum += tmp;
        }
        return sum;
    }

}
}