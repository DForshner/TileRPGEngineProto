
#include "Camera.h"
#include <SFML\Graphics.hpp>

void Camera::Move(int x, int y) {
  _position = { (float)x, (float)y };
  _target = { (float)x, (float)y };
}

void Camera::MoveCenter(int x, int y) {
  // translate center x,y to camera origin
  auto originX = x - _size.x / 2;
  auto originY = y - _size.x / 2;

  _position = { (float)originX, (float)originY };
  _target = { (float)originX, (float)originY };
}

void Camera::ScrollTo(int x, int y) {
  _target = { (float)x, (float)y };
}

void Camera::ScrollCenterTo(int x, int y) {
  // translate center x,y to camera origin
  auto originX = x - _size.x / 2;
  auto originY = y - _size.x / 2;

  _target = { (float)originX, (float)originY };
}

constexpr float MIN_DISTANCE_BEFORE_SNAP = 1.0;
constexpr int TARGET_FPS = 60;

void Camera::Update() {
  auto dx = _target.x - _position.x;
  auto dy = _target.y - _position.y;

  // Find distance between actual and target position
  auto distance = sqrt(dx * dx + dy * dy);

  if (distance < MIN_DISTANCE_BEFORE_SNAP) {
    _position.x = _target.x;
    _position.y = _target.y;
    return;
  }

  auto velocity = (distance * _speed) / TARGET_FPS;
  if (velocity < 1.0f) {
    velocity = 1.0f; // Must move at least one pixel
  }

  auto vx = dx * (velocity / distance);
  auto vy = dy * (velocity / distance);
  _position = { _position.x += vx, _position.y += vy };
}

sf::IntRect Camera::getTileBounds(int tileSize) {
  int x{ (int)_position.x / tileSize };
  int y{ (int)_position.y / tileSize };

  int width{ _size.x / tileSize + 2 };
  if (x % tileSize != 0) {
    width++;
  }

  int height{ _size.y / tileSize + 2 };
  if (y % tileSize != 0) {
    height++;
  }

  return{ x, y, width, height };
}