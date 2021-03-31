// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEXT_OPENTYPE_OPENTYPE_FONT_H_
#define CYRONENO_TEXT_OPENTYPE_OPENTYPE_FONT_H_

#include <fstream>
#include <map>

#include "tables/cmap.h"
#include "tables/cvt.h"
#include "tables/glyf.h"
#include "tables/head.h"
#include "tables/maxp.h"
#include "tables/hhea.h"
#include "tables/hmtx.h"


namespace cyro {
namespace otf {

    /**
     * 根据 OpenType 文档写成的 OpenType 字体解析器
     * https://docs.microsoft.com/zh-cn/typography/opentype/spec/
     */
    class OpenTypeFont {
    public:
        struct OffsetTable {
            uint32_t sfnt_ver;
            uint16_t table_num;
            uint16_t search_range;
            uint16_t entry_selector;
            uint16_t range_shift;
        };

        struct TableRecordEntry {
            uint8_t tag[4];
            uint32_t check_sum;
            uint32_t offset;
            uint32_t length;
        };

        struct TTCHeader {
            // Version 1.0
            uint8_t tag[4];
            uint16_t major_ver;
            uint16_t minor_ver;
            uint32_t font_num;
            std::shared_ptr<uint32_t[]> offset_tables;

            // Version 2.0
            uint32_t dsig_tag;
            uint32_t dsig_length;
            uint32_t dsig_offset;
        };

        struct GlyphInfo {
            bool empty = true;
            uint16_t glyph_idx;
            GLYF::GlyphHeader glyph_header;
            GLYF::SimpleGlyphTable glyph_table;
        };

        OpenTypeFont();

        bool parse(std::istream& s);
        bool parseGlyph(std::istream& s, uint32_t index, GlyphInfo* glyph);
        bool parseAllGlyph(std::istream& s);
        bool parseGlyphIdForChar(std::istream& s, uint16_t c, uint16_t* glyph_idx);

        const HEAD::HeadTable& getHead() const { return head_; }
        const MAXP::MaximumProfile& getProfile() const { return profile_; }
        const std::vector<GlyphInfo>& getGlyphs() const { return glyphs_; }
        void getHoriMetric(uint16_t glyph_idx, HMTX::HorMetric* hm) const { *hm = hms_[glyph_idx]; }

    private:
        bool parseOneFont(std::istream& s);
        bool parseFontCollection(std::istream& s);

        uint32_t calcTableChecksum(std::istream& s, uint32_t table, uint32_t length);

        OffsetTable ot_;
        std::map<std::string, TableRecordEntry> tr_;
        HEAD::HeadTable head_;
        HHEA::HoriHeaderTable hori_header_;
        MAXP::MaximumProfile profile_;
        CVT::CVTTable cvt_;
        CMAP cmap_;
        std::vector<uint32_t> loca_;
        std::vector<GlyphInfo> glyphs_;
        std::vector<HMTX::HorMetric> hms_;
    };

}
}

#endif  // CYRONENO_TEXT_OPENTYPE_OPENTYPE_FONT_H_