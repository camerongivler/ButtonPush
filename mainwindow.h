#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "MongoInterface.h"
#include "VRPN_AciCommand_transport.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void pushed_button();
    void released_button();
    void on_start_toggled(bool checked);


private:
    Ui::MainWindow *ui;
    MongoInterface *mi;
    std::string currBurstID;
    int startTime;
    std::string startDate;
    std::string connectTo;
    //friend int VRPN_AciCommand_transport(std::str connectTo);
};

int VRPN_AciCommand_transport(std::string connectTo);

#endif // MAINWINDOW_H
