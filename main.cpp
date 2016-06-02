#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(a.arguments().size() == 1){
        fprintf(stderr, "Not enough arguments. Please specify where to connect.\n");
        return 1;
    }
    MainWindow w;
    w.show();

    return a.exec();
}
