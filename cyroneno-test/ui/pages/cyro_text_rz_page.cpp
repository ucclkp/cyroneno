// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/ui/pages/cyro_text_rz_page.h"

#include "utils/convert.h"

#include "ukive/elements/color_element.h"
#include "ukive/elements/texteditor_element.h"
#include "ukive/resources/layout_instantiator.h"
#include "ukive/diagnostic/grid_view.h"
#include "ukive/views/button.h"
#include "ukive/views/check_box.h"
#include "ukive/views/image_view.h"
#include "ukive/views/text_view.h"
#include "ukive/views/layout/layout_view.h"
#include "ukive/window/window.h"

#include "cyroneno/rasterizer/image_rasterizer.h"
#include "cyroneno/rasterizer/image_target.h"
#include "cyroneno/text/text_rasterizer.h"

#include "cyroneno-test/resources/necro_resources_id.h"


namespace cyro {

    class GridTarget : public Target {
    public:
        explicit GridTarget(ukive::GridView* view)
            : view_(view) {}

        void drawColor(int x, int y, const Color4D& c) override {
            ukive::Color uc;
            if (view_->getPixel(x, y, &uc)) {
                switch (mode_) {
                case BM_ADD:
                    uc.r = float(c.r);
                    uc.g = float(c.g);
                    uc.b = float(c.b);
                    uc.a += float(c.a);
                    uc.a = std::clamp(uc.a, 0.f, 1.f);
                    break;

                default:
                    uc.r = float(c.r)*float(c.a) + uc.r * (1 - float(c.a));
                    uc.g = float(c.g)*float(c.a) + uc.g * (1 - float(c.a));
                    uc.b = float(c.b)*float(c.a) + uc.b * (1 - float(c.a));
                    uc.a = float(c.a) + uc.a * (1 - float(c.a));
                    break;
                }

                view_->addPixel(x, y, uc);
            } else {
                view_->addPixel(
                    x, y,
                    ukive::Color(float(c.r), float(c.g), float(c.b), float(c.a)));
            }
        }

    private:
        ukive::GridView* view_;
    };

    class GridRasterizer : public ImageRasterizer {
    public:
        using super = ImageRasterizer;

        GridRasterizer(Target* target, ukive::GridView* view)
            : ImageRasterizer(target), view_(view) {}

        void drawLine(
            const Point2& p0, const Point2& p1,
            double thick, int thick_type, const Color4D& c) override
        {
            //super::drawLine(p0, p1, thick, thick_type, c);
            auto context = view_->getContext();
            view_->addLine(
                ukive::PointD(p0.x, p0.y),
                ukive::PointD(p1.x, p1.y),
                ukive::Color::Black);
        }

        void drawLineSeg(const Point2& p0, const Point2& p1, const Color4D& c) override {
            //super::drawLineSeg(p0, p1, c);
            auto context = view_->getContext();
            view_->addLine(
                ukive::PointD(p0.x, p0.y),
                ukive::PointD(p1.x, p1.y),
                ukive::Color::Green400);
            view_->addPoint(p0.x, p0.y, context.dp2pxi(3), ukive::Color::Green400,
                utl::to_u16string(index_), ukive::Color::Green400);
            ++index_;
        }

        void drawQuadBezier(
            const Point2& p1, const Point2& p2, const Point2& p3, const Color4D& c) override
        {
            //super::drawQuadBezier(p1, p2, p3, c);
            auto context = view_->getContext();
            view_->addQBezierLine(
                ukive::PointD(p1.x, p1.y),
                ukive::PointD(p2.x, p2.y),
                ukive::PointD(p3.x, p3.y),
                ukive::Color::Pink200);
            view_->addPoint(p1.x, p1.y, context.dp2pxi(3), ukive::Color::Pink200,
                utl::to_u16string(index_), ukive::Color::Pink200);
            ++index_;
            view_->addPoint(p2.x, p2.y, context.dp2pxi(3), ukive::Color::Pink200,
                utl::to_u16string(index_), ukive::Color::Pink200);
            ++index_;
        }

    private:
        int index_ = 0;
        ukive::GridView* view_;
    };

}

namespace cyro {

    CyroTextRzPage::CyroTextRzPage(ukive::Window* w)
        : Page(w) {}

    ukive::View* CyroTextRzPage::onCreate(ukive::LayoutView* parent) {
        auto v = ukive::LayoutInstantiator::from(
            parent->getContext(), parent, Res::Layout::cyro_text_rz_page_layout_xml);

        v->setBackground(new ukive::ColorElement(ukive::Color(0.5f, 0.5f, 0.5f)));

        findViewById<ukive::View>(v, Res::Id::rl_cyro_text_rz_settings)
            ->setBackground(new ukive::ColorElement(ukive::Color::White));

        text_view_ = findViewById<ukive::TextView>(v, Res::Id::tv_cyro_text_rz_text);
        text_view_->setBackground(new ukive::TextEditorElement(parent->getContext()));
        text_view_->setIsEditable(true);
        text_view_->setIsSelectable(true);
        text_view_->autoWrap(false);

        commit_button_ = findViewById<ukive::Button>(v, Res::Id::bt_cyro_text_rz_commit);
        commit_button_->setOnClickListener(this);

        debug_cb_ = findViewById<ukive::CheckBox>(v, Res::Id::cb_cyro_text_rz_debug);
        debug_cb_->setOnCheckListener(this);

        outline_cb_ = findViewById<ukive::CheckBox>(v, Res::Id::cb_cyro_text_rz_outline);
        outline_cb_->setOnCheckListener(this);

        grid_view_ = findViewById<ukive::GridView>(v, Res::Id::gv_cyro_text_rz_page_img);
        grid_view_->setBackground(new ukive::ColorElement(ukive::Color::White));
        grid_view_->setFlippedY(true);
        grid_view_->setVisibility(ukive::View::HIDE);

        img_view_ = findViewById<ukive::ImageView>(v, Res::Id::iv_cyro_text_rz_page_img);
        return v;
    }

    void CyroTextRzPage::onInitialize() {
        text_ = u"这";
        //text_ = u"这是一行文本";
        //text = u"abcdefghijklmnopqrstuvwxyz0123456789 .,/?[]{}\\|;:'\"<>`~!@#$%^&*()-_=+";
        outline_cb_->setChecked(false);
        debug_cb_->setChecked(true);
        //generateImage();
    }

    void CyroTextRzPage::onClick(ukive::View* v) {
        if (v == commit_button_) {
            auto text = text_view_->getText();
            if (!text.empty()) {
                text_ = text;
                if (grid_view_->getVisibility() == ukive::View::SHOW) {
                    generateGrid();
                } else {
                    generateImage();
                }
            }
        }
    }

    void CyroTextRzPage::onCheckChanged(ukive::View* cb, bool checked) {
        if (cb == debug_cb_) {
            if (checked) {
                generateGrid();
                grid_view_->setVisibility(ukive::View::SHOW);
                img_view_->setVisibility(ukive::View::HIDE);
            } else {
                generateImage();
                grid_view_->setVisibility(ukive::View::HIDE);
                img_view_->setVisibility(ukive::View::SHOW);
            }
        } else if (cb == outline_cb_) {
            only_outline_ = checked;
            if (grid_view_->getVisibility() == ukive::View::SHOW) {
                generateGrid();
            }
        }
    }

    void CyroTextRzPage::generateImage() {
        TextRasterizer rasterizer;
        if (!rasterizer.initialize()) {
            return;
        }

        int font_size = 10;
        auto c = getWindow()->getContext();
        int dpi_x = int(c.getScale() * c.getDefaultDpi());

        int width, height;
        std::vector<otf::OpenTypeFont::GlyphInfo> glyphs;
        if (!rasterizer.measureText(
            text_, dpi_x, font_size, &width, &height, &glyphs))
        {
            return;
        }

        Image image(width, height, Color4D(1, 1, 1, 1));
        ImageTarget target(&image);
        ImageRasterizer rz(&target);
        rz.setAntiAlias(true);

        rasterizer.drawText(rz, width, height, glyphs, dpi_x, font_size, false, 0, 0);

        auto image_bgra = toImageBGRAInt(image);
        auto img_data_ptr = reinterpret_cast<unsigned char*>(image_bgra.data);

        // Save to file
        /*ukive::Application::getImageComponent()->saveToPNGFile(
            width, height, img_data_ptr, u"test.png");*/

        img_.reset(ukive::ImageFrame::create(
            getWindow(), width, height,
            img_data_ptr, width * height * 4, width * 4));
        img_view_->setImage(img_);
    }

    void CyroTextRzPage::generateGrid() {
        TextRasterizer rasterizer;
        if (!rasterizer.initialize()) {
            return;
        }

        int font_size = 10;
        auto c = getWindow()->getContext();
        int dpi_x = int(c.getScale() * c.getDefaultDpi());

        int width, height;
        std::vector<otf::OpenTypeFont::GlyphInfo> glyphs;
        if (!rasterizer.measureText(
            text_, dpi_x, font_size, &width, &height, &glyphs))
        {
            return;
        }

        grid_view_->clearLines();
        grid_view_->clearPixels();
        grid_view_->clearPoints();

        GridTarget target(grid_view_);
        target.setBlendMode(Target::BM_ADD);
        if (!only_outline_) {
            ImageRasterizer rz(&target);
            rz.setAntiAlias(false);
            rasterizer.drawText(rz, width, height, glyphs, dpi_x, font_size, false, 0, 0);
        }
        {
            GridRasterizer rz(&target, grid_view_);
            rz.setAntiAlias(false);
            rasterizer.drawText(rz, width, height, glyphs, dpi_x, font_size, true, 0, 0);
        }
    }

}
