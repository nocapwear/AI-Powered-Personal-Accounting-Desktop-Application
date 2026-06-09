#include "MainWindow.h"
#include "Database//DatabaseManager.h"
#include <QApplication>
#include <QSslSocket>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager::instance().init();

    MainWindow w;

    w.show();
    qDebug() << "SSL Support:"
             << QSslSocket::supportsSsl();

    qDebug() << "Build SSL:"
             << QSslSocket::sslLibraryBuildVersionString();

    qDebug() << "Run SSL:"
             << QSslSocket::sslLibraryVersionString();

    return a.exec();
}
