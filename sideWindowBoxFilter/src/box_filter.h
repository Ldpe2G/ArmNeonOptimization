#ifndef SRC_BOX_FILTER_H_
#define SRC_BOX_FILTER_H_

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

    int panel_size = height * width;
    std::vector<float> ones(panel_size, 1.0f);
    scales.resize(panel_size);

    _fastBoxFilterImpl(ones.data(), radius, height, width, scales.data());

    for (int j = 0; j < panel_size; ++j) {
      scales[j] = 1.0f / scales[j];
    }
  }

  void fastBoxFilter(const float  *input,
                     const int     radius,
                     const int     height,
                     const int     width,
                     float        *output);

private:

  void _fastBoxFilterImpl(const float *input,
                          const int    radius,
                          const int    height,
                          const int    width,
                          float       *output);

  void free() {
    std::vector<float>().swap(cache);
    std::vector<float>().swap(colSum);
  }

private:
  std::vector<float> cache;
  std::vector<float> colSum;
  std::vector<float> scales;

};

#endif // SRC_BOX_FILTER_H_
