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
    vector<Point*> child;

    
};

class findPath : public gridmap
{
public:
    findPath(vid height, vid width) : gridmap(height, width) { getMap(); };
    findPath(const string &filename) : gridmap(filename) { getMap(); };
    void getMap();
    vector<Point *> getPath();
    double calcDistance(vector<Point*> path);

    vector<vector<int>> _map;
    vector<Point *> openList;
    vector<Point *> closeList;

    Point start_point;
    Point end_point;

private:
    Point *__getPath();
    void getsurroundingPoints(Point *point) ;
    bool isCanreach(const Point point, const Point target) const;
    Point *isInlist(const vector<Point *> &list, const Point *point) const;
    Point *getLeastFpoint();

    double calcG(Point *temp_start, Point *point);
    double calcH(Point *point);
    double calcF(Point *point);

};
