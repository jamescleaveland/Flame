#ifndef __PLOT_H__
#define __PLOT_H__

#include <atomic>

// Lock-free HDR plot for multi-threaded image synthesis
class Plot
{
  public:
    struct Data
    {
      Data()
        : hits(0)
        , weight(0.0f)
      {}

      std::atomic<unsigned int> hits;
      float weight;
    };

    Plot(int width, int height);
    ~Plot();

    // Thread-safe method for plotting hits during synthesis
    inline void Record(const double& x, const double& y)
    {
      int i_x = static_cast<int>((x * 0.5 + 0.5) * (m_width - 1));
      int i_y = static_cast<int>((y * 0.5 + 0.5) * (m_height - 1));
      if (i_x >= 0 && i_x < m_width && i_y >= 0 && i_y < m_height)
      {
        ++m_data[i_y * m_width + i_x].hits;
      }
    }

    void PostProcess();
    const Data* GetRow(int rowIndex) const;
    const Data* GetAt(int x, int y) const;
    inline int GetMax() const {return m_max;}

  private:
    const int m_width;
    const int m_height;
    Data* m_data;
    unsigned int m_max;
};

#endif

