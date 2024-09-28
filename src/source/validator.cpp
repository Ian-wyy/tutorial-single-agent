#include "validator.hpp"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
using namespace movingai;

void validator::addRoute(vector<PPT> new_Rought) {
  routes.push_back(new_Rought);
}

bool validator::ifCoflict(vector<PPT> r1, vector<PPT> r2) {
  int l1 = r1.size();
  int l2 = r2.size();
  bool isCoflict = false;
  for (int i = 0; i < min(l1, l2) - 1; i++) {
    PPT p11 = r1[i], p12 = r1[i + 1];
    PPT p21 = r2[i], p22 = r2[i + 1];
    if ((p11.x - p21.x) * (p11.x - p21.x) + (p11.y - p21.y) * (p11.y - p21.y) >
        2)
      continue;
    if ((p12.x - p22.x) * (p12.x - p22.x) + (p12.y - p22.y) * (p12.y - p22.y) >
        2) {
      i++; // next comparixon doesn't need to check too
      continue;
    }
    //The model of collision we need to implement, now  is easy
    if ((p11.x == p22.x && p11.y == p22.y)) {
      cout<<"Point 1: x:"<<p11.x<<" y:"<<p11.y<<" time: "<<p11.time<<endl;
      cout<<"Point 2: x:"<<p21.x<<" y:"<<p21.y<<" time: "<<p21.time<<endl;
      cout<<"Point 1: x:"<<p12.x<<" y:"<<p12.y<<" time: "<<p12.time<<endl;
      cout<<"Point 2: x:"<<p22.x<<" y:"<<p22.y<<" time: "<<p22.time<<endl;
        isCoflict = true;
        break;
    }
    if ((p21.x == p12.x && p21.y == p12.y)) {
        isCoflict = true;
        break;
    }

  }
  return isCoflict;
}

bool validator::isValid(){
    for(int i = 0, I = routes.size(); i< I - 1; i++){
        for (int j = i + 1; j < I; j++) {
            if (ifCoflict(routes[i], routes[j])) {
             return false;
            }
        }
    }
    return true;
}