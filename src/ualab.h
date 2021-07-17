#ifndef UALAB_H
#define UALAB_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QList>
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

private:
    Ui::MainWindow *m_ui;
    QwtPlotGrid *grid;
    QwtPlotCurve *cruve;
//     QList *listports;
    QSerialPortInfo *serial;

public slots:
    void this_application();
    void rescan();
};

#endif // UALAB_H
