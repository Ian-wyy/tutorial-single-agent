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
  bool isInPt(Node* child, Node* current);
  void test_Astar(int LIMIT); //the function to test the function of this class
  void test_Validate(int LIMIT);

  gridmap grid;
  scenario_manager scen;
  vector<vector<int>> map;

  // The part for time dimension
  vector<PPT> pt; //Check when finding the path
  //vector<vector<PPT>> routes; // The path of all agents
  int time;
  void inputTimeStep(Node* current, Node* root);
  validator val;
};