#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

// callback function that will provide data for each tile
typedef void(*TileProvider)(int, int, int, sf::Color&, sf::IntRect&);

// Provides fast rendering of tilemaps
class MapRenderer : public sf::Drawable {
public:
  MapRenderer(sf::Texture *texture, TileProvider provider, float tileSize, int layers);
  ~MapRenderer() {}

  float getTileSize() const { return _tileSize; }

  int getLayers() const { return _layers; }

  // Redraw every tile on screen
  void refresh() { refreshLocal(0, 0, _width, _height); }

  // Redraw single tile
  void refresh(int x, int y);

  // Draw map on screen
  void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  // Update screen size and position of camera
  void update(sf::RenderTarget &target);

private:
  int _width;

  int _height;

  float _tileSize;

  int _layers;

  // vertices corresponding to the tiles/quads in the view
  std::vector<sf::Vertex> _vertices;

  // tile position in upper left corner
  sf::Vector2i _offset;

  // Provides Color and Texture Source from tile map
  TileProvider _provider;

  // Holds the sprite sheet
  sf::Texture* _texture;

  // Adjust the amount of vertices to the size of the screen
  void setSize(sf::Vector2f v);

  // Refresh parts of the screen if player moved
  // v = World position of top left corner of the screen
  void setCorner(sf::Vector2f v);

  // Translate from world position to index of tile under that position
  sf::Vector2i getTile(sf::Vector2f pos) const;

  // Draw textured quad on a given position
  void draw(int index, sf::FloatRect rec, sf::IntRect src, sf::Color color);

  void refreshLocal(int left, int top, int right, int bottom);
};