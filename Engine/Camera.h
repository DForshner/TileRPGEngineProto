#pragma once

#include <Tile.h>
#include <SFML\Graphics.hpp>
#include <cassert>

// View port through which the user sees the map.
class Camera {
private:
  sf::Vector2f _position;

  sf::Vector2f _target;

  sf::Vector2i _size;

  // Speed of camera.  Range [0.0, 1.0]
  float _speed;

public:
  explicit Camera(int w, int h, float speed) :
    _size({ w, h }), _speed(speed) {
    assert(speed >= 0.0f && speed <= 1.0f);
  }

  ~Camera() {};

  // Instant move cameras origin (upper-left)
  void Move(int x, int y);

  // Instant move cameras center
  void MoveCenter(int x, int y);

  // Scrolling move camera's origin (upper-left)
  void ScrollTo(int x, int y);

  // Scrolling move camera's center
  void ScrollCenterTo(int x, int y);

  void Update();

  sf::Vector2i GetPosition() const noexcept { 
    return{ (int)_position.x, (int)_position.y }; 
  }

  sf::Vector2i GetNearestTileOffset(int tileSize) const noexcept { 
    assert(tileSize > 0);
    return{ (int)_position.x % tileSize, (int)_position.y % tileSize };
  }

  // Get dimensions of tiles that can fit in camera
  sf::IntRect getTileBounds(int tileSize);
};