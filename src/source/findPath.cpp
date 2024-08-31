#include "load_scens.hpp"
#include "gridmap.hpp"
#include "findPath.hpp"

#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <math.h>

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

double findPath::calcH(Point *point, Point *end)
{
    return sqrt((double)(end->x - point->x) * (double)(end->x - point->x) + (double)(end->y - point->y) * (double)(end->y - point->y));
}

double findPath::calcF(Point *point)
{
    return point->G + point->H;
}

Point *findPath::getLeastFpoint()
{
    if (!openList.empty())
    {
        auto resPoint = openList.front();
        for (auto &point : openList)
        {
            if (point->F < resPoint->F)
                resPoint = point;
        }

        return resPoint;
    }
    return NULL;
}

Point *findPath::__getPath(Point &start_point, Point &endPoint)
{
    openList.push_back(new Point(start_point.x, start_point.y));
    while (!openList.empty())
    {
        auto curPoint = getLeastFpoint();
        openList.remove(curPoint);
        closeList.push_back(curPoint);

        auto surroundingPoint = getsurroundingPoints(curPoint);

        for (auto &target : surroundingPoint)
        {
            if (!isInlist(openList, target))
            {
                target->parent = curPoint;

                target->G = calcG(curPoint, target);
                target->H = calcH(target, &endPoint);
                target->F = calcF(target);

                openList.push_back(target);
            }
            else
            {
                int tempG = calcG(curPoint, target);
                if (tempG < target->G)
                {
                    target->parent = curPoint;

                    target->G = tempG;
                    target->F = calcF(target);
                }
            }
            Point *resPoint = isInlist(openList, &endPoint);
            if (resPoint)
                return resPoint;
        }
    }

    return NULL;
}

list<Point *> findPath::getPath(Point &startPoint, Point &endPoint)
{
    Point *result = __getPath(startPoint, endPoint);
    list<Point *> path;

    while (result)
    {
        path.push_front(result);
        result = result->parent;
    }

    openList.clear();
    closeList.clear();

    return path;
}

Point *findPath::isInlist(const list<Point *> &list, const Point *point) const
{
    for (auto p : list)
    {
        if (p->x == point->x && p->y == point->y)
            return p;
    }
    return NULL;
}

bool findPath::isCanreach(const Point *point, const Point *target) const
{
    if (target->x < 0 || target->x > width_ - 1 || target->y < 0 || target->y > height_ - 1 || _map[target->x][target->y] == 1 || (target->x == point->x && target->y == point->y) || isInlist(closeList, target))
        return false;
    else
    {
        if (_map[point->x][target->y] == 0 && _map[target->x][point->y] == 0)
            return true;
        else
            return false;
    }
}

vector<Point *> findPath::getsurroundingPoints(const Point *point) const
{
    vector<Point *> surroundPoint;

    for (int x = point->x - 1; x <= point->x + 1; x++)
    {
        for (int y = point->y - 1; x <= point->y + 1; y++)
        {
            if (isCanreach(point, new Point(x, y)))
                surroundPoint.push_back(new Point(x, y));
        }
    }

    return surroundPoint;
}

double findPath::calcDistance(list<Point*> path){
    return path.back()->G;
}