// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/ui/pages/cyro_basic_rz_page.h"

#include "ukive/elements/color_element.h"
#include "ukive/resources/layout_instantiator.h"
#include "ukive/views/image_view.h"
#include "ukive/views/layout/layout_view.h"
#include "ukive/window/window.h"
#include "ukive/window/win/window_impl_win.h"

#include "cyroneno/rasterizer/image_rasterizer.h"
#include "cyroneno/rasterizer/image_target.h"

#include "cyroneno-test/test/line_rz_unit_test.h"
#include "cyroneno-test/test/ellipse_rz_unit_test.h"
#include "cyroneno-test/test/bezier_rz_unit_test.h"

#include "cyroneno-test/resources/necro_resources_id.h"

#include <Windows.h>


namespace {
    constexpr auto IMAGE_WIDTH = 800;
    constexpr auto IMAGE_HEIGHT = 800;

    struct Data {
        uint32_t x;
        uint32_t y;
        ukive::Color color;
    };

}

namespace cyro {

    CyroBasicRzPage::CyroBasicRzPage(ukive::Window* w)
        : Page(w) {}

    ukive::View* CyroBasicRzPage::onCreate(ukive::LayoutView* parent) {
        auto v = ukive::LayoutInstantiator::from(
            parent->getContext(), parent, Res::Layout::cyro_basic_rz_page_layout_xml);

        v->setBackground(new ukive::ColorElement(ukive::Color(0.5f, 0.5f, 0.5f)));

        img_view_ = findView<ukive::ImageView>(v, Res::Id::iv_cyro_basic_rz_page_img);
        return v;
    }

    void CyroBasicRzPage::onInitialize() {
        testExamples();
        img_view_->setImage(img_);

        HWND dbg_win = ::FindWindowW(nullptr, L"Graphic Grid");
        if (dbg_win) {
            Data data;
            data.x = 10;
            data.y = 10;
            data.color = ukive::Color::Yellow500;

            COPYDATASTRUCT cds;
            cds.dwData = 0;
            cds.cbData = sizeof(Data);
            cds.lpData = &data;

            ::SendMessageW(
                dbg_win, WM_COPYDATA,
                WPARAM(static_cast<ukive::win::WindowImplWin*>(getWindow()->getImpl())->getHandle()),
                LPARAM(&cds));
        }
    }

    void CyroBasicRzPage::testExamples() {
        Image image(IMAGE_WIDTH, IMAGE_HEIGHT, Color4D(1, 1, 1, 1));
        ImageTarget target(&image);
        ImageRasterizer rasterizer(&target);

        test::TEST_LINE_AA_FLOAT(rasterizer);
        //test::TEST_ELLIPSES(rasterizer);
        //test::TEST_QUAD_BEZIERS(rasterizer);

        /*rasterizer.drawCubicBezier(
            cyro::Point2(200, 200),
            cyro::Point2(600, 900),
            cyro::Point2(10, -50),
            cyro::Point2(400, 400),
            cyro::Color(0, 0, 0, 0.5f));*/

            //test::TEST_CUBIC_BEZIERS(rasterizer);

            /*rasterizer.drawTriangle(
                {50, 50}, {70, 200}, {300, 50},
                cyro::Color(1, 0, 0, 1),
                cyro::Color(0, 1, 0, 1),
                cyro::Color(0, 0, 1, 1));*/
        auto image_bgra = toImageBGRAInt(image);
        auto img_data_ptr = reinterpret_cast<unsigned char*>(image_bgra.data);

        // Save to file
        /*ukive::Application::getWICManager()->saveToPngFile(
            IMAGE_WIDTH, IMAGE_HEIGHT, img_data_ptr, L"test.png");*/

        auto bd = ukive::ByteData::refPtr(img_data_ptr, IMAGE_WIDTH * IMAGE_HEIGHT * 4);
        img_ = ukive::ImageFrame::create(
            getWindow()->getCanvas(), IMAGE_WIDTH, IMAGE_HEIGHT, bd, IMAGE_WIDTH * 4);
    }

}
