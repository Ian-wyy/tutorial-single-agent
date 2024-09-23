#pragma once

#include "gridmap.hpp"
#include "load_scens.hpp"
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace movingai;
using namespace std;

class findPath {

public:
  findPath(const string &filename1, const string &filename2) : grid(filename1) {
    scen.load_scenario(filename2);
    getMap();
    time = 0;
  }
  void getMap();
  double getPath(Point start, Point end);
  bool isInPt(Point p);

  gridmap grid;
  scenario_manager scen;
  vector<vector<int>> map;

  // The part for time dimension
  vector<PPT> pt;
  int time;
};