#ifndef UALAB_H
#define UALAB_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class ualab;
}

class ualab : public QMainWindow
{
    Q_OBJECT

public:
    explicit ualab(QWidget *parent = nullptr);
    ~ualab() override;

private:
    QScopedPointer<Ui::ualab> m_ui;
};

#endif // UALAB_H
