#ifndef UALAB_H
#define UALAB_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QTime>
//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>
#include <QFile>
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

private:
    Ui::MainWindow *m_ui;
    QwtPlotGrid *grid;
    QwtPlotCurve *cruve;

public slots:
    void this_application();
};

#endif // UALAB_H
