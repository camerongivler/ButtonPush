#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connectTo = qApp->arguments().at(1).toStdString();
    ui->setupUi(this);

    try {
        mi = new MongoInterface();
    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    } catch (const std::string& ex) {
        std::cout << ex << std::endl;
    } catch (...) {
        std::cout << "Something went wrong";
    }
    currBurstID = "";
    startTime = 0;
    startDate = "";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mi;
}

void MainWindow::pushed_button()
{
    currBurstID = "thisisaburstid";
    JsonBox::Value *val1 = new JsonBox::Value();
    QDateTime DT = QDateTime::currentDateTimeUtc();
    startTime = DT.time().msecsSinceStartOfDay();
    startDate = DT.date().toString(Qt::ISODate).toStdString();
    (*val1)["StartTime"] = JsonBox::Value(startTime);
    (*val1)["StartDate"] = JsonBox::Value(startDate);
    (*val1)["SystemID"] = JsonBox::Value(2);
    (*val1)["BurstID"] = JsonBox::Value(currBurstID);
    (*val1)["Event"] = JsonBox::Value("thisisaneventname");
    std::cout << "entry: " << *val1 << std::endl;
    mi->insertJSON("Test", val1);
    JsonBox::Value *query = new JsonBox::Value();
    (*query)["BurstID"] = JsonBox::Value(currBurstID);
    (*query)["StartTime"] = JsonBox::Value(startTime);
    (*query)["StartDate"] = JsonBox::Value(startDate);
    std::cout << "query: " << *query << std::endl;
    std::cout << "query result: " << mi->query("Test", query) << std::endl;
    delete val1;
    delete query;
}

void MainWindow::released_button()
{
    JsonBox::Value *update = new JsonBox::Value();
    QDateTime DT = QDateTime::currentDateTimeUtc();
    std::string stopDate = DT.date().toString(Qt::ISODate).toStdString();
    (*update)["$set"]["StopTime"] = JsonBox::Value(DT.time().msecsSinceStartOfDay());
    (*update)["$set"]["StopDate"] = JsonBox::Value(stopDate);
    (*update)["$set"]["NumFrames"] = JsonBox::Value(100);
    JsonBox::Value *query = new JsonBox::Value();
    (*query)["BurstID"] = JsonBox::Value(currBurstID);
    (*query)["StartTime"] = JsonBox::Value(startTime);
    (*query)["StartDate"] = JsonBox::Value(startDate);
    std::cout << "query entry: " << mi->query("Test", query) << std::endl;
    std::cout << "update entry: " << mi->update("Test", query, update, true) << std::endl;
    std::cout << "query entry: " << mi->query("Test", query) << std::endl;
    delete update;
    delete query;
}

void MainWindow::on_start_toggled(bool checked)
{
//    if(checked)
//        pushed_button();
//    else
//        released_button();
        VRPN_AciCommand_transport(connectTo);
}

//----------------------------------------------------------------------------
// Test code below here.

//  Took out server.  This is only client code

static void VRPN_CALLBACK handle_AciCommandresponse_callback(void *userdata, VRPN_TRANSPORTACICOMMANDRESPONSECB info)
{
  // userdata points to an info pointer.  This lets the testing code get the
  // values that were passed in.
  AciCommand *userinfo = static_cast<AciCommand*>(userdata);
  *userinfo = info.response;
}

int VRPN_AciCommand_transport(std::string connectTo)
{
  //-----------------------------------------------------------------
  // Test construction of client class.
  AciCommand rcv_cmd;
  VRPN_AciCommand_transport_remote cmd_rmt(connectTo.c_str());
  cmd_rmt.register_response_handler(&rcv_cmd, handle_AciCommandresponse_callback);

  //-----------------------------------------------------------------
  // Wait until we're connected or fail if we time out.
  struct timeval start, now;
  vrpn_gettimeofday(&start, NULL);
  do {
    cmd_rmt.mainloop();
    vrpn_gettimeofday(&now, NULL);
  } while (now.tv_sec - start.tv_sec < 2);

  AciCommand cmd;
  char testbuf[512];

  //-----------------------------------------------------------------
  // Test sending a command and receiving a response
  // Make sure the response values match those requested with the rc
  // incremented by one.
  // Timeout if we don't hear back for a few seconds.
  cmd.slot = 1;
  cmd.code = CMD_START_STREAM;
  cmd.rc = 2;
  rcv_cmd.slot = 0;
  rcv_cmd.code = 0;
  rcv_cmd.rc = 0;
  if (!cmd_rmt.send_command(cmd)) {
    fprintf(stderr,"Could not send command\n");
    return 250;
  }
  vrpn_gettimeofday(&start, NULL);
  do {
    cmd_rmt.mainloop();
    vrpn_gettimeofday(&now, NULL);
  } while ( (rcv_cmd.rc != cmd.rc + 1) && (now.tv_sec - start.tv_sec < 2) );
  if (rcv_cmd.rc != cmd.rc + 1) {
    fprintf(stderr,"Timeout waiting for response from command\n");
    return 251;
  }
  if (rcv_cmd.slot != cmd.slot) {
    fprintf(stderr,"Bad slot from response (%d rather than %d)\n",
      rcv_cmd.slot, cmd.slot);
    return 252;
  }
  if (rcv_cmd.code != cmd.code) {
    fprintf(stderr,"Bad code from response (%d rather than %d)\n",
      rcv_cmd.code, cmd.code);
    return 253;
  }

  fprintf(stderr, "Finished");

  return 0;
}

