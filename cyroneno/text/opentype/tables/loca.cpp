// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "loca.h"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool LOCA::parseTable(
        std::istream& s,
        const HEAD::HeadTable& head, const MAXP::MaximumProfile& maxp,
        std::vector<uint32_t>* offsets)
    {
        if (head.idx_to_loc_format == 0) {
            for (uint16_t i = 0; i <= maxp.glyph_num; ++i) {
                uint16_t tmp;
                READ_STREAM_BE(tmp, 2);
                offsets->push_back(uint32_t(tmp) * 2);
            }
        } else {
            for (uint16_t i = 0; i <= maxp.glyph_num; ++i) {
                uint32_t tmp;
                READ_STREAM_BE(tmp, 4);
                offsets->push_back(tmp);
            }
        }
        return true;
    }

}
}