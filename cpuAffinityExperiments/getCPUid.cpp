#include <limits.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>

#ifdef __ANDROID__
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#endif

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
// refer to: https://github.com/Tencent/ncnn/blob/ee41ef4a378ef662d24f137d97f7f6a57a5b0eba/src/cpu.cpp
static int get_cpucount()
{
    int count = 0;
#ifdef __ANDROID__
    // get cpu count from /proc/cpuinfo
    FILE* fp = fopen("/proc/cpuinfo", "rb");
    if (!fp)
        return 1;

    char line[1024];
    while (!feof(fp))
    {
        char* s = fgets(line, 1024, fp);
        if (!s)
            break;

        if (memcmp(line, "processor", 9) == 0)
        {
            count++;
        }
    }

    fclose(fp);
#endif

    if (count < 1)
        count = 1;

    if (count > (int)sizeof(size_t) * 8)
    {
        fprintf(stderr, "more than %d cpu detected, thread affinity may not work properly :(\n", (int)sizeof(size_t) * 8);
    }

    return count;
}
// refer to: https://github.com/Tencent/ncnn/blob/ee41ef4a378ef662d24f137d97f7f6a57a5b0eba/src/cpu.cpp
#ifdef __ANDROID__
static int get_max_freq_khz(int cpuid)
{
    // first try, for all possible cpu
    char path[256];
    sprintf(path, "/sys/devices/system/cpu/cpufreq/stats/cpu%d/time_in_state", cpuid);
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        // second try, for online cpu
        sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/stats/time_in_state", cpuid);
        fp = fopen(path, "rb");
        if (fp)
        {
            int max_freq_khz = 0;
            while (!feof(fp))
            {
                int freq_khz = 0;
                int nscan = fscanf(fp, "%d %*d", &freq_khz);
                if (nscan != 1)
                    break;

                if (freq_khz > max_freq_khz)
                    max_freq_khz = freq_khz;
            }
            fclose(fp);
            if (max_freq_khz != 0)
                return max_freq_khz;
            fp = NULL;
        }

        if (!fp)
        {
            // third try, for online cpu
            sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", cpuid);
            fp = fopen(path, "rb");

            if (!fp)
                return -1;

            int max_freq_khz = -1;
            fscanf(fp, "%d", &max_freq_khz);

            fclose(fp);

            return max_freq_khz;
        }
    }

    int max_freq_khz = 0;
    while (!feof(fp))
    {
        int freq_khz = 0;
        int nscan = fscanf(fp, "%d %*d", &freq_khz);
        if (nscan != 1)
            break;

        if (freq_khz > max_freq_khz)
            max_freq_khz = freq_khz;
    }
    fclose(fp);
    return max_freq_khz;
}
#endif // __ANDROID__

// naive boxfilter implementation
static void Boxfilter(int radius, int height, int width) {
  float *input = new float[height * width];
  float *output = new float[height * width];
  for (int h = 0; h < height; ++h) {
    int height_sift = h * width;
    for (int w = 0; w < width; ++w) {
      int start_h = std::max(0, h - radius);
      int end_h = std::min(height - 1, h + radius);
      int start_w = std::max(0, w - radius);
      int end_w = std::min(width - 1, w + radius);
      float tmp = 0;
      for (int sh = start_h; sh <= end_h; ++sh) {
        for (int sw = start_w; sw <= end_w; ++ sw) {
          tmp += input[sh * width + sw];
        }
      }
      output[height_sift + w] = tmp;
    }
  }
  delete[] input;
  delete[] output;
}

std::mutex g_display_mutex;
void threadFun() {
    size_t cpu;
    auto syscallret = syscall(__NR_getcpu, &cpu, NULL, NULL);
    std::stringstream ss;
    ss << std::this_thread::get_id();

    g_display_mutex.lock();
    printf("thread %s, running on cpu: %d\n", ss.str().c_str(), cpu);
    g_display_mutex.unlock();

    Boxfilter(7, 500, 500);
}

static void swapSort(std::vector<int> &arr, std::vector<int> &idx, bool reverse = true) {
  if (reverse) {
    for (int i = 0; i < arr.size() - 1; ++i) {
      int maxVal = arr[i];
      int maxIdx = i;
      for (int j = i + 1; j < arr.size(); ++j) {
        if (arr[j] > maxVal) {
          maxVal = arr[j];
          maxIdx = j;
        }
      }
      // swap val
      int tmp = arr[maxIdx];
      arr[maxIdx] = arr[i];
      arr[i] = tmp;
      // swap idx
      tmp = idx[maxIdx];
      idx[maxIdx] = idx[i];
      idx[i] = tmp;
    }
  } else {
    for (int i = 0; i < arr.size() - 1; ++i) {
      int minVal = arr[i];
      int minIdx = i;
      for (int j = i + 1; j < arr.size(); ++j) {
        if (arr[j] < minVal) {
          minVal = arr[j];
          minIdx = j;
        }
      }
      // swap val
      int tmp = arr[minIdx];
      arr[minIdx] = arr[i];
      arr[i] = tmp;
      // swap idx
      tmp = idx[minIdx];
      idx[minIdx] = idx[i];
      idx[i] = tmp;
    }
  }
}

int main(int argc, char *argv[]) {

    auto g_cpucount = get_cpucount();
    printf("cpu numbers %d\n", g_cpucount);

    std::vector<int> cpu_max_freq_khz(g_cpucount);
    std::vector<int> cpu_idx(g_cpucount);
    for (int i=0; i<g_cpucount; i++)
    {
        int max_freq_khz = get_max_freq_khz(i);
        cpu_max_freq_khz[i] = max_freq_khz;
        cpu_idx[i] = i;
    }
    
    printf("before sort\n");
    for (int i = 0; i < g_cpucount; ++i) {
      printf("cpu_%d:%d, ", cpu_idx[i], cpu_max_freq_khz[i]);
    }
    printf("\n");

    swapSort(cpu_max_freq_khz, cpu_idx);

    printf("after sort\n");
    for (int i = 0; i < g_cpucount; ++i) {
      printf("cpu_%d:%d, ", cpu_idx[i], cpu_max_freq_khz[i]);
    }
    printf("\n\n");

    // distinguish big & little cores with ncnn strategy 
    int max_freq_khz_min = INT_MAX;
    int max_freq_khz_max = 0;
    for (int i = 0; i < g_cpucount; i++) {
        if (cpu_max_freq_khz[i] > max_freq_khz_max)
            max_freq_khz_max = cpu_max_freq_khz[i];
        if (cpu_max_freq_khz[i] < max_freq_khz_min)
            max_freq_khz_min = cpu_max_freq_khz[i];
    }
    int max_freq_khz_medium = (max_freq_khz_min + max_freq_khz_max) / 2;

    // warm up
    for (int i = 0; i < 5; ++i) {
      Boxfilter(7, 500, 500);
    }

    std::vector<std::thread> threads;
    const int threadNum = 4;

    // bind all cores
    printf("bind all cores ex:\n");
    auto start = std::chrono::steady_clock::now(), stop = start;

    for (int i = 0; i < threadNum; ++i) {
      std::thread t(threadFun);
      threads.push_back(std::move(t));
    }
    for (int i = 0; i < threadNum; ++i) {
      threads[i].join();
    }

    stop = std::chrono::steady_clock::now();
    auto time = (0.000001 * std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count());
    printf("bind all core time: %f\n\n", time);
    threads.clear();

    // bind little cores
    printf("bind little cores ex:\n");
    size_t mask2 = 0;
    for (int i = 0; i < g_cpucount; ++i) {
      if (cpu_max_freq_khz[i] == max_freq_khz_min) {
        mask2 |= (1 << cpu_idx[i]);
        printf("bind cpu: %d, ", cpu_idx[i]);
      }
    }
    printf("\n");
    int ret2 = set_sched_affinity(mask2);

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < threadNum; ++i) {
      std::thread t(threadFun);
      threads.push_back(std::move(t));
    }
    for (int i = 0; i < threadNum; ++i) {
      threads[i].join();
    }

    stop = std::chrono::steady_clock::now();
    time = (0.000001 * std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count());
    printf("bind little core time: %f\n\n", time);
    threads.clear();

    // bind big cores
    printf("bind big cores ex:\n");
    size_t mask = 0;
    for (int i = 0; i < g_cpucount; ++i) {
      if (cpu_max_freq_khz[i] >= max_freq_khz_medium) {
        mask |= (1 << cpu_idx[i]);
        printf("bind cpu: %d, ", cpu_idx[i]);
      }
    }
    printf("\n");
    int ret = set_sched_affinity(mask);

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < threadNum; ++i) {
      std::thread t(threadFun);
      threads.push_back(std::move(t));
    }
    for (int i = 0; i < threadNum; ++i) {
      threads[i].join();
    }

    stop = std::chrono::steady_clock::now();
    time = (0.000001 * std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count());
    printf("bind big core time: %f\n", time);

    printf("\n");

    return 0;
}
