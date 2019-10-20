#include "constant_time_median_filter_uint16.h"

#include <cstring>
#include <iostream>

#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
#include <arm_neon.h>
#endif // __ARM_NEON

static void add_histogram(uint16_t *histo1, uint16_t *histo2, int histo_len) {

  int n = 0;
#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
  n = histo_len >> 3;
#endif // __ARM_NEON
  int remain = histo_len - (n << 3);

  uint16_t *histo1_ptr = histo1;
  uint16_t *histo2_ptr = histo2;
#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
  for (; n > 0; n--) {
    uint16x8_t histo1_neon = vld1q_u16(histo1_ptr);
    uint16x8_t histo2_neon = vld1q_u16(histo2_ptr);

    uint16x8_t _sum = vaddq_u16(histo1_neon, histo2_neon);

    vst1q_u16(histo1_ptr, _sum);

    histo1_ptr += 8;
    histo2_ptr += 8;
  }
#endif // __ARM_NEON
  for (; remain > 0; --remain) {
    *histo1_ptr = *histo1_ptr + *histo2_ptr;
    histo1_ptr ++;
    histo2_ptr ++;
  }
}

static void sub_histogram(uint16_t *histo1, uint16_t *histo2, int histo_len) {

  int n = 0;
#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
  n = histo_len >> 3;
#endif // __ARM_NEON
  int remain = histo_len - (n << 3);

  uint16_t *histo1_ptr = histo1;
  uint16_t *histo2_ptr = histo2;
#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
  for (; n > 0; n--) {
    uint16x8_t histo1_neon = vld1q_u16(histo1_ptr);
    uint16x8_t histo2_neon = vld1q_u16(histo2_ptr);

    uint16x8_t _sub = vsubq_u16(histo1_neon, histo2_neon);

    vst1q_u16(histo1_ptr, _sub);

    histo1_ptr += 8;
    histo2_ptr += 8;
  }
#endif // __ARM_NEON
  for (; remain > 0; --remain) {
    *histo1_ptr = *histo1_ptr - *histo2_ptr;
    histo1_ptr ++;
    histo2_ptr ++;
  }
}

static uint16_t get_median(uint16_t *histogram, int histo_len, int window_half_len) {
  int idx = 0;
  uint16_t sum = 0;
  for (int i = 0; i < histo_len; ++i) {
    sum += histogram[i];
    if ((int)sum >= window_half_len) {
      idx = i;
      break;
    }
  }
  return idx;
}

void ConstantTimeMedianFilterUInt16::filter(const uint16_t  *input,
                                            const int        radius,
                                            const int        height,
                                            const int        width,
                                            uint16_t        *output) {

  std::fill(m_histograms.begin(), m_histograms.end(), 0);

  const int window_half_len = ((radius * 2 + 1) * (radius * 2 + 1)) / 2;
  const int w_start = -radius; 
  const int w_end   = radius;
  const int h_start = -radius; 
  const int h_end   = radius;

  const int w_first_loop_end    = std::max(0, -w_start);
  const int w_second_loop_start = w_first_loop_end;
  const int w_second_loop_end   = width - 1 - w_end;
  const int w_third_loop_start  = width - w_end;

  const int h_first_loop_end    = std::max(0, -h_start);
  const int h_second_loop_start = h_first_loop_end + 1;
  const int h_second_loop_end   = height - 1 - h_end;
  const int h_third_loop_start  = height - h_end;

  const uint16_t *row_ptr = input;
  // init col histograms
  for (int h = 0; h < h_end; ++h) {
    uint16_t *histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = 0; w < width; ++w) {
      histo_ptr[(int)row_ptr[w]] += 1;
      histo_ptr += HISTOGRAM_LEN;
    } 
    row_ptr += width;
  }

  uint16_t *kernel_histogram_ptr = m_histograms.data();
  uint16_t *out_ptr = m_out_buffer.data();
  for (int h = 0; h <= h_first_loop_end; ++h) {
    // clear kernel histogram
    std::fill(m_histograms.begin(), m_histograms.begin() + HISTOGRAM_LEN, 0);
    
    // update col histograms only add
    uint16_t *col_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = 0; w < width; ++w) {
      col_histo_ptr[(int)row_ptr[w]] += 1;
      col_histo_ptr += HISTOGRAM_LEN;
    } 
    row_ptr += width;

    // update kernel histogram & calcualte median
    // first init kernel histogram
    col_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = 0; w < w_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);
      col_histo_ptr += HISTOGRAM_LEN;
    }

    for (int w = 0; w <= w_first_loop_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      col_histo_ptr += HISTOGRAM_LEN;
    }

    uint16_t *sub_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = w_second_loop_start; w <= w_second_loop_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);
      sub_histogram(kernel_histogram_ptr, sub_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      col_histo_ptr += HISTOGRAM_LEN;
      sub_histo_ptr += HISTOGRAM_LEN;
    }

    for (int w = w_third_loop_start; w < width; ++w) {
      sub_histogram(kernel_histogram_ptr, sub_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      sub_histo_ptr += HISTOGRAM_LEN;
    }

    out_ptr += width;
  }

  const uint16_t *sub_row_ptr = input;
  for (int h = h_second_loop_start; h <= h_second_loop_end; ++h) {
    // update col histograms add & sub
    std::fill(m_histograms.begin(), m_histograms.begin() + HISTOGRAM_LEN, 0);

    // update col histograms only add
    uint16_t *col_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = 0; w < width; ++w) {
      col_histo_ptr[(int)row_ptr[w]] += 1;
      col_histo_ptr[(int)sub_row_ptr[w]] -= 1;

      col_histo_ptr += HISTOGRAM_LEN;
    }
    row_ptr += width;
    sub_row_ptr += width;

    // update kernel histogram & calcualte median
    // first init kernel histogram
    col_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = 0; w < w_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);
      col_histo_ptr += HISTOGRAM_LEN;
    }

    for (int w = 0; w <= w_first_loop_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      col_histo_ptr += HISTOGRAM_LEN;
    }

    uint16_t *sub_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = w_second_loop_start; w <= w_second_loop_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);
      sub_histogram(kernel_histogram_ptr, sub_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      col_histo_ptr += HISTOGRAM_LEN;
      sub_histo_ptr += HISTOGRAM_LEN;
    }

    for (int w = w_third_loop_start; w < width; ++w) {
      sub_histogram(kernel_histogram_ptr, sub_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      sub_histo_ptr += HISTOGRAM_LEN;
    }

    out_ptr += width;
  }

  for (int h = h_third_loop_start; h < height; ++h) {
    // update col histograms only sub
    std::fill(m_histograms.begin(), m_histograms.begin() + HISTOGRAM_LEN, 0);

    // update col histograms only add
    uint16_t *col_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = 0; w < width; ++w) {
      col_histo_ptr[(int)sub_row_ptr[w]] -= 1;

      col_histo_ptr += HISTOGRAM_LEN;
    }
    sub_row_ptr += width;

    // update kernel histogram & calcualte median
    // first init kernel histogram
    col_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = 0; w < w_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);
      col_histo_ptr += HISTOGRAM_LEN;
    }

    for (int w = 0; w <= w_first_loop_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      col_histo_ptr += HISTOGRAM_LEN;
    }

    uint16_t *sub_histo_ptr = m_histograms.data() + HISTOGRAM_LEN;
    for (int w = w_second_loop_start; w <= w_second_loop_end; ++w) {
      add_histogram(kernel_histogram_ptr, col_histo_ptr, HISTOGRAM_LEN);
      sub_histogram(kernel_histogram_ptr, sub_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      col_histo_ptr += HISTOGRAM_LEN;
      sub_histo_ptr += HISTOGRAM_LEN;
    }

    for (int w = w_third_loop_start; w < width; ++w) {
      sub_histogram(kernel_histogram_ptr, sub_histo_ptr, HISTOGRAM_LEN);

      // cal median
      out_ptr[w] = get_median(kernel_histogram_ptr, HISTOGRAM_LEN, window_half_len);

      sub_histo_ptr += HISTOGRAM_LEN;
    }

    out_ptr += width;
  }

  const int panel_size = height * width;
  memcpy(output, m_out_buffer.data(), panel_size * sizeof(uint16_t));                     
}


