#include "dataupdater.h"
#include <thread>
#include <QDebug>
DataUpdater::DataUpdater ( QObject* parent ) :
    QObject(parent)
{

}

void DataUpdater::updatedata()
{
    counter = 0;
    while(m_running)
    {
        qDebug() << counter++ << '\n';
        std::this_thread::sleep_for (std::chrono::milliseconds(200));
    }
    emit finished();
}

bool DataUpdater::running() const
{
    return m_running;
}

void DataUpdater::setRunning ( bool running )
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(running);
}


