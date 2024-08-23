#include "gridmap.hpp"
#include "load_scens.hpp"

using namespace movingai;

gridmap::gridmap(vid h, vid w)
{
  this->db.resize(h * w);
  fill(this->db.begin(), this->db.end(), false);
};

gridmap::gridmap(const string &filename)
{
  gm_parser parer(filename);
  this->height_ = parer.get_header().height_;
  this->width_ = parer.get_header().width_;
  this->db.resize(this->height_ * this->width_);
  for (vid i = 0; i < this->db.size(); i++)
  {
    auto c = parer.get_tile_at(i);
    this->db[i] = traversable(c) ? 0 : 1;
  }
}

vector<State> gridmap::get_neighbours(State c)
{
  // TODO: get neighbours of an 8-connected grid;
  auto res = vector<State>{};

  if (c.x - 1 > 0 && c.y - 1 > 0)
  {
    res.push_back({c.x - 1, c.y - 1});
  }

  if (c.x - 1 > 0)
  {
    res.push_back({c.x - 1, c.y});
  }

  if (c.x - 1 > 0 && c.y + 1 < width_)
  {
    res.push_back({c.x - 1, c.y + 1});
  }

  if (c.y + 1 < width_)
  {
    res.push_back({c.x, c.y + 1});
  }

  if (c.y + 1 < width_ && c.x + 1 < height_)
  {
    res.push_back({c.x + 1, c.y + 1});
  }

  if (c.x + 1 < height_)
  {
    res.push_back({c.x + 1, c.y});
  }

  if (c.x + 1 < height_ && c.y - 1 > 0)
  {
    res.push_back({c.x + 1, c.y - 1});
  }

  if (c.y - 1 > 0)
  {
    res.push_back({c.x, c.y - 1});
  }

  return res;
}
