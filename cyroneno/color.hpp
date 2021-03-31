// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_COLOR_HPP_
#define CYRONENO_COLOR_HPP_

#include <algorithm>


namespace cyro {

    template <typename Ty>
    class Color3T {
    public:
        Color3T()
            : r(0), g(0), b(0) {}
        Color3T(Ty r, Ty g, Ty b)
            : r(r), g(g), b(b) {}

        Color3T operator+(const Color3T& rhs) const {
            Color3T c(*this);
            c.add(rhs);
            return c;
        }
        Color3T operator-(const Color3T& rhs) const {
            Color3T c(*this);
            c.sub(rhs);
            return c;
        }
        Color3T operator*(Ty factor) const {
            Color3T c(*this);
            c.mul(factor);
            return c;
        }
        Color3T operator*(const Color3T& rhs) const {
            Color3T c(*this);
            c.mul(rhs);
            return c;
        }
        Color3T operator/(Ty factor) const {
            Color3T c(*this);
            c.div(factor);
            return c;
        }

        Color3T& operator+=(const Color3T& rhs) {
            add(rhs);
            return *this;
        }
        Color3T& operator-=(const Color3T& rhs) {
            sub(rhs);
            return *this;
        }
        Color3T& operator*=(Ty factor) {
            mul(factor);
            return *this;
        }
        Color3T& operator*=(const Color3T& rhs) {
            mul(rhs);
            return *this;
        }
        Color3T& operator/=(Ty factor) {
            div(factor);
            return *this;
        }

        Color3T& add(const Color3T& rhs) {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            return *this;
        }
        Color3T& sub(const Color3T& rhs) {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            return *this;
        }
        Color3T& mul(Ty factor) {
            r *= factor;
            g *= factor;
            b *= factor;
            return *this;
        }
        Color3T& mul(const Color3T& rhs) {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            return *this;
        }
        Color3T& div(Ty factor) {
            r /= factor;
            g /= factor;
            b /= factor;
            return *this;
        }
        Color3T& clamp(Ty min_val, Ty max_val) {
            r = std::clamp(r, min_val, max_val);
            g = std::clamp(g, min_val, max_val);
            b = std::clamp(b, min_val, max_val);
            return *this;
        }

        Ty r, g, b;
    };


    template <typename Ty>
    class Color4T {
    public:
        Color4T()
            : r(0), g(0), b(0), a(0) {}
        Color4T(Ty r, Ty g, Ty b, Ty a)
            : r(r), g(g), b(b), a(a) {}
        Color4T(const Color3T<Ty>& c, Ty a)
            : r(c.r), g(c.g), b(c.b), a(a) {}

        Color4T operator+(const Color4T& rhs) const {
            Color4T c(*this);
            c.add(rhs);
            return c;
        }
        Color4T operator-(const Color4T& rhs) const {
            Color4T c(*this);
            c.sub(rhs);
            return c;
        }
        Color4T operator*(Ty factor) const {
            Color4T c(*this);
            c.mul(factor);
            return c;
        }
        Color4T operator*(const Color4T& rhs) const {
            Color4T c(*this);
            c.mul(rhs);
            return c;
        }
        Color4T operator/(Ty factor) const {
            Color4T c(*this);
            c.div(factor);
            return c;
        }

        Color4T& operator+=(const Color4T& rhs) {
            add(rhs);
            return *this;
        }
        Color4T& operator-=(const Color4T& rhs) {
            sub(rhs);
            return *this;
        }
        Color4T& operator*=(Ty factor) {
            mul(factor);
            return *this;
        }
        Color4T& operator*=(const Color4T& rhs) {
            mul(rhs);
            return *this;
        }
        Color4T& operator/=(Ty factor) {
            div(factor);
            return *this;
        }

        Color4T& add(const Color4T& rhs) {
            r +=rhs.r;
            g +=rhs.g;
            b +=rhs.b;
            a +=rhs.a;
            return *this;
        }
        Color4T& sub(const Color4T& rhs) {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            a -= rhs.a;
            return *this;
        }
        Color4T& mul(Ty factor) {
            r *= factor;
            g *= factor;
            b *= factor;
            a *= factor;
            return *this;
        }
        Color4T& mul(const Color4T& rhs) {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            a *= rhs.b;
            return *this;
        }
        Color4T& div(Ty factor) {
            r /= factor;
            g /= factor;
            b /= factor;
            a /= factor;
            return *this;
        }
        Color4T& clamp(Ty min_val, Ty max_val) {
            r = std::clamp(r, min_val, max_val);
            g = std::clamp(g, min_val, max_val);
            b = std::clamp(b, min_val, max_val);
            a = std::clamp(a, min_val, max_val);
            return *this;
        }

        Color4T mulA(Ty a) const {
            Color4T c(*this);
            c.a *= a;
            return c;
        }

        Ty r, g, b, a;
    };


    using Color3D = Color3T<double>;
    using Color4D = Color4T<double>;

}

#endif  // CYRONENO_COLOR_HPP_