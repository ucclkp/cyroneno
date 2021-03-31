// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "text_rasterizer.h"

#include <algorithm>
#include <fstream>

#include "opentype/opentype_rasterizer.h"


namespace cyro {

    bool TextRasterizer::initialize() {
        file_.open(L"D:\\Test\\msyh.ttc", std::ios::in | std::ios::binary);
        if (!file_) {
            return false;
        }

        if (!font_.parse(file_)) {
            return false;
        }

        return true;
    }

    bool TextRasterizer::measureText(
        const std::u16string& str, int dpi, int size,
        int* width, int* height, std::vector<otf::OpenTypeFont::GlyphInfo>* glyphs)
    {
        auto& head = font_.getHead();
        auto& maxp = font_.getProfile();
        double scale = (size * dpi) / (72.0 * head.unit_per_em);

        int total_width = 0;
        for (const auto ch : str) {
            uint16_t glyph_id = 0;
            if (!font_.parseGlyphIdForChar(file_, ch, &glyph_id)) {
                return false;
            }

            otf::OpenTypeFont::GlyphInfo glyph;
            if (!font_.parseGlyph(file_, glyph_id, &glyph)) {
                return false;
            }

            otf::HMTX::HorMetric hm;
            font_.getHoriMetric(glyph_id, &hm);

            int cur_width = hm.advance_width;
            total_width += cur_width;

            glyphs->push_back(std::move(glyph));
        }

        *width = int(std::ceil(total_width * scale));
        *height = int(std::ceil((head.max_y - head.min_y + 1) * scale));
        return true;
    }

    bool TextRasterizer::drawText(
        Rasterizer& rz, int width, int height,
        const std::vector<otf::OpenTypeFont::GlyphInfo>& glyphs,
        int dpi, int size, bool outline, int x, int y)
    {
        auto& head = font_.getHead();
        auto& maxp = font_.getProfile();
        double scale = (size * dpi) / (72.0 * head.unit_per_em);

        int cur_x = int(x / scale);
        int cur_y = int(y / scale);

        cur_y -= head.min_y;
        for (const auto& glyph : glyphs) {
            otf::HMTX::HorMetric hm;
            font_.getHoriMetric(glyph.glyph_idx, &hm);

            otf::OpenTypeRasterizer::drawGlyph(
                rz, width, height,
                glyph, cur_x, cur_y,
                scale, Color4D(0, 0, 0, 0.5), !outline);

            otf::OpenTypeRasterizer::drawGlyph2(
                glyph, cur_x, cur_y, scale, [](
                    bool new_contour, bool is_line,
                    const Point2& sp, const Point2& mp, const Point2& ep)
            {
            });

            cur_x += hm.advance_width;
        }

        return true;
    }

}
