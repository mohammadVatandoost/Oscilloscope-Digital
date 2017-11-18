#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwidget.h>
#include "plot.h"
#include "plot2.h"
#include "knob.h"
#include "knob2.h"
#include "wheelbox.h"
#include "wheelbox2.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <QtMath>

#include <QtCore>
#include <QPixmap>
#include <QLabel>
#include <QtCore>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/qserialportglobal.h>

#include <QTextEdit>
#include <qtextedit.h>
#include <QDebug>
#include <time.h>
#include "QMessageBox"
#include <QProgressBar>
#include <qprogressbar.h>
#include "samplingthread.h"
#include "samplingthread2.h"
#include <time.h>
#include <QIcon>
class Plot;
class Plot2;
class Knob;
class Knob2;
class WheelBox;
class WheelBox2;



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void start();
    float temp0,temp1;
    QSerialPort *serial;
    QString come_port;
    QByteArray recieve;
    QPixmap *mypix;
    QIcon *icon;
    float voltage[3],current[2];
    QString checkbox[5];
    float test[1000];
    int k = 0;
    int l=0;
    int p=0;
    int b = 0 ;
    int s =0 ;
    int flag_test=0;
    float last_value2 = 0;
    float last_value = 0;
    QTimer *dataTimer;
    QString text_edit;
//    double signalInterval() const;
    SamplingThread *samplingThread;
    SamplingThread2 *samplingThread_1;
Q_SIGNALS:
//    void signalIntervalChanged( double );
    float getVoltage1(float);
    float getVoltage2(float);
private:
    Ui::MainWindow *ui;

    WheelBox *d_intervalWheel;
    WheelBox2 *d_intervalWheel_2;
    Plot *d_plot;
    Plot2 *d_plot_2;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void machineState();
    void realtimeDataSlot();
};

#endif // MAINWINDOW_H
