#pragma once

#include "gridmap.hpp"
#include "load_scens.hpp"
#include "validator.hpp"
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
  bool isInPt(Node* child);
  void test(int LIMIT, int CHECK_VALID = 0); //the function to test the function of this class
                                            //LIMIT is the smallest error with the right answer we can't use
                                            //CHECK_VALID is whether we use validator

  gridmap grid;
  scenario_manager scen;
  vector<vector<int>> map;

  // The part for time dimension
  vector<PPT> pt;
  int time;
  void inputTimeStep_PT(Node* current, Node* root);

  //The part of validator and checking
  validator va;
  void inputTimeStep_VA(Node* current, Node* root);
};