// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "hmtx.h"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool HMTX::parseTable(
        std::istream& s, uint16_t hm_num, uint16_t glyph_num, std::vector<HorMetric>* out)
    {
        uint16_t i;
        std::vector<HorMetric> hms;
        for (i = 0; i < hm_num; ++i) {
            HorMetric hm;
            READ_STREAM_BE(hm.advance_width, 2);
            READ_STREAM_BE(hm.left_side_bearing, 2);
            hms.push_back(hm);
        }

        if (hms.empty()) {
            return false;
        }

        auto back_aw = hms.back().advance_width;

        for (; i < glyph_num; ++i) {
            HorMetric hm;
            hm.advance_width = back_aw;
            READ_STREAM_BE(hm.left_side_bearing, 2);
            hms.push_back(hm);
        }

        *out = std::move(hms);

        return true;
    }

}
}
