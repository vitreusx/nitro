#include <iostream>
#include <nitro/nitro.h>
#include <random>
using namespace nitro;

int main() {
  set<int> x;
  for (int i = 0; i < 10; ++i)
    x.insert(i);

  std::cout << "before removal" << '\n';
  for (int i = 0; i < 10; ++i) {
    if (x[i].has_item())
      std::cout << "x[" << i << "] = " << x[i].item() << '\n';
  }

  for (int i = 0; i < 10; i += 2)
    x[i].remove();

  std::cout << "after removal" << '\n';
  for (int i = 0; i < 10; ++i) {
    if (x[i].has_item())
      std::cout << "x[" << i << "] = " << x[i].item() << '\n';
  }

  return 0;
}