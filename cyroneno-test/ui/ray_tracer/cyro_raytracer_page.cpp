// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/ui/ray_tracer/cyro_raytracer_page.h"

#include "ukive/elements/color_element.h"
#include "ukive/resources/layout_instantiator.h"
#include "ukive/views/image_view.h"
#include "ukive/views/text_view.h"
#include "ukive/views/list/linear_list_layouter.h"

#include "cyroneno-test/ui/ray_tracer/rt_light_list_source.h"
#include "cyroneno-test/ui/ray_tracer/rt_polygon_list_source.h"

#include "cyroneno-test/resources/necro_resources_id.h"


namespace {
    constexpr auto IMAGE_WIDTH = 800;
    constexpr auto IMAGE_HEIGHT = 800;
}

namespace cyro {

    CyroRayTracerPage::CyroRayTracerPage(ukive::Window* w)
        : Page(w) {}

    ukive::View* CyroRayTracerPage::onCreate(ukive::LayoutView* parent) {
        auto v = ukive::LayoutInstantiator::from(
            parent->getContext(), parent, Res::Layout::cyro_raytracer_page_layout_xml);

        v->setBackground(new ukive::ColorElement(ukive::Color(0.5f, 0.5f, 0.5f)));

        img_view_ = findView<ukive::ImageView>(v, Res::Id::iv_cyro_raytracer_page_img);
        auto left_panel = findView<ukive::View>(v, Res::Id::ll_cyro_raytracer_page_left_panel);
        left_panel->setBackground(new ukive::ColorElement(ukive::Color::Grey100));

        light_source_ = new RTLightListSource();

        light_list_ = findView<ukive::ListView>(v, Res::Id::lv_cyro_raytracer_page_lights);
        light_list_->setSource(light_source_);
        light_list_->setLayouter(new ukive::LinearListLayouter());
        light_list_->setBackground(new ukive::ColorElement(ukive::Color::Grey300));
        light_list_->setItemEventRouter(new ukive::ListItemEventRouter(this));

        polygon_source_ = new RTPolygonListSource();

        polygon_list_ = findView<ukive::ListView>(v, Res::Id::lv_cyro_raytracer_page_polygons);
        polygon_list_->setSource(polygon_source_);
        polygon_list_->setLayouter(new ukive::LinearListLayouter());
        polygon_list_->setBackground(new ukive::ColorElement(ukive::Color::Grey300));
        polygon_list_->setItemEventRouter(new ukive::ListItemEventRouter(this));

        return v;
    }

    void CyroRayTracerPage::onInitialize() {
        image_ = std::make_unique<ImageBGRAInt>(IMAGE_WIDTH, IMAGE_HEIGHT, BGRAInt());
        ray_tracer_.rayTracerAsync(ProjectionType::ORTHO, IMAGE_WIDTH, IMAGE_HEIGHT, this);

        light_source_->addItem(0, u"Light0");
        light_source_->addItem(0, u"Light1");
        light_source_->notifyDataChanged();

        polygon_source_->addItem(0, u"Sphere0");
        polygon_source_->addItem(0, u"Sphere1");
        polygon_source_->addItem(0, u"Plane0");
        polygon_source_->notifyDataChanged();
    }

    void CyroRayTracerPage::onDestroy() {
        ray_tracer_.stop();
    }

    void CyroRayTracerPage::onPixelData(const std::vector<RenderInfo>& row) {
        for (const auto& info : row) {
            auto color = info.color;
            image_->setColor(info.x, info.y, BGRAInt(color.clamp(0, 1)));
        }

        auto img_data_ptr = reinterpret_cast<uint8_t*>(image_->data);
        bmp_.reset(ukive::ImageFrame::create(
            getWindow(), IMAGE_WIDTH, IMAGE_HEIGHT,
            img_data_ptr, IMAGE_WIDTH * IMAGE_HEIGHT * 4, IMAGE_WIDTH * 4));
        img_view_->setImage(bmp_);
    }

    void CyroRayTracerPage::onRenderComplete() {
        // Save to file
        /*ukive::Application::getWICManager()->saveToPngFile(
            IMAGE_WIDTH, IMAGE_HEIGHT, img_data_ptr, L"test.png");*/
    }

    void CyroRayTracerPage::onItemPressed(
        ukive::ListView* list_view,
        ukive::ListItem* item, ukive::View* v)
    {
        if (polygon_list_ == list_view) {
            polygon_source_->selectItem(int(item->data_pos));
            polygon_source_->notifyDataChanged();

            light_source_->selectItem(-1);
            light_source_->notifyDataChanged();
        } else if (light_list_ == list_view) {
            light_source_->selectItem(int(item->data_pos));
            light_source_->notifyDataChanged();

            polygon_source_->selectItem(-1);
            polygon_source_->notifyDataChanged();
        }
    }

}
