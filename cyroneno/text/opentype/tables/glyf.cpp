// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "glyf.h"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool GLYF::parseTable(std::istream& s, GlyphHeader* out_header, SimpleGlyphTable* out_glyph) {
        GlyphHeader header;

        READ_STREAM_BE(header.contour_num, 2);
        READ_STREAM_BE(header.min_x, 2);
        READ_STREAM_BE(header.min_y, 2);
        READ_STREAM_BE(header.max_x, 2);
        READ_STREAM_BE(header.max_y, 2);

        SimpleGlyphTable table;
        if (header.contour_num >= 0) {
            // Simple glyph
            if (header.contour_num > 0) {
                table.contour_end_pts.reset(new uint16_t[header.contour_num], std::default_delete<uint16_t[]>());
                READ_STREAM(table.contour_end_pts[0], header.contour_num * 2);
                for (int16_t i = 0; i < header.contour_num; ++i) {
                    auto tmp = table.contour_end_pts[i];
                    table.contour_end_pts[i] = utl::fromToBE(tmp);
                }
            }

            READ_STREAM_BE(table.inst_length, 2);
            if (table.inst_length > 0) {
                table.insts.reset(new uint8_t[table.inst_length], std::default_delete<uint8_t[]>());
                READ_STREAM(table.insts[0], table.inst_length);
            }

            if (header.contour_num > 0) {
                int32_t cur_x_coord = 0;
                int32_t cur_y_coord = 0;
                uint16_t logic_flag_count = table.contour_end_pts[header.contour_num - 1] + 1;
                // flags
                table.flags.reset(new uint8_t[logic_flag_count], std::default_delete<uint8_t[]>());
                for (uint16_t i = 0; i < logic_flag_count; ++i) {
                    uint8_t flag;
                    READ_STREAM(flag, 1);
                    table.flags[i] = flag;
                    if (flag & REPEAT_FLAG) {
                        READ_STREAM(flag, 1);
                        std::memset(&table.flags[i + 1], table.flags[i], flag);
                        i += flag;
                    }
                }

                // x_coord
                table.x_coord.reset(new int32_t[logic_flag_count], std::default_delete<int32_t[]>());
                for (uint16_t i = 0; i < logic_flag_count; ++i) {
                    int16_t x_r_coord;
                    uint8_t flag = table.flags[i];
                    if (flag & X_SHORT_VECTOR) {
                        // 1 字节
                        uint8_t coord;
                        READ_STREAM(coord, 1);
                        bool positive = (flag & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR);
                        x_r_coord = positive ? coord : -coord;
                    } else {
                        // 2 字节
                        if (flag & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
                            x_r_coord = 0;
                        } else {
                            READ_STREAM_BE(x_r_coord, 2);
                        }
                    }

                    cur_x_coord += x_r_coord;
                    table.x_coord[i] = cur_x_coord;
                }

                // y_coord
                table.y_coord.reset(new int32_t[logic_flag_count], std::default_delete<int32_t[]>());
                for (uint16_t i = 0; i < logic_flag_count; ++i) {
                    int16_t y_r_coord;
                    uint8_t flag = table.flags[i];
                    if (flag & Y_SHORT_VECTOR) {
                        // 1 字节
                        uint8_t coord;
                        READ_STREAM(coord, 1);
                        bool positive = (flag & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR);
                        y_r_coord = positive ? coord : -coord;
                    } else {
                        // 2 字节
                        if (flag & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
                            y_r_coord = 0;
                        } else {
                            READ_STREAM_BE(y_r_coord, 2);
                        }
                    }

                    cur_y_coord += y_r_coord;
                    table.y_coord[i] = cur_y_coord;
                }
            }
        } else {
            // Composite glyph
            return false;
        }

        *out_header = header;
        *out_glyph = std::move(table);
        return true;
    }

}
}