#include "load_scens.hpp"
#include "gridmap.hpp"
#include "findPath.hpp"

#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <math.h>
#include <string.h>

#define edge 1
#define angle 1.414

using namespace std;
using namespace movingai;

void findPath::getMap()
{
    for (int i = 0; i < height_; i++)
    {
        _map.push_back(vector<int>());
        for (int j = 0; j < width_; j++)
        {
            _map[i].push_back(db[i * width_ + j]);
        }
    }
}

double findPath::calcG(Point *temp_start, Point *point)
{
    int extraG = (abs(point->x - temp_start->x) + abs(point->y - temp_start->y)) == 1 ? edge : angle;
    int parentG = point->parent == NULL ? 0 : point->parent->G;
    return extraG + parentG;
}

double findPath::calcH(Point *point)
{
    return sqrt((double)(end_point.x - point->x) * (double)(end_point.x - point->x) + (double)(end_point.y - point->y) * (double)(end_point.y - point->y));
}

double findPath::calcF(Point *point)
{
    return point->G + point->H;
}


Point *createNewPoint(int x, int y)
{
    Point *p = new Point;
    memset(p, 0, sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

Point *findPath::__getPath()
{
    Point *root = createNewPoint(start_point.x, start_point.y);
    openList.push_back(root);

    Point *current = root;
    Point *child = NULL;

    vector<Point *>::iterator it;    // 变化
    vector<Point *>::iterator itMin; // 记录最小的

    while (1)
    {
        getsurroundingPoints(current);

        it = openList.begin();
        itMin = openList.begin();

        for (; it != openList.end(); it++)
        {
            itMin = ((*itMin)->F < (*it)->F) ? itMin : it;
        }

        current = *itMin;
        openList.erase(itMin);

        if(end_point.x == current->x && end_point.y == current->y){
            return current;
        }
        if(openList.size() == 0){
            return NULL;
        }
        
    }
}

vector<Point *> findPath::getPath()
{
    Point *result = __getPath();
    vector<Point *> path;

    openList.clear();
    closeList.clear();

    return path;
}

Point *findPath::isInlist(const vector<Point *> &list, const Point *point) const
{
    for (auto p : list)
    {
        if (p->x == point->x && p->y == point->y)
            return p;
    }
    return NULL;
}

bool findPath::isCanreach(const Point point, const Point target) const
{
    if (target.x < 0 || target.x > width_ - 1 || target.y < 0 || target.y > height_ - 1 || _map[target.x][target.y] == 1 || (target.x == point.x && target.y == point.y))
        return false;
    else
    {
        if (_map[point.x][target.y] == 0 && _map[target.x][point.y] == 0)
            return true;
        else
            return false;
    }
}

void findPath::getsurroundingPoints(Point *point)
{
    vector<Point *> surroundPoint;

    for (int x = point->x - 1; x <= point->x + 1; x++)
    {
        for (int y = point->y - 1; x <= point->y + 1; y++)
        {
            Point p;
            p.x = x;
            p.y = y;
            if (isCanreach(*point, p))
            {
                Point *new_point = createNewPoint(x, y);
                
                new_point->parent = point;
                new_point->G=calcG(point, new_point);
                new_point->H = calcH(new_point);
                new_point->F = calcF(new_point);
                point->child.push_back(new_point);

                openList.push_back(new_point);
            }
        }
    }

    return;
}

double findPath::calcDistance(vector<Point *> path)
{
    return path.back()->G;
}