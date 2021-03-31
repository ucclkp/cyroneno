// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "maxp.h"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool MAXP::parseTable(std::istream& s, MaximumProfile* out) {
        MaximumProfile profile;

        READ_STREAM_BE(profile.version, 4);
        READ_STREAM_BE(profile.glyph_num, 2);

        uint16_t major_ver = profile.version >> 16;
        uint16_t minor_ver = profile.version & 0xFFFF;

        if (major_ver == 1) {
            READ_STREAM_BE(profile.max_pts, 2);
            READ_STREAM_BE(profile.max_contours, 2);
            READ_STREAM_BE(profile.max_composite_pts, 2);
            READ_STREAM_BE(profile.max_composite_contours, 2);
            READ_STREAM_BE(profile.max_zones, 2);
            READ_STREAM_BE(profile.max_twilight_pts, 2);
            READ_STREAM_BE(profile.max_storage, 2);
            READ_STREAM_BE(profile.max_func_defs, 2);
            READ_STREAM_BE(profile.max_inst_defs, 2);
            READ_STREAM_BE(profile.max_stack_elements, 2);
            READ_STREAM_BE(profile.max_inst_size, 2);
            READ_STREAM_BE(profile.max_component_elements, 2);
            READ_STREAM_BE(profile.max_component_depth, 2);
        } else if (major_ver > 1) {
            return false;
        }

        *out = profile;
        return true;
    }

}
}