#ifndef __PLOT_H__
#define __PLOT_H__

class Plot
{
  public:
    struct Data
    {
      Data()
        : hits(0)
        , weight(0.0f)
      {}

      unsigned int hits;
      float weight;
    };

    Plot(int width, int height);
    ~Plot();

    void Record(const double& x, const double& y);
    void ComputeWeights();
    const Data* GetRow(int rowIndex) const;
    const Data* GetAt(int x, int y) const;
    inline int GetMax() const {return m_max;}

  private:
    const int m_width;
    const int m_height;
    Data* m_data;
    int m_max;
};

#endif
