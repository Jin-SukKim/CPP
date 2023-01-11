// Basic usage of Reference

#include <iostream>

// simpler context than pointer
int change_val(int &p) {
  p = 3;

  return 0;
}
int main() {
  int number = 5;

  std::cout << number << std::endl;
  change_val(number);
  std::cout << number << std::endl;
}

// or reference function that returns reference
// advantage : if function's parameter struck is large, refence can return address. No matter what size of type is.
// Pointer can do the same task but it takes memory space.

int& function(int& a) {
  a = 5;
  return a;
}

int main() {
  int b = 2;
  int c = function(b);
  return 0;
}



