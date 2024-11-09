#pragma once

#include <string>
#include <vector>

namespace movingai {

using namespace std;
using vid = int;

struct State {
  vid x, y;
};


typedef struct Point //The point we do in the findPath
{
    int x, y;
    double F, G, H;
} Point;

typedef struct PPT{
  int x;
  int y;
  int time;
  PPT(int x, int y, int time): x(x), y(y), time(time){}
} PPT; //The pair to see whether the point has agent in this time

enum dirent { p_up, p_down, p_left, p_right, p_lup, p_ldown, p_rup, p_rdown, p_wait };

typedef struct Node {
  Point pos;
  vector<Node *> child;
  Node *parent;
  int time;
} Node;

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
    db[c.y * width_ + c.x] = label;
  }

  inline bool get_label(State c) { return db[c.y * width_ + c.x]; }

  vid height_, width_;
  string filename;
  // whether a location is an obstacle
  vector<bool> db;
};

} // namespace movingai
