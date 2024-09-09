#pragma once

#include <string>
#include <vector>

namespace movingai {

using namespace std;
using vid = int;

struct State {
  vid x, y;
};


typedef struct Point
{
    int x, y;
    double F, G, H;
} Point;

class gridmap {
public:
  // init an empty map
  gridmap(vid height, vid width);
  // init map based on an input file
  gridmap(const string &filename);

  vector<State> get_neighbours(State c);

  // get the label associated with the coordinate (x, y)
  inline bool is_obstacle(State c) {
    auto label = this->get_label(c);
    return label == 1;
  }

  // set the label associated with the coordinate (x, y)
  inline void set_label(State c, bool label) {
    db[c.y * height_ + c.x] = label;
  }

  inline bool get_label(State c) { return db[c.y * width_ + c.x]; }

  vid height_, width_;
  string filename;
  // whether a location is an obstacle
  vector<bool> db;
};

} // namespace movingai
