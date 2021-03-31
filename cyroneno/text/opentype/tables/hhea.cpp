// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "hhea.h"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool HHEA::parseTable(std::istream& s, HoriHeaderTable* out) {
        HoriHeaderTable head;

        READ_STREAM_BE(head.major_ver, 2);
        READ_STREAM_BE(head.minor_ver, 2);
        READ_STREAM_BE(head.ascender, 2);
        READ_STREAM_BE(head.descender, 2);
        READ_STREAM_BE(head.line_gap, 2);
        READ_STREAM_BE(head.adv_width_max, 2);
        READ_STREAM_BE(head.min_left_side_bearing, 2);
        READ_STREAM_BE(head.min_right_side_bearing, 2);
        READ_STREAM_BE(head.x_max_extent, 2);
        READ_STREAM_BE(head.caret_slope_rise, 2);
        READ_STREAM_BE(head.caret_slope_run, 2);
        READ_STREAM_BE(head.caret_offset, 2);
        READ_STREAM_BE(head.reserved0, 2);
        READ_STREAM_BE(head.reserved1, 2);
        READ_STREAM_BE(head.reserved2, 2);
        READ_STREAM_BE(head.reserved3, 2);
        READ_STREAM_BE(head.metric_data_format, 2);
        READ_STREAM_BE(head.hm_num, 2);

        *out = head;
        return true;
    }

}
}
