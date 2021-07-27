#include "ualab.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ualab w;
    w.setWindowTitle(w.windowTitle() + " " + VERSION);
    w.show();

    return app.exec();
//     w.disconnection();
}

