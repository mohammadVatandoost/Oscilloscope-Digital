#include <qwt_series_data.h>
#include <qpointer.h>

class SignalData2;

class CurveData2: public QwtSeriesData<QPointF>
{
public:
    const SignalData2 &values() const;
    SignalData2 &values();

    virtual QPointF sample( size_t i ) const;
    virtual size_t size() const;

    virtual QRectF boundingRect() const;
};
