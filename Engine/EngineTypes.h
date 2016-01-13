#pragma once

struct Tile2DPosition {
  const int x;
  const int y;

  Tile2DPosition(int x, int y) : x(x), y(y) {}
  Tile2DPosition(const Tile2DPosition& that) = delete;
  Tile2DPosition(const Tile2DPosition&& that) = delete;
};

struct Tile3DPosition : public Tile2DPosition {
  const int z;
};

struct WorldPosition {
  const int x;
  const int y;

  WorldPosition(int x, int y) : x(x), y(y) {}
  WorldPosition(const WorldPosition& that) = delete;
  WorldPosition(const WorldPosition&& that) = delete;
};

struct DisplayPosition {
  const int x;
  const int y;

  DisplayPosition(int x, int y) : x(x), y(y) {}
  DisplayPosition(const DisplayPosition& that) = delete;
  DisplayPosition(const DisplayPosition&& that) = delete;
};
