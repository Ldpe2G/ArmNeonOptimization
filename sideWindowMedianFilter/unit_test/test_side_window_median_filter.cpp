#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <chrono>

#include "gtest/gtest.h"
#include "median_filter.h"
#include "side_window_median_filter.h"

static void print(float *input, int height, int width) {
  for (int h = 0; h < height; ++h) {
    int height_sift = h * width;
    std:: cout << std::endl;
    for (int w = 0; w < width; ++w) {
      std::cout << input[height_sift + w] << " ";
    }
  }
  std::cout << std::endl;
};

static int loop = 10;
static int height = 512;
static int width = 512;
static int radius = 3;
static int printMat = 0;

TEST(netTest, medianFilterSpeedTest)
{
    std::vector<float> input;
    std::vector<float> output;

    int size = height * width;

    input.resize(size);
    output.resize(size);

    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        input[i] = rand() % 255;
    }

    for (int i = 0; i < size; ++i) {
        output[i] = 0;
    }

    MedianFilter medianFilter;
    medianFilter.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilter.fastMedianFilter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        std::cout << "          [" << i << "]" << " MedianFilfer Cost time: " << tmp << "ms" << std::endl;

    }
    std::cout << "\n          MedianFilfer Average Cost time: " << avgTime / loop << "ms" << std::endl;


    if (printMat == 1) {
        std::cout << "result: " << std::endl;
        print(&output[0], height, width);
    }
}

TEST(netTest, sideWindowMedianFilterSpeedTest)
{
    std::vector<float> input;
    std::vector<float> output;

    int size = height * width;

    input.resize(size);
    output.resize(size);

    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        input[i] = rand() % 255;
    }

    for (int i = 0; i < size; ++i) {
        output[i] = 0;
    }

    SideWindowMedianFilter medianFilter;
    medianFilter.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilter.fastSideWindowMedianFilter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        std::cout << "          [" << i << "]" << " Side Window MedianFilfer Cost time: " << tmp << "ms" << std::endl;

    }
    std::cout << "\n          Side Window MedianFilfer Average Cost time: " << avgTime / loop << "ms" << std::endl;


    if (printMat == 1) {
        std::cout << "result: " << std::endl;
        print(&output[0], height, width);
    }
}
