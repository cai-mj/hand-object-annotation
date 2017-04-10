#ifndef WIDGET_H
#define WIDGET_H
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <string>
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void showImg(Mat &crop);
    void initialize(string filename);
    void events( int e, int x, int y, int flags);
    void setBox();
    void setUiDefault();
    void restoreValue();

    Mat _src;
    Mat _dsp;
    string _wname;
    string _seqname;
    string _filename;
    bool _isQuit;
    bool _isDrag;

public slots:
    void saveValue();
    void cancel();
    void quit();

private:
    Ui::Widget *ui;
    //bounding box
    Point _lstart, _lend;
    int _hLminx;
    int _hLminy;
    int _hLmaxx;
    int _hLmaxy;
    int _hRminx;
    int _hRminy;
    int _hRmaxx;
    int _hRmaxy;
    int _oLminx;
    int _oLminy;
    int _oLmaxx;
    int _oLmaxy;
    int _oRminx;
    int _oRminy;
    int _oRmaxx;
    int _oRmaxy;
    //grasp type
    string _hLgrasp;
    string _hRgrasp;
    //object category
    string _oLclass;
    string _oRclass;
    //object attribute
    bool _isLprismatic;
    bool _isLsphere;
    bool _isLflat;
    bool _isLlarge;
    bool _isLrigid;
    bool _isRprismatic;
    bool _isRsphere;
    bool _isRflat;
    bool _isRlarge;
    bool _isRrigid;
};

#endif // WIDGET_H
