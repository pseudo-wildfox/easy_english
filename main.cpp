
#include <QApplication>
#include "mainwindow.h"
#include "xmlapi.h"
#include "proxysingleton.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(draggabletext);

    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    ProxySingleton<XmlApi>::instance().printAll();

    return app.exec();
}
