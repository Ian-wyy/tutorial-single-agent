#pragma once

#include "findPath.hpp"
#include "gridmap.hpp"
#include "load_scens.hpp"
#include <vector>
using namespace movingai;
using namespace std;

class Validator {
public:
  Validator() {}
  void addRoute(vector<PPT>);
  bool ifCoflict(vector<PPT> r1,
                 vector<PPT> r2); // Input two ordered (pos,time) series
  bool isValid();
  vector<vector<PPT>> routes;
};