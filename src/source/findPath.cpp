#include "findPath.hpp"
#include "gridmap.hpp"
#include "load_scens.hpp"

#include <iostream>
#include <list>
#include <math.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace movingai;

#define LINE 1
#define SLIDE 1.414

void findPath::getMap() {
  for (int i = 0; i < grid.height_; i++) {
    map.push_back(vector<int>());
    for (int j = 0; j < grid.width_; j++) {
      map[i].push_back((int)grid.is_obstacle({j, i}));
    }
  }
}

enum dirent { p_up, p_down, p_left, p_right, p_lup, p_ldown, p_rup, p_rdown };

typedef struct Node {
  Point pos;
  vector<Node *> child;
  Node *parent;
} Node;

Node *createTreeNode(int x, int y) {
  Node *pNew = new Node;
  memset(pNew, 0, sizeof(Node));
  pNew->pos.x = x;
  pNew->pos.y = y;
  return pNew;
}

void freeNode(Node *root) {
  if (root->child.size() == 0) {
    delete root;
    return;
  }
  /* vector<Node *>::iterator it = root->child.begin();
  for (; it < root->child.end(); it++) {
    freeNode(*it);
  } */
  for (int i = 0; i < root->child.size(); i++) {
    freeNode(root->child[i]);
  }
  delete (root);
}

double getH(Point current, Point end) {
  return sqrt(abs((current.x - end.x) * (current.x - end.x)) +
         abs((current.y - end.y) * (current.y - end.y)));
}

bool isInList(vector<Node *>& buff, Node *child) {
  for (auto x : buff) {
    if (x->pos.x == child->pos.x && x->pos.y == child->pos.y) {
      return true;
    }
  }
  return false;
}

/* bool findPath::isInPt(Point pos) {
  for (auto p : pt) {
    if (p.x == pos.x && p.y == pos.y && p.time == time) {
      return true;
    }
  }
  return false;
} */

double findPath::getPath(Point start, Point end) {
  bool pathMap[grid.height_][grid.width_] = {0};

  vector<Node *> buff;
  vector<Node *>::iterator it;    // 变化
  vector<Node *>::iterator itMin; // 记录最小的

  Node *root = createTreeNode(start.x, start.y);

  pathMap[start.y][start.x] = true;

  Node *current = root;
  //cout<<"DEBUG: Start "<<"x: "<<current->pos.x<<", y: "<<current->pos.y<<endl;
  //cout<<"DEBUG: End "<<"x: "<<end.x<<", y: "<<end.y<<endl;
  bool isFindEnd = true;

  //time = 0; // each time finding a new object, time get 0 again

  while (1) {
    //time++; // every search step, time ++

    for (int i = 0; i < 8; i++) {
      Node *child = createTreeNode(current->pos.x, current->pos.y);
      switch (i) {
      case p_up:
        child->pos.y--;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_down:
        child->pos.y++;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_left:
        child->pos.x--;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_right:
        child->pos.x++;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_lup:
        child->pos.x--;
        child->pos.y--;
        child->pos.G = current->pos.G + SLIDE;
        break;
      case p_ldown:
        child->pos.y++;
        child->pos.x--;
        child->pos.G = current->pos.G + SLIDE;
        break;
      case p_rdown:
        child->pos.x++;
        child->pos.y++;
        child->pos.G = current->pos.G + SLIDE;
        break;
      case p_rup:
        child->pos.x++;
        child->pos.y--;
        child->pos.G = current->pos.G + SLIDE;
        break;
      }
      
      if (child->pos.x >= 0 && child->pos.y >= 0 && child->pos.x < grid.width_ &&
          child->pos.y < grid.height_ &&
          pathMap[child->pos.y][child->pos.x] == 0 &&
          map[child->pos.y][child->pos.x] == 0 &&
          map[child->pos.y][current->pos.x] == 0 &&
          map[current->pos.y][child->pos.x] == 0) {
        child->pos.H = getH(child->pos, end);
        child->pos.F = child->pos.H + child->pos.G;
        if (isInList(buff, child)) {
          delete child;
          continue;
        } /* else if (isInPt(child->pos)) {
          delete child;
          continue;
        } */
        else {
          current->child.push_back(child);
          child->parent = current;
          buff.push_back(child);
        }
      } else {
        delete child;
      }
    }

    if(buff.size() == 0){ //TODO:test
      isFindEnd = false;
      break;
    }

    it = buff.begin();
    itMin = buff.begin();
    
    for (; it != buff.end(); it++) {
      itMin = ((*itMin)->pos.F < (*it)->pos.F) ? itMin : it;
    }

    current = *itMin;
    pathMap[current->pos.y][current->pos.x] = true;
    buff.erase(itMin);
    
    /* PPT p(current->pos.x, current->pos.y,
          time); // The pt is used to record the time and step
    pt.push_back(p); */

    if (end.x == current->pos.x && end.y == current->pos.y) {
      break;
    }
    if (buff.size() == 0) {
      isFindEnd = false;
      break;
    }
  }

  if (isFindEnd) {
    double length = current->pos.G;
    freeNode(root);
    return length;
  } else {
    freeNode(root);
    return -1;
  }
}