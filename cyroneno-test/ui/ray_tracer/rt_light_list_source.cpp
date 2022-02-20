// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/ui/ray_tracer/rt_light_list_source.h"

#include "utils/numbers.hpp"

#include "ukive/views/image_view.h"
#include "ukive/views/text_view.h"
#include "ukive/views/layout/layout_view.h"
#include "ukive/views/list/list_item_event_router.h"
#include "ukive/resources/layout_instantiator.h"
#include "ukive/elements/color_element.h"

#include "cyroneno-test/resources/necro_resources_id.h"


namespace cyro {

    ukive::ListItem* RTLightListSource::onCreateListItem(
        ukive::LayoutView* parent, ukive::ListItemEventRouter* router, size_t position)
    {
        auto view = ukive::LayoutInstantiator::from(
            parent->getContext(), parent, Res::Layout::rt_light_list_item_layout_xml);
        view->setOnInputEventDelegate(router);

        auto item = new RTLightItem(view);
        item->img_view = static_cast<ukive::ImageView*>(view->findView(Res::Id::iv_rt_light_item_img));
        item->name_view = static_cast<ukive::TextView*>(view->findView(Res::Id::tv_rt_light_item_name));

        return item;
    }

    void RTLightListSource::onSetListItemData(
        ukive::LayoutView* parent, ukive::ListItemEventRouter* router,
        ukive::ListItem* item)
    {
        auto& data = data_[item->data_pos];

        auto light_item = static_cast<RTLightItem*>(item);
        light_item->name_view->setText(data.name);

        auto b = light_item->item_view->getBackground();
        if (data.selected) {
            if (b) {
                static_cast<ukive::ColorElement*>(b)->setColor(ukive::Color::Blue200);
            } else {
                light_item->item_view->setBackground(new ukive::ColorElement(ukive::Color::Blue200));
            }
        } else {
            if (b) {
                static_cast<ukive::ColorElement*>(b)->setColor(ukive::Color::Transparent);
            }
        }
    }

    size_t RTLightListSource::onGetListDataCount(ukive::LayoutView* parent) const {
        return data_.size();
    }

    void RTLightListSource::addItem(int img_id, std::u16string name) {
        Data dat{ false, img_id, std::move(name) };
        data_.push_back(std::move(dat));
    }

    void RTLightListSource::selectItem(int sel_pos) {
        if (sel_pos < -1 ||
            sel_pos >= utl::num_cast<int>(data_.size()))
        {
            return;
        }

        if (sel_pos >= 0) {
            data_[sel_pos].selected = true;
        }
        if (sel_pos_ != -1 && sel_pos_ != sel_pos) {
            data_[sel_pos_].selected = false;
        }
        sel_pos_ = sel_pos;
    }

}
