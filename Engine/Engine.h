#pragma once

#include "ImageManager.h"
#include "Camera.h"
#include "Level.h"
#include "Tile.h"
#include <SFML\Graphics.hpp>
#include <memory>

constexpr int WIDTH = 1000;
constexpr int HEIGHT = 800;
constexpr int BIT_DEPTH = 32;

constexpr int MAP_WIDTH = 20;
constexpr int MAP_HEIGHT = 20;

constexpr int TILE_SIZE = 32;

class Engine {
private:
  std::unique_ptr<sf::RenderWindow> _window;
  ImageManager _imageManager;
  Camera _camera;
  Level _level;
  Tile* _testTile;
  bool _mouseDown;
  sf::Texture _testText;

  void init();

  void loadImages();

  void loadLevel();

  void mainLoop();

  void renderFrame();

  void processInput();

  void update();

public:
  Engine() :
    _camera(WIDTH, HEIGHT, 0.3f),
    _level(MAP_WIDTH, MAP_HEIGHT),
    _mouseDown(false)
  {}

  ~Engine() {}

  void start();
};