#include "functions.h"
#include "Scroller.h"
#include <set>

Component Window(std::string &title, Component component)
{
  return Renderer(component, [component, title] { //
    return window(text(title), component->Render()) | flex;
  });
}

Component Window2(std::string title, Element &e, std::string &pk)
{
  return Scroller(Renderer([&e, title, &pk]
                           { return frame(vbox({e, text("Partitioned by:"), text(pk)})); }));
}

Component FilesMenu(Values &v, std::vector<std::vector<std::string>> &menu_entries, MenuOption &option)
{
  return Container::Vertical({Menu(&menu_entries[3], &v.menu4, option),
                              Renderer([]
                                       { return separator(); }),
                              Renderer([&v]
                                       { return text("Page " + std::to_string(v.current_page + 1) + " of " + std::to_string(v.pages + 1)); }),
                              Renderer([]
                                       { return separator(); }),
                              Container::Horizontal({
                                  Button("|<", [&]
                                         { backToBeginning(v, menu_entries); }),
                                  Button("<", [&]
                                         { backOnePage(v, menu_entries); }),
                                  Button(">", [&]
                                         { forwardOnePage(v, menu_entries); }),
                                  Button(">|", [&]
                                         { forwardToEnd(v, menu_entries); }),
                              })});
}

void loadProfileFile(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries, std::string &path)
{
  d = std::make_shared<DeltaSharing::DeltaSharingClient>(DeltaSharing::DeltaSharingClient(path, boost::none));
  menu_entries[0].clear();
  menu_entries[1].clear();
  menu_entries[2].clear();
  menu_entries[3].clear();
  auto x = d->ListShares(0, "");
  v.shares = x;
  menu_entries[0].clear();
  for (auto it = x->begin(); it != x->end(); it++)
  {
    menu_entries[0].push_back(it->name);
  }
  v.show_selection = true;
}

void backToBeginning(Values &v, std::vector<std::vector<std::string>> &menu_entries)
{
  v.current_page = 0;
  menu_entries[3].clear();
  for (auto it = v.file_array.at(v.current_page).begin(); it != v.file_array.at(v.current_page).end(); it++)
  {
    menu_entries[3].push_back(it->second);
  }
}

void backOnePage(Values &v, std::vector<std::vector<std::string>> &menu_entries)
{
  if (v.current_page > 0)
  {
    v.current_page--;
    menu_entries[3].clear();
    for (auto it = v.file_array.at(v.current_page).begin(); it != v.file_array.at(v.current_page).end(); it++)
    {
      menu_entries[3].push_back(it->second);
    }
  }
}

void forwardOnePage(Values &v, std::vector<std::vector<std::string>> &menu_entries)
{
  if (v.current_page < v.pages)
  {
    v.current_page++;
    menu_entries[3].clear();
    for (auto it = v.file_array.at(v.current_page).begin(); it != v.file_array.at(v.current_page).end(); it++)
    {
      menu_entries[3].push_back(it->second);
    }
  }
}

void forwardToEnd(Values &v, std::vector<std::vector<std::string>> &menu_entries)
{
  v.current_page = v.pages;
  menu_entries[3].clear();
  for (auto it = v.file_array.at(v.current_page).begin(); it != v.file_array.at(v.current_page).end(); it++)
  {
    menu_entries[3].push_back(it->second);
  }
}

void shareOnEnter(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries)
{
  auto x = v.shares->at(v.menu1);
  auto s = d->ListSchemas(x, 0, "");
  v.schemas = s;
  menu_entries[1].clear();
  for (auto it = s->begin(); it != s->end(); it++)
  {
    menu_entries[1].push_back(it->name);
  }
}

void schemaOnEnter(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries)
{
  auto share = v.shares->at(v.menu1);
  auto schema = v.schemas->at(v.menu2);
  auto tables = d->ListTables(schema, 0, "");
  v.tables = tables;
  menu_entries[2].clear();
  for (auto it = tables->begin(); it != tables->end(); it++)
  {
    menu_entries[2].push_back(it->name);
  }
}

void tableOnChange(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries, Table &t)
{
  auto table = v.tables->at(v.menu3);
  auto m = d->QueryTableMetadata(table);
  v.schema_title = "Schema for: " + table.name;
  json obj = m;
  v.pk = obj["partitionColumns"].dump();
  auto j = nlohmann::json::parse(m.schemaString);
  auto j2 = j["fields"];
  std::vector<std::vector<std::string>> table_data;
  table_data.clear();
  std::vector<std::string> header = {"Name", "Type", "Nullable", "Metadata"};
  table_data.push_back(header);
  for (auto it = j2.begin(); it != j2.end(); it++)
  {
    std::vector<std::string> row;
    auto p = (*it)["name"];
    row.push_back((*it)["name"]);
    row.push_back((*it)["type"]);
    row.push_back((*it)["nullable"] == true ? "Yes" : "No");
    row.push_back((*it)["metadata"].dump());
    table_data.push_back(row);
  }
  t = Table(table_data);
  t.SelectAll().Border(LIGHT);
  t.SelectColumn(0).Border(LIGHT);
  t.SelectRow(0).Decorate(bold);
  t.SelectRow(0).SeparatorVertical(LIGHT);
  t.SelectRow(0).Border(LIGHT);
  t.SelectColumn(2).DecorateCells(align_right);
  auto cent = t.SelectRows(1, -1);
  cent.SeparatorVertical(LIGHT);
  cent.DecorateCellsAlternateRow(color(Color::LightSkyBlue1), 3, 0);
  cent.DecorateCellsAlternateRow(color(Color::LightCoral), 3, 1);
  cent.DecorateCellsAlternateRow(color(Color::GrayLight), 3, 2);
  v.table = t.Render();
  v.g->Render();
};

void tableOnEnter(std::shared_ptr<DeltaSharing::DeltaSharingClient> &d, Values &v, std::vector<std::vector<std::string>> &menu_entries)
{
  auto table = v.tables->at(v.menu3);
  auto f = d->ListFilesInTable(table);
  v.files = f;
  auto len = f->size();
  v.pages = len / v.num_items;
  v.current_page = 0;
  v.file_array.clear();
  v.file_array.resize(v.pages + 1);
  for (int i = 0; i < len; i++)
  {
    auto p = i / v.num_items;
    v.file_array.at(p).push_back(std::make_pair(f->at(i), f->at(i).id.get_value_or("id not set") + " - (" + std::to_string((1.0 * f->at(i).size) / 1024 / 1024) + " mb)"));
  }
  menu_entries[3].clear();
  for (auto it = v.file_array.at(v.current_page).begin(); it != v.file_array.at(v.current_page).end(); it++)
  {
    menu_entries[3].push_back(it->second);
  }
};

void fileOnChange(Values &v){
auto selection = v.file_array.at(v.current_page);
    auto file = selection.at(v.menu4).first;
    if (file.stats.find_first_of("[{") != std::string::npos)
    {
      v.filestats_title = "File statistics for: "+file.id.get_value_or("not set");
      auto j = json::parse(file.stats);
      auto jMax = j["maxValues"];
      auto jMin = j["minValues"];
      auto nullValues = j["nullCount"];
      std::map<std::string, std::string> m[3];
      std::set<std::string> keys;
      for (auto it = jMax.begin(); it != jMax.end(); it++)
      {

        m[0][it.key()] = to_string(it.value());
        keys.insert(it.key());
      }
      for (auto it = jMin.begin(); it != jMin.end(); it++)
      {
        m[1][it.key()] = to_string(it.value());
        keys.insert(it.key());
      }
      for (auto it = nullValues.begin(); it != nullValues.end(); it++)
      {
        m[2][it.key()] = to_string(it.value());
        keys.insert(it.key());
      }
      std::vector<std::vector<std::string>> stats_table;
      std::vector<std::string> header;
      header.push_back("File: ");
      header.push_back(file.id.get_value_or("unknown"));
      header.push_back("Size: ");
      header.push_back(std::to_string(file.size) + " bytes, " +  to_string(j["numRecords"]) + " rows");
      stats_table.push_back(header);
      header.clear();
      header.push_back("Field");
      header.push_back("Min Value");
      header.push_back("Max Value");
      header.push_back("Null Count");
      stats_table.push_back(header);


      for (auto it = keys.begin(); it != keys.end(); it++)
      {
        std::vector<std::string> row;
        auto maxV = m[0][*it];
        auto minV = m[1][*it];
        auto nullV = m[2][*it];
        row.push_back(*it);
        row.push_back(minV.empty() == false ? minV : "na");
        row.push_back(maxV.empty() == false ? maxV : "na");
        row.push_back(nullV.empty() == false ? nullV : "na");
        stats_table.push_back(row);
      }
      auto tv = Table(stats_table);
      tv.SelectAll().Border(LIGHT);
      tv.SelectRow(0).Decorate(bold);
      tv.SelectRow(0).SeparatorVertical(LIGHT);
      tv.SelectRow(0).Border(ROUNDED);
      tv.SelectRow(1).Decorate(bold);
      tv.SelectRow(1).SeparatorVertical(LIGHT);
      tv.SelectRow(1).Border(LIGHT);
      tv.SelectColumn(1).DecorateCells(align_right);
      tv.SelectColumn(2).DecorateCells(align_right);
       tv.SelectColumn(3).DecorateCells(align_right);
      auto cent = tv.SelectRows(2, -1);
      cent.SeparatorVertical(LIGHT);
      cent.DecorateCellsAlternateRow(color(Color::LightSkyBlue1), 3, 1);
      cent.DecorateCellsAlternateRow(color(Color::LightCoral), 3, 2);
      cent.DecorateCellsAlternateRow(color(Color::GrayLight), 3, 3);
      v.statsTable = tv.Render();
    }
    else {
      auto tv = Table();
      v.statsTable = tv.Render();
    }
};
