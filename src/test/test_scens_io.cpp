#include "findPath.hpp"
#include "load_scens.hpp"
#include "rand_select.hpp"
#include <chrono>
#include <iostream>
#include <string>

#define SELECT_N 20
#define LIMIT 5
#define RAND_SELECT 1
#define CHECK_VALID 1

void test_scen_io(std::string filename1, std::string filename2) {

  int n = SELECT_N;
  if (RAND_SELECT) {
    randSelect(filename2.c_str(), n);
    filename2 = "test_seg";
  }

  findPath testA(filename1, filename2);
  testA.test_Validate(LIMIT);
}

int main(int argc, char **argv) {
  std::string filename1 = std::string(argv[1]);
  std::string filename2 = std::string(argv[2]);
  test_scen_io(filename1, filename2);
}
