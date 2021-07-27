#ifndef DATAUPDATER_H
#define DATAUPDATER_H

#include <QObject>

/**
 * @todo write docs
 */
class DataUpdater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

public:
    explicit DataUpdater(QObject *parent = nullptr);
    bool running() const;
    bool m_running;
    int counter;

signals:
    void finished();
    void runningChanged(bool running);


public slots:
    void updatedata();
    void setRunning(bool running);
};

#endif // DATAUPDATER_H
