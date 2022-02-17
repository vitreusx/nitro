#include <iostream>
#include <nitro/vec3.h>
#include <nitro/vector.h>
#include <random>

int main() {
  size_t seed = time(nullptr);
  std::default_random_engine eng(seed);
  std::uniform_real_distribution<float> dist;

  vector<vec3f> v(10);
  for (int i = 0; i < v.size(); ++i)
    v[i] = vec3f(dist(eng), dist(eng), dist(eng));

  for (int i = 0; i < v.size(); ++i)
    std::cout << v[i] << '\n';

  return 0;
}