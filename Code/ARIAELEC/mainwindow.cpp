#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Disconnect
    ui->pushButton_2->setDisabled(true);
    //Wait
    ui->progressBar->setValue(0);
    ui->lineEdit->setText("Disconnect");
    // Logo
    mypix = new QPixmap("2.jpg");
    ui->label->setPixmap(*mypix);
    //Icon
    icon = new QIcon("AriaElec.icon");
    MainWindow::setWindowIcon(*icon);
  //  statusBar()->addWidget(icon);
    // Serial Port
      serial = new QSerialPort(this);
      serial->close();
      serial->setBaudRate(QSerialPort::Baud115200);
      serial->setDataBits(QSerialPort::Data8);
      serial->setParity(QSerialPort::NoParity);
      serial->setStopBits(QSerialPort::OneStop);
      serial->setFlowControl(QSerialPort::NoFlowControl);

    //Channel 1
    d_plot = new Plot( this );
    d_plot->setIntervalLength( 2 );
    d_plot->setTitle("Channel 1");


    d_intervalWheel = new WheelBox( "Displayed [s]", 0.1, 10.0, 0.1, this );
    d_intervalWheel->setValue( 2 );

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget( d_intervalWheel );
    vLayout1->addStretch( 10 );

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 10 );
    layout->addLayout( vLayout1 );

    ui->widget->setLayout(layout);

    connect( d_intervalWheel, SIGNAL( valueChanged( double ) ),
        d_plot, SLOT( setIntervalLength( double ) ) );

    samplingThread = new SamplingThread(this);
    samplingThread->setInterval(1);
    samplingThread->start();
    connect(this, SIGNAL(getVoltage1(float)),samplingThread , SLOT(takeValue(float)) );
    //Channel 2
    d_plot_2 = new Plot2( this );
    d_plot_2->setIntervalLength( 2);
    d_plot_2->setTitle("Channel 2");

    d_intervalWheel_2 = new WheelBox2( "Displayed [s]", 0.1, 10.0, 0.1, this );
    d_intervalWheel_2->setValue( 2 );

    QVBoxLayout* vLayout2 = new QVBoxLayout();
    vLayout2->addWidget( d_intervalWheel_2 );
    vLayout2->addStretch( 10 );

    QHBoxLayout *layout_2 = new QHBoxLayout( this );
    layout_2->addWidget( d_plot_2, 10 );
    layout_2->addLayout( vLayout2 );

    ui->widget_2->setLayout(layout_2);

    connect( d_intervalWheel_2, SIGNAL( valueChanged( double ) ),
        d_plot_2, SLOT( setIntervalLength( double ) ) );

    samplingThread_1 = new SamplingThread2(this);
    samplingThread_1->setInterval( 1 );
    samplingThread_1->start();
    connect(this, SIGNAL(getVoltage2(float)),samplingThread_1 , SLOT(takeValue(float)) );

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
//    dataTimer = new QTimer(this);
//    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//        dataTimer->start(1); // Interval 0 means to refresh as fast as possible
}

MainWindow::~MainWindow()
{
    serial->close();
    samplingThread->stop();
    samplingThread->wait( 1000 );
//    samplingThread_1->stop();
//    samplingThread_1->wait( 1000 );

    delete serial;
    delete samplingThread;
    delete samplingThread_1;
    delete d_plot;
    delete d_plot_2;
    delete d_intervalWheel;
    delete d_intervalWheel_2;
    delete ui;
}

void MainWindow::start()
{
    d_plot->start();
    d_plot_2->start();
}
//***************************
void MainWindow::realtimeDataSlot()
{
   machineState();
}


//*******************
void MainWindow::machineState()
{
//    p++;
//    qDebug() << "p : " << p ;
    QByteArray data;
    data = serial->readAll();
//   qDebug() <<"data" << data ;
    uint8_t data_buffr[10],temp;
//    uint8_t test_data[10];
    //debug
//    QByteArray test;
//    test_data[0] = data[0] ;
//    test_data[1] = data[1] ;
//    test_data[2] = data[2] ;
//    test_data[3] = data[3] ;
//    test_data[4] = data[4] ;
//    test_data[5] = data[5] ;
//    test_data[6] = data[6] ;
//    test_data[7] = data[7] ;
//    test_data[8] = data[8] ;
//    qDebug() <<"test_data[0]" << test_data[0] ;
//    qDebug() <<"test_data[1]" << test_data[1] ;
//    qDebug() <<"test_data[2]" << test_data[2] ;
//    qDebug() <<"test_data[3]" << test_data[3] ;
//    qDebug() <<"test_data[4]" << test_data[4] ;
//    qDebug() <<"test_data[5]" << test_data[5] ;
//    qDebug() <<"test_data[6]" << test_data[6] ;
//    qDebug() <<"test_data[7]" << test_data[7] ;
//    qDebug() <<"test_data[8]" << test_data[8] ;
    //********
//    int j=0;
//    qDebug() << "data.size() : " << data.size() ;
 if( data.size() > 3) {
    for(int i=0 ; i < data.size()-2 ; i++ )
    {
        temp = data[i];
//        test_data[0] = data[0] ;
//        if(temp == 9)
//        {
//            s++;
//          qDebug() <<"s :" << s ;
//        }
        if(temp == 1 )
        {
            i++;
            data_buffr[0] = 0 ;
            i++;
            data_buffr[1] = 0 ;
            voltage[2] = ((( data_buffr[0]*255 ) + ( data_buffr[1] ))*0.00413);
        } else if(temp == 3 )
        {
            i++;
            data_buffr[0] = 0 ;
            i++;
            data_buffr[1] = data[i] ;
            voltage[2] = ((( data_buffr[0]*255 ) + ( data_buffr[1] ))*0.00413);
        } else if(temp == 5 )
        {
            i++;
            data_buffr[0] = data[i] ;
            i++;
            data_buffr[1] = 0 ;
            voltage[2] = ((( data_buffr[0]*255 ) + ( data_buffr[1] ))*0.00413);
        } else if(temp == 7 )
        {
            i++;
            data_buffr[0] = data[i] ;
            i++;
            data_buffr[1] = data[i] ;
            voltage[2] = ((( data_buffr[0]*255 ) + ( data_buffr[1] ))*0.00413);
        } else if(temp == 31 )
        {
            i++;
            data_buffr[2] = 0 ;
            i++;
            data_buffr[3] = 0 ;
            voltage[0] = ((( data_buffr[2]*255 ) + ( data_buffr[3] ))*0.001203);
        } else if(temp == 62 )
        {
            i++;
            data_buffr[2] = 0 ;
            i++;
            data_buffr[3] = data[i] ;
            voltage[0] = ((( data_buffr[2]*255 ) + ( data_buffr[3] ))*0.001203);
        } else if(temp == 124 )
        {
            i++;
            data_buffr[2] = data[i] ;
            i++;
            data_buffr[3] = 0 ;
            voltage[0] = ((( data_buffr[2]*255 ) + ( data_buffr[3] ))*0.001203);
        } else if(temp == 248 )
        {
            i++;
            data_buffr[2] = data[i] ;
            i++;
            data_buffr[3] = data[i] ;
            voltage[0] = ((( data_buffr[2]*255 ) + ( data_buffr[3] ))*0.001203);
        } else if(temp == 17 )
        {
            i++;
            data_buffr[4] = 0 ;
            i++;
            data_buffr[5] = 0 ;
            voltage[1] = ((( data_buffr[4]*255 ) + ( data_buffr[5] ))*0.001203);
        } else if(temp == 19 )
        {
            i++;
            data_buffr[4] = 0 ;
            i++;
            data_buffr[5] = data[i] ;
            voltage[1] = ((( data_buffr[4]*255 ) + ( data_buffr[5] ))*0.001203);
        } else if(temp == 21 )
        {
            i++;
            data_buffr[4] = data[i] ;
            i++;
            data_buffr[5] = 0 ;
            voltage[1] = ((( data_buffr[4]*255 ) + ( data_buffr[5] ))*0.001203);
        } else if(temp == 23 )
        {
            i++;
            data_buffr[4] = data[i] ;
            i++;
            data_buffr[5] = data[i] ;
            voltage[1] = ((( data_buffr[4]*255 ) + ( data_buffr[5] ))*0.001203);
        }

        if(abs((last_value - voltage[0])) > 0.01)
        {
            emit getVoltage1(voltage[0]);
            last_value = voltage[0];
        }
        if(abs((last_value2 - voltage[1])) > 0.01)
        {
            emit getVoltage2(voltage[1]);
            last_value = voltage[1];
        }

        text_edit = QString::number(voltage[0]);
        ui->textEdit_6->setText(text_edit);

        text_edit = QString::number(voltage[1]);
        ui->textEdit_7->setText(text_edit);

        text_edit = QString::number(voltage[2]);
        ui->textEdit_8->setText(text_edit);

                    s++;
                    qDebug() << "s :" << s ;
//        qDebug() << "i :" << i ;
//        qDebug() << "b :" << b ;
//        qDebug() << "voltage[0] :" << voltage[0] ;
//        qDebug() << "voltage[1] :" << voltage[1] ;
//        qDebug() << "data_buffr[1] :" << data_buffr[0] ;
//        qDebug() << "data_buffr[1] :" << data_buffr[1] ;
//        qDebug() << "data_buffr[2] :" << data_buffr[2] ;
//        qDebug() << "data_buffr[3] :" << data_buffr[3] ;
//        qDebug() << "data_buffr[4] :" << data_buffr[4] ;
//        qDebug() << "data_buffr[5] :" << data_buffr[5] ;
//        qDebug() << "data_buffr[6] :" << data_buffr[6] ;
//        qDebug() << "data_buffr[7] :" << data_buffr[7] ;
//        qDebug() << "data_buffr[8] :" << data_buffr[8] ;
//        qDebug() << "data_buffr[9] :" << data_buffr[9] ;
    }
  }
//    qDebug() << "End";
}

//***************************

void MainWindow::on_pushButton_clicked()
{

    ui->lineEdit->setText("Wait..");
    ui->progressBar->setValue(60);

    if(ui->comboBox->currentText() != NULL) {

        serial->close();
        come_port = ui->comboBox->currentText();
//        qDebug() << come_port;
        serial->setPortName(come_port);

        serial->close();
//        qDebug() << "2";
        while(!serial->open(QIODevice::ReadWrite));

        if(serial->waitForReadyRead(1000))
        {
            ui->progressBar->setValue(0);
            ui->lineEdit->setText("Try again");
        } else
        { 
            ui->progressBar->setValue(100);
            ui->lineEdit->setText("Connected");
        }
//        qDebug() << "3";
        ui->pushButton->setDisabled(true);
        ui->pushButton_2->setEnabled(true);
        connect(serial, SIGNAL(readyRead()), SLOT(machineState()));
//        dataTimer->start(1);
//        samplingThread->setInterval( 1 );
//        samplingThread->start();
//        connect(this, SIGNAL(getVoltage1(float)),samplingThread , SLOT(takeValue(float)) );
//        d_plot->start();
    }

}

void MainWindow::on_pushButton_2_clicked()
{
//    samplingThread->stop();
//    samplingThread->wait( 1000 );

    voltage[0] = 0;
    voltage[1] = 0;

    emit getVoltage1(voltage[1]);
    emit getVoltage2(voltage[0]);

    ui->lineEdit->setText("WaIT...");
    ui->progressBar->setValue(60);

    serial->close();
    ui->pushButton_2->setDisabled(true);
    ui->pushButton->setEnabled(true);


    ui->progressBar->setValue(0);
    ui->lineEdit->setText("Disconnect");
}


void MainWindow::on_pushButton_3_clicked()
{
//    update Come Port
    ui->comboBox->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {


           ui->comboBox->addItem(port.portName());

    }
}
