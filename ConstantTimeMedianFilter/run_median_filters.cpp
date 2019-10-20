
#include <iostream>
#include <vector>

#include "normal_median_filter_float.h"
#include "normal_median_filter_uint16.h"
#include "constant_time_median_filter_uint16.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

template<typename T>
static void rgb2gray(const unsigned char *rgbImage,
                     const int            height,
                     const int            width,
                     T                   *grayImage) {
  int size = height * width;
  unsigned char *src = rgbImage;
  for (int i = 0; i < size; ++i ) {
    // float gray = 0.3 * src[0] + 0.59 * src[1] + 0.11 * src[2];
    int gray = (src[0] * 77) + (src[1] * 151) + (src[2] * 28);
    // undo the scale by 256 and write to memory
    gray = gray >> 8;
    gray = std::max(std::min(gray, 255), 0);
    grayImage[i] = static_cast<T>(gray);
    src += 3;
  }
}

template<typename T>
static void arrayT2UnsignedChar(const T       *src,
                                const int      size,
                                unsigned char *dst) {
  for (int i = 0; i < size; ++i) {
    dst[i] = static_cast<unsigned char>(std::max(std::min(src[i], T(255)), T(0)));
  }
}

template<typename T>
static void splitChannels(const unsigned char           *image,
                          const int                      height,
                          const int                      width,
                          const int                      channel,
                          std::vector<std::vector<T> >  &channels) {
  int size = height * width;
  const unsigned char *src = image;
  for (int i = 0; i < size; ++i ) {
    for (int c = 0; c < channel; ++c) {
      channels[c][i] = static_cast<float>(src[c]);
    }
    src += channel;
  }
}

template<typename T>
static void mergeChannels(const std::vector<std::vector<T> >      &channels,
                          const int                                height,
                          const int                                width,
                          const int                                channel,
                          unsigned char                           *rgb) {
  int size = height * width;
  std::vector<unsigned char> tmp;
  tmp.resize(size);

  for (int c = 0; c < channel; ++c) {
    unsigned char *dst = rgb;
    arrayT2UnsignedChar(&channels[c][0], size, &tmp[0]);
    for (int i = 0; i < size; ++i ) {
      dst[c] = tmp[i];
      dst += channel;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc != 5) {
    printf("Usage: <input_image_path> <filter_radius> <filter_iteration_num> <median_filter_output_path_prefix>\n");
    return 0;
  }

  std::string input_path                                = std::string(argv[1]);
  int radius                                            = atoi(argv[2]);
  int iteration                                         = atoi(argv[3]) - 1;
  std::string median_filter_out_path_prefix             = std::string(argv[4]);

  int width, height, channel;
  unsigned char *inputImage = stbi_load(input_path.c_str(), &width, &height, &channel, 0);
  if (nullptr == inputImage) {
      std::cout << "Can't open " << input_path << std::endl;
      return 0;
  }

  std::cout << "input image size: width: " << width << ", height: " << height << ", channel: " << channel << std::endl;

  // rum normal float median filter
  std::vector<std::vector<float> > channels;
  channels.resize(channel);
  std::vector<std::vector<float> > results;
  results.resize(channel);

  for (int i = 0; i < channel; ++i) {
    channels[i].resize(height * width);
    results[i].resize(height * width);
  }

  splitChannels(inputImage, height, width, channel, channels);

  NormalMedianFilterFloat normal_median_filter_float;
  normal_median_filter_float.init(height, width, radius);

  for (int i = 0; i < channel; ++i) {
    normal_median_filter_float.filter(channels[i].data(), radius, height, width, results[i].data());
  }
  for (int i = 0; i < iteration; ++i) {
    for (int c = 0; c < channel; ++c) {
      normal_median_filter_float.filter(results[c].data(), radius, height, width, results[c].data());
    }
  }

  std::vector<unsigned char> result;
  result.resize(channel * height * width);

  mergeChannels(results, height, width, channel, &result[0]);
  std::string normal_median_filter_result_path = median_filter_out_path_prefix + "_normal_float_median_filter.jpg";
  stbi_write_jpg(normal_median_filter_result_path.c_str(), width, height, channel, result.data(), 100);

  // run normal uint16 median filter
  std::vector<std::vector<uint16_t> > channels_u16;
  channels_u16.resize(channel);
  std::vector<std::vector<uint16_t> > results_u16;
  results_u16.resize(channel);

  for (int i = 0; i < channel; ++i) {
    channels_u16[i].resize(height * width);
    results_u16[i].resize(height * width);
  }

  splitChannels(inputImage, height, width, channel, channels_u16);

  NormalMedianFilterUInt16 normal_median_filter_uint16;
  normal_median_filter_uint16.init(height, width, radius);

  for (int i = 0; i < channel; ++i) {
    normal_median_filter_uint16.filter(channels_u16[i].data(), radius, height, width, results_u16[i].data());
  }
  for (int i = 0; i < iteration; ++i) {
    for (int c = 0; c < channel; ++c) {
      normal_median_filter_uint16.filter(results_u16[c].data(), radius, height, width, results_u16[c].data());
    }
  }

  mergeChannels(results_u16, height, width, channel, &result[0]);
  normal_median_filter_result_path = median_filter_out_path_prefix + "_normal_uint16_median_filter.jpg";
  stbi_write_jpg(normal_median_filter_result_path.c_str(), width, height, channel, result.data(), 100);

  // run constant time uint16 median filter
  std::vector<std::vector<uint16_t> > channels_u16_2;
  channels_u16_2.resize(channel);
  std::vector<std::vector<uint16_t> > results_u16_2;
  results_u16_2.resize(channel);

  for (int i = 0; i < channel; ++i) {
    channels_u16_2[i].resize(height * width);
    results_u16_2[i].resize(height * width);
  }

  splitChannels(inputImage, height, width, channel, channels_u16_2);

  ConstantTimeMedianFilterUInt16 const_time_median_filter_uint16;
  const_time_median_filter_uint16.init(height, width, radius);

  for (int i = 0; i < channel; ++i) {
    const_time_median_filter_uint16.filter(channels_u16_2[i].data(), radius, height, width, results_u16_2[i].data());
  }
  for (int i = 0; i < iteration; ++i) {
    for (int c = 0; c < channel; ++c) {
      const_time_median_filter_uint16.filter(results_u16_2[c].data(), radius, height, width, results_u16_2[c].data());
    }
  }

  mergeChannels(results_u16_2, height, width, channel, &result[0]);
  normal_median_filter_result_path = median_filter_out_path_prefix + "_const_time_uint16_median_filter.jpg";
  stbi_write_jpg(normal_median_filter_result_path.c_str(), width, height, channel, result.data(), 100);

  stbi_image_free(inputImage);
  return 0;
}

