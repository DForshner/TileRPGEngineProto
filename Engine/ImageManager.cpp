
#include "ImageManager.h"
#include <rapidxml\rapidxml.hpp>
#include <fstream>
#include <istream>
#include <string>
#include <exception>

using namespace rapidxml;

void ImageManager::addImage(std::unique_ptr<sf::Texture> texture, int id) {
  _textures.emplace_back(std::move(texture));
  _idToIdx[id] = _textures.size() - 1;
}

sf::Texture* ImageManager::getTexture(int id) {
  return _textures[_idToIdx[id]].get();
}

std::string buildString(std::ifstream* fin) {
  std::string contents;
  std::string line;
  while (std::getline(*fin, line)) {
    contents += line;
  }
  return contents;
}

constexpr const char * TILESET_NODE = "imagefile";
constexpr const char * TILE_NODE = "tile";

struct TileToLoad {
  int x;
  int y;
  int frames;
  int id;
};

struct TileSetToLoad {
  std::string path;
  std::vector<TileToLoad> tiles;

  TileSetToLoad(std::string path) : path(path) {}
};

void ImageManager::loadTileset(const std::string& fileName) {
  std::ifstream fin("./Assets/" + fileName);
  if (!fin) {
    throw std::exception(std::string("Could not open tile set: " + fileName).c_str());
  }

  auto contents = buildString(&fin);
  std::vector<char> xmlData(contents.cbegin(), contents.cend());
  xmlData.emplace_back('\0');

  rapidxml::xml_document<> doc;
  doc.parse<parse_no_data_nodes>(&xmlData[0]);

  // Root node
  xml_node<>* root = doc.first_node();

  // Parse tile sets
  std::vector<TileSetToLoad> tilesToLoad;
  for (auto imageNode = root->first_node(TILESET_NODE); imageNode; imageNode = imageNode->next_sibling()) {
    std::string path = imageNode->first_attribute("path")->value();
    TileSetToLoad tileSet(path);

    // Parse tile nodes
    std::vector<xml_node<>*> tiles;
    for (auto tileNode = imageNode->first_node(TILE_NODE); tileNode; tileNode = tileNode->next_sibling()) {
      int x{ atoi(tileNode->first_attribute("x")->value()) };
      int y{ atoi(tileNode->first_attribute("y")->value()) };
      int frames{ atoi(tileNode->first_attribute("frames")->value()) };
      int id{ atoi(tileNode->first_attribute("id")->value()) };
      TileToLoad tile{ x, y, frames, id };
      tileSet.tiles.emplace_back(tile);
    }

    tilesToLoad.emplace_back(tileSet);
  }

  for (const auto& set : tilesToLoad) {
    sf::Image tileSet;
    if (!tileSet.loadFromFile("./Assets/" + set.path)) {
      throw std::exception(std::string("Could not load image file: " + set.path).c_str());
    }

    for (const auto& tile : set.tiles) {
      sf::Image tileImage;
      tileImage.create(32, 32);
      tileImage.copy(tileSet, 0, 0, { tile.x * 32, tile.y * 32, tile.frames * 32, 32 }, true);

      std::unique_ptr<sf::Texture> texture(new sf::Texture());
      texture->loadFromImage(tileImage);
      addImage(std::move(texture), tile.id);
    }
  }
}