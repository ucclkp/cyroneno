// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "tti_interpreter.h"

#define NEXT_BYTE()      \
    ++idx;               \
    if (idx >= length) { \
        *erc = ERC_BMI_OPCODE;  \
        return false;    \
    }


namespace {

    uint16_t toF2DOT14(double val) {
        int16_t i2 = int16_t(std::floor(val));
        uint16_t d14 = uint16_t(std::round(std::abs(val - i2) * 16384));
        return (i2 << 14) | d14;
    }

    double fromF2DOT14(uint16_t val) {
        int16_t i2 = int16_t(val) >> 14;
        double d14 = (val & 0x3FFF) / 16384.0;
        return d14 + i2;
    }

}

namespace cyro {
namespace otf {

    TTInterpreter::TTInterpreter(
        CVT::CVTTable& cvt, GraphicsState& gs,
        int32_t* x_coord, int32_t* y_coord, size_t count)
        : cvt_(cvt), gs_(gs), x_coord_(x_coord), y_coord_(y_coord), coord_count_(count) {}

    bool TTInterpreter::decode(const uint8_t* opcode, uint32_t length) {
        if (length == 0) {
            return true;
        }

        for (uint32_t i = 0; i < length;) {
            ERC erc;
            uint32_t cur_op_length = 0;
            if (!decodeNext(opcode, length - i, &cur_op_length, &erc)) {
                return false;
            }
            i += cur_op_length;
        }

        return true;
    }

    bool TTInterpreter::decodeNext(
        const uint8_t* opcode, uint32_t length, uint32_t* op_length, ERC* erc)
    {
        if (length == 0) {
            *erc = ERC_BMI_OPCODE;
            return false;
        }

        uint32_t idx = 0;
        uint8_t cur_op = opcode[0];
        switch (cur_op) {
        case 0x40:
        {
            // NPUSHB
            NEXT_BYTE();
            uint8_t n = opcode[idx];
            for (uint8_t i = 0; i < n; ++i) {
                NEXT_BYTE();
                stack_.push(uint32_t(opcode[idx]));
            }
            *op_length = 1 + 1 + n;
            break;
        }

        case 0x41:
        {
            // NPUSHW
            NEXT_BYTE();
            uint8_t n = opcode[idx];
            for (uint8_t i = 0; i < n; ++i) {
                NEXT_BYTE();
                uint16_t tmp = uint16_t(opcode[idx]) << 8;
                NEXT_BYTE();
                tmp |= opcode[idx];
                stack_.push(int32_t(int16_t(tmp)));
            }
            *op_length = 1 + 1 + n * 2;
            break;
        }

        case 0xB0: case 0xB1: case 0xB2: case 0xB3:
        case 0xB4: case 0xB5: case 0xB6: case 0xB7:
        {
            // PUSHB
            uint8_t n = cur_op - 0xB0 + 1;
            for (uint8_t i = 0; i < n; ++i) {
                NEXT_BYTE();
                stack_.push(uint32_t(opcode[idx]));
            }
            *op_length = 1 + n;
            break;
        }

        case 0xB8: case 0xB9: case 0xBA: case 0xBB:
        case 0xBC: case 0xBD: case 0xBE: case 0xBF:
        {
            // PUSHW
            uint8_t n = cur_op - 0xB8 + 1;
            for (uint8_t i = 0; i < n; ++i) {
                NEXT_BYTE();
                uint16_t tmp = uint16_t(opcode[idx]) << 8;
                NEXT_BYTE();
                tmp |= opcode[idx];
                stack_.push(int32_t(int16_t(tmp)));
            }
            *op_length = 1 + n * 2;
            break;
        }

        case 0x43:
        {
            // RS
            auto loc = uint32_t(stack_.top());
            stack_.pop();
            stack_.push(uint32_t(storage_[loc]));
            *op_length = 1;
            break;
        }

        case 0x42:
        {
            // WS
            auto val = uint32_t(stack_.top());
            stack_.pop();
            auto loc = uint32_t(stack_.top());
            stack_.pop();
            storage_[loc] = val;
            *op_length = 1;
            break;
        }

        case 0x44:
        {
            // WCVTP
            auto val = stack_.top();  // F26Dot6
            stack_.pop();
            auto loc = uint32_t(stack_.top());
            stack_.pop();
            cvt_.cvt[loc] = val;
            *op_length = 1;
            break;
        }

        case 0x70:
        {
            // WCVTF
            auto val = uint32_t(stack_.top());
            stack_.pop();
            auto loc = uint32_t(stack_.top());
            stack_.pop();
            cvt_.cvt[loc] = val;
            *op_length = 1;
            break;
        }

        case 0x45:
        {
            // RCVT
            auto loc = uint32_t(stack_.top());
            stack_.pop();
            stack_.push(cvt_.cvt[loc]); // F26Dot6
            *op_length = 1;
            break;
        }

        case 0x00: case 0x01:
        {
            // SVTCA
            auto a = cur_op - 0x00;
            uint16_t val = uint16_t(1) << 14;
            if (a == 0x00) {
                gs_.freedom_vector = { 0, val };
                gs_.proj_vector = { 0, val };
            } else {
                gs_.freedom_vector = { val, 0 };
                gs_.proj_vector = { val, 0 };
            }
            *op_length = 1;
            break;
        }

        case 0x02: case 0x03:
        {
            // SPVTCA
            auto a = cur_op - 0x02;
            uint16_t val = uint16_t(1) << 14;
            if (a == 0x00) {
                gs_.proj_vector = { 0, val };
            } else {
                gs_.proj_vector = { val, 0 };
            }
            *op_length = 1;
            break;
        }

        case 0x04: case 0x05:
        {
            // SFVTCA
            auto a = cur_op - 0x04;
            uint16_t val = uint16_t(1) << 14;
            if (a == 0x00) {
                gs_.freedom_vector = { 0, val };
            } else {
                gs_.freedom_vector = { val, 0 };
            }
            *op_length = 1;
            break;
        }

        case 0x06: case 0x07:
        {
            // SPVTL
            auto a = cur_op - 0x06;
            auto p1_i = uint32_t(stack_.top());
            stack_.pop();
            auto p2_i = uint32_t(stack_.top());
            stack_.pop();

            Point p1;
            if (!getPointByIdx(p1_i, gs_.gep2, &p1, erc)) {
                return false;
            }

            Point p2;
            if (!getPointByIdx(p2_i, gs_.gep1, &p2, erc)) {
                return false;
            }

            if (!setVecToLine(&gs_.proj_vector, p1, p2, a, erc)) {
                return true;
            }
            break;
        }

        case 0x08: case 0x09:
        {
            // SFVTL
            auto a = cur_op - 0x08;
            auto p1_i = uint32_t(stack_.top());
            stack_.pop();
            auto p2_i = uint32_t(stack_.top());
            stack_.pop();

            Point p1;
            if (!getPointByIdx(p1_i, gs_.gep2, &p1, erc)) {
                return false;
            }

            Point p2;
            if (!getPointByIdx(p2_i, gs_.gep1, &p2, erc)) {
                return false;
            }

            if (!setVecToLine(&gs_.freedom_vector, p1, p2, a, erc)) {
                return true;
            }
            break;
        }

        case 0x0E:
        {
            // SFVTPV
            gs_.freedom_vector = gs_.proj_vector;
            break;
        }

        case 0x86: case 0x87:
        {
            // SDPVTL
            auto a = cur_op - 0x08;
            auto p1_i = uint32_t(stack_.top());
            stack_.pop();
            auto p2_i = uint32_t(stack_.top());
            stack_.pop();

            Point p1;
            if (!getPointByIdx(p1_i, gs_.gep2, &p1, erc)) {
                return false;
            }

            Point p2;
            if (!getPointByIdx(p2_i, gs_.gep1, &p2, erc)) {
                return false;
            }

            if (!setVecToLine(&gs_.dual_proj_vectors, p1, p2, a, erc)) {
                return true;
            }
            break;
        }

        case 0x0A:
        {
            // SPVFS
            gs_.proj_vector.y = uint16_t(stack_.top() & 0xFFFF);
            stack_.pop();
            gs_.proj_vector.x = uint16_t(stack_.top() & 0xFFFF);
            stack_.pop();
            break;
        }

        case 0x0B:
        {
            // SFVFS
            gs_.freedom_vector.y = uint16_t(stack_.top() & 0xFFFF);
            stack_.pop();
            gs_.freedom_vector.x = uint16_t(stack_.top() & 0xFFFF);
            stack_.pop();
            break;
        }

        case 0x0C:
        {
            // GPV
            stack_.push(gs_.proj_vector.x);
            stack_.push(gs_.proj_vector.y);
            break;
        }

        case 0x0D:
        {
            // GFV
            stack_.push(gs_.freedom_vector.x);
            stack_.push(gs_.freedom_vector.y);
            break;
        }

        case 0x10:
        {
            // SRP0
            gs_.rp0 = stack_.top();
            stack_.pop();
            break;
        }

        case 0x11:
        {
            // SRP1
            gs_.rp1 = stack_.top();
            stack_.pop();
            break;
        }

        case 0x12:
        {
            // SRP2
            gs_.rp2 = stack_.top();
            stack_.pop();
            break;
        }

        case 0x13:
        {
            // SZP0
            auto n = stack_.top();
            stack_.pop();

            if (n < 0 || n > 1) {
                *erc = ERC_INVALID_ZONE_NUM;
                return false;
            }

            gs_.gep0 = n;
            break;
        }

        case 0x14:
        {
            // SZP1
            auto n = stack_.top();
            stack_.pop();

            if (n < 0 || n > 1) {
                *erc = ERC_INVALID_ZONE_NUM;
                return false;
            }

            gs_.gep1 = n;
            break;
        }

        case 0x15:
        {
            // SZP2
            auto n = stack_.top();
            stack_.pop();

            if (n < 0 || n > 1) {
                *erc = ERC_INVALID_ZONE_NUM;
                return false;
            }

            gs_.gep2 = n;
            break;
        }

        case 0x16:
        {
            // SZPS
            auto n = stack_.top();
            stack_.pop();

            if (n < 0 || n > 1) {
                *erc = ERC_INVALID_ZONE_NUM;
                return false;
            }

            gs_.gep0 = n;
            gs_.gep1 = n;
            gs_.gep2 = n;
            break;
        }

        case 0x19:
        {
            // RTHG
            gs_.round_state = 0;
            break;
        }

        case 0x18:
        {
            // RTG
            gs_.round_state = 1;
            break;
        }

        case 0x3D:
        {
            // RTDG
            gs_.round_state = 2;
            break;
        }

        case 0x7D:
        {
            // RDTG
            gs_.round_state = 3;
            break;
        }

        case 0x7C:
        {
            // RUTG
            gs_.round_state = 4;
            break;
        }

        case 0x7A:
        {
            // ROFF
            gs_.round_state = 5;
            break;
        }

        case 0x76:
        {
            // SROUND
            gs_.round_state = 6;  // ???
            break;
        }

        case 0x77:
        {
            // S45ROUND
            gs_.round_state = 7;  // ???
            break;
        }

        case 0x17:
        {
            // SLOOP
            // TODO: reset
            gs_.loop = int32_t(stack_.top());
            stack_.pop();
            break;
        }

        case 0x1A:
        {
            // SMD
            gs_.min_distance = stack_.top();
            stack_.pop();
            break;
        }

        case 0x8E:
        {
            // INSTCTRL (Only in CVT)
            int32_t s = int32_t(stack_.top());
            stack_.pop();
            uint16_t value = uint16_t(stack_.top()); // USHORT
            stack_.pop();

            // TODO: 需要确认
            if (s == 1) {
                gs_.inst_ctrl = value;
            }
            break;
        }

        case 0x85:
        {
            // SCANCTRL
            uint16_t n = uint16_t(stack_.top() & 0xFFFF);
            stack_.pop();

            gs_.scan_ctrl &= 0xFFFF0000;
            gs_.scan_ctrl |= n;
            break;
        }

        case 0x8D:
        {
            // SCANTYPE
            uint16_t n = uint16_t(stack_.top() & 0xFFFF);
            stack_.pop();

            gs_.scan_ctrl &= 0x0000FFFF;
            gs_.scan_ctrl |= uint32_t(n) << 16;
            break;
        }

        default:
            *erc = ERC_UNKNOWN_OP;
            return false;
        }

        return true;
    }

    bool TTInterpreter::setVecToLine(
        Vec* vec, const Point& p1, const Point& p2, uint8_t dir, ERC* erc)
    {
        auto dx = p2.x - p1.x;
        auto dy = p2.y - p1.y;
        auto sl = dx * dx + dy * dy;
        if (sl == 0) {
            *erc = ERC_DIV_ZERO;
            return false;
        }
        double distance = std::sqrt(dx * dx + dy * dy);

        if (dir == 0x00) {
            // parallel
            vec->x = toF2DOT14(dx / distance);
            vec->y = toF2DOT14(dy / distance);
        } else {
            // perpendicular
            vec->x = toF2DOT14(-dy / distance);
            vec->y = toF2DOT14(dx / distance);
        }
        return true;
    }

    bool TTInterpreter::getPointByIdx(
        size_t index, uint16_t zp, Point* pt, ERC* erc) const
    {
        if (zp == 0) {
            // Zone 0
            if (index >= twilight_zone_.size()) {
                *erc = ERC_BMI_ZONE0;
                return false;
            }
            *pt = twilight_zone_[index];
        } else {
            // Zone 1
            if (index >= coord_count_) {
                *erc = ERC_BMI_ZONE1;
                return false;
            }
            pt->x = x_coord_[index];
            pt->y = y_coord_[index];
        }
        return true;
    }

}
}