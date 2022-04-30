// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEST_UI_PAGES_CYRO_TEXT_RZ_PAGE_H_
#define CYRONENO_TEST_UI_PAGES_CYRO_TEXT_RZ_PAGE_H_

#include "ukive/views/check_listener.h"
#include "ukive/views/click_listener.h"
#include "ukive/graphics/images/image_frame.h"
#include "ukive/page/page.h"


namespace ukive {
    class Button;
    class CheckBox;
    class TextView;
    class GridView;
    class ImageView;
}

namespace cyro {

    class CyroTextRzPage :
        public ukive::Page,
        public ukive::OnClickListener,
        public ukive::OnCheckListener
    {
    public:
        explicit CyroTextRzPage(ukive::Window* w);

        // ukive::Page
        ukive::View* onCreate(ukive::LayoutView* parent) override;
        void onInitialize() override;
        void onShow(bool show) override;

        // ukive::OnClickListener
        void onClick(ukive::View* v) override;

        // ukive::OnCheckListener
        void onCheckChanged(ukive::View* cb, bool checked) override;

    private:
        void generateImage();
        void generateGrid();

        std::u16string text_;
        bool only_outline_ = false;

        ukive::TextView* text_view_ = nullptr;
        ukive::Button* commit_button_ = nullptr;
        ukive::CheckBox* debug_cb_ = nullptr;
        ukive::CheckBox* outline_cb_ = nullptr;
        ukive::GridView* grid_view_ = nullptr;
        ukive::ImageView* img_view_ = nullptr;
        ukive::GPtr<ukive::ImageFrame> img_;
    };

}

#endif  // CYRONENO_TEST_UI_PAGES_CYRO_TEXT_RZ_PAGE_H_