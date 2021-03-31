// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEST_UI_RAY_TRACER_CYRO_RAYTRACER_RZ_PAGE_H_
#define CYRONENO_TEST_UI_RAY_TRACER_CYRO_RAYTRACER_RZ_PAGE_H_

#include "ukive/graphics/images/image_frame.h"
#include "ukive/page/page.h"
#include "ukive/views/list/list_view.h"

#include "cyroneno/ray_tracer/ray_tracer.h"


namespace ukive {
    class ImageView;
    class ListView;
}

namespace cyro {

    class RTLightListSource;
    class RTPolygonListSource;

    class CyroRayTracerPage :
        public ukive::Page,
        public RayTracerListener,
        public ukive::ListItemSelectedListener {
    public:
        explicit CyroRayTracerPage(ukive::Window* w);

        ukive::View* onCreate(ukive::LayoutView* parent) override;
        void onInitialize() override;
        void onDestroy() override;

    protected:
        // cyro::RayTracerListener
        void onPixelData(const std::vector<RenderInfo>& row) override;
        void onRenderComplete() override;

        // ukive::ListItemSelectedListener
        void onItemPressed(ukive::ListView* lv, ukive::ListItem* item) override;

    private:
        ukive::ListView* light_list_ = nullptr;
        ukive::ListView* polygon_list_ = nullptr;

        RTLightListSource* light_source_ = nullptr;
        RTPolygonListSource* polygon_source_ = nullptr;

        ukive::ImageView* img_view_ = nullptr;

        std::shared_ptr<ukive::ImageFrame> bmp_;
        RayTracer ray_tracer_;
        std::unique_ptr<ImageBGRAInt> image_;
    };

}

#endif  // CYRONENO_TEST_UI_RAY_TRACER_CYRO_RAYTRACER_RZ_PAGE_H_