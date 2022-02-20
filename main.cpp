#include <iostream>
#include <nitro/lane.h>
#include <nitro/vec3.h>
#include <nitro/vector.h>
#include <random>
using namespace nitro;

void saxpy1(vector<vec3f> const &x, vector<vec3f> const &y, vector<vec3f> &z,
            float a) {
  for (int i = 0; i < x.size(); ++i)
    z[i] = a * x[i] + y[i];
}

void saxpy2(vector<vec3f> const &x, vector<vec3f> const &y, vector<vec3f> &z,
            float a) {
  for (int i = 0; i < x.num_lanes<8>(); ++i) {
    z.lane_at<8>(i) = a * x.lane_at<8>(i) + y.lane_at<8>(i);
  }
}

int main() {
  uint64_t seed = time(nullptr);
  std::default_random_engine eng(seed);
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

  int n = 1024;
  vector<vec3f> x(n), y(n);

  for (int i = 0; i < n; ++i) {
    x[i] = vec3f(dist(eng), dist(eng), dist(eng));
    y[i] = vec3f(dist(eng), dist(eng), dist(eng));
  }

  vector<vec3f> z1(n);
  saxpy1(x, y, z1, 2.0f);
  std::cout << "z1[512] = " << z1[512] << '\n';

  vector<vec3f> z2(n);
  saxpy2(x, y, z2, 2.0f);
  std::cout << "z2[512] = " << z2[512] << '\n';

  return 0;
}