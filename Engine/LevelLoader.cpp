
#include "LevelLoader.h"
#include "Level.h"
#include "ImageManager.h"
#include "base64.h"
#include <rapidxml\rapidxml.hpp>
#include <fstream>
#include <vector>
#include <istream>
#include <string>
#include <exception>
#include <algorithm>
#include <cctype>
#include <iostream>

// Kludge: Wrap in namespace to prevent conflict with rapidxml. See: http://sourceforge.net/p/rapidxml/bugs/13/
namespace ZLib {
  #include "zlib.h"
}

using namespace rapidxml;

// TODO: this is duplicated in image manager
std::string buildString3(std::ifstream* fin) {
  std::string contents;
  std::string line;
  while (std::getline(*fin, line)) {
    contents += line;
  }
  return contents;
}

struct TileToLoad {
  int id;
  int x;
  int y;
  bool walkable;
};

struct LevelToLoad {
  int width;
  int height;
  int tileSize;
  std::vector<std::string> tilesets;
  std::vector<TileToLoad> tiles;

  LevelToLoad() : width(0), height(0), tileSize(0), tilesets(0), tiles(0) {}
};

constexpr const char * TILESET_NODE = "tileset";
constexpr const char * TILE_NODE = "tile";

Level LevelLoader::loadLevel(const std::string& fileName, ImageManager& imageManager) {
  std::ifstream fin("./Assets/" + fileName);
  if (!fin) {
    throw std::exception(std::string("Could not open tile set: " + fileName).c_str());
  }

  auto contents = buildString3(&fin);
  std::vector<char> xmlData(contents.cbegin(), contents.cend());
  xmlData.emplace_back('\0');

  rapidxml::xml_document<> doc;
  doc.parse<parse_no_data_nodes>(&xmlData[0]);

  // TODO: Above is duplicated in image manager

  xml_node<>* root = doc.first_node("map");

  LevelToLoad toLoad;

  // Get level attributes
  //auto mapNode = root->first_node("map");
  toLoad.width = atoi(root->first_attribute("width")->value());
  toLoad.height = atoi(root->first_attribute("height")->value());
  toLoad.tileSize = atoi(root->first_attribute("tileheight")->value());

  // Parse tileset nodes
  for (auto tileSetNode = root->first_node("tileset"); tileSetNode; tileSetNode = tileSetNode->next_sibling("tileset")) {
    std::string name{ tileSetNode->first_attribute("name")->value() };
    toLoad.tilesets.emplace_back("test");
  }

  // Parse layer nodes
  for (auto layerNode = root->first_node("layer"); layerNode; layerNode = layerNode->next_sibling("layer")) {
    std::string name{ layerNode->first_attribute("name")->value() };
    auto dataNode = layerNode->first_node();
    std::string encoding{ dataNode->first_attribute("encoding")->value() };
    assert(encoding == "base64");

    std::string compression{ dataNode->first_attribute("compression")->value() };
    assert(compression == "zlib");

    std::string data{ dataNode->value() };
    data.erase(std::remove_if(data.begin(), data.end(), std::isspace), data.end());
    auto decodedBytes = base64_decode(data);


    using namespace ZLib;


    // original string len = 36
    char a[50] = "Hello Hello Hello Hello Hello Hello!";

    // placeholder for the compressed (deflated) version of "a" 
    char b[50];

    printf("Uncompressed size is: %lu\n", strlen(a));
    printf("Uncompressed string is: %s\n", a);


    printf("\n----------\n\n");

    // STEP 1.
    // deflate a into b. (that is, compress a into b)

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt)strlen(a) + 1; // size of input, string + terminator
    defstream.next_in = (Bytef *)a; // input char array
    defstream.avail_out = (uInt)sizeof(b); // size of output
    defstream.next_out = (Bytef *)b; // output char array
                                     
    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);


    printf("Compressed size is: %lu\n", strlen(b));
    printf("Compressed string is: %s\n", b);


    z_stream infstream;
    infstream.zalloc = (alloc_func)0;
    infstream.zfree = (free_func)0;
    infstream.opaque = (voidpf)0;

    infstream.avail_in = (uInt)((char*)defstream.next_out - b); // size of input
    infstream.next_in = (Bytef *)b; // input char array

    //infstream.next_in = &decodedBytes[0];  //(Bytef *)data.data(); // input char array
    //infstream.avail_in = decodedBytes.size(); // size of input

    //infstream.next_out = (Bytef *)newdata.data(); // output char array
    //infstream.avail_out = (uInt)newdata.size(); // size of output

    int32_t err = Z_OK;
    inflateInit(&infstream);

    const int BUFFER_SIZE = 100;
    std::vector<char> newdata;
    for (;;) {
      uint8_t buff[BUFFER_SIZE];
      infstream.next_out = (Bytef *)&buff[0];
      infstream.avail_out = (uInt)BUFFER_SIZE;

      err = inflate(&infstream, Z_NO_FLUSH);
      if (err == Z_STREAM_END) {
        // Reached end of stream

        // Buffer may only be partially filled
        for (int i = 0; i < (BUFFER_SIZE - infstream.avail_out); i++) {
          newdata.emplace_back(buff[i]);
        }
        if (infstream.avail_in == 0) {
          break;
        }
      }

      // err should be inflate 

      // Check for errors
      for (int i = 0; i < (BUFFER_SIZE - infstream.avail_out); i++) {
        newdata.emplace_back(buff[i]);
      }
    }

    //*outLength = bufferSize - d_stream.avail_out;
    err = inflateEnd(&infstream);
    // err should be inflateEnd

    printf("ZLib size is: %lu\n", infstream.total_out);

    std::cout << std::endl;
    for(auto const& t : newdata) {
      std::cout << t;
    }
  }

  // Resize level
  //this->_width = toLoad.width;
  //this->_height = toLoad.height;
  //SetDimensions(toLoad.width, toLoad.height);

  // Load tilesets
  //for (const auto& tileSetName : toLoad.tilesets) {
    //imageManager.loadTileset(tileSetName);
  //}

  // Setup map tiles
  //for (const auto& loc : toLoad.tiles) {
    //std::unique_ptr<Tile> tile (new Tile(imageManager.getTexture(loc.id)));
    //addTile(loc.x, loc.y, std::move(tile));
  //}
}