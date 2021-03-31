// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "utils/log.h"
#include "utils/platform_utils.h"

#include "ukive/app/application.h"

#include "cyroneno-test/ui/cyroneno_window.h"


GUI_MAIN()
{
    utl::Log::Params log_params;
    log_params.file_name = u"cyroneno-debug.log";
    log_params.short_file_name = true;
    log_params.target = utl::Log::OutputTarget::DEBUGGER | utl::Log::OutputTarget::FILE;
    utl::InitLogging(log_params);

    LOG(Log::INFO) << "=========== cyroneno-test start.";

    ukive::Application::Options options;
    options.is_auto_dpi_scale = false;
    auto app = std::make_shared<ukive::Application>(options);

    auto cyro_window = std::make_shared<cyro::CyronenoWindow>();
    cyro_window->init(ukive::Window::InitParams());
    cyro_window->setTitle(u"Cyroneno");
    cyro_window->setWidth(ukive::Application::dp2pxi(850));
    cyro_window->setHeight(ukive::Application::dp2pxi(700));
    cyro_window->center();
    cyro_window->show();

    app->run();

    LOG(Log::INFO) << "cyroneno-test exit.\n";

    utl::UninitLogging();

    return 0;
}