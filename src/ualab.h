#ifndef UALAB_H
#define UALAB_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QList>
#include <QColor>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_symbol.h>

namespace Ui {
class MainWindow;
}

class ualab : public QMainWindow
{
    Q_OBJECT

public:
    explicit ualab(QWidget *parent = nullptr);
    ~ualab() override;
    const QList<QColor> graphcolor = {QColor(0, 0, 255), QColor(0, 170, 0),     QColor(255, 0, 0),
                                      QColor(0, 0, 0),   QColor(255, 85, 0),    QColor(0, 170, 255),
                                      QColor(0, 255, 0), QColor(255, 170, 255), QColor(111, 111, 111),
                                      QColor(170, 85, 0)};
    int progressframes;
    double dataY[10][1000];
    double dataX[1000];

private:
    Ui::MainWindow *m_ui;
    QTimer *tmr;
    QwtPlotGrid *grid;
    QwtPlotCurve *cruve[10];
    QSerialPortInfo *serial;

public slots:
    void this_application();
    void rescan();
    void updatrgraph();
    void start_stop_handler();
};

#endif // UALAB_H
