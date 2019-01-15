#include "eg.h"
#include <string>

int main(int argc, char* argv[])
{
  std::string s;
  std::getline(std::cin, s);

  EG g1 = EG();
  int i = 0;
  g1.read(s,i);

  if(g1.check_consistency(0))
    std::cout << "consistent\n";
  else
    std::cout << "inconsistent\n";

  return 0;
}
