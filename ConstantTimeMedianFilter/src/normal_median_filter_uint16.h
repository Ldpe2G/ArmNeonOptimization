#ifndef SRC_NORMAL_MEDIAN_FILTER_UINT16_H_
#define SRC_NORMAL_MEDIAN_FILTER_UINT16_H_

#include <vector>
#include <cstdint>

class NormalMedianFilterUInt16 {
public:
  NormalMedianFilterUInt16() {}

  ~NormalMedianFilterUInt16() {
    free();
  }

  void init(int height, int width, int radius) {
    free();
    m_cache.resize((2 * radius + 1) * (2 * radius + 1) * 8);
    m_out_buffer.resize(height * width);
  }

  void filter(const uint16_t  *input,
              const int        radius,
              const int        height,
              const int        width,
              uint16_t        *output);

private:
  void free() {
    std::vector<uint16_t>().swap(m_cache);
  }

private:
  std::vector<uint16_t> m_cache;
  std::vector<uint16_t> m_out_buffer;

};

#endif // SRC_NORMAL_MEDIAN_FILTER_UINT16_H_
