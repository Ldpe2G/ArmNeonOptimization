
#include <iostream>
#include <vector>

#include "box_filter.h"
#include "side_window_box_filter.h"

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

static void arrayFloat2UnsignedChar(const float   *src,
                                    const int      size,
                                    unsigned char *dst) {
  for (int i = 0; i < size; ++i) {
    dst[i] = static_cast<unsigned char>(std::max(std::min(src[i], 255.0f), 0.0f));
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

static void mergeChannels(const std::vector<std::vector<float> >  &channels,
                          const int                                height,
                          const int                                width,
                          const int                                channel,
                          unsigned char                           *rgb) {
  int size = height * width;
  std::vector<unsigned char> tmp;
  tmp.resize(size);

  for (int c = 0; c < channel; ++c) {
    unsigned char *dst = rgb;
    arrayFloat2UnsignedChar(&channels[c][0], size, &tmp[0]);
    for (int i = 0; i < size; ++i ) {
      dst[c] = tmp[i];
      dst += channel;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc != 6) {
    printf("Usage: <input_image_path> <filter_radius> <filter_iteration_num> <boxfilter_output_path> <side_window_box_filter_output_path>\n");
    return 0;
  }

  std::string input_path                      = std::string(argv[1]);
  int radius                                  = atoi(argv[2]);
  int iteration                               = atoi(argv[3]) - 1;
  std::string box_filter_out_path             = std::string(argv[4]);
  std::string side_window_box_filter_out_path = std::string(argv[5]);

  int width, height, channel;
  unsigned char *inputImage = stbi_load(input_path.c_str(), &width, &height, &channel, 0);
  if (nullptr == inputImage) {
      std::cout << "Can't open " << input_path << std::endl;
      return 0;
  }

  std::cout << "input image size: width: " << width << ", height: " << height << ", channel: " << channel << std::endl;

  std::vector<std::vector<float> > channels;
  channels.resize(channel);
  std::vector<std::vector<float> > results;
  results.resize(channel);

  for (int i = 0; i < channel; ++i) {
    channels[i].resize(height * width);
    results[i].resize(height * width);
  }

  splitChannels(inputImage, height, width, channel, channels);

  BoxFilter box_filter;
  box_filter.init(height, width, radius);

  for (int i = 0; i < channel; ++i) {
    box_filter.fastBoxFilter(channels[i].data(), radius, height, width, results[i].data());
  }
  for (int i = 0; i < iteration; ++i) {
    for (int c = 0; c < channel; ++c) {
      box_filter.fastBoxFilter(results[c].data(), radius, height, width, results[c].data());
    }
  }

  std::vector<unsigned char> result;
  result.resize(channel * height * width);

  mergeChannels(results, height, width, channel, &result[0]);
  stbi_write_jpg(box_filter_out_path.c_str(), width, height, channel, result.data(), 100);

  SideWindowBoxFilter side_window_box_filter;
  side_window_box_filter.init(height, width, radius);

  for (int i = 0; i < channel; ++i) {
    side_window_box_filter.fastSideWindowBoxFilter(channels[i].data(), radius, height, width, results[i].data());
  }
  for (int i = 0; i < iteration; ++i) {
    for (int c = 0; c < channel; ++c) {
      side_window_box_filter.fastSideWindowBoxFilter(results[c].data(), radius, height, width, results[c].data());
    }
  }

  mergeChannels(results, height, width, channel, &result[0]);
  stbi_write_jpg(side_window_box_filter_out_path.c_str(), width, height, channel, result.data(), 100);

  stbi_image_free(inputImage);
  return 0;
}

