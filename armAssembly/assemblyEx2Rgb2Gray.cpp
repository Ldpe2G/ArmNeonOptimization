
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#ifdef __ANDROID__
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#endif

#if defined(__ARM_NEON)
#include <arm_neon.h>
#endif // __ARM_NEON

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static int set_sched_affinity(size_t thread_affinity_mask)
{
#ifdef __GLIBC__
    pid_t pid = syscall(SYS_gettid);
#else
#ifdef PI3
    pid_t pid = getpid();
#else
    pid_t pid = gettid();
#endif
#endif
    int syscallret = syscall(__NR_sched_setaffinity, pid, sizeof(thread_affinity_mask), &thread_affinity_mask);
    if (syscallret)
    {
        fprintf(stderr, "syscall error %d\n", syscallret);
        return -1;
    }
    return 0;
}

void rgb2grayFloat(const unsigned char *rgbImage,
                   const int            height,
                   const int            width,
                   unsigned char       *grayImage) {
    int size = height * width;
    const unsigned char *src = rgbImage;

    for (int i = 0; i < size; ++i ) {
        float gray = 0.3 * src[0] + 0.59 * src[1] + 0.11 * src[2];
        gray = std::max(std::min(gray, 255.0f), 0.0f);
        grayImage[i] = static_cast<unsigned char>(gray);
        src += 3;
    }
}

void rgb2gray(const unsigned char *rgbImage,
              const int            height,
              const int            width,
              unsigned char       *grayImage) {
    int size = height * width;
    const unsigned char *src = rgbImage;
    
    uint16_t rW = 77;
    uint16_t gW = 151;
    uint16_t bW = 28;

    for (int i = 0; i < size; ++i ) {
        uint16_t gray = (static_cast<uint16_t>(src[0]) * rW) + 
                        (static_cast<uint16_t>(src[1]) * gW) + 
                        (static_cast<uint16_t>(src[2]) * bW);

        // undo the scale by 256 and write to memory
        gray = gray >> 8;
        gray = std::max(std::min(gray, (uint16_t)255), (uint16_t)0);
        grayImage[i] = static_cast<unsigned char>(gray);
        src += 3;
    }
}

void rgb2grayAssembly(const unsigned char *rgbImage,
                      const int            height,
                      const int            width,
                      unsigned char       *grayImage) {
    int size = height * width;
    const unsigned char *src = rgbImage;
    unsigned char *dst = grayImage;

    int neonLen = size >> 4;
    int remain  = size - (neonLen << 4); 

#ifdef __aarch64__  // armv8
    __asm__ volatile(
        // [rW, rW, rW, rW, rW, rW, rW, rW]
        "movi   v0.8b, #77                          \n"
        // [gW, gW, gW, gW, gW, gW, gW, gW]
        "movi   v1.8b, #151                         \n"
        // [bW, bW, bW, bW, bW, bW, bW, bW]
        "movi   v2.8b, #28                          \n"
        
        "0:                                         \n"

        "prfm  pldl1keep, [%[src], #512]            \n"

        // load [rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb] 
        // into 
        // [r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r] 
        // [g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g]
        // [b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b]
        "ld3	{v3.16b-v5.16b}, [%[src]], #48      \n"

        // reverse lower and higher half 64bit of v register 
        "ext	v6.16b, v3.16b, v3.16b, #8          \n"
        "ext	v7.16b, v4.16b, v4.16b, #8          \n"
        "ext	v8.16b, v5.16b, v5.16b, #8          \n"

        // [r,r,r,r,r,r,r,r] * [rW,rW,rW,rW,rW,rW,rW,rW]
        // bitwidth of results(uint16_t) are 
        // wider than elements in inputs(uint8_t)
        "umull	v9.8h,  v3.8b, v0.8b                \n"
        "umull	v10.8h, v6.8b, v0.8b                \n"

        // [g,g,g,g,g,g,g,g] * [gW,gW,gW,gW,gW,gW,gW,gW]
        "umull	v11.8h, v4.8b, v1.8b                \n"

        // r*rW + [b,b,b,b,b,b,b,b] * [bW,bW,bW,bW,bW,bW,bW,bW]
        "umlal	v9.8h,  v5.8b, v2.8b                \n"

        // [g,g,g,g,g,g,g,g] * [gW,gW,gW,gW,gW,gW,gW,gW]
        "umull	v12.8h, v7.8b, v1.8b                \n"

        // r*rW + [b,b,b,b,b,b,b,b] * [bW,bW,bW,bW,bW,bW,bW,bW]
        "umlal	v10.8h, v8.8b, v2.8b                \n"

        // writes the most significant half of the 
        // add result to the lower half of 
        // the v13 register and clears its upper half
        "addhn	v13.8b,  v9.8h,  v11.8h             \n"

        // writes the most significant half of the 
        // add result to the upper half of 
        // the v13 register without affecting the other bits
        "addhn2	v13.16b, v10.8h, v12.8h             \n"

        "subs    %[neonLen], %[neonLen], #1         \n"

        "st1    {v13.16b}, [%[dst]], #16            \n"

        "bgt        0b                              \n"

        :[src]        "+r"(src),
         [dst]        "+r"(dst),
         [neonLen]    "+r"(neonLen)
        :
        :"cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13"
    );
#else   // armv7
    __asm__ volatile(
        // [rW, rW, rW, rW, rW, rW, rW, rW]
        "vmov.u8   d0, #77                  \n"
        // [gW, gW, gW, gW, gW, gW, gW, gW]
        "vmov.u8   d1, #151                 \n"
        // [bW, bW, bW, bW, bW, bW, bW, bW]
        "vmov.u8   d2, #28                  \n"

        "0:                                 \n"

        "pld        [%[src], #512]          \n"

        // load [rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb] into 
        // [r,r,r,r,r,r,r,r], [g,g,g,g,g,g,g,g], [b,b,b,b,b,b,b,b]
        "vld3.u8   {d3,d4,d5}, [%[src]]!    \n"
        // load next [rgb,rgb,rgb,rgb,rgb,rgb,rgb,rgb] into 
        // [r,r,r,r,r,r,r,r], [g,g,g,g,g,g,g,g], [b,b,b,b,b,b,b,b]
        "vld3.u8   {d6,d7,d8}, [%[src]]!    \n"

        // [r,r,r,r,r,r,r,r] * [rW,rW,rW,rW,rW,rW,rW,rW]
        // bitwidth of results(uint16_t) are 
        // wider than elements in inputs(uint8_t)
        "vmull.u8	q5, d3, d0              \n"
        "vmull.u8	q6, d6, d0              \n"

        // [g,g,g,g,g,g,g,g] * [gW,gW,gW,gW,gW,gW,gW,gW]
        // bitwidth of results(uint16_t) are 
        // wider than elements in inputs(uint8_t)
        "vmull.u8	q7, d4, d1              \n"
        "vmull.u8	q8, d7, d1              \n"

        // r*rW + [b,b,b,b,b,b,b,b] * [bW,bW,bW,bW,bW,bW,bW,bW]
        "vmlal.u8	q5, d5, d2              \n"
        "vmlal.u8	q6, d8, d2              \n"

        // (r*rW + g*gW + b*bW) >> 8
        "vaddhn.u16	d18, q5, q7             \n"
        "vaddhn.u16	d19, q6, q8             \n"

        "subs       %[neonLen], #1          \n"

        "vst1.u8   {d18-d19}, [%[dst]]!     \n"

        "bgt        0b                      \n"

        :[src]        "+r"(src),
         [dst]        "+r"(dst),
         [neonLen]    "+r"(neonLen)
        :
        :"cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10"
    );
#endif
    uint16_t rW = 77;
    uint16_t gW = 151;
    uint16_t bW = 28;
    for (; remain > 0; remain -- ) {
        // float gray = 0.3 * src[0] + 0.59 * src[1] + 0.11 * src[2];
        uint16_t gray = (static_cast<uint16_t>(src[0]) * rW) + 
                        (static_cast<uint16_t>(src[1]) * gW) + 
                        (static_cast<uint16_t>(src[2]) * bW);

        // undo the scale by 256 and write to memory
        gray = gray >> 8;
        gray = std::max(std::min(gray, (uint16_t)255), (uint16_t)0);
        *dst = static_cast<unsigned char>(gray);
        
        src += 3;
        dst ++;
    }
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: <input_image_path> <output_name>\n");
        return 0;
    }

    size_t mask = 0;
    for (int i = 0; i < 8; ++i) {
      if (i >= 5) {
        mask |= (1 << i);
      }
    }
    int ret = set_sched_affinity(mask);

    std::string input_path  = std::string(argv[1]);
    std::string output_nameC = "resultC-" + std::string(argv[2]);
    std::string output_nameNeonIntrinsics = "resultNeonIntrinsics-" + std::string(argv[2]);
    std::string output_nameAssembly = "resultAssembly-" + std::string(argv[2]);

    int width, height, channel;
    unsigned char *inputImage = stbi_load(input_path.c_str(), &width, &height, &channel, 0);
    if (nullptr == inputImage) {
        std::cout << "Can't open " << input_path << std::endl;
        return 0;
    }

    int arrLen = height * width;
    unsigned char *outGrayC = new unsigned char[arrLen];
    unsigned char *outGrayNeonIntrinsics = new unsigned char[arrLen];
    unsigned char *outGrayAssembly = new unsigned char[arrLen];

    int loop = 1000;

    float durationC = 0.0f;
    for (int i = 0; i < loop; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        rgb2grayFloat(inputImage, height, width, outGrayC);
        auto stop = std::chrono::high_resolution_clock::now();
        durationC += std::chrono::duration<double, std::milli>(stop - start).count();
    }
    float avgDurationC = durationC / loop;
    
    float durationNeonIntrinsics = 0.0f;
    for (int i = 0; i < loop; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        rgb2gray(inputImage, height, width, outGrayNeonIntrinsics);
        auto stop = std::chrono::high_resolution_clock::now();
        durationNeonIntrinsics += std::chrono::duration<double, std::milli>(stop - start).count();
    }
    float avgDurationNeonIntrinsics = durationNeonIntrinsics / loop;

    float durationAssembly = 0.0f;
    for (int i = 0; i < loop; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        rgb2grayAssembly(inputImage, height, width, outGrayAssembly);
        auto stop = std::chrono::high_resolution_clock::now();
        durationAssembly += std::chrono::duration<double, std::milli>(stop - start).count();
    }
    float avgDurationAssembly = durationAssembly / loop;

    // check result
    int failCount = 0;
    for (int i=0; i<arrLen; i++) {
        if (outGrayC[i]!=outGrayNeonIntrinsics[i] || outGrayC[i]!=outGrayAssembly[i]) {
            failCount ++;
        }
    }

    if (failCount>0) {
        printf("Error! Results mismatch, fail count = %d\n", failCount);
    } else {
        printf("Correct! Results mattch\n");
    }
    printf("image size (%d, %d) rgb2gray time:\n", height, width);
    printf("    plain C        : %f ms\n", avgDurationC);
    printf("    neon intrinsics: %f ms\n", avgDurationNeonIntrinsics);
    printf("    assembly       : %f ms\n", avgDurationAssembly);

    stbi_write_jpg(output_nameC.c_str(), width, height, 1, outGrayC, 100);
    stbi_write_jpg(output_nameNeonIntrinsics.c_str(), width, height, 1, outGrayNeonIntrinsics, 100);
    stbi_write_jpg(output_nameAssembly.c_str(), width, height, 1, outGrayAssembly, 100);

    stbi_image_free(inputImage);
    delete[] outGrayC;
    delete[] outGrayNeonIntrinsics;
    delete[] outGrayAssembly;

    return 0;
}
