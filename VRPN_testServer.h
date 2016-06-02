#pragma once

#include <ACITypes.h>
#include <vrpn_BaseClass.h>
#include <string>
#include "VRPN_AciCommand_transport.h"
#include <iostream>

//static void VRPN_CALLBACK handle_AciCommandserver_callback(void *userdata, const VRPN_TRANSPORTACICOMMANDSERVERCB &info);
int VRPN_AciCommand_transport_server_test(void);
