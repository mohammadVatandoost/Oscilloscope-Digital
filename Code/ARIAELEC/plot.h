#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>


#include "curvedata.h"
#include "signaldata.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_glcanvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_curve_fitter.h>
#include <qwt_painter.h>
#include <qevent.h>

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotDirectPainter;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget * = NULL );
    virtual ~Plot();
    CurveData *data;
    int counter = 0;
    void start();
    virtual void replot();

    virtual bool eventFilter( QObject *, QEvent * );

public Q_SLOTS:
    void setIntervalLength( double );

protected:
    virtual void showEvent( QShowEvent * );
    virtual void resizeEvent( QResizeEvent * );
    virtual void timerEvent( QTimerEvent * );

private:
    void updateCurve();
    void incrementInterval();

    QwtPlotMarker *d_origin;
    QwtPlotCurve *d_curve,*d_curve_2;
    int d_paintedPoints,d_paintedPoints_2;

    QwtPlotDirectPainter *d_directPainter,*d_directPainter_2;

    QwtInterval d_interval;
    int d_timerId;

    QwtSystemClock d_clock;
};
