#include "VRPN_testServer.h"

static void VRPN_CALLBACK handle_AciCommandserver_callback(void *userdata, const VRPN_TRANSPORTACICOMMANDSERVERCB &info)
{
  VRPN_AciCommand_transport_server *me =
	static_cast<VRPN_AciCommand_transport_server *>(userdata);

  // Copy the command info into a response message.
  AciCommand response = info.command;

  printf("Code: %d\n", response.code);
  // Increment the return code by one and leave the rest of the
  // command data alone.
  response.rc++;

  // Send a response.
  if (!me->send_response(response)) {
	fprintf(stderr,"handle_test_AciCommandserver_callback(): response failed\n");
  }
}

int VRPN_AciCommand_transport_server_test(void)
{
  //-----------------------------------------------------------------
  //-----------------------------------------------------------------
  // Test AciCommand transport classes.

  //-----------------------------------------------------------------
  // Test construction of server class.
  vrpn_Connection *c2 = vrpn_create_server_connection(3884);
  VRPN_AciCommand_transport_server cmd_svr("AciCommandTest", c2);
  cmd_svr.set_command_handler(&cmd_svr, handle_AciCommandserver_callback);

  //-----------------------------------------------------------------
  // Wait until we're connected or fail if we time out.
  struct timeval start, now;
  vrpn_gettimeofday(&start, NULL);

  do {
    cmd_svr.mainloop();
    c2->mainloop();
  } while (true);

  std::cout << "broke out of loop\n";

  // We added one when we created the connection explictly.
  // This will delete the connection because its reference count
  // will go to zero.
  c2->removeReference();

  return 0;
}

int main(){
    return VRPN_AciCommand_transport_server_test();
}
