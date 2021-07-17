#include "ualab.h"
#include "ui_ualab.h"
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include <qwt_picker_machine.h>

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
        m_ui->graphWidget->canvas() );
    d_picker->setRubberBandPen( QColor( Qt::red ) );
    d_picker->setTrackerPen( QColor( Qt::black ) );
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );
    m_ui->graphWidget->enableAxis(QwtPlot::yRight, true);
    m_ui->graphWidget->setAxisTitle(QwtPlot::yLeft, "Voltage, V");
    m_ui->graphWidget->setAxisTitle(QwtPlot::xBottom, "Time, s");
    m_ui->graphWidget->setAxisScale(QwtPlot::yLeft, 0, 3.3, 0.5);
    m_ui->graphWidget->setAxisScale(QwtPlot::yRight, 0, 3.3, 0.5);
    m_ui->graphWidget->replot();

    m_ui->disconnect_button->setEnabled(false);
    m_ui->start_stop_recording->setEnabled(false);
    m_ui->start_stop->setEnabled(false);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->action_start_stop_recording->setEnabled(false);
    m_ui->action_start_stop_getting_data->setEnabled(false);

//     QSerialPortInfo *serial = new QSerialPortInfo();
//     QList<QSerialPortInfo> serials = serial->availablePorts();
//     m_ui->comboBox_ports->addItems(Qstring:: QSerialPortInfo::availablePorts());

    connect(m_ui->actionThis_Application, SIGNAL(triggered(bool)), this, SLOT(this_application()));
    connect(m_ui->actionRescan, SIGNAL(triggered(bool)), this, SLOT(rescan()));
    rescan();
//     connect(m_ui->, SIGNAL(triggered(bool)), this, SLOT(this_application()));
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

