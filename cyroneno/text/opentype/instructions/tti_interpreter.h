// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEXT_OPENTYPE_INSTRUCTION_TTI_INTERPRETER_H_
#define CYRONENO_TEXT_OPENTYPE_INSTRUCTION_TTI_INTERPRETER_H_

#include <cstdint>
#include <stack>
#include <vector>

#include "../tables/cvt.h"
#include "graphics_state.h"


namespace cyro {
namespace otf {

    // https://docs.microsoft.com/zh-cn/typography/opentype/spec/tt_instructions
    class TTInterpreter {
    public:
        TTInterpreter(
            CVT::CVTTable& cvt, GraphicsState& gs,
            int32_t* x_coord, int32_t* y_coord, size_t count);

        bool decode(const uint8_t* opcode, uint32_t length);

    private:
        enum ERC {
            ERC_NONE,
            ERC_BMI_OPCODE,
            ERC_BMI_ZONE0,
            ERC_BMI_ZONE1,
            ERC_BMI_STORAGE,
            ERC_DIV_ZERO,
            ERC_INVALID_ZONE_NUM,
            ERC_UNKNOWN_OP,
        };

        struct Point {
            int32_t x = 0;
            int32_t y = 0;
        };

        bool decodeNext(
            const uint8_t* opcode, uint32_t length, uint32_t* op_length, ERC* erc);

        bool setVecToLine(Vec* vec, const Point& p1, const Point& p2, uint8_t dir, ERC* erc);
        bool getPointByIdx(size_t index, uint16_t zp, Point* pt, ERC* erc) const;

        std::stack<int32_t> stack_;
        std::vector<uint32_t> storage_;
        std::vector<Point> twilight_zone_;

        CVT::CVTTable& cvt_;
        GraphicsState& gs_;
        int32_t* x_coord_;
        int32_t* y_coord_;
        size_t coord_count_;
    };

}
}

#endif  // CYRONENO_TEXT_OPENTYPE_INSTRUCTION_TTI_INTERPRETER_H_