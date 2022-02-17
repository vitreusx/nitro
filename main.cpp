#include <iostream>
#include <nitro/flat_repr.h>
#include <nitro/pprint.h>
#include <nitro/vec3.h>
#include <nitro/vector.h>
#include <random>

#define cf const float *
#define f float *
void saxpy0(cf xx, cf xy, cf xz, cf yx, cf yy, cf yz, f zx, f zy, f zz, float a,
            int n) {
  for (int i = 0; i < n; ++i) {
    zx[i] = a * xx[i] + yx[i];
    zy[i] = a * xy[i] + yy[i];
    zz[i] = a * xz[i] + yz[i];
  }
}
#undef f
#undef cf

#define cf const float *__restrict__
#define f float *__restrict__
void saxpy1(cf xx, cf xy, cf xz, cf yx, cf yy, cf yz, f zx, f zy, f zz, float a,
            int n) {
  for (int i = 0; i < n; ++i) {
    zx[i] = a * xx[i] + yx[i];
    zy[i] = a * xy[i] + yy[i];
    zz[i] = a * xz[i] + yz[i];
  }
}
#undef f
#undef cf

void saxpy2(vector<vec3f> const &x, vector<vec3f> const &y, vector<vec3f> &z,
            float a) {
  for (int i = 0; i < x.size(); ++i) {
    z[i] = a * x[i] + y[i];
  }
}

#define cf const float *__restrict__
#define f float *__restrict__
void saxpy3(cf xx, cf xy, cf xz, cf yx, cf yy, cf yz, f zx, f zy, f zz, float a,
            int n) {
  saxpy0(xx, xy, xz, yx, yy, yz, zx, zy, zz, a, n);
}
#undef f
#undef cf

int main() {
  uint64_t seed = time(nullptr);
  std::default_random_engine  eng(seed);
  std::uniform_real_distribution<float> dist;

  vector<vec3f> x(10), y(10), z(10);
  for (int i = 0; i < 10; ++i) {
    x[i] = vec3f { dist(eng), dist(eng), dist(eng) };
    y[i] = vec3f { dist(eng), dist(eng), dist(eng) };
  }

  saxpy2(x, y, z, 2.0f);
  for (int i = 0; i < 10; ++i)
    std::cout << "z[" << i << "] = " << z[i] << '\n';

  return 0;
}