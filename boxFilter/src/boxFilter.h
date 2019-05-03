#include <vector>

class BoxFilter {
public:
  BoxFilter() {}
  ~BoxFilter() {
    free();
  }

  void init(int height, int width, int radius) {
    free();
    cache.resize(height * width);
    colSum.resize(width);
  }

  void filter(float *input, int radius, int height, int width, float *output);

  void fastFilter(float *input, int radius, int height, int width, float *output);

  void fastFilterV2(float *input, int radius, int height, int width, float *output);

  void fastFilterV2NeonIntrinsics(float *input, int radius, int height, int width, float *output);

  void fastFilterV2NeonAsm(float *input, int radius, int height, int width, float *output);

  void fastFilterV2NeonAsmV2(float *input, int radius, int height, int width, float *output);

  void fastFilterV2NeonAsmV3(float *input, int radius, int height, int width, float *output);


private:
  void free() {
    std::vector<float>().swap(cache);
    std::vector<float>().swap(colSum);
  }

private:
  std::vector<float> cache;
  std::vector<float> colSum;
};

