// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/ui/pages/cyro_pipeline_page.h"

#include "ukive/elements/color_element.h"
#include "ukive/resources/layout_instantiator.h"
#include "ukive/views/image_view.h"
#include "ukive/views/layout/layout_view.h"

#include "cyroneno/pipeline/pipeline.h"

#include "cyroneno-test/resources/necro_resources_id.h"


namespace {
    constexpr auto IMAGE_WIDTH = 800;
    constexpr auto IMAGE_HEIGHT = 800;
}

namespace cyro {

    CyroPipelinePage::CyroPipelinePage(ukive::Window* w)
        : Page(w) {}

    ukive::View* CyroPipelinePage::onCreate(ukive::LayoutView* parent) {
        auto v = ukive::LayoutInstantiator::from(
            parent->getContext(), parent, Res::Layout::cyro_pipeline_page_layout_xml);

        v->setBackground(new ukive::ColorElement(ukive::Color(0.5f, 0.5f, 0.5f)));

        img_view_ = findViewById<ukive::ImageView>(v, Res::Id::iv_cyro_pipeline_page_img);
        return v;
    }

    void CyroPipelinePage::onInitialize() {
        testExamples();
        img_view_->setImage(img_);
    }

    void CyroPipelinePage::testExamples() {
        Image image(IMAGE_WIDTH, IMAGE_HEIGHT, Color4D(1, 1, 1, 1));
        ImageTarget target(&image);
        Pipeline pipeline(&target);
        pipeline.launch(IMAGE_WIDTH, IMAGE_HEIGHT);

        auto image_bgra = toImageBGRAInt(image);
        auto img_data_ptr = reinterpret_cast<unsigned char*>(image_bgra.data);

        // Save to file
        /*ukive::Application::getWICManager()->saveToPngFile(
            IMAGE_WIDTH, IMAGE_HEIGHT, img_data_ptr, L"test.png");*/

        img_.reset(ukive::ImageFrame::create(
            getWindow(), IMAGE_WIDTH, IMAGE_HEIGHT,
            img_data_ptr, IMAGE_WIDTH * IMAGE_HEIGHT * 4, IMAGE_WIDTH * 4));
    }

}
