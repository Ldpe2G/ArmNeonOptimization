#ifndef SRC_SIDE_WINDOW_MEDIAN_FILTER_H_
#define SRC_SIDE_WINDOW_MEDIAN_FILTER_H_

#include <vector>

enum class SIDE_MEDIAN_DIRECTIONS: int {
  L                         = 0,
  R                         = 1,
  U                         = 2,
  D                         = 3,
  NW                        = 4,
  NE                        = 5,
  SW                        = 6,
  SE                        = 7,
  SIDE_MEDIAN_DIRECTIONS_COUNT = 8
};

const int side_median_directions_count = static_cast<int>(SIDE_MEDIAN_DIRECTIONS::SIDE_MEDIAN_DIRECTIONS_COUNT);

enum class XY_START_END: int {
  WIDTH_START        = 0,
  WIDTH_END          = 1,
  HEIGHT_START       = 2,
  HEIGHT_END         = 3,
  XY_START_END_COUNT = 4
};

class SideWindowMedianFilter {
public:
  SideWindowMedianFilter() {}

  ~SideWindowMedianFilter() {}

  /**
   * initialize the cache to perform filtering, user must call this function first before calling the `fastSideWindowMedianFilter` fucntion.
   *
   * @param[in]  height  input data height
   * @param[in]  width   input data wdith
   * @param[in]  radius  filtering radius, the actual filter size = (radius * 2) + 1
   * 
   */
  void init(int height,
            int width,
            int radius) {
    int panel_size = height * width;

    m_cache.resize((2 * radius + 1) * (2 * radius + 1) * 4);

    m_side_window_params.resize(side_median_directions_count);

    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::L)]  = {-radius,      0, -radius, radius};
    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::R)]  = {      0, radius, -radius, radius};
    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::U)]  = {-radius, radius, -radius,      0};
    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::D)]  = {-radius, radius,       0, radius};
    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::NW)] = {-radius,      0, -radius,      0};
    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::NE)] = {      0, radius, -radius,      0};
    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::SW)] = {-radius,      0,       0, radius};
    m_side_window_params[static_cast<int>(SIDE_MEDIAN_DIRECTIONS::SE)] = {      0, radius,       0, radius};

    m_side_window_median_filter_results.resize(side_median_directions_count);
    for (int i = 0; i < side_median_directions_count; ++i) {
      m_side_window_median_filter_results[i].resize(panel_size);
    }

  }

  /**
   * implementation of main function of side window median filter.
   *
   * @param[in]  input  input data
   * @param[in]  radius filter radius, must be the same with the radius passed to `init` function
   * @param[in]  height input data height, must be the same with the radius passed to `init` function
   * @param[in]  width  input data width, must be the same with the radius passed to `init` function
   * @param[out] output pointer to save filter result
   * 
   */
  void fastSideWindowMedianFilter(const float  *input,
                                  const int     radius,
                                  const int     height,
                                  const int     width,
                                  float        *output);

private:

  /**
   * implementation of core function of side window median filter.
   *
   * @param[in]  input      input data
   * @param[in]  radius     filter radius, must be the same with the radius passed to `init` function
   * @param[in]  height     input data height, must be the same with the radius passed to `init` function
   * @param[in]  width      input data width, must be the same with the radius passed to `init` function
   * @param[in]  direction  direction of the side window median filter
   * @param[out] output     pointer to save filter result
   * 
   */
  void _sideWindowMedianFilterImpl(const float                   *input,
                                   const int                      radius,
                                   const int                      height, 
                                   const int                      width,
                                   const SIDE_MEDIAN_DIRECTIONS   direction,
                                   float                         *output);

private:
  std::vector<float> m_cache;

  std::vector<std::vector<float> > m_side_window_median_filter_results;
  std::vector<std::vector<int> >   m_side_window_params;
};

#endif // SRC_SIDE_WINDOW_MEDIAN_FILTER_H_
