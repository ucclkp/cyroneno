// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEXT_OPENTYPE_TABLES_CVT_H_
#define CYRONENO_TEXT_OPENTYPE_TABLES_CVT_H_

#include <cstdint>
#include <fstream>
#include <vector>


namespace cyro {
namespace otf {

    /****** cvt
     * https://docs.microsoft.com/zh-cn/typography/opentype/spec/cvt
     */
    class CVT {
    public:
        struct CVTTable {
            std::vector<int16_t> cvt;
        };

        static bool parseTable(std::istream& s, uint32_t length, CVTTable* out_cvt);
    };

}
}

#endif  // CYRONENO_TEXT_OPENTYPE_TABLES_CVT_H_