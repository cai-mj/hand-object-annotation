#include "widget.h"
#include "ui_widget.h"

static void wevents( int e, int x, int y, int flags, void* ptr );

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::showImg(Mat &crop)
{
    char imgname[100];
    sprintf(imgname, "crop.jpg");
    imwrite(string(imgname), crop);
    QPixmap mypix(imgname);
    int w = ui->label_image->width();
    int h = ui->label_image->height();
    ui->label_image->setPixmap(mypix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void Widget::initialize(string filename)
{
    _filename = filename;
    _src.copyTo(_dsp);
    _wname = string("display:" + _seqname + "-" + _filename);
    namedWindow(_wname);
    moveWindow(_wname, 0, 0);
    setMouseCallback(_wname, wevents, this);
    imshow(_wname, _dsp);
    waitKey(100);

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveValue()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(quit()));
    _isQuit = false;
    _isDrag = false;
    this->setUiDefault();
    this->restoreValue();
    this->update();

    this->move(QApplication::desktop()->screen()->rect().topRight() - this->rect().topRight());
    show();
}

void Widget::events(int e, int x, int y, int flags)
{
    if(flags){;}
    switch(e) {
    case EVENT_LBUTTONDOWN:
        _lstart = Point(x,y);
        _isDrag = true;
        break;
    case EVENT_LBUTTONUP:
        _lend = Point(x,y);
        _isDrag = false;
        _src.copyTo(_dsp);
        if(_lend.x-_lstart.x>5 && _lend.y-_lstart.y>5)
        {
            Rect box(_lstart.x, _lstart.y, _lend.x-_lstart.x+1, _lend.y-_lstart.y+1);
            if(box.x+box.width > _src.cols) box.width = _src.cols-box.x;
            if(box.y+box.height > _src.rows) box.height = _src.rows-box.y;
            rectangle(_dsp, box, CV_RGB(0,200,200));
  //          line(_dsp, _lstart, _lend, CV_RGB(200,0,0));
            imshow(_wname, _dsp);
            Mat crop;
            _src(box).copyTo(crop);
            showImg(crop);
            setBox();
        }
        break;
    case EVENT_MOUSEMOVE:
        if(_isDrag)
        {
            _src.copyTo(_dsp);
            rectangle(_dsp, Rect(_lstart.x, _lstart.y, x-_lstart.x+1, y-_lstart.y+1), CV_RGB(0,200,200));
            imshow(_wname, _dsp);
        }
        break;
    default:
        break;
    };
}

void Widget::setBox()
{
    if(ui->radioButton_hL->isChecked())
    {
        char buffer[50];
        sprintf(buffer, "%d", _lstart.x);
        ui->lineEdit_hLminx->setText(QString(buffer));
        sprintf(buffer, "%d", _lstart.y);
        ui->lineEdit_hLminy->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.x);
        ui->lineEdit_hLmaxx->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.y);
        ui->lineEdit_hLmaxy->setText(QString(buffer));
        update();
    }
    if(ui->radioButton_hR->isChecked())
    {
        char buffer[50];
        sprintf(buffer, "%d", _lstart.x);
        ui->lineEdit_hRminx->setText(QString(buffer));
        sprintf(buffer, "%d", _lstart.y);
        ui->lineEdit_hRminy->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.x);
        ui->lineEdit_hRmaxx->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.y);
        ui->lineEdit_hRmaxy->setText(QString(buffer));
        update();
    }
    if(ui->radioButton_oL->isChecked())
    {
        char buffer[50];
        sprintf(buffer, "%d", _lstart.x);
        ui->lineEdit_oLminx->setText(QString(buffer));
        sprintf(buffer, "%d", _lstart.y);
        ui->lineEdit_oLminy->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.x);
        ui->lineEdit_oLmaxx->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.y);
        ui->lineEdit_oLmaxy->setText(QString(buffer));
        update();
    }
    if(ui->radioButton_oR->isChecked())
    {
        char buffer[50];
        sprintf(buffer, "%d", _lstart.x);
        ui->lineEdit_oRminx->setText(QString(buffer));
        sprintf(buffer, "%d", _lstart.y);
        ui->lineEdit_oRminy->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.x);
        ui->lineEdit_oRmaxx->setText(QString(buffer));
        sprintf(buffer, "%d", _lend.y);
        ui->lineEdit_oRmaxy->setText(QString(buffer));
        update();
    }
}

void Widget::setUiDefault()
{
    //active component
    ui->radioButton_hL->setChecked(false);
    ui->radioButton_hR->setChecked(false);
    ui->radioButton_oL->setChecked(false);
    ui->radioButton_oR->setChecked(false);
    //bounding box
    ui->lineEdit_hLminx->setText("");
    ui->lineEdit_hLminy->setText("");
    ui->lineEdit_hLmaxx->setText("");
    ui->lineEdit_hLmaxy->setText("");
    ui->lineEdit_hRminx->setText("");
    ui->lineEdit_hRminy->setText("");
    ui->lineEdit_hRmaxx->setText("");
    ui->lineEdit_hRmaxy->setText("");
    ui->lineEdit_oLminx->setText("");
    ui->lineEdit_oLminy->setText("");
    ui->lineEdit_oLmaxx->setText("");
    ui->lineEdit_oLmaxy->setText("");
    ui->lineEdit_oRminx->setText("");
    ui->lineEdit_oRminy->setText("");
    ui->lineEdit_oRmaxx->setText("");
    ui->lineEdit_oRmaxy->setText("");
    //grasp type
//    ui->comboBox_hL->setCurrentIndex(0);
//    ui->comboBox_hR->setCurrentIndex(0);
    //object category
//    ui->comboBox_oL->setCurrentIndex(0);
//    ui->comboBox_oR->setCurrentIndex(0);
    //object attribute
//    ui->checkBox_oLp->setChecked(false);
//    ui->checkBox_oLs->setChecked(false);
//    ui->checkBox_oLf->setChecked(false);
//    ui->checkBox_oLl->setChecked(false);
//    ui->checkBox_oLr->setChecked(false);
//    ui->checkBox_oRp->setChecked(false);
//    ui->checkBox_oRs->setChecked(false);
//    ui->checkBox_oRf->setChecked(false);
//    ui->checkBox_oRl->setChecked(false);
//    ui->checkBox_oRr->setChecked(false);
}

void Widget::restoreValue()
{
    //check if yaml file exists
    string ymlfile = _seqname+"/"+_filename+".yml";
    FileStorage fs(ymlfile, FileStorage::READ);
    if(!fs.isOpened())
    {
        return;
    }

    //restore value from yaml file...
    if((int)fs["lefthand"]["visible"])
    {
        fs["lefthand"]["grasp"] >> _hLgrasp;
        fs["lefthand"]["bndbox"]["xmin"] >> _hLminx;
        fs["lefthand"]["bndbox"]["ymin"] >> _hLminy;
        fs["lefthand"]["bndbox"]["xmax"] >> _hLmaxx;
        fs["lefthand"]["bndbox"]["ymax"] >> _hLmaxy;
        int index = ui->comboBox_hL->findText(QString::fromStdString(_hLgrasp));
        ui->comboBox_hL->setCurrentIndex(index);
        ui->lineEdit_hLminx->setText(QString::number(_hLminx));
        ui->lineEdit_hLminy->setText(QString::number(_hLminy));
        ui->lineEdit_hLmaxx->setText(QString::number(_hLmaxx));
        ui->lineEdit_hLmaxy->setText(QString::number(_hLmaxy));
        rectangle(_dsp, Rect(_hLminx, _hLminy, _hLmaxx-_hLminx+1, _hLmaxy-_hLminy+1), CV_RGB(0,200,200));
        imshow(_wname, _dsp);
    }
    if((int)fs["righthand"]["visible"])
    {
        fs["righthand"]["grasp"] >> _hRgrasp;
        fs["righthand"]["bndbox"]["xmin"] >> _hRminx;
        fs["righthand"]["bndbox"]["ymin"] >> _hRminy;
        fs["righthand"]["bndbox"]["xmax"] >> _hRmaxx;
        fs["righthand"]["bndbox"]["ymax"] >> _hRmaxy;
        int index = ui->comboBox_hR->findText(QString::fromStdString(_hRgrasp));
        ui->comboBox_hR->setCurrentIndex(index);
        ui->lineEdit_hRminx->setText(QString::number(_hRminx));
        ui->lineEdit_hRminy->setText(QString::number(_hRminy));
        ui->lineEdit_hRmaxx->setText(QString::number(_hRmaxx));
        ui->lineEdit_hRmaxy->setText(QString::number(_hRmaxy));
        rectangle(_dsp, Rect(_hRminx, _hRminy, _hRmaxx-_hRminx+1, _hRmaxy-_hRminy+1), CV_RGB(0,200,200));
        imshow(_wname, _dsp);
    }
    if((int)fs["leftobject"]["visible"])
    {
        fs["leftobject"]["name"] >> _oLclass;
        fs["leftobject"]["bndbox"]["xmin"] >> _oLminx;
        fs["leftobject"]["bndbox"]["ymin"] >> _oLminy;
        fs["leftobject"]["bndbox"]["xmax"] >> _oLmaxx;
        fs["leftobject"]["bndbox"]["ymax"] >> _oLmaxy;
        fs["leftobject"]["attribute"]["prismatic"] >> _isLprismatic;
        fs["leftobject"]["attribute"]["sphere"] >> _isLsphere;
        fs["leftobject"]["attribute"]["flat"] >> _isLflat;
        fs["leftobject"]["attribute"]["large"] >> _isLlarge;
        fs["leftobject"]["attribute"]["rigid"] >> _isLrigid;
        int index = ui->comboBox_oL->findText(QString::fromStdString(_oLclass));
        ui->comboBox_oL->setCurrentIndex(index);
        ui->lineEdit_oLminx->setText(QString::number(_oLminx));
        ui->lineEdit_oLminy->setText(QString::number(_oLminy));
        ui->lineEdit_oLmaxx->setText(QString::number(_oLmaxx));
        ui->lineEdit_oLmaxy->setText(QString::number(_oLmaxy));
        ui->checkBox_oLp->setChecked(_isLprismatic);
        ui->checkBox_oLs->setChecked(_isLsphere);
        ui->checkBox_oLf->setChecked(_isLflat);
        ui->checkBox_oLl->setChecked(_isLlarge);
        ui->checkBox_oLr->setChecked(_isLrigid);
        rectangle(_dsp, Rect(_oLminx, _oLminy, _oLmaxx-_oLminx+1, _oLmaxy-_oLminy+1), CV_RGB(0,200,200));
        imshow(_wname, _dsp);
    }
    if((int)fs["rightobject"]["visible"])
    {
        fs["rightobject"]["name"] >> _oRclass;
        fs["rightobject"]["bndbox"]["xmin"] >> _oRminx;
        fs["rightobject"]["bndbox"]["ymin"] >> _oRminy;
        fs["rightobject"]["bndbox"]["xmax"] >> _oRmaxx;
        fs["rightobject"]["bndbox"]["ymax"] >> _oRmaxy;
        fs["rightobject"]["attribute"]["prismatic"] >> _isRprismatic;
        fs["rightobject"]["attribute"]["sphere"] >> _isRsphere;
        fs["rightobject"]["attribute"]["flat"] >> _isRflat;
        fs["rightobject"]["attribute"]["large"] >> _isRlarge;
        fs["rightobject"]["attribute"]["rigid"] >> _isRrigid;
        int index = ui->comboBox_oR->findText(QString::fromStdString(_oRclass));
        ui->comboBox_oR->setCurrentIndex(index);
        ui->lineEdit_oRminx->setText(QString::number(_oRminx));
        ui->lineEdit_oRminy->setText(QString::number(_oRminy));
        ui->lineEdit_oRmaxx->setText(QString::number(_oRmaxx));
        ui->lineEdit_oRmaxy->setText(QString::number(_oRmaxy));
        ui->checkBox_oRp->setChecked(_isRprismatic);
        ui->checkBox_oRs->setChecked(_isRsphere);
        ui->checkBox_oRf->setChecked(_isRflat);
        ui->checkBox_oRl->setChecked(_isRlarge);
        ui->checkBox_oRr->setChecked(_isRrigid);
        rectangle(_dsp, Rect(_oRminx, _oRminy, _oRmaxx-_oRminx+1, _oRmaxy-_oRminy+1), CV_RGB(0,200,200));
        imshow(_wname, _dsp);
    }

}

void Widget::saveValue()
{
    //bounding box
    _hLminx = ui->lineEdit_hLminx->text().toInt();
    _hLminy = ui->lineEdit_hLminy->text().toInt();
    _hLmaxx = ui->lineEdit_hLmaxx->text().toInt();
    _hLmaxy = ui->lineEdit_hLmaxy->text().toInt();
    _hRminx = ui->lineEdit_hRminx->text().toInt();
    _hRminy = ui->lineEdit_hRminy->text().toInt();
    _hRmaxx = ui->lineEdit_hRmaxx->text().toInt();
    _hRmaxy = ui->lineEdit_hRmaxy->text().toInt();
    _oLminx = ui->lineEdit_oLminx->text().toInt();
    _oLminy = ui->lineEdit_oLminy->text().toInt();
    _oLmaxx = ui->lineEdit_oLmaxx->text().toInt();
    _oLmaxy = ui->lineEdit_oLmaxy->text().toInt();
    _oRminx = ui->lineEdit_oRminx->text().toInt();
    _oRminy = ui->lineEdit_oRminy->text().toInt();
    _oRmaxx = ui->lineEdit_oRmaxx->text().toInt();
    _oRmaxy = ui->lineEdit_oRmaxy->text().toInt();
    //grasp type
    _hLgrasp = ui->comboBox_hL->currentText().toStdString();
    _hRgrasp = ui->comboBox_hR->currentText().toStdString();
    //object category
    _oLclass = ui->comboBox_oL->currentText().toStdString();
    _oRclass = ui->comboBox_oR->currentText().toStdString();
    //object attribute
    _isLprismatic = ui->checkBox_oLp->isChecked();
    _isLsphere = ui->checkBox_oLs->isChecked();
    _isLflat = ui->checkBox_oLf->isChecked();
    _isLlarge = ui->checkBox_oLl->isChecked();
    _isLrigid = ui->checkBox_oLr->isChecked();
    _isRprismatic = ui->checkBox_oRp->isChecked();
    _isRsphere = ui->checkBox_oRs->isChecked();
    _isRflat = ui->checkBox_oRf->isChecked();
    _isRlarge = ui->checkBox_oRl->isChecked();
    _isRrigid = ui->checkBox_oRr->isChecked();

    //save to yaml file
    string ymlfile = _seqname+"/"+_filename+".yml";
    FileStorage fs(ymlfile, FileStorage::WRITE);
    if(!fs.isOpened())
    {
        cout << "[Widget::saveValue]ERROR: failed to open file: " << ymlfile << endl;
        this->close();
    }
    fs << "seqname" << _seqname;
    fs << "filename" << _filename;
    fs << "annotator" << "Minjie Cai";
    fs << "size" << "{:" << "width" << _src.cols << "height" << _src.rows << "depth" << _src.channels() << "}";
    fs << "lefthand" << "{";
    fs << "visible" << (_hLmaxx != 0);
    fs << "grasp" << _hLgrasp;
    fs << "bndbox" << "{:" << "xmin" << _hLminx << "ymin" << _hLminy << "xmax" << _hLmaxx << "ymax" << _hLmaxy << "}";
    fs << "}";
    fs << "righthand" << "{";
    fs << "visible" << (_hRmaxx != 0);
    fs << "grasp" << _hRgrasp;
    fs << "bndbox" << "{:" << "xmin" << _hRminx << "ymin" << _hRminy << "xmax" << _hRmaxx << "ymax" << _hRmaxy << "}";
    fs << "}";
    fs << "leftobject" << "{";
    fs << "visible" << (_oLmaxx != 0);
    fs << "name" << _oLclass;
    fs << "attribute" << "{:";
    fs << "prismatic" << _isLprismatic;
    fs << "sphere" << _isLsphere;
    fs << "flat" << _isLflat;
    fs << "large" << _isLlarge;
    fs << "rigid" << _isLrigid;
    fs << "}";
    fs << "bndbox" << "{:" << "xmin" << _oLminx << "ymin" << _oLminy << "xmax" << _oLmaxx << "ymax" << _oLmaxy << "}";
    fs << "}";
    fs << "rightobject" << "{";
    fs << "visible" << (_oRmaxx != 0);
    fs << "name" << _oRclass;
    fs << "attribute" << "{:";
    fs << "prismatic" << _isRprismatic;
    fs << "sphere" << _isRsphere;
    fs << "flat" << _isRflat;
    fs << "large" << _isRlarge;
    fs << "rigid" << _isRrigid;
    fs << "}";
    fs << "bndbox" << "{:" << "xmin" << _oRminx << "ymin" << _oRminy << "xmax" << _oRmaxx << "ymax" << _oRmaxy << "}";
    fs << "}";
    fs.release();


    destroyWindow(_wname);
    this->close();
}

void Widget::cancel()
{
    destroyWindow(_wname);
    this->close();
}

void Widget::quit()
{
    destroyWindow(_wname);
    _isQuit = true;
    this->close();
}

static void wevents( int e, int x, int y, int flags, void* ptr)
{
    Widget *mptr =  (Widget*)ptr;

    if(mptr != NULL) mptr->events(e,x,y, flags);

}
