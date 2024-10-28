#include "findPath.hpp"
#include "gridmap.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>

using namespace std;
using namespace movingai;

#define LINE 1
#define SLIDE 1.414
#define TIME_LINE 1
#define TIME_SLIDE 1.414

#define DEBUG 0
#define TIME_STEP 1

void findPath::getMap() {
  for (int i = 0; i < grid.height_; i++) {
    map.push_back(vector<int>());
    for (int j = 0; j < grid.width_; j++) {
      map[i].push_back((int)grid.is_obstacle({j, i}));
    }
  }
}

Node *createTreeNode(int x, int y, int time = 0) {
  Node *pNew = new Node;
  memset(pNew, 0, sizeof(Node));
  pNew->pos.x = x;
  pNew->pos.y = y;
  pNew->time = time;
  pNew->parent = nullptr;
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
  delete root;
}

double getH(Point current, Point end) {
  return sqrt(abs((current.x - end.x) * (current.x - end.x)) +
              abs((current.y - end.y) * (current.y - end.y)));
}

bool isInList(vector<Node *> &buff, Node *child) {
  for (vector<Node *>::iterator it = buff.begin(); it < buff.end(); it++) {
    if ((*it)->pos.x == child->pos.x && (*it)->pos.y == child->pos.y) {
      if (child->pos.F < (*it)->pos.F) {
        Node *useless = *it;
        Node *p = useless->parent;
        buff.erase(it);

        for (vector<Node *>::iterator itt = p->child.begin();
             itt < p->child.end(); itt++) {
          if (*itt == useless) {
            p->child.erase(itt);
          }
        }
        delete useless;
        return false;
      }
      return true;
    }
  }
  return false;
}

void findPath::inputTimeStep(Node *current,
                             Node *root) { // also log whole routes
  map[current->pos.y][current->pos.x] =
      1; // The agent will stay at the end point
  vector<PPT> new_route;
  while (current != root) {
      PPT p(current->pos.x, current->pos.y, current->time);
      PPT pp(current->pos.x, current->pos.y, current->parent->time);
      pt.push_back(p);
      pt.push_back(pp);
      //we need to log the specific routes
      new_route.insert(new_route.begin(),p);
    current = current->parent;
  }
  PPT p(root->pos.x, root->pos.y, root->time);
  pt.push_back(p);
  new_route.insert(new_route.begin(), p);
  // put the route in class
  val.addRoute(new_route);
}

bool findPath::isInPt(Node *child,Node* current) {
  for (auto p : pt) {
    if (p.x == child->pos.x && p.y == child->pos.y && p.time == child->time) {
      return true;
    }
    if (p.x == child->pos.x && p.y == child->pos.y && p.time == current->time) {
      return true;
    }
  }
  return false;
}

double findPath::getPath(Point start, Point end) {
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
  if (DEBUG) {
    cout << "DEBUG: Start "
         << "x: " << current->pos.x << ", y: " << current->pos.y << endl;
    cout << " DEBUG : End "
         << " x : " << end.x << ",y : " << end.y << endl;
  }
  bool isFindEnd = true;

  // time = 0; // each time finding a new object, time get 0 again

  while (1) {
    // time++; // every search step, time ++
    if (DEBUG) {
      cout << "DEBUG: "
           << "x: " << current->pos.x << ", y: " << current->pos.y << endl;
    }

    for (int i = 0; i < 8; i++) {
      Node *child = createTreeNode(current->pos.x, current->pos.y);
      switch (i) {
      case p_up:
        child->pos.y--;
        child->time = current->time + TIME_LINE;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_down:
        child->pos.y++;
        child->time = current->time + TIME_LINE;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_left:
        child->pos.x--;
        child->time = current->time + TIME_LINE;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_right:
        child->pos.x++;
        child->time = current->time + TIME_LINE;
        child->pos.G = current->pos.G + LINE;
        break;
      case p_lup:
        child->pos.x--;
        child->pos.y--;
        child->time = current->time + TIME_SLIDE;
        child->pos.G = current->pos.G + SLIDE;
        break;
      case p_ldown:
        child->pos.y++;
        child->pos.x--;
        child->time = current->time + TIME_SLIDE;
        child->pos.G = current->pos.G + SLIDE;
        break;
      case p_rdown:
        child->pos.x++;
        child->pos.y++;
        child->time = current->time + TIME_SLIDE;
        child->pos.G = current->pos.G + SLIDE;
        break;
      case p_rup:
        child->pos.x++;
        child->pos.y--;
        child->time = current->time + TIME_SLIDE;
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
        if (isInPt(child,current)) {
          delete child;
          continue;
        } else if (isInList(buff, child)) {
          delete child;
          continue;
        } else {
          current->child.push_back(child);
          child->parent = current;
          buff.push_back(child);
        }
      } else {
        delete child;
      }
    }

    if (buff.size() == 0) { // If the point can't move at all and no point in
                            // opnelist list now, then break
      isFindEnd = false;
      break;
    }

    it = buff.begin();
    itMin = buff.begin();

    for (; it != buff.end(); it++) {
      itMin = ((*itMin)->pos.F < (*it)->pos.F) ? itMin : it;
    }

    current = *itMin;
    pathMap[current->pos.y][current->pos.x] = 1;
    buff.erase(itMin);

    if (end.x == current->pos.x && end.y == current->pos.y) {
      break;
    }
  }

  if (isFindEnd) {
    double length = current->pos.G;
    if (TIME_STEP) {
      inputTimeStep(current, root);
    } // If we find a path, then simulating the
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

void findPath::test_Astar(int LIMIT) {
  auto tstart =
      std::chrono::steady_clock::now(); // limit the time of the pathFinding

  int flag = 1;
  int count = 0;

  for (int i = 0; i < scen.num_experiments(); i++) {
    auto expr = scen.get_experiment(i);
    Point start = {(int)expr->startx(), (int)expr->starty()};
    Point end = {(int)expr->goalx(), (int)expr->goaly()};
    int distance = getPath(start, end);
    if (abs(distance - expr->distance()) < LIMIT) {
      count++; // see how many path can be find in the time limit
      continue;
    } else {
      flag = 0;
      cout << "the path that longer than the answer over " << LIMIT << ":"
           << endl;
      cout << start.x << ", " << start.y << endl;
      cout << end.x << ", " << end.y << endl;
      cout << "testA: " << distance << " answer: " << expr->distance() << endl
           << endl;
    }
    auto tnow = std::chrono::steady_clock::now();
    double t = std::chrono::duration<double>(tnow - tstart).count();
    if (t > 5) {
      cout << "time is end" << endl;
      cout << "you have successfully find " << count << "path in " << t
           << " seconds" << endl;
      break;
    }
  }
  if (flag) {
    cout << "OK" << endl;
  } else {
    cout << "ERROR" << endl;
  }
}

void findPath::test_Validate(int LIMIT){
    auto tstart =
      std::chrono::steady_clock::now(); // limit the time of the pathFinding

  int flag = 1;
  int count = 0;

  for (int i = 0; i < scen.num_experiments(); i++) {
    auto expr = scen.get_experiment(i);
    Point start = {(int)expr->startx(), (int)expr->starty()};
    Point end = {(int)expr->goalx(), (int)expr->goaly()};
    int distance = getPath(start, end);
    if (abs(distance - expr->distance()) < LIMIT) {
      count++; // see how many path can be find in the time limit
      continue;
    } else {
      flag = 0;
      cout << "the path that longer than the answer over " << LIMIT << ":"
           << endl;
      cout << start.x << ", " << start.y << endl;
      cout << end.x << ", " << end.y << endl;
      cout << "testA: " << distance << " answer: " << expr->distance() << endl
           << endl;
    }
  }
  if (flag) {
    cout << "OK" << endl;
  } else {
    cout << "ERROR, " << "we get " << count << " valid routes." << endl;
  }

  if (val.isValid()) {
    cout<< "Valid path!"<<endl;
  }
}