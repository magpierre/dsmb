#include "Content.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Menu, Renderer, Horizontal, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, window, flex, vbox
using namespace ftxui;

Content::Content(Values &v, int &menu_selected_global) : menu_selected_global(menu_selected_global) {

 this->x = Renderer([&] {
    int g = this->menu_selected_global;
    return ftxui::window(text("Content"),  //
                  vbox({
                      text("menu_selected_global = " + std::to_string(g)),
                      text("menu_selected[0]     = " +
                           std::to_string(v.menu1)),
                      text("menu_selected[1]     = " +
                           std::to_string(v.menu2)),
                      text("menu_selected[2]     = " +
                           std::to_string(v.menu3)),
                  })) |
           flex;
  });
};

Content::~Content() {

};

 std::shared_ptr<Component> Content::GetWindow() {
    return std::make_shared<Component>(this->x);
};