// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cvt.h"

#include "utils/stream_utils.h"


namespace cyro {
namespace otf {

    bool CVT::parseTable(std::istream& s, uint32_t length, CVTTable* out_cvt) {
        auto n = length / 2;
        CVTTable table;
        for (uint32_t i = 0; i < n; ++i) {
            int16_t val;
            READ_STREAM_BE(val, 2);
            table.cvt.push_back(val);
        }
        *out_cvt = std::move(table);
        return true;
    }

}
}