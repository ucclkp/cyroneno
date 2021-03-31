// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEST_UI_CYRONENO_WINDOW_H_
#define CYRONENO_TEST_UI_CYRONENO_WINDOW_H_

#include "ukive/window/window.h"


namespace cyro {

    class CyronenoWindow : public ukive::Window {
    public:
        CyronenoWindow();

        void onCreated() override;
        void onDestroy() override;
    };

}

#endif  // CYRONENO_TEST_UI_CYRONENO_WINDOW_H_