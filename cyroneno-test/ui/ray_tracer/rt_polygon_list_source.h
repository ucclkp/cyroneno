// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEST_UI_RAY_TRACER_RT_POLYGON_LIST_SOURCE_H_
#define CYRONENO_TEST_UI_RAY_TRACER_RT_POLYGON_LIST_SOURCE_H_

#include <vector>

#include "ukive/views/list/list_item.h"
#include "ukive/views/list/list_source.h"


namespace ukive {
    class ImageView;
    class TextView;
}

namespace cyro {

    class RTPolygonListSource : public ukive::ListSource {
    public:
        class RTPolygonItem : public ukive::ListItem {
        public:
            explicit RTPolygonItem(ukive::View* v)
                : ListItem(v) {}

            ukive::ImageView* img_view = nullptr;
            ukive::TextView* name_view = nullptr;
        };

        ukive::ListItem* onListCreateItem(ukive::LayoutView* parent, int position) override;
        void onListSetItemData(ukive::ListItem* item, int position) override;
        int onListGetDataCount() override;

        void addItem(int img_id, std::u16string name);
        void selectItem(int sel_pos);

    private:
        struct Data {
            bool selected;
            int img_id;
            std::u16string name;
        };

        int sel_pos_ = -1;
        std::vector<Data> data_;
    };

}

#endif  // CYRONENO_TEST_UI_RAY_TRACER_RT_POLYGON_LIST_SOURCE_H_