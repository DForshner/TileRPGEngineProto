
#include "Level.h"
#include "ImageManager.h"
#include <rapidxml\rapidxml.hpp>
#include <fstream>
#include <istream>
#include <string>
#include <exception>

using namespace rapidxml;

// TODO: this is duplicated in image manager
std::string buildString2(std::ifstream* fin) {
  std::string contents;
  std::string line;
  while (std::getline(*fin, line)) {
    contents += line;
  }
  return contents;
}

struct TileToLoad {
  int id;
  int x;
  int y;
  bool walkable;
};

struct MapToLoad {
  int width;
  int height;
  std::vector<std::string> tilesets;
  std::vector<TileToLoad> tiles;
};

constexpr const char * TILESET_NODE = "tileset";
constexpr const char * TILE_NODE = "tile";

void Level::loadLevel(const std::string& fileName, ImageManager& imageManager) {
  std::ifstream fin("./Assets/" + fileName);
  if (!fin) {
    throw std::exception(std::string("Could not open tile set: " + fileName).c_str());
  }

  auto contents = buildString2(&fin);
  std::vector<char> xmlData(contents.cbegin(), contents.cend());
  xmlData.emplace_back('\0');

  rapidxml::xml_document<> doc;
  doc.parse<parse_no_data_nodes>(&xmlData[0]);

  // TODO: Above is duplicated in image manager

  xml_node<>* root = doc.first_node();

  MapToLoad toLoad;

  //Get level attributes
  toLoad.width = atoi(root->first_attribute("width")->value());
  toLoad.height = atoi(root->first_attribute("height")->value());

  auto tileSetNode = root->first_node(TILESET_NODE);

  // Parse tileset node
  //for (auto tileSetNode = root->first_node(TILESET_NODE); tileSetNode; tileSetNode = tileSetNode->next_sibling()) {
    std::string path{ tileSetNode->first_attribute("path")->value() };
    toLoad.tilesets.emplace_back(path);
  //}

  // Parse tile location nodes
  for (auto tileNode = root->first_node(TILE_NODE); tileNode; tileNode = tileNode->next_sibling()) {
    std::string walkString = tileNode->first_attribute("walkable")->value();
    bool walkable = (walkString == "true") ? true : false;

    TileToLoad tile {
      atoi(tileNode->first_attribute("baseid")->value()),
      atoi(tileNode->first_attribute("x")->value()),
      atoi(tileNode->first_attribute("y")->value()),
      walkable
    };
    toLoad.tiles.emplace_back(tile);
  }

  // Resize level
  this->_width = toLoad.width;
  this->_height = toLoad.height;
  SetDimensions(toLoad.width, toLoad.height);

  // Load tilesets
  for (const auto& tileSetName : toLoad.tilesets) {
    imageManager.loadTileset(tileSetName);
  }

  // Setup map tiles
  for (const auto& loc : toLoad.tiles) {
    std::unique_ptr<Tile> tile (new Tile(imageManager.getTexture(loc.id)));
    addTile(loc.x, loc.y, std::move(tile));
  }
}