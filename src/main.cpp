#include <stdlib.h> // for EXIT_SUCCESS
#include <memory>   // for allocator, __shared_ptr_access
#include <string>   // for string, operator+, basic_string, to_string, char_traits
#include <vector>   // for vector, __alloc_traits<>::value_type
#include <filesystem>
#include <set>
#include "Content.h"
#include "functions.h"
#include "Scroller.h"
#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"      // for Menu, Renderer, Horizontal, Vertical
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                 // for text, Element, operator|, window, flex, vbox
#include "ftxui/dom/table.hpp"
#include <DeltaSharing/DeltaSharingClient.h>
#include <DeltaSharing/functions.h>
#include <boost/optional.hpp>

using namespace ftxui;
using json = nlohmann::json;

int main(int argc, const char *argv[])
{
  std::vector<std::vector<std::string>> menu_entries = {{}, {}, {}, {}};

  int menu_selected_global = 0;
  Values v;
  v.filestats_title = "File statistics";
  v.schema_title = "Schema";
  v.num_items = 10;
  v.current_page = 0;
  v.show_selection = true;
  std::string path;
  Component i = Input(&path, "<path>");
  Node::Status s;
  std::shared_ptr<DeltaSharing::DeltaSharingClient> d;
  std::string delta_title("Delta sharing profile");
  auto input_header = Container::Horizontal(
      {
          Window(delta_title, Container::Horizontal({i})),
          Button("Load", [&]
                 {
                   if (std::filesystem::exists(path) && !std::filesystem::is_empty(path))
                   {
                     loadProfileFile(d, v, menu_entries, path);
                   }
                   else {
                    std::cerr << path << " could not be found..." << std::endl;
                   } }),
      });

  Table t = Table();
  Table statsTable = Table();

  MenuOption share_option = MenuOption::Vertical();
  MenuOption schema_option = MenuOption::Vertical();
  MenuOption table_option = MenuOption::Vertical();
  MenuOption file_option = MenuOption::Vertical();
  share_option.on_enter = [&]
  { shareOnEnter(d, v, menu_entries); };
  schema_option.on_enter = [&]
  { schemaOnEnter(d, v, menu_entries); };
  table_option.on_change = [&]
  { tableOnChange(d, v, menu_entries, t); };
  table_option.on_enter = [&]
  { tableOnEnter(d, v, menu_entries); };

  file_option.on_change = [&v]
  {
    fileOnChange(v);
  };

  std::string empty;
  std::string selection_title("Selection");
  auto menu_global = Container::Horizontal(
      {Window(selection_title,
              Container::Horizontal({
                  Window(empty, Collapsible("Shares", Menu(&menu_entries[0], &v.menu1, share_option), true)),
                  Window(empty, Collapsible("Schemas", Menu(&menu_entries[1], &v.menu2, schema_option), true)),
                  Window(empty, Collapsible("Tables", Menu(&menu_entries[2], &v.menu3, table_option), true)),
                  Window(empty, Collapsible("Files", FilesMenu(v, menu_entries, file_option), true)),
              }))},
      &menu_selected_global);

  t.SelectAll().Border(LIGHT);

  v.table = t.Render();
  v.statsTable = statsTable.Render();

  auto ww = Window2("Schema", v.table, v.pk);
  auto global = Container::Vertical({
      Collapsible("Profile", input_header, true),
      Collapsible("Selection", menu_global, v.show_selection),
      Collapsible(v.schema_title, Window(v.schema_title,ww), false),
      Collapsible(v.filestats_title, Window(v.filestats_title,Scroller(Renderer([&]
                                                  { return frame(vbox(v.statsTable)); }))),
                  false),
  });

  v.g = global;

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(global);
  return EXIT_SUCCESS;
}