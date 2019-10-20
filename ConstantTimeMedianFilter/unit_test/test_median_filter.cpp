#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <chrono>

#include "gtest/gtest.h"
#include "normal_median_filter_float.h"
#include "normal_median_filter_uint16.h"
#include "constant_time_median_filter_uint16.h"

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
static int printMat = 0;

TEST(netTest, medianFilterFloatRaduis1SpeedTest)
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

    int radius = 1;

    NormalMedianFilterFloat medianFilterFloat;
    medianFilterFloat.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterFloat.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Float MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Float MedianFilfer Radius 1 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;

    if (printMat == 1) {
        std::cout << "result: " << std::endl;
        print(&output[0], height, width);
    }
}

TEST(netTest, medianFilterUint16Raduis1SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 1;

    NormalMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Uint16 MedianFilfer Radius 1 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, constantTimeMedianFilterUint16Raduis1SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 1;

    ConstantTimeMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " constant time Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Constant time Uint16 MedianFilfer Radius 1 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, medianFilterFloatRaduis2SpeedTest)
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

    int radius = 2;

    NormalMedianFilterFloat medianFilterFloat;
    medianFilterFloat.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterFloat.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Float MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Float MedianFilfer Radius 2 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;

    if (printMat == 1) {
        std::cout << "result: " << std::endl;
        print(&output[0], height, width);
    }
}

TEST(netTest, medianFilterUint16Raduis2SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 2;

    NormalMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Uint16 MedianFilfer Radius 2 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, constantTimeMedianFilterUint16Raduis2SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 2;

    ConstantTimeMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " constant time Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Constant Time Uint16 MedianFilfer Radius 2 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, medianFilterFloatRaduis3SpeedTest)
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

    int radius = 3;

    NormalMedianFilterFloat medianFilterFloat;
    medianFilterFloat.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterFloat.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Float MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Float MedianFilfer Radius 3 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;

    if (printMat == 1) {
        std::cout << "result: " << std::endl;
        print(&output[0], height, width);
    }
}

TEST(netTest, medianFilterUint16Raduis3SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 3;

    NormalMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Uint16 MedianFilfer Radius 3 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, constantTimeMedianFilterUint16Raduis3SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 3;

    ConstantTimeMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " constant time Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Constant time Uint16 MedianFilfer Radius 3 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, medianFilterFloatRaduis4SpeedTest)
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

    int radius = 4;

    NormalMedianFilterFloat medianFilterFloat;
    medianFilterFloat.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterFloat.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Float MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Float MedianFilfer Radius 4 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;

    if (printMat == 1) {
        std::cout << "result: " << std::endl;
        print(&output[0], height, width);
    }
}

TEST(netTest, medianFilterUint16Raduis4SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 4;

    NormalMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Uint16 MedianFilfer Radius 4 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, constantTimeMedianFilterUint16Raduis4SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 4;

    ConstantTimeMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " constant time Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Constant time Uint16 MedianFilfer Radius 4 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, medianFilterFloatRaduis5SpeedTest)
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

    int radius = 5;

    NormalMedianFilterFloat medianFilterFloat;
    medianFilterFloat.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterFloat.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Float MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Float MedianFilfer Radius 5 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;

    if (printMat == 1) {
        std::cout << "result: " << std::endl;
        print(&output[0], height, width);
    }
}

TEST(netTest, medianFilterUint16Raduis5SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 5;

    NormalMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " Normal Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Normal Uint16 MedianFilfer Radius 5 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}

TEST(netTest, constantTimeMedianFilterUint16Raduis5SpeedTest)
{
    std::vector<uint16_t> input;
    std::vector<uint16_t> output;

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

    int radius = 5;

    ConstantTimeMedianFilterUInt16 medianFilterUint16;
    medianFilterUint16.init(height, width, radius);

    float avgTime = 0;
    float tmp;
    for (int i = 0; i < loop; ++i) {
        auto startClock = std::chrono::system_clock::now();
        medianFilterUint16.filter(&input[0], radius, height, width, &output[0]);
        auto endClock = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock);
        tmp = double(duration.count()) * std::chrono::microseconds::period::num / 1000;
        avgTime += tmp;
        // std::cout << "          [" << i << "]" << " constant time Uint16 MedianFilfer Radius 3 Cost time: " << tmp << "ms" << std::endl;
    }
    std::cout << "\n#################Constant time Uint16 MedianFilfer Radius 5 Average Cost time: " << avgTime / loop << "ms\n" << std::endl;
}
