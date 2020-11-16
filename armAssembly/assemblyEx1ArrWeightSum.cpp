#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <chrono>

#ifdef __ANDROID__
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#endif

#if defined(__ARM_NEON)
#include <arm_neon.h>
#endif // __ARM_NEON

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

bool arrWeightedAvg(const float *arr1,
                    const float arr1Weight,
                    const float *arr2,
                    const float arr2Weight,
                    const int len,
                    float *resultArr) {
  if (arr1 == nullptr || arr2 == nullptr || resultArr == nullptr) {
    return false;
  }

  for (int i = 0; i < len; ++i) {
    resultArr[i] = arr1[i] * arr1Weight + arr2[i] * arr2Weight;
  }

  return true;
}

#if defined(__ARM_NEON)
bool arrWeightedAvgIntrinsic(const float *arr1,
                             const float arr1Weight,
                             const float *arr2,
                             const float arr2Weight,
                             const int len,
                             float *resultArr) {
  if (arr1 == nullptr || arr2 == nullptr || resultArr == nullptr) {
    return false;
  }

  int neonLen = len >> 2;
  int remain = len - (neonLen << 2);

  float *resultArrPtr  = resultArr;
  const float *arr1Ptr = arr1;
  const float *arr2Ptr = arr2;

  // float32x4_t arr1Wf4 = vdupq_n_f32(arr1Weight);
  // float32x4_t arr2Wf4 = vdupq_n_f32(arr2Weight);
  // for (int i = 0; i < neonLen; ++i) {
  //   float32x4_t arr1f4 = vld1q_f32(arr1Ptr);
  //   float32x4_t arr2f4 = vld1q_f32(arr2Ptr);
    
  //   arr1f4 = vmulq_f32(arr1f4, arr1Wf4);
  //   arr2f4 = vmulq_f32(arr2f4, arr2Wf4);

  //   float32x4_t resultf4 = vaddq_f32(arr1f4, arr2f4);

  //   vst1q_f32(resultArrPtr, resultf4);

  //   arr1Ptr += 4;
  //   arr2Ptr += 4;
  //   resultArrPtr += 4;
  // }

  float32x4_t arr1Wf4 = vdupq_n_f32(arr1Weight);
  float32x4_t arr2Wf4 = vdupq_n_f32(arr2Weight);
  do {

    float32x4_t arr1f4 = vld1q_f32(arr1Ptr);
    arr1Ptr += 4;
    
    float32x4_t arr2f4 = vld1q_f32(arr2Ptr);
    arr2Ptr += 4;
    
    arr1f4 = vmulq_f32(arr1f4, arr1Wf4);
    arr2f4 = vmulq_f32(arr2f4, arr2Wf4);

    float32x4_t resultf4 = vaddq_f32(arr1f4, arr2f4);

    vst1q_f32(resultArrPtr, resultf4);
    resultArrPtr += 4;
    
    neonLen --;
  } while (neonLen > 0);

  for (; remain > 0; remain --) {
    *resultArrPtr = (*arr1Ptr) * arr1Weight + (*arr2Ptr) * arr2Weight;
    resultArrPtr ++;
    arr1Ptr ++;
    arr2Ptr ++;
  }

  return true;
}

bool arrWeightedAvgAssembly(const float *arr1,
                            const float arr1Weight,
                            const float *arr2,
                            const float arr2Weight,
                            const int len,
                            float *resultArr) {
  if (arr1 == nullptr || arr2 == nullptr || resultArr == nullptr) {
    return false;
  }

  int neonLen = len >> 2;
  int remain = len - (neonLen << 2);

  float *resultArrPtr  = resultArr;
  const float *arr1Ptr = arr1;
  const float *arr2Ptr = arr2;

#ifdef __aarch64__  // armv8
  __asm__ volatile(
    "mov   x0, %[arr1Weight]                  \n"
    "dup   v0.4s, w0                          \n"
    
    "mov   x1, %[arr2Weight]                  \n"
    "dup   v1.4s, w1                          \n"
    
    "0:                                       \n"

    "prfm  pldl1keep, [%[arr1Ptr], #128]      \n"
    "ld1      {v2.4s}, [%[arr1Ptr]], #16      \n"

    "prfm  pldl1keep, [%[arr2Ptr], #128]      \n"
    "ld1      {v3.4s}, [%[arr2Ptr]], #16      \n"
    
    "fmul       v4.4s, v2.4s, v0.4s           \n"
    "fmul       v5.4s, v3.4s, v1.4s           \n"

    "fadd       v6.4s, v4.4s, v5.4s           \n"

    "subs       %[neonLen], %[neonLen], #1    \n"
    
    "st1    {v6.4s}, [%[resultArrPtr]], #16   \n"
    
    "bgt        0b                            \n"

    :[arr1Ptr]        "+r"(arr1Ptr),
     [arr2Ptr]        "+r"(arr2Ptr),
     [resultArrPtr]   "+r"(resultArrPtr),
     [neonLen]        "+r"(neonLen)
    :[arr1Weight]     "r"(arr1Weight),
     [arr2Weight]     "r"(arr2Weight)
    :"cc", "memory", "x0", "x1", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7"
  );
#else   // armv7
  __asm__ volatile(
    "vdup.f32   q0, %[arr1Weight]        \n"
    "vdup.f32   q1, %[arr2Weight]        \n"

    "0:                                  \n"

    "pld        [%[arr1Ptr], #128]       \n"
    "vld1.f32   {d4-d5}, [%[arr1Ptr]]!   \n"

    "pld        [%[arr2Ptr], #128]       \n"
    "vld1.f32   {d6-d7}, [%[arr2Ptr]]!   \n"
    
    "vmul.f32   q4, q0, q2 \n"
    "vmul.f32   q5, q1, q3 \n"

    "vadd.f32   q6, q4, q5 \n"

    "subs       %[neonLen], #1                \n"
    
    "vst1.f32   {d12-d13}, [%[resultArrPtr]]! \n"
    
    "bgt        0b                  \n"
    :[arr1Ptr]        "+r"(arr1Ptr),
     [arr2Ptr]        "+r"(arr2Ptr),
     [resultArrPtr]   "+r"(resultArrPtr),
     [neonLen]        "+r"(neonLen)
    :[arr1Weight]     "r"(arr1Weight),
     [arr2Weight]     "r"(arr2Weight)
    :"cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7"
  );
#endif

  for (; remain > 0; remain --) {
    *resultArrPtr = (*arr1Ptr) * arr1Weight + (*arr2Ptr) * arr2Weight;
    resultArrPtr ++;
    arr1Ptr ++;
    arr2Ptr ++;
  }

  return true;
}

#endif // __ARM_NEON

int main(int argc, char *argv[]) {

    size_t mask = 0;
    for (int i = 0; i < 8; ++i) {
      if (i >= 5) {
        mask |= (1 << i);
      }
    }
    int ret = set_sched_affinity(mask);

    int arrLen = 10000000;
    float *arr1 = new float[arrLen];
    float *arr2 = new float[arrLen];
    float *resultArr = new float[arrLen];

    float weight1 = rand() % 256;
    float weight2 = rand() % 256;
    for (int i = 0; i < arrLen; ++i) {
      arr1[i] = rand() % 256;
      arr2[i] = rand() % 256;
    }

    float duration = 0.0f;
    int loop = 1000;
    for (int i = 0; i < loop; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      // arrWeightedAvg(arr1, weight1, arr2, weight2, arrLen, resultArr);
      arrWeightedAvgIntrinsic(arr1, weight1, arr2, weight2, arrLen, resultArr);
      // arrWeightedAvgAssembly(arr1, weight1, arr2, weight2, arrLen, resultArr);
      auto stop = std::chrono::high_resolution_clock::now();
      duration += std::chrono::duration<double, std::milli>(stop - start).count();;
    }
    printf("arr weighted sum time: %f ms\n", duration / loop);

    for (int i = 0; i < arrLen; ++i) {
      resultArr[i] = resultArr[i] * arrLen;
    }

    delete[] arr1;
    delete[] arr2;
    delete[] resultArr;

    return 0;
}
