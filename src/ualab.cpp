#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include <qwt_picker_machine.h>
#include <qwt_scale_engine.h>

#include <iostream>
#include <math.h>
#include <limits.h>

#include "ualab.h"
#include "ui_ualab.h"

UALab::UALab (QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(Qt::gray,   1, Qt::SolidLine));
    grid->setMinPen(QPen(Qt::gray, 0.5, Qt::DotLine));
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
        cruve[i]->setPen(graphcolor.at(i)/*, 2, Qt::SolidLine*/);
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

    m_ui->button_disconnect->setEnabled(false);
    m_ui->action_disconnect->setEnabled(false);
    m_ui->button_start_stop_recording->setEnabled(false);
    m_ui->action_start_stop_recording->setEnabled(false);
    m_ui->button_start_stop->setEnabled(false);
    m_ui->action_start_stop->setEnabled(false);

    tmr = new QTimer(this);

    connect(tmr, SIGNAL(timeout()), this, SLOT(updategraph()));
    connect(m_ui->action_this_application, SIGNAL(triggered(bool)), this, SLOT(this_application()));
    connect(m_ui->action_rescan, SIGNAL(triggered(bool)), this, SLOT(rescan()));
    connect(m_ui->action_start_stop, SIGNAL(triggered(bool)), this, SLOT(start_stop_handler()));
    connect(m_ui->action_connect, SIGNAL(triggered(bool)), this, SLOT(connection()));
    connect(m_ui->action_disconnect, SIGNAL(triggered(bool)), this, SLOT(disconnection()));
    connect(m_ui->action_save, SIGNAL(triggered(bool)), this, SLOT(save_handler()));
    connect(m_ui->action_start_stop_recording, SIGNAL(triggered(bool)), this, SLOT(start_stop_recording_handler()));

    connect(m_ui->comboBox_period_val, SIGNAL(activated(int)), this, SLOT(change_period()));

    connect(&thread_data, &QThread::started, &dataupdaerobj, &DataUpdater::updatedata);
    connect(&dataupdaerobj, &DataUpdater::finished, &thread_data, &QThread::terminate);

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

    dataupdaerobj.moveToThread(&thread_data);

    rescan();
}

void UALab::reset()
{
    start = clock_t::now();
}

double UALab::elapsed() const
{
    return std::chrono::duration_cast<second_t>(clock_t::now() - start).count();
}


UALab::~UALab()
{
    usbadc10_close_device(&ualab_device);
    delete m_ui;
}


void UALab::this_application()
{
    QMessageBox msbox;
    msbox.setWindowTitle("About");
    msbox.setStandardButtons(QMessageBox::Ok);
    msbox.setDefaultButton(QMessageBox::Ok);
    msbox.setText("This is a simple cross-platform application for the usbadc10 device.\nCopyright © 2021 Nikita Presnov\npresnovnikita@yandex.ru");
    msbox.exec();
}

void UALab::rescan()
{
    m_ui->comboBox_ports->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        m_ui->comboBox_ports->addItem(info.portName());
    }
}

void UALab::updategraph()
{
    if(progressframes < NUMBERFRAMES)
    {
        if(progressframes==0)reset();
        dataX[progressframes] = elapsed();
        usbadc10_get_conversion(ualab_device, &ualab_data);
        if(start_stop_recording_status)
        {
            record_data.push_back(std::vector <double>());
            record_data.back().push_back(dataX[progressframes]);
        }
        for(int j = 0; j < 10; j++)
        {
            dataY[j][progressframes] = (double)ualab_data.data[j] / 10000;
            if(start_stop_recording_status)
            {
                record_data.back().push_back(dataY[j][progressframes]);
            }
        }
        progressframes++;
    }
    else
    {
        for(int i = 0; i < NUMBERFRAMES-1; i++)
        {
            dataX[i] = dataX[i+1];
        }
        dataX[NUMBERFRAMES-1] = elapsed();
        usbadc10_get_conversion(ualab_device, &ualab_data);

        if(start_stop_recording_status)
        {
            record_data.push_back(std::vector <double>());
            record_data.back().push_back(dataX[NUMBERFRAMES-1]);
        }
        for(int j = 0; j < 10; j++)
        {
            for(int i = 0; i < NUMBERFRAMES-1; i++)
            {
                dataY[j][i] = dataY[j][i+1];
            }
            dataY[j][NUMBERFRAMES-1] = (double)ualab_data.data[j] / 10000;
            if(start_stop_recording_status)
            {
                record_data.back().push_back(dataY[j][progressframes]);
            }
        }
    }

    for(int i = 0; i < 10; i++)
    {
        if(gstates[i])
            cruve[i]->setSamples(dataX, dataY[i], progressframes);
        else
            cruve[i]->setSamples(0, 0, 0);
    }
    m_ui->graphWidget->replot();
}

void UALab::start_stop_handler()
{
    int period = m_ui->comboBox_period_val->currentData().toInt();
    start_stop_recording_status = false;
    m_ui->status_recording->setPixmap(QPixmap(QString::fromUtf8("../record_green.svg")));

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
        dataupdaerobj.setRunning(true);
        thread_data.start();
        m_ui->action_start_stop_recording->setEnabled(true);
        m_ui->button_start_stop_recording->setEnabled(true);
        tmr->setInterval(period);
        tmr->start();
    }
    else
    {
        m_ui->action_save->setEnabled(true);
        m_ui->button_save->setEnabled(true);
        m_ui->action_start_stop_recording->setEnabled(false);
        m_ui->button_start_stop_recording->setEnabled(false);
        start_stop_status = false;
        dataupdaerobj.setRunning(false);
        tmr->stop();
    }
}
void UALab::connection()
{
    usbadc10_close_device(&ualab_device);

    #ifdef Q_OS_WIN32
    QString ualab_devicename = "com:\\\\.\\" + m_ui->comboBox_ports->currentText();
    #else
        #ifdef Q_OS_LINUX
    QString ualab_devicename = "com:///dev/" + m_ui->comboBox_ports->currentText();
        #endif
    #endif

    ualab_device = usbadc10_open_device(ualab_devicename.toStdString().c_str());


    if(ualab_device == device_undefined)
    {
        QMessageBox msbox;
        if(m_ui->comboBox_ports->count() == 0)
        {

            msbox.setWindowTitle("UALab");
            msbox.setText("No avalible device.");
        }
        else
        {
            msbox.setWindowTitle("UALab");
            msbox.setText(m_ui->comboBox_ports->currentText() + " is not correct device.");
        }
        msbox.exec();
        rescan();
    }
    else
    {
        m_ui->action_disconnect->setEnabled(true);
        m_ui->button_disconnect->setEnabled(true);
        m_ui->action_rescan->setEnabled(false);
        m_ui->button_rescan->setEnabled(false);
        m_ui->action_start_stop->setEnabled(true);
        m_ui->button_start_stop->setEnabled(true);
        m_ui->action_connect->setEnabled(false);
        m_ui->button_connect->setEnabled(false);
    }
}

void UALab::disconnection()
{
    usbadc10_close_device(&ualab_device);

    m_ui->status_recording->setPixmap(QPixmap(QString::fromUtf8("../record_green.svg")));

    m_ui->action_disconnect->setEnabled(false);
    m_ui->button_disconnect->setEnabled(false);
    m_ui->action_rescan->setEnabled(true);
    m_ui->button_rescan->setEnabled(true);
    m_ui->action_start_stop->setEnabled(false);
    m_ui->button_start_stop->setEnabled(false);
    m_ui->action_connect->setEnabled(true);
    m_ui->button_connect->setEnabled(true);
    m_ui->action_save->setEnabled(true);
    m_ui->button_save->setEnabled(true);
    m_ui->action_start_stop_recording->setEnabled(false);
    m_ui->button_start_stop_recording->setEnabled(false);
    m_ui->action_save->setEnabled(true);
    m_ui->button_save->setEnabled(true);

    start_stop_status = false;
    dataupdaerobj.setRunning(false);
    start_stop_recording_status = false;
    tmr->stop();

    rescan();
}

void UALab::change_period()
{
    start_stop_status = !start_stop_status;
    dataupdaerobj.setRunning(false);
    start_stop_recording_status = false;
    m_ui->action_save->setEnabled(true);
    m_ui->button_save->setEnabled(true);
    start_stop_handler();
}

void UALab::save_handler()
{
    filename = QFileDialog::getSaveFileName(this,
                                            tr("Save File"),
                                            QDateTime::currentDateTime().toString("dd-MM-yyyy_HH-mm-ss"),// + ".csv",
                                            tr("CSV Files (*.csv)"));
    outputfile.setFileName(filename);
    if(!outputfile.open(QIODevice::WriteOnly))
    {
        QMessageBox msbox;
        msbox.setWindowTitle("UALab");
        msbox.setText("Error while touch file. No directory.");
        msbox.exec();
    }
    else
    {
        outputfile.write("Time, s\t");
        QTextStream outdata (&outputfile);

        for(int i = 0; i < 10; i++)
        {
            outdata << "ADC" << i+1 << '\t';
        }
        outdata << '\n';
        for(;!record_data.empty();record_data.erase(record_data.begin()))
        {
            outdata << record_data.front().at(0) << '\t';
            for(int i = 0; i < 10; i++)
            {
                outdata << record_data.front().at(i+1) << '\t';
            }
            outdata << '\n';
        }
        outputfile.close();
    }
}

void UALab::start_stop_recording_handler()
{
    start_stop_recording_status = !start_stop_recording_status;
    if(start_stop_recording_status)
    {
        m_ui->status_recording->setPixmap(QPixmap(QString::fromUtf8("../record_red.svg")));
        m_ui->action_save->setEnabled(false);
        m_ui->button_save->setEnabled(false);
    }
    else
    {
        m_ui->status_recording->setPixmap(QPixmap(QString::fromUtf8("../record_green.svg")));
        m_ui->action_save->setEnabled(true);
        m_ui->button_save->setEnabled(true);
    }

}

void UALab::change_state01()
{
    gstates[0] = m_ui->action01->isChecked();
}

void UALab::change_state02()
{
    gstates[1] = m_ui->action02->isChecked();
}

void UALab::change_state03()
{
    gstates[2] = m_ui->action03->isChecked();
}

void UALab::change_state04()
{
    gstates[3] = m_ui->action04->isChecked();
}

void UALab::change_state05()
{
    gstates[4] = m_ui->action05->isChecked();
}

void UALab::change_state06()
{
    gstates[5] = m_ui->action06->isChecked();
}

void UALab::change_state07()
{
    gstates[6] = m_ui->action07->isChecked();
}

void UALab::change_state08()
{
    gstates[7] = m_ui->action08->isChecked();
}

void UALab::change_state09()
{
    gstates[8] = m_ui->action09->isChecked();
}

void UALab::change_state10()
{
    gstates[9] = m_ui->action10->isChecked();
}






