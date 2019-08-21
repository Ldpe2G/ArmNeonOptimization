#ifndef SRC_SIDE_WINDOW_BOX_FILTER_H_
#define SRC_SIDE_WINDOW_BOX_FILTER_H_

#include <vector>

enum class SIDE_BOX_DIRECTIONS: int {
  L                         = 0,
  R                         = 1,
  U                         = 2,
  D                         = 3,
  NW                        = 4,
  NE                        = 5,
  SW                        = 6,
  SE                        = 7,
  SIDE_BOX_DIRECTIONS_COUNT = 8
};

const int side_box_directions_count = static_cast<int>(SIDE_BOX_DIRECTIONS::SIDE_BOX_DIRECTIONS_COUNT);

enum class XY_START_END: int {
  WIDTH_START        = 0,
  WIDTH_END          = 1,
  HEIGHT_START       = 2,
  HEIGHT_END         = 3,
  XY_START_END_COUNT = 4
};

class SideWindowBoxFilter {
public:
  SideWindowBoxFilter() {}

  ~SideWindowBoxFilter() {}

  /**
   * initialize the cache to perform filtering, user must call this function first before calling the `fastSideWindowBoxFilter` fucntion.
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

    m_cache.resize(panel_size);
    m_col_sum.resize(width);

    m_side_window_params.resize(side_box_directions_count);

    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::L)]  = {-radius,      0, -radius, radius};
    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::R)]  = {      0, radius, -radius, radius};
    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::U)]  = {-radius, radius, -radius,      0};
    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::D)]  = {-radius, radius,       0, radius};
    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::NW)] = {-radius,      0, -radius,      0};
    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::NE)] = {      0, radius, -radius,      0};
    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::SW)] = {-radius,      0,       0, radius};
    m_side_window_params[static_cast<int>(SIDE_BOX_DIRECTIONS::SE)] = {      0, radius,       0, radius};

    m_side_window_box_filter_scales.resize(side_box_directions_count);

    std::vector<float> identity(panel_size, 1.0f);

    for (int i = 0; i < side_box_directions_count; ++i) {
      m_side_window_box_filter_scales[i].resize(panel_size);
      _sideWindowBoxFilterImpl(identity.data(), radius, height, width, static_cast<SIDE_BOX_DIRECTIONS>(i), m_side_window_box_filter_scales[i].data());

      for (int j = 0; j < panel_size; ++j) {
        m_side_window_box_filter_scales[i][j] = 1.0f / m_side_window_box_filter_scales[i][j];
      }
    }

    m_side_window_box_filter_results.resize(side_box_directions_count);
    for (int i = 0; i < side_box_directions_count; ++i) {
      m_side_window_box_filter_results[i].resize(panel_size);
    }

  }

  /**
   * implementation of main function of side window box filter.
   *
   * @param[in]  input  input data
   * @param[in]  radius filter radius, must be the same with the radius passed to `init` function
   * @param[in]  height input data height, must be the same with the radius passed to `init` function
   * @param[in]  width  input data width, must be the same with the radius passed to `init` function
   * @param[out] output pointer to save filter result
   * 
   */
  void fastSideWindowBoxFilter(const float  *input,
                               const int     radius,
                               const int     height,
                               const int     width,
                               float        *output);

private:

  /**
   * implementation of core function of side window box filter.
   *
   * @param[in]  input      input data
   * @param[in]  radius     filter radius, must be the same with the radius passed to `init` function
   * @param[in]  height     input data height, must be the same with the radius passed to `init` function
   * @param[in]  width      input data width, must be the same with the radius passed to `init` function
   * @param[in]  direction  direction of the side window box filter
   * @param[out] output     pointer to save filter result
   * 
   */
  void _sideWindowBoxFilterImpl(const float                *input,
                                const int                   radius,
                                const int                   height, 
                                const int                   width,
                                const SIDE_BOX_DIRECTIONS   direction,
                                float                      *output);

private:
  std::vector<float> m_cache;
  std::vector<float> m_col_sum;

  std::vector<std::vector<float> > m_side_window_box_filter_results;
  std::vector<std::vector<float> > m_side_window_box_filter_scales;
  std::vector<std::vector<int> >   m_side_window_params;

};

#endif // SRC_SIDE_WINDOW_BOX_FILTER_H_
