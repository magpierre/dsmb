#ifndef __CONTENT_H__
#define __CONTENT_H__

#include <stdlib.h>  // for EXIT_SUCCESS
#include <memory>    // for allocator, __shared_ptr_access
#include <string>  // for string, operator+, basic_string, to_string, char_traits
#include <vector>  // for vector, __alloc_traits<>::value_type

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Menu, Renderer, Horizontal, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, window, flex, vbox
#include "functions.h"
#include <DeltaSharing/DeltaSharingClient.h>

using namespace ftxui;



class Content {
    public:
    Content(Values &v, int &menu_selected_global);
    ~Content();
    std::shared_ptr<Component> GetWindow();

    private:
    Component x;
    std::vector<std::vector<std::string>> menu_entries;
    int &menu_selected_global;
    Component menu_global;
    int menu_selected[3];
};

#endif