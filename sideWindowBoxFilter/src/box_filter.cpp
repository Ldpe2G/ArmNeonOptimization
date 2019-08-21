#include "box_filter.h"
#include <chrono>
#include <random>
#include <iostream>

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

void BoxFilter::fastBoxFilter(const float  *input,
                              const int     radius,
                              const int     height,
                              const int     width,
                              float        *output) {

    _fastBoxFilterImpl(input, radius, height, width, output);
    int panel_size = height * width;
    for (int j = 0; j < panel_size; ++j) {
      output[j] = output[j] * scales[j];
    }
}

#if __ARM_NEON

void BoxFilter::_fastBoxFilterImpl(const float *input,
                                   const int    radius,
                                   const int    height,
                                   const int    width,
                                   float       *output) {
  float *cachePtr = &(cache[0]);
  // sum horizonal
  for (int h = 0; h < height; ++h) {
    int shift = h * width;

    float tmp = 0;
    for (int w = 0; w < radius; ++w) {
      tmp += input[shift + w];
    }

    for (int w = 0; w <= radius; ++w) {
      tmp += input[shift + w + radius];
      cachePtr[shift + w] = tmp;
    }

    int start = radius + 1;
    int end = width - 1 - radius;
    for (int w = start; w <= end; ++w) {
      tmp += input[shift + w + radius];
      tmp -= input[shift + w - radius - 1];
      cachePtr[shift + w] = tmp;
    }

    start = width - radius;
    for (int w = start; w < width; ++w) {
      tmp -= input[shift + w - radius - 1];
      cachePtr[shift + w] = tmp;
    }
  }

  float *colSumPtr = &(colSum[0]);

  for (int indexW = 0; indexW < width; ++indexW) {
    colSumPtr[indexW] = 0;
  } 

  int n = width >> 2;
  int re = width - (n << 2);
  // sum vertical
  for (int h = 0; h < radius; ++h) {
    int shift = h * width;

    float *tmpColSumPtr = colSumPtr;
    float *tmpCachePtr = cachePtr + shift;
    int indexW = 0;

    int nn = n;
    int remain = re;
    for (; nn > 0; nn--) {
      float32x4_t _colSum = vld1q_f32(tmpColSumPtr);
      float32x4_t _cache = vld1q_f32(tmpCachePtr);

      float32x4_t _tmp = vaddq_f32(_colSum, _cache);

      vst1q_f32(tmpColSumPtr, _tmp); 

      tmpColSumPtr += 4;
      tmpCachePtr += 4;
    }
    for (; remain > 0; --remain) {
      *tmpColSumPtr += *tmpCachePtr;
      tmpColSumPtr ++;
      tmpCachePtr ++;
    }
  }

  for (int h = 0; h <= radius; ++h) {
    float *addPtr = cachePtr + (h + radius) * width;
    int shift = h * width;
    float *outPtr = output + shift; 
    int indexW = 0;

    float *tmpOutPtr = outPtr;
    float *tmpColSumPtr = colSumPtr;
    float *tmpAddPtr = addPtr;

    int nn = n;
    int remain = re;
    for (; nn > 0; nn--) {
      float32x4_t _add = vld1q_f32(tmpAddPtr);
      float32x4_t _colSum = vld1q_f32(tmpColSumPtr);

      float32x4_t _tmp = vaddq_f32(_colSum, _add);

      vst1q_f32(tmpColSumPtr, _tmp);
      vst1q_f32(tmpOutPtr, _tmp);

      tmpAddPtr += 4;
      tmpColSumPtr += 4;
      tmpOutPtr += 4;
    }
    for (; remain > 0; --remain) {
      *tmpColSumPtr += *tmpAddPtr;
      *tmpOutPtr = *tmpColSumPtr;
      tmpAddPtr ++;
      tmpColSumPtr ++;
      tmpOutPtr ++;
    }

  }

  int start = radius + 1;
  int end = height - 1 - radius;
  for (int h = start; h <= end; ++h) {
    float *addPtr = cachePtr + (h + radius) * width;
    float *subPtr = cachePtr + (h - radius - 1) * width;
    int shift = h * width;
    float *outPtr = output + shift; 
    int indexW = 0;
    float *tmpOutPtr = outPtr;
    float *tmpColSumPtr = colSumPtr;
    float *tmpAddPtr = addPtr;
    float *tmpSubPtr = subPtr;

    int nn = n;
    int remain = re;
    for (; nn > 0; nn--) {
      float32x4_t _add = vld1q_f32(tmpAddPtr);
      float32x4_t _sub = vld1q_f32(tmpSubPtr);
      float32x4_t _colSum = vld1q_f32(tmpColSumPtr);

      float32x4_t _tmp = vaddq_f32(_colSum, _add);
      _tmp = vsubq_f32(_tmp, _sub);

      vst1q_f32(tmpColSumPtr, _tmp);
      vst1q_f32(tmpOutPtr, _tmp);

      tmpAddPtr += 4;
      tmpSubPtr += 4;
      tmpColSumPtr += 4;
      tmpOutPtr += 4;
    }
    for (; remain > 0; --remain) {
      *tmpColSumPtr += *tmpAddPtr;
      *tmpColSumPtr -= *tmpSubPtr;
      *tmpOutPtr = *tmpColSumPtr;
      tmpAddPtr ++;
      tmpColSumPtr ++;
      tmpOutPtr ++;
      tmpSubPtr ++;
    }
  }

  start = height - radius;
  for (int h = start; h < height; ++h) {
    float *subPtr = cachePtr + (h - radius - 1) * width;
    int shift = h * width;
    float *outPtr = output + shift; 

    int indexW = 0;
    float *tmpOutPtr = outPtr;
    float *tmpColSumPtr = colSumPtr;
    float *tmpSubPtr = subPtr;

    int nn = n;
    int remain = re;
    for (; nn > 0; nn--) {
      float32x4_t _sub = vld1q_f32(tmpSubPtr);
      float32x4_t _colSum = vld1q_f32(tmpColSumPtr);

      float32x4_t _tmp = vsubq_f32(_colSum, _sub);

      vst1q_f32(tmpColSumPtr, _tmp);
      vst1q_f32(tmpOutPtr, _tmp);

      tmpSubPtr += 4;
      tmpColSumPtr += 4;
      tmpOutPtr += 4;
    }
    for (; remain > 0; --remain) {
      *tmpColSumPtr -= *tmpSubPtr;
      *tmpOutPtr = *tmpColSumPtr;
      tmpColSumPtr ++;
      tmpOutPtr ++;
      tmpSubPtr ++;
    }
  }
}

#else

void BoxFilter::_fastBoxFilterImpl(const float *input,
                                   const int    radius,
                                   const int    height,
                                   const int    width,
                                   float       *output) {
  float *cachePtr = &(cache[0]);
  // sum horizonal
  for (int h = 0; h < height; ++h) {
    int shift = h * width;

    float tmp = 0;
    for (int w = 0; w < radius; ++w) {
      tmp += input[shift + w];
    }

    for (int w = 0; w <= radius; ++w) {
      tmp += input[shift + w + radius];
      cachePtr[shift + w] = tmp;
    }

    int start = radius + 1;
    int end = width - 1 - radius;
    for (int w = start; w <= end; ++w) {
      tmp += input[shift + w + radius];
      tmp -= input[shift + w - radius - 1];
      cachePtr[shift + w] = tmp;
    }

    start = width - radius;
    for (int w = start; w < width; ++w) {
      tmp -= input[shift + w - radius - 1];
      cachePtr[shift + w] = tmp;
    }
  }
  
  std::fill(colSum.begin(), colSum.end(), 0);
  float *colSumPtr = &(colSum[0]);

  // sum vertical
  for (int h = 0; h < radius; ++h) {
    int shift = h * width;
    for (int w = 0; w < width; ++w) {
      colSumPtr[w] += cachePtr[shift + w];
    }
  }

  for (int h = 0; h <= radius; ++h) {
    float *addPtr = cachePtr + (h + radius) * width;
    int shift = h * width;
    float *outPtr = output + shift; 
    for (int w = 0; w < width; ++w) {
      colSumPtr[w] += addPtr[w];
      outPtr[w] = colSumPtr[w];
    }
  }

  int start = radius + 1;
  int end = height - 1 - radius;
  for (int h = start; h <= end; ++h) {
    float *addPtr = cachePtr + (h + radius) * width;
    float *subPtr = cachePtr + (h - radius - 1) * width;
    int shift = h * width;
    float *outPtr = output + shift;
    for (int w = 0; w < width; ++w) {
      colSumPtr[w] += addPtr[w];
      colSumPtr[w] -= subPtr[w];
      outPtr[w] = colSumPtr[w];
    }
  }

  start = height - radius;
  for (int h = start; h < height; ++h) {
    float *subPtr = cachePtr + (h - radius - 1) * width;
    int shift = h * width;
    float *outPtr = output + shift; 
    for (int w = 0; w < width; ++w) {
      colSumPtr[w] -= subPtr[w];
      outPtr[w] = colSumPtr[w];
    }
  }
}

#endif

