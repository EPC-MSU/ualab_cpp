#ifndef UALAB_H
#define UALAB_H

#include <QMainWindow>
#include <QThread>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QList>
#include <QColor>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_symbol.h>

#include <vector>

#include <QDebug>

#include "usbadc10.h"
#include "dataupdater.h"

#if defined(Q_CC_MSVC)
#  define QT_STATIC_CONST static
#  define QT_STATIC_CONST_IMPL
#else
#  define QT_STATIC_CONST static const
#  define QT_STATIC_CONST_IMPL const
#endif


#define VERSION "dev"
#define NUMBERFRAMES 1000


namespace Ui {
class MainWindow;
}

class UALab : public QMainWindow
{
    Q_OBJECT

public:
    explicit UALab (QWidget *parent = nullptr);
    ~UALab();
    const QList<QColor> graphcolor = {QColor(0, 0, 255), QColor(0, 170, 0),     QColor(255, 0, 0),
                                      QColor(0, 0, 0),   QColor(255, 85, 0),    QColor(0, 170, 255),
                                      QColor(0, 255, 0), QColor(255, 170, 255), QColor(111, 111, 111),
                                      QColor(170, 85, 0)};
    int progressframes;
    double dataY[10][NUMBERFRAMES];
    double dataX[NUMBERFRAMES];
    bool gstates[10] = {true, true, true, true, true, true, true, true, true, true};
    bool start_stop_status = false;
    bool start_stop_recording_status = false;
    void reset();
    double elapsed() const;
    std::vector<std::vector<double>> record_data;

private:
    using clock_t = std::chrono::steady_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
    Ui::MainWindow *m_ui;
    QTimer *tmr;
    QwtPlotGrid *grid;
    QwtPlotCurve *cruve[10];
    QSerialPortInfo *serial;
    device_t ualab_device;
    QFile outputfile;
    QString filename;
    usbadc10_get_conversion_t ualab_data;
    std::chrono::time_point<clock_t> start;
    QThread thread_data;
    DataUpdater dataupdaerobj;

public slots:
    void this_application();
    void rescan();
    void updategraph();
    void start_stop_handler();
    void start_stop_recording_handler();
    void connection();
    void disconnection();
    void change_period();
    void save_handler();

//     I am like China code))))0))0)
//     Sorry, may be I will fix it.
    void change_state01();
    void change_state02();
    void change_state03();
    void change_state04();
    void change_state05();
    void change_state06();
    void change_state07();
    void change_state08();
    void change_state09();
    void change_state10();

};

#endif // UALAB_H
