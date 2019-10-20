#include "normal_median_filter_uint16.h"

#include <cstring>

#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
#include <arm_neon.h>
#endif // __ARM_NEON

static void sortArr(uint16_t *arr, int len) {
  const int middle = len / 2;
  for (int i = 0; i <= middle; ++i) {
    uint16_t min = arr[i];
    int min_idx = i;
    for (int j = i + 1; j < len; ++j) {
      if (min > arr[j]) {
        min_idx = j;
        min = arr[j];
      }
    }
    // swap idx i and min_idx
    uint16_t tmp = arr[min_idx];
    arr[min_idx] = arr[i];
    arr[i] = tmp;
  }
}

#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
static void sortC4ArrNeon(uint16_t *arr, int len) {
  const int actual_len = len / 8;
  const int middle = actual_len / 2;
  uint16_t *arr_ptr = arr;
  for (int i = 0; i <= middle; ++i) {
    uint16x8_t  min = vld1q_u16(arr_ptr);
    uint16x8_t   min_idx = vdupq_n_u16(i);

    uint16_t *inner_arr_ptr = arr_ptr + 8;
    for (int j = i + 1; j < actual_len; ++j) {
      uint16x8_t curr =  vld1q_u16(inner_arr_ptr);
      uint16x8_t   curr_idx = vdupq_n_u16(j);
      uint16x8_t  if_greater_than = vcgtq_u16(min, curr);
      min     = vbslq_u16(if_greater_than, curr, min);
      min_idx = vbslq_u16(if_greater_than, curr_idx, min_idx);
      inner_arr_ptr += 8;
    }
    // swap idx i and min_idx
    for (int q = 0; q < 8; ++q) {
      float tmp = arr[min_idx[q] * 8 + q];
      arr[min_idx[q] * 8 + q] = arr[i * 8 + q];
      arr[i * 8 + q] = tmp;
    }
    arr_ptr += 8;
  }
}
#endif // __ARM_NEON

void NormalMedianFilterUInt16::filter(const uint16_t  *input,
                                      const int        radius,
                                      const int        height,
                                      const int        width,
                                      uint16_t        *output) {
  const int w_start = -radius; 
  const int w_end   = radius;
  const int h_start = -radius; 
  const int h_end   = radius;

  const int w_first_loop_end    = std::max(0, -w_start);
  const int w_second_loop_start = w_first_loop_end;
  const int w_second_loop_end   = width - 1 - w_end;
  const int w_third_loop_start  = width - w_end;

  int neon_width = w_second_loop_end - w_second_loop_start + 1;
#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
  int nn = neon_width >> 3;
#else
  int nn = 0;
#endif
  int remain_start = (nn << 3) + w_second_loop_start;

  int out_idx = 0;
  for (int h = 0; h < height; ++h) {
    const int h_lower_bound = std::max(0, h + h_start);
    const int h_upper_bound = std::min(height - 1, h + h_end);
    const int h_interval = h_upper_bound - h_lower_bound + 1;

    for (int w = 0; w < w_first_loop_end; ++w) {
      const int w_left_bound = std::max(0, w + w_start);
      const int w_right_bound = std::min(width - 1, w + w_end);
      const int arr_len = h_interval * (w_right_bound - w_left_bound + 1);

      int idx = 0;
      for (int i = h_lower_bound; i <= h_upper_bound; ++i) {
        const int h_idx = i * width;
        for (int j = w_left_bound; j <= w_right_bound; ++j) {
          m_cache[idx ++] = input[h_idx + j];
        }
      }

      sortArr(m_cache.data(), arr_len);
      m_out_buffer[out_idx ++] = m_cache[arr_len / 2];
    }

#if defined(USE_NEON_INTRINSIC) && defined(__ARM_NEON)
    int neon_arr_len = h_interval * (w_end - w_start + 1) * 8;
    for (int w = w_second_loop_start; w < remain_start; w += 8) {
      const int w_left_bound = std::max(0, w + w_start);
      const int w_right_bound = std::min(width - 1, w + w_end);

      int idx = 0;
      for (int i = h_lower_bound; i <= h_upper_bound; ++i) {
        const int h_idx = i * width;
        for (int j = w_left_bound; j <= w_right_bound; ++j) {
          for (int q = 0; q < 8; ++q) {
            m_cache[idx ++] = input[h_idx + j + q];
          }
        }
      }

      sortC4ArrNeon(m_cache.data(), neon_arr_len);
      for (int i = 0; i < 8; ++i) {
        m_out_buffer[out_idx ++] = m_cache[(neon_arr_len / 8 / 2) * 8 + i];
      }
    }
#endif
    int remain_arr_len = h_interval * (w_end - w_start + 1);
    for (int w = remain_start; w <= w_second_loop_end; ++w) {
      const int w_left_bound = std::max(0, w + w_start);
      const int w_right_bound = std::min(width - 1, w + w_end);

      int idx = 0;
      for (int i = h_lower_bound; i <= h_upper_bound; ++i) {
        const int h_idx = i * width;
        for (int j = w_left_bound; j <= w_right_bound; ++j) {
          m_cache[idx ++] = input[h_idx + j];
        }
      }

      sortArr(m_cache.data(), remain_arr_len);
      m_out_buffer[out_idx ++] = m_cache[remain_arr_len / 2];
    }

    for (int w = w_third_loop_start; w < width; ++w) {
      const int w_left_bound = std::max(0, w + w_start);
      const int w_right_bound = std::min(width - 1, w + w_end);
      const int arr_len = h_interval * (w_right_bound - w_left_bound + 1);

      int idx = 0;
      for (int i = h_lower_bound; i <= h_upper_bound; ++i) {
        const int h_idx = i * width;
        for (int j = w_left_bound; j <= w_right_bound; ++j) {
          m_cache[idx ++] = input[h_idx + j];
        }
      }

      sortArr(m_cache.data(), arr_len);
      m_out_buffer[out_idx ++] = m_cache[arr_len / 2];
    }
  }

  const int panel_size = height * width;
  memcpy(output, m_out_buffer.data(), panel_size * sizeof(uint16_t));                     
}


