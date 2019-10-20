#ifndef SRC_CONSTANT_TIME_MEDIAN_FILTER_UINT16_H_
#define SRC_CONSTANT_TIME_MEDIAN_FILTER_UINT16_H_

#include <vector>
#include <cstdint>

static const int BIT_LEVEL = 8;
static const int HISTOGRAM_LEN = 2 << BIT_LEVEL;

class ConstantTimeMedianFilterUInt16 {
public:
  ConstantTimeMedianFilterUInt16() {}

  ~ConstantTimeMedianFilterUInt16() {
    free();
  }

  void init(int height, int width, int radius) {
    free();
    m_histograms.resize(HISTOGRAM_LEN * (width + 1));
    m_out_buffer.resize(height * width);
  }

  void filter(const uint16_t  *input,
              const int        radius,
              const int        height,
              const int        width,
              uint16_t        *output);

private:
  void free() {
    std::vector<uint16_t>().swap(m_histograms);
  }

private:
  std::vector<uint16_t> m_histograms;
  std::vector<uint16_t> m_out_buffer;

};

#endif // SRC_CONSTANT_TIME_MEDIAN_FILTER_UINT16_H_
