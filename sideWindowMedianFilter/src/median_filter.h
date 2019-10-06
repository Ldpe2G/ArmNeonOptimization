#ifndef SRC_MEDIAN_FILTER_H_
#define SRC_MEDIAN_FILTER_H_

#include <vector>

class MedianFilter {
public:
  MedianFilter() {}

  ~MedianFilter() {
    free();
  }

  void init(int height, int width, int radius) {
    free();
    m_cache.resize((2 * radius + 1) * (2 * radius + 1) * 4);
    m_out_buffer.resize(height * width);
  }

  void fastMedianFilter(const float  *input,
                        const int     radius,
                        const int     height,
                        const int     width,
                        float        *output);

private:
  void free() {
    std::vector<float>().swap(m_cache);
  }

private:
  std::vector<float> m_cache;
  std::vector<float> m_out_buffer;

};

#endif // SRC_MEDIAN_FILTER_H_
