#pragma once

#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include "gridmap.hpp"
#include "load_scens.hpp"

using namespace movingai;
using namespace std;

class findPath
{

public:
    findPath(const string &filename1, const string &filename2) : grid(filename1)
    {
        scen.load_scenario(filename2);
        map = (int **)malloc(sizeof(int *) * grid.height_);
        for (int i = 0; i < grid.height_; i++)
        {
            map[i] = (int *)malloc(sizeof(int) * grid.width_);
        }
        getMap();
    }
    ~findPath()
    {
        for (int i = 0; i < grid.height_; i++)
        {
            free(map[i]);
        }
        free(map);
    }
    void getMap();
    double getPath(Point start, Point end);

    gridmap grid;
    scenario_manager scen;
    int **map;
};