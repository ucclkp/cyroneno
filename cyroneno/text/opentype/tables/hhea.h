// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEXT_OPENTYPE_TABLES_HHEA_H_
#define CYRONENO_TEXT_OPENTYPE_TABLES_HHEA_H_

#include <istream>


namespace cyro {
namespace otf {

    /****** hhea
     * 用于提供水平布局时所需的信息
     * https://docs.microsoft.com/zh-cn/typography/opentype/spec/hhea
     */
    class HHEA {
    public:
        struct HoriHeaderTable {
            uint16_t major_ver;
            uint16_t minor_ver;
            /*FWORD*/  int16_t ascender;
            /*FWORD*/  int16_t descender;
            /*FWORD*/  int16_t line_gap;
            /*UFWORD*/ uint16_t adv_width_max;
            /*FWORD*/  int16_t min_left_side_bearing;
            /*FWORD*/  int16_t min_right_side_bearing;
            /*FWORD*/  int16_t x_max_extent;
            int16_t caret_slope_rise;
            int16_t caret_slope_run;
            int16_t caret_offset;
            int16_t reserved0;
            int16_t reserved1;
            int16_t reserved2;
            int16_t reserved3;
            int16_t metric_data_format;
            uint16_t hm_num;
        };

        static bool parseTable(std::istream& s, HoriHeaderTable* out);
    };

}
}

#endif  // CYRONENO_TEXT_OPENTYPE_TABLES_HHEA_H_