//==============================================================================
/*!\file ACIHeaders.h
 * \brief AWARE Camera API 
 *
 *<hr>
 * \author S. D. Feller 2013
 *
 *<hr>
 * \section VersionControl
 * $Source: /srv/cvs/ADM/Test_Tools/include/tool_utils.h,v $
 * $Date: 2013-06-04 00:05:55 $
 * $Revision: 1.1 $
*/
//==============================================================================
#ifndef ACITYPES_H
#define ACITYPES_H

#include <list>
#include <stdint.h>
//#include <aplDefs.h>

#define DEFAULT_DEBUG_MODE 10
#define DEFAULT_GUID       9999

//Packet definitions
#define MAX_PAYLOADSIZE 256
#define ACI_MAX_IP_LEN  16
#define ACI_MAX_NAME_LEN 256
#define MAX_ACQ_THREADS 256

//Packet Codes
#define PKT_STREAM         1             //Turns streaming on/off
#define PKT_LOCK           2             //Lock image buffer
#define PKT_UNLOCK         3             //Unlock image buffer
#define PKT_SETROI         4             //Set Image ROI
#define PKT_SENDFRAME      5             //Senda single frame from Camera
#define PKT_EXIT           6             //Tells client/server to close. No transmission

//Command states
#define PKT_NONE     0b00000000              //Ready to process
#define PKT_READY    0b00000001              //Ready to process
#define PKT_FAIL     0b00000010              //Previous command failed
#define PKT_WAIT     0b00000100              //Waiting to be used
#define PKT_BUSY     0b00001000              //Busy processing command (being processed)
#define PKT_DONE     0b00010000              //Done processing.
#define PKT_ABORT    0b00100000              //Abort current process (need to be careful to ensure thread safe)
#define PKT_TIMEOUT  0b01000000

//Image Modes
#define MODE_GRAY 1
#define MODE_RGB  2
#define MODE_RGBA 3
#define MODE_RAW  4

#define GRAY_CHAN  1
#define RGB_CHAN   3
#define RGBA_CHAN  4
#define RAW_CHAN   1


//ROI MOdes
#define BIN2_ROI_MODE 2
#define ALIGNED_ROI_MODE 4

/************************************************************
 * Ethernet packet structures
 *
 * Each Command type must have the save initial form as the PktCommand
 * structure to ensure proper memory mapping
 ************************************************************/
/************************************************************
 * General PktCommandList structure
 ************************************************************
//Baseline Struct
typedef struct
{
   pthread_mutex_t * mutex;
   pthread_cond_t * cond;
   int8_t slot;
   int8_t code;
   int8_t rc;
   char   * data;
} PktCommand;
*/
/************************************************************
 * PktCommandList structure 
 ************************************************************
typedef struct
{
   pthread_mutex_t mutex;
   pthread_cond_t  cond;
   std::list <PktCommand *> list;
} PktCommandList;
*/
//***********************************************************
/*!\struct  AciImgRoi 
 * \brief defines the ROI for an image
 *
 * This structure is used to define a subset of pixels on the sensor.
 * It can be used to indicate what part of the sensor an AciImage 
 * object is generated from or to set regions for autofocus, 
 * auto exposure, etc. 
 */
/************************************************************
typedef struct
{
   int16_t width;                           //!< width of the region in pixels
   int16_t height;                          //!< height of the region in pixels
   int16_t hoffset;                         //!< horizontal offset of the first pixel
   int16_t voffset;                         //!< vertical offset of the first pixel
   int16_t scale;                           //!< bin level of the region
} AciImgRoi;
*/
/////////////////////////////////////////////
// Command Structures
/////////////////////////////////////////////
//************************************************************
/*!\struct PktAcquire
 *
 * \brief Structure used to request an image from the camera
 *
 * This structure contains all of the information to acquire 
 * an image from the camera and is passed in.
 *
 *
 ************************************************************
typedef struct
{
   AciImgRoi roi;                           //!< Region of pixels request
   char    ip[ACI_MAX_IP_LEN];              //!< IP address to send image to
   int16_t port;                            //!< port address to send image to
} PktAcquire;
*/
//============================================================
/************************************************************
 * Image Acquisition Variable definitions
 ************************************************************/
//Binning modes
#define BIN2_ROI_MODE    2
#define ALIGNED_ROI_MODE 4


//============================================================
//Command Codesa


//ACI Status definition
#define ID_STOP     0b00000000              //Undefined state. Used at startup
#define ID_RUNNING  0b00000001              //Undefined state. Used at startup

#define ID_NONE     0b00000000              //Undefined state. Used at startup
#define ID_READY    0b00000001              //Ready to process
#define ID_FAIL     0b00000010              //Previous command failed
#define ID_WAIT     0b00000100              //Waiting to be used
#define ID_BUSY     0b00001000              //Busy processing command (being processed)
#define ID_DONE     0b00010000              //Done processing.
#define ID_ABORT    0b00100000              //Abort current process (need to be careful to ensure thread safe)
#define ID_TIMEOUT  0b01000000

//************************************************************
/*!\struct AciImage
 *
 * \brief Structure with minimal data to represent an image.
 *
 * This structure is intended to become a base type that will be
 * the basic method for representing images. This will allow us to 
 * ensure compatibility as we write functions for image processing.
 *
 * \internal - needs to template data for simplicity
 */
/************************************************************
typedef struct
{
   uint16_t width;                          //!< Image Width
   uint16_t height;                         //!< Image height
   uint8_t  bpp;                            //!< bits per pixel
   uint8_t  mode;                           //!< Pixel mode (MODE_RAW, MODE_RGB, MODE_GRAY
   void * data;                             //!< pixel data buffer
} AciImage;
*/

//************************************************************
/*!\struct AciRoi - 
 * \brief Defines a ROI in an image
 *
 * Inidicates a rectangular subset of pixels in an image. All 
 * values are referenced in CCD pixel space 
 */
//************************************************************
typedef struct
{
   uint16_t width;                          //!< width of image to acquire
   uint16_t height;                         //!<height of image image acquire
   uint16_t offx;                           //!< offset of the current camera tile in x-axis
   uint16_t offy;                           //!< offset of the current camera tile in y-axis
   uint16_t scale;                          //!< image scaling divisor (1, 2, 4, 8, 16, etc.) 
   uint16_t bpp;                            //!< requested bits per pixel
} AciRoi;

//***********************************************************
/*!\struct AciRoiList
 *
 * \brief List of ROI objects that can be shared between threads.
 */
/************************************************************
typedef  struct
{
   pthread_mutex_t mutex;                   //!< mutex for threadsafe operation
   pthread_cond_t cond;                     //!< conditional for signaling item added to list
   std::list <AciRoi *> list;               //!< List of AciRoi items
} AciRoiList;
*/
//*********************************************
/*!\brief Structure that defines the senor state for a frame
 *
 * All of this information is in the initial header when 
 * a frame is sent.
 */
 /*********************************************
 typedef struct
 {
   int16_t slot;                            //!< microcamera generating data
   int16_t exposure;                        //!<Exposure when image was taken
   int16_t focusPos;                        //!<Focus position
   bool    AEC;                             //!<AEC Parameter
   int16_t gainR;                           //!<Red Gain Register
   int16_t gainGR;                          //!<GR Gain Register
   int16_t gainGB;                          //!<GB Gain Register
   int16_t gainB;                           //!<B Gain Register

//Speculative for now
   int16_t iso;                             //!< iso setting
   int16_t wb;                              //!< WB setting
   int16_t captureMode;                     //!< Capture mode (FULL, BinX2)
} AciSensorState;
*/
//*********************************************
/*!\brief Structure that defines a specific frame (ROI and guid)
 *
 * This structure defines an image coming from a microcamera. The image
 * data and sensor state are part of the initial header packet. 
 *
 *\internal Need to clarify how the ROI is returned from the camera
 */
/*********************************************
typedef struct
{
   int16_t  slot;                           //!< microcamera that generated the frame
   uint16_t frameId;                        //!< microcamera that generated the frame
   uint16_t guid;                           //!< unique ID assigned to a frame
   int16_t  age;                            //!< frame id( age from guid )
   uint32_t time;                           //!< time image was taken
   double   burstId;                        //!< burst id
   uint32_t systemId;                       //!< systemId
   AciRoi   roi;                            //!< roi of the frame
   AciSensorState state;                    //!< sensor status information
   AciImage image;                          //!< image data for the frame
} AciFrame;
*/

//************************************************************
/*!\struct AciFrameList
 *
 * \brief A list of frames received from the camera. This mechanism
 * is used to pass image data between the acquisition threads and the
 * AciComm process.
 */
/************************************************************
typedef struct
{
   pthread_mutex_t mutex;                   //!< mutex for threadsafe operation
   pthread_cond_t  cond;                    //!< conditional for signaling
   std::list<AciFrame *> list;              //!< list of AciFrame object pointers
} AciFrameList;
*/

//************************************************************
/*!\struct AciCommand
 *
 * \brief Structure used to pass messages between threads.
 *
 * Mutexes and conditionals are used to signal when the command
 * has been processed by the remote client. A positive return code 
 * indicates success, a negative return code is failure. 0 indicates
 * the command is waiting to be processed.
 */
/************************************************************
typedef struct
{
   int8_t code;                             //!< Command code
   int16_t slot;                            //!< microcamera top operate on
   int8_t rc;                               //!< return code
   pthread_mutex_t * mutex;                 //!< mutex for threadsafe operation
   pthread_cond_t * cond;                   //!< conditional for signalling completion
   int16_t dataSize;                        //!< size of the data in bytes
   void * data;                             //!< Additional command data if needed
} AciCommand;
*/

/************************************************************
 * VRPN Supported Structures
 ************************************************************/
//Define Size of Command components
#define MIN_CMD_DIRECTIVE 0
   #define CMD_EXIT            0
   #define CMD_GET_PKT_DELAY   1            
   #define CMD_GET_PKT_GAP     2           
   #define CMD_GET_ISO         3
   #define CMD_GET_AEC         4
   #define CMD_GET_EXPOSURE    6
   #define CMD_GET_AFC         7
   #define CMD_GET_FOCUS       8
   #define CMD_GET_BINMODE     9
   #define CMD_SET_AEC_ON     10
   #define CMD_SET_AEC_OFF    11
   #define CMD_SET_AFC_ON     12
   #define CMD_SET_AFC_OFF    13
   #define CMD_FORCEUNLOCK    16
   #define CMD_SET_MASTER     17
   #define CMD_CLR_MASTER     18
   #define CMD_SET_SYNCSAFE   20
   #define CMD_GET_STATE      21
   #define CMD_START_STREAM   22
   #define CMD_STOP_STREAM    23
   #define CMD_INIT           24
   #define CMD_NONE           25
   #define CMD_BUFFER_ON      26           //Turns on buffering in Receivers
   #define CMD_BUFFER_OFF     27           //Turns off buffering in Receivers
   #define CMD_STORE_ON       28
   #define CMD_STORE_OFF      29

   #define CMD_SYNC           30
   #define CMD_SYNC_OFF       32            //Clears Sync state and puts cameras in freerun
   #define CMD_SAVE_STATE     33
   #define CMD_RESTORE_STATE  34
   #define CMD_SYNC_FRAME     35            //SYNCS the frames at a given timestamp
#define MAX_CMD_DIRECTIVE 49

#define MIN_CMD_VALUE 50
   #define CMD_GET_REG        50             //Get Register
   #define CMD_SET_PKT_DELAY  51             //Set mask bits
   #define CMD_SET_PKT_GAP    52             //Set mask bits
   #define CMD_SET_ISO        53   
   #define CMD_UNLOCK         54
   #define CMD_SET_EXPOSURE   55
   #define CMD_SET_FOCUS      56
   #define CMD_SET_BINMODE    57
   #define CMD_ERROR          58
#define MAX_CMD_VALUE 99    

#define MIN_CMD_KEY   100
   #define CMD_SET_REG       100             //Set Register
   #define CMD_SET_MASK      101             //Set mask bits
   #define CMD_CLEAR_MASK    102             //Set mask bits
#define MAX_CMD_KEY   200

#define CMD_SEND_STATE    201                //Sends state values from DMCore


//Local only`
//Sequence of commands needed for a SYNC_RESET to control timing
#define CMD_GET_FSYNC      245
#define CMD_SET_FSYNC      246
#define CMD_SYNC_INIT      247               //Initialized sync 
#define CMD_SYNC_CLEAR     248               //Reset Sync
#define CMD_SYNC_ENABLE    249               //Reset Sync
#define CMD_LOCK           251
#define CMD_STREAM_ON      252
#define CMD_STREAM_OFF     253



//Error codes
#define ERROR_LISTSIZE     -10
#define ERROR_INVALID_CMD  -11
#define ERROR_INVALID_SLOT -12




//************************************************************
/**
 *!\brief structure for passing lock requests via AciCommand structure
 **/
//************************************************************
typedef struct
{
   uint16_t guid;                           //!< GUID to assign to lock
   uint16_t age;                            //!< Age of the lock (buffer 1 is more recent)
} LockData;

//************************************************************
//************************************************************
typedef struct
{
   uint16_t key;                            //!< register address to reference
   uint16_t value;                          //!< Value of key
} KeyValueData;

//Data needed to tell processes how to stream
typedef struct
{
   char destAddr[ACI_MAX_IP_LEN];
   uint16_t port;
   uint16_t guid;
} StreamData;

//Data sent to all processes to sync frame ID
typedef struct
{
   int16_t frameId;
   uint64_t timestamp;
   double  fps;
} SyncData;

typedef struct
{
   //Sync data
   uint16_t exposure;
   uint16_t aecState;
} MCamState;

typedef struct
{
   AciRoi roi;
   int16_t maxSlot;
} InitStruct;

//************************************************************
/**
 * \brief union used to pass messages between threads. 
 *
 * The subsequent structures are passed as data and reference by 
 * the appropriate code.
 *
 * - value: uint16_t value 
 * - dval : double   value
 * - lock : LockData structure
 * - keyValue: KeyValueData structure
 **/
//************************************************************
union CommandData
{
   uint16_t     value;                      //!< Set/get value
   double       dval;                       //!< double value to set
   LockData     lock;                       //!< lock datastructure
   KeyValueData keyValue;                   //!<Set/get value for key
   InitStruct   init;                        //!< roi data that defines an image
   StreamData   stream;
   SyncData     sync;
   MCamState    mcam;                       //!< microcamera state
};

//************************************************************
/*!\struct AciCommand
 *
 * \brief Generic structure used to pass messages between threads. 
 *
 * The subsequent structures are passed as data and reference by the appropriate code
 *
 ************************************************************/
typedef struct
{
   int16_t slot;                            //!< microcamera top operate on
   uint8_t  code;                           //!< Command code
   int8_t  rc;                              //!< return code
   CommandData data;                        //!< Additional command data if needed
} AciCommand;




/**
 *!\brief This structure is used to pass a key/value pair  as part of the 
 * AciCommand structure. 
 **/
typedef struct
{
   uint16_t key;                            //!< Key 
   uint16_t count;                          //!< Number of elements in value
   uint16_t value;                          //!< Array of value data
} AciKeyCmd;

//*********************************************
/*!\brief Structure that defines a specific frame (ROI and guid)
 *
 * This structure defines an image coming from a microcamera. The image
 * data and sensor state are part of the initial header packet. 
 *
 *\internal Need to clarify how the ROI is returned from the camera
 * 
 *********************************************
typedef struct
{
   uint16_t guid;                           //!< unique ID assigned to a frame
   int16_t  age;                            //!< frame id( age from guid, -1 = stream )
   uint32_t ipaddr;                         //!< destination ip address
   uint16_t port;                           //!< destiation port
   AplRoi   roi;                            //!< roi of the frame
} AciFrameRequest;
*/
#endif
