// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEST_UI_PAGES_CYRO_BASIC_RZ_PAGE_H_
#define CYRONENO_TEST_UI_PAGES_CYRO_BASIC_RZ_PAGE_H_

#include "ukive/graphics/images/image_frame.h"
#include "ukive/page/page.h"


namespace ukive {
    class ImageView;
}

namespace cyro {

    class CyroBasicRzPage : public ukive::Page {
    public:
        explicit CyroBasicRzPage(ukive::Window* w);

        ukive::View* onCreate(ukive::LayoutView* parent) override;
        void onInitialize() override;

    private:
        void testExamples();

        ukive::ImageView* img_view_ = nullptr;
        std::shared_ptr<ukive::ImageFrame> img_;
    };

}

#endif  // CYRONENO_TEST_UI_PAGES_CYRO_BASIC_RZ_PAGE_H_