#include "ualab.h"
#include "ui_ualab.h"
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include <qwt_picker_machine.h>
#include <qwt_scale_engine.h>
#include <math.h>
#include <limits.h>

ualab::ualab(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->setMajorPen(QPen(Qt::gray,   1, Qt::SolidLine));
    grid->setMinorPen(QPen(Qt::gray, 0.5, Qt::DotLine));
    grid->attach(m_ui->graphWidget);
    QwtPlotPicker *d_picker = new QwtPlotPicker(
        QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::CrossRubberBand,
        QwtPicker::ActiveOnly,
        m_ui->graphWidget->canvas());
    d_picker->setRubberBandPen( QColor( Qt::red ) );
    d_picker->setTrackerPen( QColor( Qt::black ) );
    d_picker->setStateMachine(new QwtPickerDragPointMachine());

    for(int i=0; i < 10; i++)
    {
        cruve[i] = new QwtPlotCurve();
        cruve[i]->setPen(graphcolor.at(i), 2, Qt::SolidLine);
        cruve[i]->attach(m_ui->graphWidget);
    }

    m_ui->graphWidget->enableAxis(QwtPlot::yRight, true);
    m_ui->graphWidget->setAxisTitle(QwtPlot::yLeft, "Voltage, V");
    m_ui->graphWidget->setAxisTitle(QwtPlot::xBottom, "Time, s");
    m_ui->graphWidget->setAxisScale(QwtPlot::yLeft, 0, 3.3, 0.5);
    m_ui->graphWidget->setAxisScale(QwtPlot::yRight, 0, 3.3, 0.5);
    m_ui->graphWidget->axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating, true);
    m_ui->graphWidget->replot();

    double period_vals[] = {0.001, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 5, 10, 60, 300, 600};
    for(int i=0; i < 13; i++)
    {
        m_ui->comboBox_period_val->setItemData(i, period_vals[i]);
    }

    m_ui->disconnect_button->setEnabled(false);
    m_ui->start_stop_recording->setEnabled(false);
    m_ui->start_stop->setEnabled(false);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->action_start_stop_recording->setEnabled(false);
    m_ui->action_start_stop_getting_data->setEnabled(false);

    tmr = new QTimer(this);

    connect(tmr, SIGNAL(timeout()), this, SLOT(updatrgraph()));
    connect(m_ui->actionThis_Application, SIGNAL(triggered(bool)), this, SLOT(this_application()));
    connect(m_ui->actionRescan, SIGNAL(triggered(bool)), this, SLOT(rescan()));
    connect(m_ui->action_start_stop_getting_data, SIGNAL(triggered(bool)), this, SLOT(start_stop_handler()));
    rescan();
    start_stop_handler();
}

ualab::~ualab() = default;


void ualab::this_application()
{
    QMessageBox msbox;
    msbox.setText("This is a simple cross-platform application for the usbadc10 device.\nVersion dev\nCopyright © 2021 Nikita Presnov\npresnovnikita@yandex.ru");
    msbox.exec();
}

void ualab::rescan()
{
    m_ui->comboBox_ports->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        m_ui->comboBox_ports->addItem(info.portName());
    }
}

void ualab::updatrgraph()
{
    if(progressframes < 1000)
    {
        dataX[progressframes] = progressframes;
        for(int j = 0; j < 10; j++)
        {
            dataY[j][progressframes] = sin((float)progressframes / 100 + (float)j / 20) + 1.5;
        }
        for(int i = 0; i < 10; i++)
        {
            cruve[i]->setSamples(dataX, dataY[i], progressframes+1);
        }
    }
    else if(progressframes < INT_MAX/1000)
    {
        for(int i = 0; i < 1000-1; i++)
        {
            dataX[i] = dataX[i+1];
        }
        dataX[1000-1] = progressframes;

        for(int j = 0; j < 10; j++)
        {
            for(int i = 0; i < 1000-1; i++)
            {
                dataY[j][i] = dataY[j][i+1];
            }
            dataY[j][1000-1] = sin((float)progressframes / 100 + (float)j / 20) + 1.5;
        }
        for(int i = 0; i < 10; i++)
        {
            cruve[i]->setSamples(dataX, dataY[i], 1000);
        }
    }
    else
        progressframes = 0;
    progressframes++;
    m_ui->graphWidget->replot();
}

void ualab::start_stop_handler()
{
    for(int i = 0; i < 1000; i++)
    {
        dataX[i] = 0;
        for(int j = 0; j < 10; j++)
        {
            dataY[j][i] = 0;
        }
    }
    progressframes = 0;
    int period = 1000 * m_ui->comboBox_period_val->currentData().toInt();
    tmr->setInterval(period);
    tmr->start();
}



