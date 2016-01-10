
#include "Tile.h"
#include <SFML\Graphics.hpp>
#include <cassert>
#include <iostream>

void Tile::draw(Position p, sf::RenderWindow& rw) {
  std::cout << p.x << ','<< p.y << ' ' << rw.getSize().x << ',' << rw.getSize().y << std::endl;
  assert(p.x >= 0 && p.x <= rw.getSize().x);
  assert(p.y >= 0 && p.y <= rw.getSize().y);
  
  _base.setPosition(p.x, p.y);
  rw.draw(_base);
}