#pragma once

#include "Position.h"
#include <SFML\Graphics.hpp>

// Manage loaded image assets
class Tile {
private:
  sf::Sprite _base;

public:
  Tile::Tile(const sf::Texture& t) {
    _base.setTexture(t, true);
  }

  Tile::Tile(sf::Texture* t) {
    _base.setTexture(*t, true);
  }

  ~Tile() {};

  Tile(Tile*) = delete;
  Tile(const Tile& other) = delete;
  Tile(Tile&& other) = delete;

  void draw(Position p, sf::RenderWindow& rw);
};