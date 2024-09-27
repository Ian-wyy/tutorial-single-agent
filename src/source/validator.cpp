#include "validator.hpp"
#include <cmath>
#include <vector>
using namespace std;
using namespace movingai;

void Validator::addRoute(vector<PPT> new_Rought) {
  routes.push_back(new_Rought);
}

bool Validator::ifCoflict(vector<PPT> r1, vector<PPT> r2) {
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

bool Validator::isValid(){
    for(int i = 0, I = routes.size(); i< I - 1; i++){
        for (int j = i + 1; j < I; j++) {
            if (ifCoflict(routes[i], routes[j])) {
             return false;
            }
        }
    }
    return true;
}