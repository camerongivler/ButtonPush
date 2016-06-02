///
/// @section LICENSE
/// Copyright Aqueti 2014
///
/// @author Russell M. Taylor II, working through Navitas Research for Aqueti
///

// Only include this header file once in any compile.
#pragma once

#include <ACITypes.h>
#include <vrpn_BaseClass.h>
#include <string>

/** Base class for server and remote object that know how to send an
    AciCommand across a network, packing and unpacking all required
    data so that it will work between architectures. */
class VRPN_AciCommand_transport_base : public vrpn_BaseClass {
public:
  /** Constructor.  Told the name of the device being handled and
      the connection to use to communicate over. */
  VRPN_AciCommand_transport_base(const char *name, vrpn_Connection *c = NULL);

protected:
  virtual int register_types(void);      //!< Register the types below to VRPN
  vrpn_int32	d_command_send_m_id;     //!< ID of the message type describing a command
  vrpn_int32	d_command_response_m_id; //!< ID of the message type describing a response

  /** Helper function that encodes an iCommand into a buffer.  Returns
      the number of bytes encoded, or -1 on failure. */
  int encode_AciCommand_to(const AciCommand &cmd  //!< Command to be encoded
			   , char *buf            //!< Buffer to encode it into
			   , size_t buflen        //!< Length of the buffer
			   );
  /** Helper function that decodes an AciCommand from a buffer.
      Callee is must delete [] any arrays and delete
      the substructures and the main structure if it is not NULL.  Returns
      the number of bytes decoded, or -1 on failure. */
  int decode_AciCommand_from(AciCommand &cmd  //!< Command to fill in
			  , const char *buf   //!< Buffer to read from
			  , size_t buflen     //!< Length of the buffer
			  );

  friend int VRPN_AciCommand_transport_test(void);
};

/** Description of callback handler and structure it uses for letting the server
    application know when a new command has arrived.  The application should
    handle the command and then edit its return value (and other parameters)
    in place. */

typedef struct {
  struct timeval	    msg_time;   //!< Timestamp of the request
  AciCommand                command;    //!< Command that is to be executed
} VRPN_TRANSPORTACICOMMANDSERVERCB;

/** Callback handler passed to the server that is using this object.  The
    info pointer indicates which command is received.  Unlike most VRPN callbacks,
    this one is a call-by-reference handler.
    The caller is supposed to fill in the response pointer in the info based
    on the command.  If no response is available, the response pointer
    should be left NULL.  */
typedef void (VRPN_CALLBACK *VRPN_TRANSPORTACICOMMANDSERVERHANDLER) (void *userdata,
				 const VRPN_TRANSPORTACICOMMANDSERVERCB &info);

/** Server class that will receive commands send responses.  */

class VRPN_AciCommand_transport_server : public VRPN_AciCommand_transport_base {
public:

  /// Constructor told the name of its VRPN object and connection to send over.
  VRPN_AciCommand_transport_server(const char *name, vrpn_Connection *c);

  /** A server application using this class must register a handler that takes
      a pointer to an AciCommand structure that describes the command to
      execute.
      Unlike most VRPN classes, this one does not have a list of handlers but
      just a single handler that can be set. */
  virtual int set_command_handler(void *userdata, const VRPN_TRANSPORTACICOMMANDSERVERHANDLER handler)
  {
    d_command_userdata = userdata;
    d_command_handler = handler;
    return 0;
  };

  /// Send a response to the client.
  bool send_response( const AciCommand &response );

  /// Handles calling mainloop on required base classes.
  virtual void mainloop(void) {
    server_mainloop();
  }

protected:
  /// Handler for request message from the client.
  static int VRPN_CALLBACK handle_command_message(void *userdata, vrpn_HANDLERPARAM p);
  VRPN_TRANSPORTACICOMMANDSERVERHANDLER  d_command_handler;
  void *d_command_userdata;
};

/** Description of callback handler and structure it uses for letting the client
    application know when a new command response has arrived. */
typedef struct {
  struct timeval  msg_time;   //!< Timestamp of the request
  AciCommand  response;       //!< Command response
} VRPN_TRANSPORTACICOMMANDRESPONSECB;

typedef void (VRPN_CALLBACK *VRPN_TRANSPORTACICOMMANDRESPONSEHANDLER) (void *userdata,
					  const VRPN_TRANSPORTACICOMMANDRESPONSECB info);

/** Remote class that will send commands and receive responses.
    The client should set a callback handler for when a response
    has arrived.  They should then request images and call
    mainloop repeatedly on the class.
    */

class VRPN_AciCommand_transport_remote
  : public VRPN_AciCommand_transport_base {
public:

  /// Constructor told the name of its VRPN object and connection to receive on
  VRPN_AciCommand_transport_remote(const char *name, vrpn_Connection *c = NULL);

  /// Send a command to the server.
  bool send_command( const AciCommand &command );

  /// Call this each time through the program's main loop
  virtual void	mainloop(void) {
    if (d_connection) { d_connection->mainloop(); }
    client_mainloop();
  };

  /** Register a handler to deal with each new response as it arrives.  */
  virtual int register_response_handler(void *userdata,
	VRPN_TRANSPORTACICOMMANDRESPONSEHANDLER handler)
  {
    return d_response_list.register_handler(userdata, handler);
  };
  virtual int unregister_response_handler(void *userdata,
	VRPN_TRANSPORTACICOMMANDRESPONSEHANDLER handler)
  {
    return d_response_list.unregister_handler(userdata, handler);
  };

protected:
  /// List of callback handlers to call when a frame has arrived.
  vrpn_Callback_List<VRPN_TRANSPORTACICOMMANDRESPONSECB> d_response_list;

  /// Handler for response message from the server.
  static int VRPN_CALLBACK handle_response_message(void *userdata, vrpn_HANDLERPARAM p);
};

//----------------------------------------------------------------------------------
/** Run tests on the above classes to make sure they are working
    as expected.  Returns 0 on success and integer related to what failed
    on failure.  Each time a new method or capability is added, tests
    should be added. */
int VRPN_AciCommand_transport_test(void);

