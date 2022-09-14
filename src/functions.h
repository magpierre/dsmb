#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Menu, Renderer, Horizontal, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, window, flex, vbox
#include <DeltaSharing/DeltaSharingClient.h>
#include <DeltaSharing/functions.h>
#include "ftxui/dom/table.hpp"

using namespace ftxui;

struct Values {
    public:
    int menu1;
    int menu2;
    int menu3;
    int menu4;
    int num_items;
    int pages;
    int current_page;
    bool show_selection;
    std::shared_ptr<std::vector<DeltaSharing::DeltaSharingProtocol::Share>> shares;
    std::shared_ptr<std::vector<DeltaSharing::DeltaSharingProtocol::Schema>> schemas;
    std::shared_ptr<std::vector<DeltaSharing::DeltaSharingProtocol::Table>> tables;
    std::shared_ptr<std::vector<DeltaSharing::DeltaSharingProtocol::File>> files;
    std::vector<std::vector<std::pair<DeltaSharing::DeltaSharingProtocol::File,std::string>>> file_array;
    Element table;
    Element statsTable;
    std::string pk;
    std::string statsPk;
    Component g;
    std::string filestats_title;
    std::string schema_title;
};

Component Window(std::string &title, Component component);
Component Window2(std::string title, Element &e, std::string &pk);
Component FilesMenu(Values &v,std::vector<std::vector<std::string>> &menu_entries, MenuOption &option);
/* event handlers */
void loadProfileFile(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v,std::vector<std::vector<std::string>> &menu_entries, std::string &path);
void backToBeginning(Values &v, std::vector<std::vector<std::string>> &menu_entries);
void backOnePage(Values &v, std::vector<std::vector<std::string>> &menu_entries);
void forwardOnePage(Values &v, std::vector<std::vector<std::string>> &menu_entries);
void forwardToEnd(Values &v, std::vector<std::vector<std::string>> &menu_entries);
void shareOnEnter(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v,std::vector<std::vector<std::string>> &menu_entries);
void schemaOnEnter(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries);
void tableOnChange(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries, Table &t);
void tableOnEnter(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries);
void fileOnChange(Values &v);    
#endif