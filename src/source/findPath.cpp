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

using namespace std;
using namespace movingai;

#define LINE 1
#define SLIDE 1.414

void findPath::getMap()
{
    for (int i = 0; i < grid.height_; i++)
    {   
        for (int j = 0; j < grid.width_; j++)
        {
            map[i][j] = grid.is_obstacle({j,i});
        }
    }
}

enum dirent
{
    p_up,
    p_down,
    p_left,
    p_right,
    p_lup,
    p_ldown,
    p_rup,
    p_rdown
};

typedef struct Node
{
    Point pos;
    vector<Node *> child;
    Node *parent;
} Node;

Node *createTreeNode(int x, int y)
{
    Node *pNew = new Node;
    memset(pNew, 0, sizeof(Node));
    pNew->pos.x = x;
    pNew->pos.y = y;
    return pNew;
}

void freeNode(Node *root)
{
    vector<Node *>::iterator it = root->child.begin();
    for (; it < root->child.end(); it++)
    {
        freeNode(*it);
    }
    delete (root);
}

double getH(Point current, Point end)
{
    return abs((current.x - end.x) * (current.x - end.x)) + abs((current.y - end.y) * (current.y - end.y));
}

double findPath::getPath(Point start, Point end)
{
    bool pathMap[grid.height_][grid.width_] = {0};

    vector<Node *> buff;
    vector<Node *>::iterator it;    // 变化
    vector<Node *>::iterator itMin; // 记录最小的

    Node *root = createTreeNode(start.x, start.y);
    pathMap[start.x][start.y] = true;

    Node *current = root;
    Node *child = NULL;

    bool isFindEnd = true;

    while (1)
    {
        for (int i = 0; i < 8; i++)
        {
            child = createTreeNode(current->pos.x, current->pos.y);
            switch (i)
            {
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

            if (child->pos.x > 0 && child->pos.y > 0 && child->pos.x < grid.width_ && child->pos.y < grid.height_ && pathMap[child->pos.y][child->pos.x] == 0 && map[child->pos.y][child->pos.x] == 0 && map[child->pos.y][current->pos.x] == 0 && map[child->pos.x][current->pos.y] == 0)
            {
                child->pos.H = getH(child->pos, end);
                child->pos.F = child->pos.H + child->pos.G;
                current->child.push_back(child);
                child->parent = current;

                buff.push_back(child);
            }
            else
            {
                delete child;
            }
        }

        it = buff.begin();
        itMin = buff.begin();

        for (; it != buff.end(); it++)
        {
            itMin = ((*itMin)->pos.F < (*it)->pos.F) ? itMin : it;
        }

        current = *itMin;
        pathMap[current->pos.y][current->pos.x] = true;
        buff.erase(itMin);

        if (end.x == current->pos.x && end.y == current->pos.y)
        {
            break;
        }
        if (buff.size() == 0)
        {
            isFindEnd = false;
            break;
        }
    }

    if (isFindEnd)
    {
        double length = current->pos.G;
        freeNode(root);
        return length;
    }
    else
    {
        freeNode(root);
        return -1;
    }
}