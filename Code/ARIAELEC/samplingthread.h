#include <qwt_sampling_thread.h>

class SamplingThread: public QwtSamplingThread
{
    Q_OBJECT

public:
    SamplingThread( QObject *parent = NULL );

//    double frequency() const;
    double amplitude() const;
    int i=0;
    float Channel_value=0;
//    float getChannelValue(float f);
public Q_SLOTS:
    void setAmplitude( double );
//    void setFrequency( double );

protected:
    virtual void sample( double elapsed );

private:
//    virtual double value( double timeStamp ) ;

    double d_frequency;
    double d_amplitude;
public slots:
   float takeValue(float d ) ;

};
