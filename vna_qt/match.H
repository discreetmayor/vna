#ifndef MATCH_H
#define MATCH_H
#include <QWidget>
#include <complex>


using namespace std;

class Match : public QWidget
{
  Q_OBJECT

public:
  Match();
  void setValue(complex<double> s11, double freqHz, double z0=50.);
  float  c1;
  float  c2;
  float  l1;
  float  l2;
  double freq;
  bool   highZ;
  bool   match1SeriesCapacitor = false;
  bool   match2SeriesInductor = false;

private:
  float validate(float val);
  void  calc(double freq);
  float conductanceCW;  
  float conductanceCCW;
  float resistanceCW;
  float resistanceCCW; 
};
#endif
