#ifndef __PLOT_H__
#define __PLOT_H__

class Plot
{
  public:
    typedef unsigned short Data;

    Plot(int width, int height);
    ~Plot();

    void Record(const double& x, const double& y);
    const Data* GetRow(int rowIndex) const;
    inline int GetMax() const {return m_max;}

  private:
    const int m_width;
    const int m_height;
    Data* m_data;
    int m_max;
};

#endif
