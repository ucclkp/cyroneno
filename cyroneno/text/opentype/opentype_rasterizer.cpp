// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "opentype_rasterizer.h"

#include <cassert>

#include "cyroneno/vector.hpp"


namespace cyro {
namespace otf {

    // static
    void OpenTypeRasterizer::drawGlyph(
        Rasterizer& rz, int width, int height,
        const OpenTypeFont::GlyphInfo& glyph,
        int off_x, int off_y, double scale, const Color4D& c, bool use_glyph_rz)
    {
        if (glyph.empty) {
            return;
        }

        auto contour_num = glyph.glyph_header.contour_num;
        if (contour_num <= 0) {
            return;
        }

        GlyphRz glyph_rz;

        int cur_x = off_x;
        int cur_y = off_y;
        uint16_t j = 0;
        for (int16_t i = 0; i < contour_num; ++i) {
            if (use_glyph_rz) {
                glyph_rz.newContour();
            }

            // 绘制每个轮廓
            Point2 cur_start;
            Point2 bezier_start, bezier_mid;
            bool prev_has_beizer = false;
            uint16_t cur_end_pt = glyph.glyph_table.contour_end_pts[i];
            for (bool first = true; j <= cur_end_pt; ++j) {
                Point2 start{ cur_x * scale, cur_y * scale };

                cur_x = glyph.glyph_table.x_coord[j] + off_x;
                cur_y = glyph.glyph_table.y_coord[j] + off_y;
                Point2 end{ cur_x * scale, cur_y * scale };

                if (!first) {
                    if (glyph.glyph_table.flags[j] & GLYF::ON_CURVE_POINT) {
                        if (prev_has_beizer) {
                            if (use_glyph_rz) {
                                glyph_rz.addQBezier(bezier_start, bezier_mid, end);
                            } else {
                                rz.drawQuadBezier(bezier_start, bezier_mid, end, c);
                            }
                        } else {
                            if (use_glyph_rz) {
                                glyph_rz.addLine(start, end);
                            } else {
                                rz.drawLineSeg(start, end, c);
                            }
                        }

                        if (j == cur_end_pt) {
                            if (use_glyph_rz) {
                                glyph_rz.addLine(end, cur_start);
                            } else {
                                rz.drawLineSeg(end, cur_start, c);
                            }
                        }
                        prev_has_beizer = false;
                    } else {
                        if (prev_has_beizer) {
                            auto tmp = bezier_mid + ((end - bezier_mid) / 2.0);
                            if (use_glyph_rz) {
                                glyph_rz.addQBezier(bezier_start, bezier_mid, tmp);
                            } else {
                                rz.drawQuadBezier(bezier_start, bezier_mid, tmp, c);
                            }
                            bezier_start = tmp;
                        } else {
                            prev_has_beizer = true;
                            bezier_start = start;
                        }
                        bezier_mid = end;

                        if (j == cur_end_pt) {
                            if (use_glyph_rz) {
                                glyph_rz.addQBezier(bezier_start, bezier_mid, cur_start);
                            } else {
                                rz.drawQuadBezier(bezier_start, bezier_mid, cur_start, c);
                            }
                        }
                    }
                } else {
                    assert(glyph.glyph_table.flags[j] & GLYF::ON_CURVE_POINT);
                    cur_start = end;
                    first = false;
                }
            }
        }

        Point2 p{ 0, 14.8046875 };
        if (use_glyph_rz) {
            glyph_rz.draw(width, height, rz);
            //glyph_rz.hitTest(p.x, p.y);
        } else {
            // test
            //rz.drawLine(p, { p.x + 20, p.y }, 1, 0, c);
        }
    }

    // static
    void OpenTypeRasterizer::drawGlyph2(
        const OpenTypeFont::GlyphInfo& glyph,
        int off_x, int off_y, double scale, Func func)
    {
        if (glyph.empty) {
            return;
        }

        auto contour_num = glyph.glyph_header.contour_num;
        if (contour_num <= 0) {
            return;
        }

        GlyphRz glyph_rz;

        int cur_x = off_x;
        int cur_y = off_y;
        uint16_t j = 0;
        for (int16_t i = 0; i < contour_num; ++i) {
            func(true, false, {}, {}, {});

            // 绘制每个轮廓
            Point2 cur_start;
            Point2 bezier_start, bezier_mid;
            bool prev_has_beizer = false;
            uint16_t cur_end_pt = glyph.glyph_table.contour_end_pts[i];
            for (bool first = true; j <= cur_end_pt; ++j) {
                Point2 start{ cur_x * scale, cur_y * scale };

                cur_x += glyph.glyph_table.x_coord[j];
                cur_y += glyph.glyph_table.y_coord[j];
                Point2 end{ cur_x * scale, cur_y * scale };

                if (!first) {
                    if (glyph.glyph_table.flags[j] & GLYF::ON_CURVE_POINT) {
                        if (prev_has_beizer) {
                            func(false, false, bezier_start, bezier_mid, end);
                        } else {
                            func(false, true, start, {}, end);
                        }

                        if (j == cur_end_pt) {
                            func(false, true, end, {}, cur_start);
                        }
                        prev_has_beizer = false;
                    } else {
                        if (prev_has_beizer) {
                            auto tmp = bezier_mid + ((end - bezier_mid) / 2.0);
                            func(false, false, bezier_start, bezier_mid, tmp);
                            bezier_start = tmp;
                        } else {
                            prev_has_beizer = true;
                            bezier_start = start;
                        }
                        bezier_mid = end;

                        if (j == cur_end_pt) {
                            func(false, false, bezier_start, bezier_mid, cur_start);
                        }
                    }
                } else {
                    assert(glyph.glyph_table.flags[j] & GLYF::ON_CURVE_POINT);
                    cur_start = end;
                    first = false;
                }
            }
        }
    }

}
}
