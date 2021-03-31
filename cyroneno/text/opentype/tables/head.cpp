// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "head.h"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool HEAD::parseTable(std::istream& s, HeadTable* out) {
        HeadTable head;

        READ_STREAM_BE(head.major_ver, 2);
        READ_STREAM_BE(head.minor_ver, 2);
        READ_STREAM_BE(head.font_rev, 4);
        READ_STREAM_BE(head.checksum_adj, 4);
        READ_STREAM_BE(head.magic, 4);

        if (head.magic != kMagicNumber) {
            return false;
        }

        READ_STREAM_BE(head.flags, 2);
        READ_STREAM_BE(head.unit_per_em, 2);
        READ_STREAM_BE(head.created, 8);
        READ_STREAM_BE(head.modified, 8);
        READ_STREAM_BE(head.min_x, 2);
        READ_STREAM_BE(head.min_y, 2);
        READ_STREAM_BE(head.max_x, 2);
        READ_STREAM_BE(head.max_y, 2);
        READ_STREAM_BE(head.mac_style, 2);
        READ_STREAM_BE(head.lowest_rec_PPEM, 2);
        READ_STREAM_BE(head.font_dir_hint, 2);
        READ_STREAM_BE(head.idx_to_loc_format, 2);
        READ_STREAM_BE(head.glyph_data_format, 2);

        *out = head;
        return true;
    }

}
}