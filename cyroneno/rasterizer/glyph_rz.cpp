// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "glyph_rz.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <list>
#include <numeric>

#include "utils/log.h"

#include "common.h"
#include "rasterizer.h"

#define IS_EZ(val)       std::abs(val) <= std::numeric_limits<double>::epsilon()
#define IS_NEZ(val)      std::abs(val) > std::numeric_limits<double>::epsilon()
#define IS_EQ(v1, v2)    IS_EZ((v1) - (v2))
#define IS_NEQ(v1, v2)   IS_NEZ((v1) - (v2))


namespace cyro {

    template <typename T>
    bool isInBounds(T b1, T b2, T val) {
        static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type!");
        if (b1 > b2) {
            return val <= b1 && val >= b2;
        } else {
            return val >= b1 && val <= b2;
        }
    }

    bool isPoint2Equal(const Point2& p1, const Point2& p2) {
        return IS_EQ(p1.x, p2.x) && IS_EQ(p1.y, p2.y);
    }

    class IntersectionManager {
    public:
        enum Into {
            INTO_IN,
            INTO_OUT,
            INTO_UNKNOWN,
        };

        struct Intersect {
            bool is_two = false;
            Point2 p1, p2;
            Into into1 = INTO_UNKNOWN, into2 = INTO_UNKNOWN;
        };

        void addPoint(const GlyphRz::Part& part, bool is_start) {
            auto p = is_start ? part.s : part.e;
            for (auto it = its.begin(); it != its.end(); ++it) {
                if (it->is_two) {
                    if (isPoint2Equal(p, it->p1)) {
                        it->into1 = getPartInto(part, is_start);
                        return;
                    }
                    if (isPoint2Equal(p, it->p2)) {
                        it->into2 = getPartInto(part, is_start);
                        return;
                    }
                } else {
                    if (isPoint2Equal(p, it->p1)) {
                        auto into = getPartInto(part, is_start);
                        DCHECK(into != INTO_UNKNOWN);
                        if (into != it->into1) {
                            it = its.erase(it);
                        }
                        return;
                    }
                }
            }

            Intersect intersect;
            intersect.is_two = false;
            intersect.p1 = p;
            intersect.into1 = getPartInto(part, is_start);
            its.push_back(intersect);
        }

        void addLinePoints(const GlyphRz::Part& part) {
            Intersect intersect;
            intersect.is_two = true;
            intersect.p1 = part.s;
            intersect.p2 = part.e;

            bool into1_set = false;
            bool into2_set = false;
            for (auto it = its.begin(); it != its.end();) {
                if (it->is_two) {
                    ++it;
                    continue;
                }

                if (isPoint2Equal(part.s, it->p1) && !into1_set) {
                    intersect.into1 = it->into1;
                    into1_set = true;
                    it = its.erase(it);
                } else if (isPoint2Equal(part.e, it->p1) && !into2_set) {
                    intersect.into2 = it->into2;
                    into2_set = true;
                    it = its.erase(it);
                } else {
                    ++it;
                }
            }

            its.push_back(intersect);
        }

        void clear() {
            its.clear();
        }

        static Into getPartInto(const GlyphRz::Part& part, bool is_start) {
            Into result;
            if (part.is_bezier) {
                if (is_start) {
                    result = getPointsInto(part.s, part.m);
                } else {
                    result = getPointsInto(part.m, part.e);
                }
            } else {
                result = getPointsInto(part.s, part.e);
            }
            return result;
        }

        static Into getPointsInto(const Point2& p1, const Point2& p2) {
            if (IS_NEQ(p1.y, p2.y)) {
                return p1.y > p2.y ? INTO_OUT : INTO_IN;
            }
            return INTO_UNKNOWN;
        }

        std::vector<Intersect> its;
    };

    bool bezierInsect(
        const GlyphRz::Part& part, double t,
        double x, double y,
        int32_t* count, IntersectionManager* mgr)
    {
        auto ins_x = (1 - t)*(1 - t)*part.s.x + 2 * t*(1 - t)*part.m.x + t * t*part.e.x;
        if (ins_x == x) {
            return true;
        }
        if (ins_x > x) {
            if (IS_EQ(y, part.s.y)) {
                mgr->addPoint(part, true);
            } else if (IS_EQ(y, part.e.y)) {
                mgr->addPoint(part, false);
            } else {
                auto ne = (part.e.y + part.s.y - 2 * part.m.y)*t - part.s.y + part.m.y;
                if (IS_EZ(ne)) {
                    return false;
                }
                if (ne > 0) {
                    ++(*count);
                } else {
                    --(*count);
                }
            }
        }
        return false;
    }

    bool lineInsect(
        const GlyphRz::Part& part,
        double x, double y,
        int32_t* count, IntersectionManager* mgr)
    {
        auto ins_x = (y - part.s.y) / (part.e.y - part.s.y) * (part.e.x - part.s.x) + part.s.x;
        if (ins_x == x) {
            return true;
        }
        if (ins_x > x) {
            if (IS_EQ(y, part.s.y)) {
                mgr->addPoint(part, true);
            } else if (IS_EQ(y, part.e.y)) {
                mgr->addPoint(part, false);
            } else {
                auto into = IntersectionManager::getPointsInto(part.s, part.e);
                DCHECK(into != IntersectionManager::INTO_UNKNOWN);
                if (into == IntersectionManager::INTO_IN) {
                    ++(*count);
                } else {
                    --(*count);
                }
            }
        }
        return false;
    }

}

namespace cyro {

    GlyphRz::GlyphRz() {}

    void GlyphRz::draw(int w, int h, Rasterizer& rz) {
        //rz.setAntiAlias(false);

        /*for (double y = 0; y < h; y += 1) {
            drawHScanline(y + 0.5, w, rz);
        }*/

        const int sample = 1;
        const double interval = 1.0 / (sample + 1);
        const double alpha = 1.0 / (sample * sample);

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {

                double y = interval;
                for (int m = 0; m < sample; ++m) {
                    double x = interval;
                    for (int n = 0; n < sample; ++n) {
                        if (isInner(j + x, i + y)) {
                            rz.drawPoint(Point2(j, i), Color4D(0, 0, 0, alpha));
                        }
                        x += interval;
                    }
                    y += interval;
                }
            }
        }
    }

    void GlyphRz::addLine(const Point2& s, const Point2& e) {
        Part part;
        part.is_bezier = false;
        part.s = s;
        part.e = e;
        contours_.back().parts.push_back(part);
    }

    void GlyphRz::addQBezier(const Point2& s, const Point2& m, const Point2& e) {
        Part part;
        part.is_bezier = true;
        part.s = s;
        part.m = m;
        part.e = e;
        contours_.back().parts.push_back(part);
    }

    void GlyphRz::newContour() {
        contours_.push_back(Contour());
    }

    bool GlyphRz::hitTest(double x, double y) {
        return isInner(x, y);
    }

    void GlyphRz::drawHScanline(double y, int w, Rasterizer& rz) {
        struct Intersect {
            double x;
            Into into;
        };

        std::vector<Intersect> its;
        for (const auto& cont : contours_) {
            for (const auto& part : cont.parts) {
                double x;
                auto into = getInto(part);
                if (intersectH(y, part, &x)) {
                    its.push_back(Intersect{ x, into });
                }
            }
        }

        if (its.empty()) {
            return;
        }

        std::sort(
            its.begin(), its.end(),
            [](const Intersect& i1, const Intersect& i2)->bool
        {
            if (i1.x < i2.x) return true;
            if (i1.x == i2.x) {
                if (i1.into < i2.into) return true;
            }
            return false;
        });

        auto p_it = its.begin();
        for (auto it = p_it + 1; it != its.end();) {
            if (it->x == p_it->x &&
                it->into == p_it->into &&
                it->into != INTO_UNKNOWN)
            {
                it = its.erase(it);
            } else {
                p_it = it;
                ++it;
            }
        }

        p_it = its.begin();
        for (auto it = p_it + 1; it != its.end(); ++it) {
            if (it->x == p_it->x &&
                it->into != p_it->into &&
                it->into != INTO_UNKNOWN && p_it->into != INTO_UNKNOWN &&
                p_it != its.begin())
            {
                auto pp_it = p_it - 1;
                if (pp_it->x != it->x &&
                    pp_it->into == INTO_IN)
                {
                    auto tmp = *it;
                    *it = *p_it;
                    *p_it = tmp;
                }
            }
            p_it = it;
        }

        bool first = true;
        Intersect prev{};
        for (auto it = its.begin(); it != its.end(); ++it) {
            if (it->into == INTO_UNKNOWN) {
                rz.drawPoint(Point2(it->x, y), Color4D(0, 0, 0, 1));
                continue;
            }

            if (first) {
                prev = *it;
                DCHECK(prev.into == INTO_IN);
                first = false;
                continue;
            }

            if (prev.into == INTO_IN && it->into == INTO_OUT) {
                if(prev.x == it->x) {
                    rz.drawPoint(Point2(it->x, y), Color4D(0, 0, 0, 1));
                } else {
                    rz.drawLine(Point2(prev.x, y), Point2(it->x, y), 1, THICK_START, Color4D(0, 0, 0, 1));
                }
                first = true;
            }
        }
    }

    bool GlyphRz::isInner(double x, double y) {
        int32_t count = 0;
        IntersectionManager mgr;

        for (const auto& cont : contours_) {
            for (const auto& part : cont.parts) {
                if (!isInBounds(part.s.y, part.e.y, y)) {
                    continue;
                }

                if (!part.is_bezier) {
                    // 直线
                    if (IS_EQ(part.s.y, part.e.y)) {
                        // 横线
                        if (isInBounds(part.s.x, part.e.x, x)) {
                            return true;
                        }
                        auto ins_x = std::min(part.s.x, part.e.x);
                        if (ins_x > x) {
                            mgr.addLinePoints(part);
                        }
                    } else {
                        if (lineInsect(part, x, y, &count, &mgr)) {
                            return true;
                        }
                    }
                } else {
                    // 二次贝塞尔曲线
                    if (IS_EQ(part.e.y, part.s.y) && IS_EQ(part.e.y, part.m.y)) {
                        // 横线
                        if (isInBounds(part.s.x, part.e.x, x)) {
                            return true;
                        }
                        auto ins_x = std::min(part.s.x, part.e.x);
                        if (ins_x > x) {
                            mgr.addLinePoints(part);
                        }
                    } else {
                        auto sigma = part.e.y + part.s.y - 2 * part.m.y;
                        if (IS_EZ(sigma)) {
                            // 退化为直线
                            if (lineInsect(part, x, y, &count, &mgr)) {
                                return true;
                            }
                            continue;
                        }

                        auto delta = std::pow(part.m.y - part.s.y, 2) - sigma * (part.s.y - y);
                        if (delta < 0) {
                            // 没交点
                            continue;
                        }
                        if (delta == 0) {
                            // 一个交点
                            auto t = -(part.m.y - part.s.y) / sigma;
                            if (t >= 0 && t <= 1) {
                                if (bezierInsect(part, t, x, y, &count, &mgr)) {
                                    return true;
                                }
                            }
                            continue;
                        }

                        // 两个交点
                        auto t1 = (-(part.m.y - part.s.y) + std::sqrt(delta)) / sigma;
                        if (t1 >= 0 && t1 <= 1) {
                            if (bezierInsect(part, t1, x, y, &count, &mgr)) {
                                return true;
                            }
                        }

                        auto t2 = (-(part.m.y - part.s.y) - std::sqrt(delta)) / sigma;
                        if (t2 >= 0 && t2 <= 1) {
                            if (bezierInsect(part, t2, x, y, &count, &mgr)) {
                                return true;
                            }
                        }
                    }
                }
            }
        }

        for (const auto& in : mgr.its) {
            if (in.is_two) {
                DCHECK(in.into1 != INTO_UNKNOWN);
                DCHECK(in.into2 != INTO_UNKNOWN);
                if (in.into1 == in.into2) {
                    if (in.into1 == INTO_IN) {
                        ++count;
                    } else {
                        --count;
                    }
                }
            } else {
                DCHECK(in.into1 != INTO_UNKNOWN);
                if (in.into1 == INTO_IN) {
                    ++count;
                } else {
                    --count;
                }
            }
        }

        return count != 0;
    }

    bool GlyphRz::intersectH(double y, const Part& part, double* x) {
        if (!part.is_bezier) {
            // 直线
            if (!isInBounds(part.s.y, part.e.y, y)) {
                return false;
            }

            if (IS_EQ(part.s.y, part.e.y)) {
                *x = std::min(part.s.x, part.e.x);
            } else {
                *x = (y - part.s.y) / (part.e.y - part.s.y) * (part.e.x - part.s.x) + part.s.x;
            }
            return true;
        }

        // 二次贝塞尔曲线
        if (!isInBounds(part.s.y, part.e.y, y)) {
            return false;
        }

        if (IS_EQ(part.e.y, part.s.y) && IS_EQ(part.e.y, part.m.y)) {
            // 横线
            *x = std::min(part.s.x, part.e.x);
            return true;
        }

        auto sigma = part.e.y + part.s.y - 2 * part.m.y;
        if (IS_EZ(sigma)) {
            // 退化为直线
            if (IS_EQ(part.s.y, part.e.y)) {
                *x = std::min(part.s.x, part.e.x);
            } else {
                *x = (y - part.s.y) / (part.e.y - part.s.y) * (part.e.x - part.s.x) + part.s.x;
            }
            return true;
        }

        auto delta = std::pow(part.m.y - part.s.y, 2) - sigma * (part.s.y - y);
        if (delta < 0) {
            // 没交点
            return false;
        }
        if (delta == 0) {
            // 一个交点
            auto t = -(part.m.y - part.s.y) / sigma;
            if (t >= 0 && t <= 1) {
                *x = (1 - t)*(1 - t)*part.s.x + 2 * t*(1 - t)*part.m.x + t * t*part.e.x;
                return true;
            }
            return false;
        }

        // 两个交点
        auto t1 = (-(part.m.y - part.s.y) + std::sqrt(delta)) / sigma;
        if (t1 >= 0 && t1 <= 1) {
            *x = (1 - t1)*(1 - t1)*part.s.x + 2 * t1*(1 - t1)*part.m.x + t1 * t1*part.e.x;
            return true;
        }

        auto t2 = (-(part.m.y - part.s.y) - std::sqrt(delta)) / sigma;
        if (t2 >= 0 && t2 <= 1) {
            *x = (1 - t2)*(1 - t2)*part.s.x + 2 * t2*(1 - t2)*part.m.x + t2 * t2*part.e.x;
            return true;
        }
        return false;
    }

    GlyphRz::Into GlyphRz::getInto(const Part& part) {
        if (IS_NEZ(part.s.y - part.e.y)) {
            return part.s.y > part.e.y ? INTO_OUT : INTO_IN;
        }
        if (IS_NEZ(part.s.x - part.e.x)) {
            if (IS_EZ(part.s.y - part.e.y)) {
                return INTO_IN;
            }
            return part.s.x > part.e.x ? INTO_OUT : INTO_IN;
        }
        return INTO_UNKNOWN;
    }

}
