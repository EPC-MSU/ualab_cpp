#ifndef DATAUPDATER_H
#define DATAUPDATER_H

#include <QObject>

/**
 * @todo write docs
 */
class DataUpdater : public QObject
{
    Q_OBJECT

public:
    explicit DataUpdater(QObject *parent = 0);

signals:
    void finished();

public slots:
    void updatedata();
};

#endif // DATAUPDATER_H
