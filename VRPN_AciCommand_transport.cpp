///
/// @section LICENSE
/// Copyright Aqueti 2014
///
/// @author Russell M. Taylor II, working through Navitas Research for Aqueti
///

#include <VRPN_AciCommand_transport.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

VRPN_AciCommand_transport_base::VRPN_AciCommand_transport_base(const char *name, vrpn_Connection *c) :
  vrpn_BaseClass(name, c)
{
  vrpn_BaseClass::init();
}

int VRPN_AciCommand_transport_base::register_types(void)
{
  d_command_send_m_id = d_connection->register_message_type("vrpn_Transport AciCommand");
  d_command_response_m_id = d_connection->register_message_type("vrpn_Transport AciCommand_response");
  if ((d_command_send_m_id == -1) ||
      (d_command_response_m_id == -1) ) {
    return -1;
  } else {
    return 0;
  }
}

int VRPN_AciCommand_transport_base::encode_AciCommand_to(
	const AciCommand &cmd
	, char *buf
	, size_t buflen
)
{
  int remaining = static_cast<int>(buflen);

  // *** Need to pack/unpack them in an order that will make them
  //     aligned correctly.
  // *** Make sure this matches the unbuffering order and procedure.
  if (-1 == vrpn_buffer(&buf, &remaining, cmd.slot)) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
    return -1;
  }
  if (-1 == vrpn_buffer(&buf, &remaining, cmd.code)) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
    return -1;
  }
  if (-1 == vrpn_buffer(&buf, &remaining, cmd.rc)) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
    return -1;
  }
  // Encode the data field as needed for the specific type of command.
  if ( (cmd.code >= MIN_CMD_DIRECTIVE) && (cmd.code <= MAX_CMD_DIRECTIVE) ) {
	// Nothing to do here -- there is no data for this type of command.
  } else if ( (cmd.code >= MIN_CMD_VALUE) && (cmd.code <= MAX_CMD_VALUE) ) {

	// These commands have a uint16_t type, referred to as value
	if (-1 == vrpn_buffer(&buf, &remaining, cmd.data.value)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
	  return -1;
	}

  } else if ( (cmd.code >= MIN_CMD_KEY) && (cmd.code <= MAX_CMD_KEY) ) {

	// These commands have a KeyValueData type, referred to as keyValue
	if (-1 == vrpn_buffer(&buf, &remaining, cmd.data.keyValue.key)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
	  return -1;
	}
	if (-1 == vrpn_buffer(&buf, &remaining, cmd.data.keyValue.value)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
	  return -1;
	}

  } else if (cmd.code == CMD_LOCK) {

	// These commands have a LockData type, referred to as lock
	if (-1 == vrpn_buffer(&buf, &remaining, cmd.data.lock.guid)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
	  return -1;
	}
	if (-1 == vrpn_buffer(&buf, &remaining, cmd.data.lock.age)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Can't buffer\n");
	  return -1;
	}

  } else {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::encode_AciCommand_to(): Unrecognized command type (command %d)\n", cmd.code);
	  return -1;
  }

  // Tell how many characters we sent.
  return static_cast<int>(buflen) - remaining;
}

int VRPN_AciCommand_transport_base::decode_AciCommand_from(
	AciCommand &cmd
	, const char *buf
	, size_t buflen
)
{
  //--------------------------------------------------------------------
  // Get the parameters for the command from the buffer.
  // *** Need to pack/unpack them in an order that will make them
  //     aligned correctly.
  // *** This needs to match the encoding routine.
  const char *bufptr = buf;
  if (buflen < sizeof(cmd.slot)) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
    return -1;
  }
  if (vrpn_unbuffer(&bufptr, &(cmd.slot))) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer slot\n");
    return -1;
  }
  if (buflen < sizeof(cmd.code)) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
    return -1;
  }
  if (vrpn_unbuffer(&bufptr, &(cmd.code))) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer code\n");
    return -1;
  }
  if (buflen < sizeof(cmd.rc)) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
    return -1;
  }
  if (vrpn_unbuffer(&bufptr, &(cmd.rc))) {
    fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer rc\n");
    return -1;
  }
  if ( (cmd.code >= MIN_CMD_DIRECTIVE) && (cmd.code <= MAX_CMD_DIRECTIVE) ) {
        // Nothing to do here -- there is no data for this type of command.
  } else if ( (cmd.code >= MIN_CMD_VALUE) && (cmd.code <= MAX_CMD_VALUE) ) {

	if (buflen < sizeof(cmd.data.value)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
	  return -1;
	}
	if (vrpn_unbuffer(&bufptr, &(cmd.data.value))) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer value\n");
	  return -1;
	}

  } else if ( (cmd.code >= MIN_CMD_KEY) && (cmd.code <= MAX_CMD_KEY) ) {

	if (buflen < sizeof(cmd.data.keyValue.key)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
	  return -1;
	}
	if (vrpn_unbuffer(&bufptr, &(cmd.data.keyValue.key))) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer key\n");
	  return -1;
	}
	if (buflen < sizeof(cmd.data.keyValue.value)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
	  return -1;
	}
	if (vrpn_unbuffer(&bufptr, &(cmd.data.keyValue.value))) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer count\n");
	  return -1;
	}

  } else if (cmd.code == CMD_LOCK) {

	if (buflen < sizeof(cmd.data.lock.guid)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
	  return -1;
	}
	if (vrpn_unbuffer(&bufptr, &(cmd.data.lock.guid))) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer guid\n");
	  return -1;
	}
	if (buflen < sizeof(cmd.data.lock.age)) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Not enough data\n");
	  return -1;
	}
	if (vrpn_unbuffer(&bufptr, &(cmd.data.lock.age))) {
	  fprintf(stderr,"VRPN_AciCommand_transport_base::decode_AciCommand_from(): Can't unbuffer age\n");
	  return -1;
	}

  } else {
	fprintf(stderr,"VRPN_AciCommand_transport_base::handle_request_message(): Unrecognized command (%d)\n", cmd.code);
	return -1;
  }

  // Return the number of bytes read.
  return static_cast<int>(bufptr - buf);
}

VRPN_AciCommand_transport_server::VRPN_AciCommand_transport_server(
        const char *name, vrpn_Connection *c)
  : VRPN_AciCommand_transport_base(name, c)
{
  // Register a handler for the command message.
  register_autodeleted_handler(d_command_send_m_id, handle_command_message, this, d_sender_id);
}

int VRPN_AciCommand_transport_server::handle_command_message(void *userdata,
	vrpn_HANDLERPARAM p)
{
  VRPN_AciCommand_transport_server  *me =
	(VRPN_AciCommand_transport_server*)userdata;

  // Structure to pass to the user-level callback handler.
  VRPN_TRANSPORTACICOMMANDSERVERCB callback_info;

  // Time of the message.
  callback_info.msg_time.tv_sec = p.msg_time.tv_sec;
  callback_info.msg_time.tv_usec = p.msg_time.tv_usec;

  //--------------------------------------------------------------------
  // Get the parameters for the command from the buffer.
  // Put the information into the structure we're going to
  // pass back to the caller.
  int ret;
  ret = me->decode_AciCommand_from(callback_info.command, p.buffer, p.payload_len);
  if (ret != p.payload_len) {
    fprintf(stderr,"VRPN_AciCommand_transport_server::handle_command_message(): Incorrect message length (%d vs. %d)\n", ret, p.payload_len);
    return -1;
  }

  //--------------------------------------------------------------------
  // Call the handler which should copy anything it needs to run the
  // command.
  if (me->d_command_handler) {
    me->d_command_handler(me->d_command_userdata, callback_info);
  }

  return 0;
}

bool VRPN_AciCommand_transport_server::send_response(const AciCommand &response)
{
  //--------------------------------------------------------------------
  // Send the structure as a response message.
  char outbuf[512];
  int len;
  if ( (len = encode_AciCommand_to(response, outbuf, sizeof(outbuf))) < 0) {
      fprintf(stderr,"VRPN_AciCommand_transport_server::send_response(): Can't encode response\n");
      return false;
  }
  struct timeval now;
  vrpn_gettimeofday(&now, NULL);
  if (d_connection->pack_message(len, now,
		d_command_response_m_id, d_sender_id, outbuf,
		vrpn_CONNECTION_RELIABLE)) {
      fprintf(stderr,"VRPN_AciCommand_transport_server::send_response(): Can't pack response\n");
      return false;
  }
  return true;
}

VRPN_AciCommand_transport_remote::VRPN_AciCommand_transport_remote(
        const char *name, vrpn_Connection *c)
  : VRPN_AciCommand_transport_base(name, c)
{
  // Register a handler for the image-response message.
  register_autodeleted_handler(d_command_response_m_id, handle_response_message, this, d_sender_id);
}

bool  VRPN_AciCommand_transport_remote::send_command( const AciCommand &command )
{
  // msgbuf must be float64-aligned!
  vrpn_float64 fbuf [vrpn_CONNECTION_TCP_BUFLEN/sizeof(vrpn_float64)];
  char	  *msgbuf = (char *) fbuf;
  int	  buflen = sizeof(fbuf);
  struct  timeval timestamp;

  // Pack the command into the buffer
  int ret;
  if (-1 == (ret = encode_AciCommand_to( command, msgbuf, buflen )) ) {
    fprintf(stderr,"VRPN_AciCommand_transport_remote::request_image(): Can't pack message info, tossing\n");
    return false;
  }
  buflen -= ret;

  // Pack the buffer into the connection's outgoing reliable queue, if we have
  // a valid connection.
  vrpn_int32  len = sizeof(fbuf) - buflen;
  vrpn_gettimeofday(&timestamp, NULL);
  if (d_connection && d_connection->pack_message(len, timestamp,
                               d_command_send_m_id, d_sender_id, (char *)(void*)fbuf,
                               vrpn_CONNECTION_RELIABLE)) {
    fprintf(stderr,"VRPN_AciCommand_transport_remote::request_image(): cannot write message: tossing\n");
    return false;
  }

  return true;
}

int VRPN_AciCommand_transport_remote::handle_response_message(void *userdata,
	vrpn_HANDLERPARAM p)
{
  const char *bufptr = p.buffer;
  VRPN_AciCommand_transport_remote  *me =
	(VRPN_AciCommand_transport_remote*)userdata;

  // Structure to pass to the user-level callback handler.
  VRPN_TRANSPORTACICOMMANDRESPONSECB callback_info;

  //--------------------------------------------------------------------
  // Get the parameters for the requested image from the buffer
  callback_info.msg_time.tv_sec = p.msg_time.tv_sec;
  callback_info.msg_time.tv_usec = p.msg_time.tv_usec;
  if ( -1 == me->decode_AciCommand_from(callback_info.response, bufptr, p.payload_len) ) {
    fprintf(stderr,"VRPN_AciCommand_transport_remote::handle_response_message(): Can't unbuffer command\n");
    return -1;
  }

  // Go down the list of callbacks that have been registered.
  // Fill in the parameter and call each.
  me->d_response_list.call_handlers(callback_info);

  return 0;
}

//----------------------------------------------------------------------------
// Test code below here.
/*
static void VRPN_CALLBACK handle_test_AciCommandserver_callback(void *userdata, const VRPN_TRANSPORTACICOMMANDSERVERCB &info)
{
  VRPN_AciCommand_transport_server *me =
	static_cast<VRPN_AciCommand_transport_server *>(userdata);

  // Copy the command info into a response message.
  AciCommand response = info.command;

  // Increment the return code by one and leave the rest of the
  // command data alone.
  response.rc++;

  // Send a response.
  if (!me->send_response(response)) {
	fprintf(stderr,"handle_test_AciCommandserver_callback(): response failed\n");
  }
}

static void VRPN_CALLBACK handle_test_AciCommandresponse_callback(void *userdata, VRPN_TRANSPORTACICOMMANDRESPONSECB info)
{
  // userdata points to an info pointer.  This lets the testing code get the
  // values that were passed in.
  AciCommand *userinfo = static_cast<AciCommand*>(userdata);
  *userinfo = info.response;
}

int VRPN_AciCommand_transport_test(void)
{
  //-----------------------------------------------------------------
  //-----------------------------------------------------------------
  // Test AciCommand transport classes.

  //-----------------------------------------------------------------
  // Test construction of server class.
  vrpn_Connection *c2 = vrpn_create_server_connection(3884);
  VRPN_AciCommand_transport_server cmd_svr("AciCommandTest", c2);
  cmd_svr.set_command_handler(&cmd_svr, handle_test_AciCommandserver_callback);

  //-----------------------------------------------------------------
  // Test construction of client class.
  AciCommand rcv_cmd;
  VRPN_AciCommand_transport_remote cmd_rmt("AciCommandTest@localhost:3884");
  cmd_rmt.register_response_handler(&rcv_cmd, handle_test_AciCommandresponse_callback);

  //-----------------------------------------------------------------
  // Wait until we're connected or fail if we time out.
  struct timeval start, now;
  vrpn_gettimeofday(&start, NULL);
  do {
    cmd_rmt.mainloop();
    cmd_svr.mainloop();
    c2->mainloop();
    vrpn_gettimeofday(&now, NULL);
  } while ( !c2->connected() && (now.tv_sec - start.tv_sec < 2) );
  if (!c2->connected()) {
    fprintf(stderr,"Timeout waiting for connection to command server\n");
    return 201;
  }

  //-----------------------------------------------------------------
  // Test encoding and decoding of all known command types
  AciCommand cmd;
  char testbuf[512];

  cmd.slot = 1;
  cmd.code = MIN_CMD_DIRECTIVE;
  cmd.rc = 2;
  rcv_cmd.slot = cmd.slot + 1;
  rcv_cmd.code = cmd.code + 1;
  rcv_cmd.rc = cmd.rc + 1;
  if ( (cmd.slot == rcv_cmd.slot) || (cmd.code == rcv_cmd.code) ||
	(cmd.rc == rcv_cmd.rc) ) {
	fprintf(stderr,"Bad initial test of directive command\n");
	return 201;
  }
  if (-1 == cmd_svr.encode_AciCommand_to(cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not encode directive command\n");
	return 202;
  }
  if (-1 == cmd_svr.decode_AciCommand_from(rcv_cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not decode directive command\n");
	return 203;
  }
  if ( (cmd.slot != rcv_cmd.slot) || (cmd.code != rcv_cmd.code) ||
	(cmd.rc != rcv_cmd.rc) ) {
	fprintf(stderr,"Bad transport of directive command\n");
	return 204;
  }

  cmd.slot = 1;
  cmd.code = MIN_CMD_VALUE;
  cmd.rc = 2;
  cmd.data.value = 3;
  rcv_cmd.slot = cmd.slot + 1;
  rcv_cmd.code = cmd.code + 1;
  rcv_cmd.rc = cmd.rc + 1;
  rcv_cmd.data.value = cmd.data.value + 1;
  if ( (cmd.slot == rcv_cmd.slot) || (cmd.code == rcv_cmd.code) ||
	(cmd.rc == rcv_cmd.rc) || (cmd.data.value == rcv_cmd.data.value) ) {
	fprintf(stderr,"Bad initial test of value command\n");
	return 205;
  }

  if (-1 == cmd_svr.encode_AciCommand_to(cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not encode value command\n");
	return 206;
  }
  if (-1 == cmd_svr.decode_AciCommand_from(rcv_cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not decode value command\n");
	return 207;
  }
  if ( (cmd.slot != rcv_cmd.slot) || (cmd.code != rcv_cmd.code) ||
	(cmd.rc != rcv_cmd.rc) || (cmd.data.value != rcv_cmd.data.value) ) {
	fprintf(stderr,"Bad transport of value command\n");
	return 208;
  }

  cmd.slot = 1;
  cmd.code = MIN_CMD_KEY;
  cmd.rc = 2;
  cmd.data.keyValue.key = 3;
  cmd.data.keyValue.value = 4;
  rcv_cmd.slot = cmd.slot + 1;
  rcv_cmd.code = cmd.code + 1;
  rcv_cmd.rc = cmd.rc + 1;
  rcv_cmd.data.keyValue.key = cmd.data.keyValue.key + 1;
  rcv_cmd.data.keyValue.value = cmd.data.keyValue.value + 1;
  if ( (cmd.slot == rcv_cmd.slot) || (cmd.code == rcv_cmd.code) ||
	(cmd.rc == rcv_cmd.rc) || 
	(cmd.data.keyValue.key == rcv_cmd.data.keyValue.key) ||
	(cmd.data.keyValue.value == rcv_cmd.data.keyValue.value) ) {
	fprintf(stderr,"Bad initial test of key-value command\n");
	return 209;
  }

  if (-1 == cmd_svr.encode_AciCommand_to(cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not encode key-value command\n");
	return 210;
  }
  if (-1 == cmd_svr.decode_AciCommand_from(rcv_cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not decode key-value command\n");
	return 211;
  }
  if ( (cmd.slot != rcv_cmd.slot) || (cmd.code != rcv_cmd.code) ||
	(cmd.rc != rcv_cmd.rc) ||
	(cmd.data.keyValue.key != rcv_cmd.data.keyValue.key) ||
	(cmd.data.keyValue.value != rcv_cmd.data.keyValue.value) ) {
	fprintf(stderr,"Bad transport of key-value command\n");
	return 212;
  }

  cmd.slot = 1;
  cmd.code = CMD_LOCK;
  cmd.rc = 2;
  cmd.data.lock.guid = 3;
  cmd.data.lock.age = 4;
  rcv_cmd.slot = cmd.slot + 1;
  rcv_cmd.code = cmd.code + 1;
  rcv_cmd.rc = cmd.rc + 1;
  rcv_cmd.data.lock.guid = cmd.data.lock.guid + 1;
  rcv_cmd.data.lock.age = cmd.data.lock.age + 1;
  if ( (cmd.slot == rcv_cmd.slot) || (cmd.code == rcv_cmd.code) ||
	(cmd.rc == rcv_cmd.rc) || 
	(cmd.data.lock.guid == rcv_cmd.data.lock.guid) ||
	(cmd.data.lock.age == rcv_cmd.data.lock.age) ) {
	fprintf(stderr,"Bad initial test of lock-value command\n");
	return 213;
  }

  if (-1 == cmd_svr.encode_AciCommand_to(cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not encode lock-value command\n");
	return 214;
  }
  if (-1 == cmd_svr.decode_AciCommand_from(rcv_cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could not decode lock-value command\n");
	return 215;
  }
  if ( (cmd.slot != rcv_cmd.slot) || (cmd.code != rcv_cmd.code) ||
	(cmd.rc != rcv_cmd.rc) ||
	(cmd.data.lock.guid != rcv_cmd.data.lock.guid) ||
	(cmd.data.lock.age != rcv_cmd.data.lock.age) ) {
	fprintf(stderr,"Bad transport of lock-value command\n");
	return 216;
  }

  //-----------------------------------------------------------------
  // Test sending a command and receiving a response
  // Make sure the response values match those requested with the rc
  // incremented by one.
  // Timeout if we don't hear back for a few seconds.
  cmd.slot = 1;
  cmd.code = MIN_CMD_DIRECTIVE;
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
    cmd_svr.mainloop();
    c2->mainloop();
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

  fprintf(stderr,"===============================================\n");
  fprintf(stderr,"Begin section where error messages are expected\n");

  //-----------------------------------------------------------------
  // Test encoding of unknown command type
  cmd.code = CMD_LOCK + 1;
  if (-1 != cmd_svr.encode_AciCommand_to(cmd, testbuf, sizeof(testbuf))) {
	fprintf(stderr,"Could (improperly) encode unknown command type\n");
	return 290;
  }

  fprintf(stderr,"End section where error messages are expected\n");
  fprintf(stderr,"===============================================\n");

  // We added one when we created the connection explictly.
  // This will delete the connection because its reference count
  // will go to zero.
  c2->removeReference();

  return 0;
}
*/
