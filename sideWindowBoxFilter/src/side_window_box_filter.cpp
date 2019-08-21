#include "side_window_box_filter.h"

#undef __ARM_NEON

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

void SideWindowBoxFilter::fastSideWindowBoxFilter(const float  *input,
                                                  const int    radius,
                                                  const int    height,
                                                  const int    width,
                                                  float        *output) {
  int panel_size = height * width;       

  for (int i = 0; i < side_box_directions_count; ++i) {
    _sideWindowBoxFilterImpl(input, radius, height, width, static_cast<SIDE_BOX_DIRECTIONS>(i), m_side_window_box_filter_results[i].data());

    float *result_ptr = m_side_window_box_filter_results[i].data();
    float *scale_ptr  = m_side_window_box_filter_scales[i].data();
    
    for (int j = 0; j < panel_size; ++j) {
      result_ptr[j] *= scale_ptr[j];
    }
  }

  for (int i = 0; i < panel_size; ++i) {
    float tmp     = m_side_window_box_filter_results[0][i] - input[i];
    float min     = tmp * tmp;
    int   min_idx = 0;
    for (int j = 1; j < side_box_directions_count; ++j) {
      tmp = m_side_window_box_filter_results[j][i] - input[i];
      tmp = tmp * tmp;
      if (min > tmp) {
        min_idx = j;
        min = tmp;
      }
    }

    output[i] = m_side_window_box_filter_results[min_idx][i];
  }
                                                
}

void SideWindowBoxFilter::_sideWindowBoxFilterImpl(const float                *input,
                                                   const int                  radius,
                                                   const int                  height, 
                                                   const int                  width,
                                                   const SIDE_BOX_DIRECTIONS  direction,
                                                   float                      *output) {
                              
  const int w_start = m_side_window_params[static_cast<int>(direction)][static_cast<int>(XY_START_END::WIDTH_START)];
  const int w_end   = m_side_window_params[static_cast<int>(direction)][static_cast<int>(XY_START_END::WIDTH_END)];
  const int h_start = m_side_window_params[static_cast<int>(direction)][static_cast<int>(XY_START_END::HEIGHT_START)];
  const int h_end   = m_side_window_params[static_cast<int>(direction)][static_cast<int>(XY_START_END::HEIGHT_END)];

  float *cache_ptr = &(m_cache[0]);
  
  const int w_first_loop_end    = std::max(0, -w_start);
  const int w_second_loop_start = w_first_loop_end + 1;
  const int w_second_loop_end   = width - 1 - w_end;
  const int w_third_loop_start  = width - w_end;

  // sum horizonal
  for (int h = 0; h < height; ++h) {
    int shift = h * width;

    float tmp = 0;
    for (int w = 0; w < w_end; ++w) {
      tmp += input[shift + w];
    }

    for (int w = 0; w <= w_first_loop_end; ++w) {
      tmp += input[shift + w + w_end];
      cache_ptr[shift + w] = tmp;
    }

    for (int w = w_second_loop_start; w <= w_second_loop_end; ++w) {
      tmp += input[shift + w + w_end];
      tmp -= input[shift + w + w_start - 1];
      cache_ptr[shift + w] = tmp;
    }

    for (int w = w_third_loop_start; w < width; ++w) {
      tmp -= input[shift + w + w_start - 1];
      cache_ptr[shift + w] = tmp;
    }
  }

  std::fill(m_col_sum.begin(), m_col_sum.end(), 0);
  float *col_sum_ptr = &(m_col_sum[0]);

  const int h_first_loop_end    = std::max(0, -h_start);
  const int h_second_loop_start = h_first_loop_end + 1;
  const int h_second_loop_end   = height - 1 - h_end;
  const int h_third_loop_start  = height - h_end;

  // sum vertical
  for (int h = 0; h < h_end; ++h) {
    int shift = h * width;
    for (int w = 0; w < width; ++w) {
      col_sum_ptr[w] += cache_ptr[shift + w];
    }
  }

  for (int h = 0; h <= h_first_loop_end; ++h) {
    float *add_ptr = cache_ptr + (h + h_end) * width;
    int shift = h * width;
    float *out_ptr = output + shift; 
    for (int w = 0; w < width; ++w) {
      col_sum_ptr[w] += add_ptr[w];
      out_ptr[w] = col_sum_ptr[w];
    }
  }

  int n = 0;
#ifdef __ARM_NEON
  n = width >> 2;
#endif // __ARM_NEON
  int re  = width - (n << 2);

  for (int h = h_second_loop_start; h <= h_second_loop_end; ++h) {
    float *add_ptr  = cache_ptr + (h + h_end) * width;
    float *sub_ptr  = cache_ptr + (h + h_start - 1) * width;
    int shift       = h * width;
    float *out_ptr  = output + shift;

    int index_w             = 0;
    float *tmp_out_ptr      = out_ptr;
    float *tmp_col_sum_ptr  = col_sum_ptr;
    float *tmp_add_ptr      = add_ptr;
    float *tmp_sub_ptr      = sub_ptr;
    int remain              = re;

#ifdef __ARM_NEON 
    int nn                  = n;
    for (; nn > 0; nn--) {
      float32x4_t _add = vld1q_f32(tmp_add_ptr);
      float32x4_t _sub = vld1q_f32(tmp_sub_ptr);
      float32x4_t _col_sum = vld1q_f32(tmp_col_sum_ptr);

      float32x4_t _tmp = vaddq_f32(_col_sum, _add);
      _tmp = vsubq_f32(_tmp, _sub);

      vst1q_f32(tmp_col_sum_ptr, _tmp);
      vst1q_f32(tmp_out_ptr, _tmp);

      tmp_add_ptr += 4;
      tmp_sub_ptr += 4;
      tmp_col_sum_ptr += 4;
      tmp_out_ptr += 4;
    }
#endif // __ARM_NEON

    for (; remain > 0; --remain) {
      *tmp_col_sum_ptr += *tmp_add_ptr;
      *tmp_col_sum_ptr -= *tmp_sub_ptr;
      *tmp_out_ptr = *tmp_col_sum_ptr;
      tmp_add_ptr ++;
      tmp_col_sum_ptr ++;
      tmp_out_ptr ++;
      tmp_sub_ptr ++;
    }
  }

  for (int h = h_third_loop_start; h < height; ++h) {
    float *sub_ptr = cache_ptr + (h + h_start - 1) * width;
    int shift = h * width;
    float *out_ptr = output + shift; 
    for (int w = 0; w < width; ++w) {
      col_sum_ptr[w] -= sub_ptr[w];
      out_ptr[w] = col_sum_ptr[w];
    }
  }                                                   
}
