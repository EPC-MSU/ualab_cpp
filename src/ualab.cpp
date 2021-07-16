#include "ualab.h"
#include "ui_ualab.h"

ualab::ualab(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ualab)
{
    m_ui->setupUi(this);
}

ualab::~ualab() = default;
