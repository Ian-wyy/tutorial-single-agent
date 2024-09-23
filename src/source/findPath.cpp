#include "findPath.hpp"
#include "gridmap.hpp"

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string.h>
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
  for (int i = 0; i < root->child.size(); i++) {
    freeNode(root->child[i]);
  }
  delete (root);
}

double getH(Point current, Point end) {
  return sqrt(abs((current.x - end.x) * (current.x - end.x)) +
              abs((current.y - end.y) * (current.y - end.y)));
}

bool isInList(vector<Node *> &buff, Node *child) {
  for (auto x : buff) {
    if (x->pos.x == child->pos.x && x->pos.y == child->pos.y) {
      return true;
    }
  }
  return false;
}

void findPath::inputTimeStep(Node *&current, Node *&root) {}

bool findPath::isInPt(Point pos) {
  for (auto p : pt) {
    if (p.x == pos.x && p.y == pos.y && p.time == time) {
      return true;
    }
  }
  return false;
}

double findPath::getPath(Point start, Point end) {
  // bool pathMap[grid.height_][grid.width_] = {0};

  // As we can't use VLA, we need to malloc area for the path_map
  int **pathMap = (int **)malloc(sizeof(int *) * grid.height_);
  for (int i = 0; i < grid.height_; i++) {
    pathMap[i] = (int *)malloc(sizeof(int) * grid.width_);
  }
  for (int i = 0; i < grid.height_; i++) {
    for (int j = 0; j < grid.width_; j++) {
      pathMap[i][j] = 0;
    }
  }

  vector<Node *> buff;
  vector<Node *>::iterator it;    // 变化
  vector<Node *>::iterator itMin; // 记录最小的

  Node *root = createTreeNode(start.x, start.y);

  pathMap[start.y][start.x] = true;

  Node *current = root;
  // cout<<"DEBUG: Start "<<"x: "<<current->pos.x<<", y:
  // "<<current->pos.y<<endl; cout<<"DEBUG: End "<<"x: "<<end.x<<", y:
  // "<<end.y<<endl;
  bool isFindEnd = true;

  time = 0; // each time finding a new object, time get 0 again

  while (1) {
    // time++; // every search step, time ++
    cout << "DEBUG: "
         << "x: " << current->pos.x << ", y: " << current->pos.y << endl;

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

      if (child->pos.x >= 0 && child->pos.y >= 0 &&
          child->pos.x < grid.width_ && child->pos.y < grid.height_ &&
          pathMap[child->pos.y][child->pos.x] == 0 &&
          map[child->pos.y][child->pos.x] == 0 &&
          map[child->pos.y][current->pos.x] == 0 &&
          map[current->pos.y][child->pos.x] == 0) {
        child->pos.H = getH(child->pos, end);
        child->pos.F = child->pos.H + child->pos.G;
        if (isInList(buff, child)) {
          delete child;
          continue;
        } else if (isInPt(child->pos)) {
          delete child;
          continue;
        }  
        else {
          current->child.push_back(child);
          child->parent = current;
          buff.push_back(child);
        }
      } else {
        delete child;
      }
    }

    if (buff.size() == 0) { // If the first point can't move at all, then break
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
    inputTimeStep(current, root); // If we find a path, then simulating the
                                  // agent moving on the path
    freeNode(root);
    for (int i = 0; i < grid.height_; i++) {
      free(pathMap[i]);
    }
    free(pathMap);
    return length;
  } else {
    freeNode(root);
    for (int i = 0; i < grid.height_; i++) {
      free(pathMap[i]);
    }
    free(pathMap);
    return -1;
  }
}