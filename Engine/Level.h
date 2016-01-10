#pragma once

#include "Tile.h"
#include "ImageManager.h"
#include <vector>
#include <cassert>
#include <memory>
#include <SFML\Graphics.hpp>

class Level {
private:
  std::vector<std::vector<std::unique_ptr<Tile>>> _map;
  int _width; // # tiles
  int _height; // # tiles
  sf::Texture _default;

  void SetDimensions(int w, int h) {
    _map.resize(w);
    for (auto& y : _map) {
      for (int i = 0; i < h; i++) {
        y.emplace_back(std::make_unique<Tile>(_default));
      }
    }
  }

public:
  Level(int w, int h) : 
    _width(w), 
    _height(h) {

    sf::Image img;
    img.create(32, 32, sf::Color::Blue);
    _default.loadFromImage(img);

    SetDimensions(w, h);
  }

  ~Level() {}

  void addTile(int x, int y, std::unique_ptr<Tile> tile) {
    assert(x < _map.size() && y < _map[0].size());
    _map[x][y] = std::move(tile);
  }

  Tile* getTile(int x, int y) {
    assert(x < _map.size() && y < _map[0].size());
    return _map[x][y].get();
  }

  void Level::loadLevel(const std::string& filename, ImageManager& imageManager);

  int getWidth() { return _height; }
  int getHeight() { return _width; }
};