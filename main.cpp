#include <iostream>

#include "BigInt.h"

int main() {
  BigInt n, m;
  std::cin >> n >> m;
  std::cout << (n * m) + (m & n);
  return 0;
}
