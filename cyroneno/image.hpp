// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_IMAGE_HPP_
#define CYRONENO_IMAGE_HPP_

#include <cassert>

#include "color.hpp"


namespace cyro {

    template <typename Ty>
    class ImageT {
    public:
        ImageT(int width, int height)
            : width(width), height(height)
        {
            data = new Ty[width * height];
        }

        ImageT(int width, int height, const Ty& c)
            : width(width), height(height)
        {
            data = new Ty[width * height];
            for (int i = 0; i < width * height; ++i) {
                data[i] = c;
            }
        }

        ImageT(const ImageT& rhs)
            : width(rhs.width), height(rhs.height),
              flip_y(rhs.flip_y)
        {
            data = new Ty[width * height];
            std::memcpy(data, rhs.data, sizeof(Ty) * width * height);
        }

        ImageT(ImageT&& rhs) noexcept
            : width(rhs.width), height(rhs.height),
              flip_y(rhs.flip_y), data(rhs.data)
        {
            rhs.width = 0;
            rhs.height = 0;
            rhs.data = nullptr;
        }

        ~ImageT() {
            delete[] data;
        }

        ImageT& operator=(const ImageT& rhs) {
            width = rhs.width;
            height = rhs.height;
            flip_y = rhs.flip_y;
            std::memcpy(data, rhs.data, sizeof(Ty) * width * height);
            return *this;
        }

        ImageT& operator=(ImageT&& rhs) noexcept {
            width = rhs.width;
            height = rhs.height;
            flip_y = rhs.flip_y;
            data = rhs.data;

            rhs.width = 0;
            rhs.height = 0;
            rhs.data = nullptr;
            return *this;
        }

        void setFlippedY(bool flip) {
            flip_y = flip;
        }

        void setColor(int x, int y, const Ty& c) {
            if (x < 0 || x >= width || y < 0 || y >= height) return;

            if (!flip_y) {
                y = height > 0 ? (height - y - 1) : 0;
            }
            int index = x + y * width;
            data[index] = c;
        }

        template <typename Blend>
        void drawColor(int x, int y, const Ty& c, Blend _blend) {
            if (x < 0 || x >= width || y < 0 || y >= height) return;

            auto prev = getColor(x, y);
            Ty blend = _blend(prev, c);

            if (!flip_y) {
                y = height > 0 ? (height - y - 1) : 0;
            }
            int index = x + y * width;
            data[index] = blend;
        }

        const Ty& getColor(int x, int y) const {
            assert(x >= 0 && x < width && y >= 0 && y < height);

            if (!flip_y) {
                y = height > 0 ? (height - y - 1) : 0;
            }
            int index = x + y * width;
            return data[index];
        }

        int width, height;
        bool flip_y = false;
        Ty* data;
    };


    struct BGRAInt {
        BGRAInt()
            : b(0), g(0), r(0), a(0) {}
        BGRAInt(uint8_t b, uint8_t g, uint8_t r, uint8_t a)
            : b(b), g(g), r(r), a(a) {}
        explicit BGRAInt(const Color3D& c)
            : b(uint8_t(c.b * 255)), g(uint8_t(c.g * 255)), r(uint8_t(c.r * 255)), a(255) {}
        explicit BGRAInt(const Color4D& c)
            : b(uint8_t(c.b * 255)), g(uint8_t(c.g * 255)), r(uint8_t(c.r * 255)), a(uint8_t(c.a * 255)) {}

        uint8_t b, g, r, a;
    };

    using Image = ImageT<Color4D>;
    using ImageBGRAInt = ImageT<BGRAInt>;

    inline Color4D blend_c4d(const Color4D& dst, const Color4D& src) {
        Color4D result;
        result.r = src.r * src.a + (1 - src.a) * dst.r;
        result.g = src.g * src.a + (1 - src.a) * dst.g;
        result.b = src.b * src.a + (1 - src.a) * dst.b;
        result.a = src.a + (1 - src.a) * dst.a;
        return result;
    }

    inline Color4D blend_c4d_add(const Color4D& prev, const Color4D& cur) {
        auto color = cur;
        color.a += prev.a;
        color.clamp(0, 1);
        return color;
    }

    inline ImageBGRAInt toImageBGRAInt(const Image& img) {
        ImageBGRAInt result(img.width, img.height);
        for (int i = 0; i < img.width*img.height; ++i) {
            result.data[i] = BGRAInt(img.data[i]);
        }
        return result;
    }

}

#endif  // CYRONENO_IMAGE_HPP_