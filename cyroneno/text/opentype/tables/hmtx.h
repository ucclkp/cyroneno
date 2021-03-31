// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEXT_OPENTYPE_TABLES_HMTX_H_
#define CYRONENO_TEXT_OPENTYPE_TABLES_HMTX_H_

#include <istream>
#include <vector>


namespace cyro {
namespace otf {

    /****** hmtx
     * 包含水平布局的轴承和步进值
     * https://docs.microsoft.com/zh-cn/typography/opentype/spec/hmtx
     */
    class HMTX {
    public:
        struct HorMetric {
            uint16_t advance_width;
            int16_t left_side_bearing;
        };

        static bool parseTable(
            std::istream& s, uint16_t hm_num, uint16_t glyph_num, std::vector<HorMetric>* out);
    };

}
}

#endif  // CYRONENO_TEXT_OPENTYPE_TABLES_HMTX_H_