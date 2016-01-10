
#include "MapRenderer.h"
#include <string>
#include <exception>
#include <cassert>

MapRenderer::MapRenderer(sf::Texture *texture, TileProvider provider, float tileSize, int layers) {
  if (provider == 0 || layers <= 0) throw "hey";
  this->_provider = provider;
  this->_texture = texture;
  this->_tileSize = tileSize;
  this->_layers = layers;
}

void MapRenderer::refreshLocal(int left, int top, int right, int bottom) {
  for (int y = top; y < bottom; y++) {
    for (int x = left; x < right; x++) {
      refresh(x + _offset.x, y + _offset.y);
    }
  }
}

void MapRenderer::setSize(sf::Vector2f v) {
  int w = static_cast<int>(v.x / _tileSize) + 2;
  int h = static_cast<int>(v.y / _tileSize) + 2;
  if (w == _width && h == _height) { return; }

  _width = w;
  _height = h;

  // Allocate quad per tile per layer
  auto size = _width * _height * 4 * _layers;
  if (_vertices.size() < size) {
    _vertices.resize(size);
  }
  refresh();
}

sf::Vector2i MapRenderer::getTile(sf::Vector2f pos) const {
  int x = static_cast<int>(pos.x / _tileSize);
  int y = static_cast<int>(pos.y / _tileSize);

  // Why?
  if (pos.x < 0) { x--; }
  if (pos.y < 0) { y--; }

  return sf::Vector2i(x, y);
}

void MapRenderer::setCorner(sf::Vector2f v) {
  auto tile = getTile(v);
  auto dif = tile - _offset;

  if (dif.x == 0 && dif.y == 0) {
    return; // Player did not move
  }

  _offset = tile;

  if ((abs(dif.x) > _width / 4) 
    || (abs(dif.y) > _height / 4)) {
    refresh(); // Difference is too big so just refresh everything
    return;
  }

  // Refresh only tiles that have appeared since last update

  if (dif.x > 0) {
    // Player moved right so refresh column on right side of screen
    refreshLocal(_width - dif.x, 0, _width, _height);
  } else {
    // Player moved left so refresh column on left side of screen
    refreshLocal(0, 0, -dif.x, _height);
  }

  if (dif.y > 0) {
    // Player moved down so refresh column on bottom of screen
    refreshLocal(0, _height - dif.y, _width, _height);
  } else {
    // Player moved up so refresh column on top of screen
    refreshLocal(0, 0, _width, -dif.y);
  }
}

// Inserts color and texture data into vertex array for a single tile (quad)
void MapRenderer::draw(int index, sf::FloatRect display, sf::IntRect texture, sf::Color color) {

  // Perf: use pointers to avoid array bound checks (optimization)
  auto ptr = &_vertices[index];

  // top left corner
  ptr->position.x = display.left;
  ptr->position.y = display.top;
  ptr->texCoords.x = texture.left;
  ptr->texCoords.y = texture.top;
  ptr->color = color;
  ptr++;

  // top right corner
  ptr->position.x = display.left + display.width;
  ptr->position.y = display.top;
  ptr->texCoords.x = texture.left + texture.width;
  ptr->texCoords.y = texture.top;
  ptr->color = color;
  ptr++;

  // bottom right corner
  ptr->position.x = display.left + display.width;
  ptr->position.y = display.top + display.height;
  ptr->texCoords.x = texture.left + texture.width;
  ptr->texCoords.y = texture.top + texture.height;
  ptr->color = color;
  ptr++;

  // bottom left corner
  ptr->position.x = display.left;
  ptr->position.y = display.top + display.height;
  ptr->texCoords.x = texture.left;
  ptr->texCoords.y = texture.top + texture.height;
  ptr->color = color;
}
  
// Redraw quads that belong to given tile
void MapRenderer::refresh(int x, int y) {
  if (x < _offset.x || x >= _offset.x + _width || y < _offset.y || y >= _offset.y + _height)
    return; //check if tile is visible
            
  //vertices works like 2d ring buffer
  auto vx = x % _width;
  auto vy = y % _height;
  if (vx < 0) vx += _width;
  if (vy < 0) vy += _height;
  //we can do this, because some tiles are never to be seen at once

  auto index = (vx + vy * _width) * 4 * _layers;
  auto rec = sf::FloatRect(x * _tileSize, y * _tileSize, _tileSize, _tileSize);

  for (int i = 0; i < _layers; i++) {
    sf::Color color;
    sf::IntRect src;
    _provider(x, y, i, color, src);
    //get color and texture rectangle from our providing function

    draw(index, rec, src, color);
    //insert quad in vertex array
    index += 4;
  }
}

// Update offset (based on Target's position) and draw it
void MapRenderer::draw(sf::RenderTarget &rt, sf::RenderStates states) const {
  states.texture = _texture;
  rt.draw(&_vertices[0], _vertices.size(), sf::PrimitiveType::Quads, states);
}

void MapRenderer::update(sf::RenderTarget &rt) {
  auto view = rt.getView();
  setSize(view.getSize());
  setCorner(rt.mapPixelToCoords(sf::Vector2i()));
}
