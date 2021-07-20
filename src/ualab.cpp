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

    int period_vals[] = {1, 10, 20, 50, 100, 200, 500, 1000, 5000, 10000, 60000, 300000, 600000};
    for(int i=0; i < 13; i++)
    {
        m_ui->comboBox_period_val->setItemData(i, period_vals[i]);
    }

    m_ui->disconnect_button->setEnabled(false);
    m_ui->start_stop_recording->setEnabled(false);
    m_ui->start_stop->setEnabled(false);
    m_ui->action_disconnect->setEnabled(false);
    m_ui->action_start_stop_recording->setEnabled(false);
    m_ui->action_start_stop->setEnabled(false);

    tmr = new QTimer(this);

    connect(tmr, SIGNAL(timeout()), this, SLOT(updatrgraph()));
    connect(m_ui->action_this_application, SIGNAL(triggered(bool)), this, SLOT(this_application()));
    connect(m_ui->action_rescan, SIGNAL(triggered(bool)), this, SLOT(rescan()));
    connect(m_ui->action_start_stop, SIGNAL(triggered(bool)), this, SLOT(start_stop_handler()));
    connect(m_ui->action_connect, SIGNAL(triggered(bool)), this, SLOT(connection()));
    connect(m_ui->action_disconnect, SIGNAL(triggered(bool)), this, SLOT(disconnection()));

    connect(m_ui->action01, SIGNAL(toggled(bool)), this, SLOT(change_state01()));
    connect(m_ui->action02, SIGNAL(toggled(bool)), this, SLOT(change_state02()));
    connect(m_ui->action03, SIGNAL(toggled(bool)), this, SLOT(change_state03()));
    connect(m_ui->action04, SIGNAL(toggled(bool)), this, SLOT(change_state04()));
    connect(m_ui->action05, SIGNAL(toggled(bool)), this, SLOT(change_state05()));
    connect(m_ui->action06, SIGNAL(toggled(bool)), this, SLOT(change_state06()));
    connect(m_ui->action07, SIGNAL(toggled(bool)), this, SLOT(change_state07()));
    connect(m_ui->action08, SIGNAL(toggled(bool)), this, SLOT(change_state08()));
    connect(m_ui->action09, SIGNAL(toggled(bool)), this, SLOT(change_state09()));
    connect(m_ui->action10, SIGNAL(toggled(bool)), this, SLOT(change_state10()));


    rescan();
//     start_stop_handler();
}

ualab::~ualab()
{
    usbadc10_close_device(&ualab_device);
    delete m_ui;
}


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
    if(progressframes < NUMBERFRAMES)
    {
        dataX[progressframes] = progressframes;
        for(int j = 0; j < 10; j++)
        {
            dataY[j][progressframes] = sin((float)progressframes / 100 + (float)j / 20) + 1.5;
        }
        for(int i = 0; i < 10; i++)
        {
            if(gstates[i])
                cruve[i]->setSamples(dataX, dataY[i], progressframes+1);
            else
                cruve[i]->setSamples(0, 0, 0);
        }
    }
    else if(progressframes < INT_MAX/NUMBERFRAMES)
    {
        for(int i = 0; i < NUMBERFRAMES-1; i++)
        {
            dataX[i] = dataX[i+1];
        }
        dataX[NUMBERFRAMES-1] = progressframes;

        for(int j = 0; j < 10; j++)
        {
            for(int i = 0; i < NUMBERFRAMES-1; i++)
            {
                dataY[j][i] = dataY[j][i+1];
            }
            dataY[j][NUMBERFRAMES-1] = sin((float)progressframes / 100 + (float)j / 20) + 1.5;
        }
        for(int i = 0; i < 10; i++)
        {
            if(gstates[i])
                cruve[i]->setSamples(dataX, dataY[i], NUMBERFRAMES);
            else
                cruve[i]->setSamples(0, 0, 0);
        }
    }
    else
        progressframes = 0;
    progressframes++;
    m_ui->graphWidget->replot();
}

void ualab::start_stop_handler()
{
    int period = m_ui->comboBox_period_val->currentData().toInt();
    if(!start_stop_status)
    {
        for(int i = 0; i < NUMBERFRAMES; i++)
        {
            dataX[i] = 0;
            for(int j = 0; j < 10; j++)
            {
                dataY[j][i] = 0;
            }
        }
        progressframes = 0;
        start_stop_status = true;
        tmr->setInterval(period);
        tmr->start();
    }
    else
    {
        start_stop_status = false;
        tmr->stop();
    }
}
void ualab::connection()
{
    usbadc10_close_device(&ualab_device);

    #ifdef Q_OS_WIN32
    QString devisename = "com:\\\\.\\" + m_ui->comboBox_ports->currentText();
    #else
        #ifdef Q_OS_LINUX
    QString ualab_devicename = "com:///dev/" + m_ui->comboBox_ports->currentText();
        #endif
    #endif

    ualab_device = usbadc10_open_device(ualab_devicename.toStdString().c_str());

    if(ualab_device == device_undefined)
    {
        QMessageBox msbox;
        msbox.setText(m_ui->comboBox_ports->currentText() + " is not correct device.");
        msbox.exec();
    }
    else
    {
        m_ui->action_disconnect->setEnabled(true);
        m_ui->action_rescan->setEnabled(false);
        m_ui->action_start_stop->setEnabled(true);
        m_ui->action_connect->setEnabled(false);
        m_ui->start_stop->setEnabled(true);
        m_ui->rescan_botton->setEnabled(false);
        m_ui->disconnect_button->setEnabled(true);
        m_ui->connect_button->setEnabled(false);
    }
}

void ualab::disconnection()
{
    usbadc10_close_device(&ualab_device);
    m_ui->action_disconnect->setEnabled(false);
    m_ui->action_rescan->setEnabled(true);
    m_ui->action_start_stop->setEnabled(false);
    m_ui->action_connect->setEnabled(true);
    m_ui->start_stop->setEnabled(false);
    m_ui->rescan_botton->setEnabled(true);
    m_ui->disconnect_button->setEnabled(false);
    m_ui->connect_button->setEnabled(true);
}


void ualab::change_state01()
{
    gstates[0] = m_ui->action01->isChecked();
}

void ualab::change_state02()
{
    gstates[1] = m_ui->action02->isChecked();
}

void ualab::change_state03()
{
    gstates[2] = m_ui->action03->isChecked();
}

void ualab::change_state04()
{
    gstates[3] = m_ui->action04->isChecked();
}

void ualab::change_state05()
{
    gstates[4] = m_ui->action05->isChecked();
}

void ualab::change_state06()
{
    gstates[5] = m_ui->action06->isChecked();
}

void ualab::change_state07()
{
    gstates[6] = m_ui->action07->isChecked();
}

void ualab::change_state08()
{
    gstates[7] = m_ui->action08->isChecked();
}

void ualab::change_state09()
{
    gstates[8] = m_ui->action09->isChecked();
}

void ualab::change_state10()
{
    gstates[9] = m_ui->action10->isChecked();
}






