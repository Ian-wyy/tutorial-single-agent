#include "findPath.hpp"
#include "load_scens.hpp"
#include <iostream>
#include <string>

void test_scen_io(std::string filename1, std::string filename2) {
  /* movingai::scenario_manager scenmgr;
  scenmgr.load_scenario(filename);

  for (int i=0; i<scenmgr.num_experiments(); i++) {
    auto expr = scenmgr.get_experiment(i);
    std::cout << expr->mapheight() << " " << expr->mapwidth() << " "
              << expr->startx() << " " << expr->starty() << " "
              << expr->goalx() << " " << expr->goaly() << " "
              << expr->distance() << std::endl;

  } */

  findPath testA(filename1, filename2);

  int flag = 1;

  for (int i = 0; i < testA.scen.num_experiments(); i++) {
    auto expr = testA.scen.get_experiment(i);
    Point start = {(int)expr->startx(), (int)expr->starty()};
    Point end = {(int)expr->goalx(), (int)expr->goaly()};
    if (abs(testA.getPath(start, end) - expr->distance()) < 1) {
      continue;
    } else {
      flag = 0;
      cout<<start.x<<", "<<start.y<<endl;
      cout<<end.x<<", "<<end.y<<endl;
      cout<<"testA: "<<testA.getPath(start, end)<<" answer: "<<expr->distance()<<endl;
    }
    /* std::cout << testA.getPath(start, end) << " " << expr->distance()
              << std::endl; */
  }
  if (flag) {
    cout << "OK" << endl;
  } else {
    cout << "ERROR" << endl;
  }
}

int main(int argc, char **argv) {
  std::string filename1 = std::string(argv[1]);
  std::string filename2 = std::string(argv[2]);
  test_scen_io(filename1, filename2);
}
