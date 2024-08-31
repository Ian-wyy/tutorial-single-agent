#pragma once

#include <string>
#include <vector>
#include <list>
#include "gridmap.hpp"
#include "load_scens.hpp"

using namespace movingai;
using namespace std;

struct Point
{
    int x, y;
    double G, H, F;
    Point *parent;

    Point(int _x, int _y) : x(_x), y(_y), F(0), G(0), H(0), parent(NULL)
    {
    }
};

class findPath : public gridmap
{
public:
    findPath(vid height, vid width) : gridmap(height, width) { getMap(); };
    findPath(const string &filename) : gridmap(filename) { getMap(); };
    void getMap();
    list<Point *> getPath(Point &startpoint, Point &endPoint);
    double calcDistance(list<Point*> path);

    vector<vector<int>> _map;
    list<Point *> openList;
    list<Point *> closeList;

private:
    Point *__getPath(Point &startPoint, Point &endPoint);
    vector<Point *> getsurroundingPoints(const Point *point) const;
    bool isCanreach(const Point *point, const Point *target) const;
    Point *isInlist(const list<Point *> &list, const Point *point) const;
    Point *getLeastFpoint();

    double calcG(Point *temp_start, Point *point);
    double calcH(Point *point, Point *end);
    double calcF(Point *point);

};
