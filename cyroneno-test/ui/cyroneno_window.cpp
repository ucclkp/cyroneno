// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/ui/cyroneno_window.h"

#include "ukive/elements/color_element.h"
#include "ukive/views/tab/tab_strip_view.h"
#include "ukive/views/tab/tab_view.h"

#include "cyroneno-test/ui/pages/cyro_pipeline_page.h"
#include "cyroneno-test/ui/pages/cyro_basic_rz_page.h"
#include "cyroneno-test/ui/pages/cyro_text_rz_page.h"
#include "cyroneno-test/ui/ray_tracer/cyro_raytracer_page.h"

#include "cyroneno-test/resources/necro_resources_id.h"


namespace cyro {

    CyronenoWindow::CyronenoWindow() {
    }

    void CyronenoWindow::onCreated() {
        Window::onCreated();

        showTitleBar();
        setBackgroundColor(ukive::Color::Transparent);
        setContentView(Res::Layout::cyroneno_layout_xml);

        getContentView()->setBackground(new ukive::ColorElement(ukive::Color::White));

        auto strip_view = findView<ukive::TabStripView>(Res::Id::tsv_cyroneno_strip);
        strip_view->setBackground(new ukive::ColorElement(ukive::Color::White));
        strip_view->setShadowRadius(getContext().dp2pxi(2));

        auto tab_view = findView<ukive::TabView>(Res::Id::tv_cyroneno_table);
        tab_view->setStripView(strip_view);

        tab_view->addPage(new CyroRayTracerPage(this), u"RayTracer");
        tab_view->addPage(new CyroPipelinePage(this), u"Pipeline");
        tab_view->addPage(new CyroBasicRzPage(this), u"BasicRZ");
        tab_view->addPage(new CyroTextRzPage(this), u"TextRZ");
    }

    void CyronenoWindow::onDestroy() {
        Window::onDestroy();
    }

}