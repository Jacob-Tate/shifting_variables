//
// Created by night on 5/9/19.
//

#include "shifting_variables.h"

int main()
{
  int z = 1;
  shifting_var<int> x;

  while(true)
  {
    x += z;
    std::cout << "Z: " << z << " X: " << x << "\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}