#ifndef NITOO_H
#define NITOO_H

#include "parameters.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define NITOO_MAX_FRAME_LENGTH      20

#define NITOO_LM058A_ID             ((unsigned int)0xFFFFF)
#define NITOO_LM058A_NODE           0x0     // 2.3.0
#define NITOO_LM058A_UNIT_ZB_LG     0x1     // 2.3.0
#define NITOO_LM058A_UNIT_ZB_AG     0x2     // 2.3.0

#define NITOO_INDICATION_PARAMETER_FRAME_LENGTH     14
#define NITOO_NODE_INDEX_FRAME_LENGTH               11
#define NITOO_UNIT_DESCIPTION_STATUS                11 


#define NITOO_FRAME_PREAMBLE        0xAA

//PREAMBLE
#define PREAMBLE 0xAA

//HEADER
#define FAMILY_TYPE_MASK 0xE0
#define     FAMILY_TYPE_TOPDOG 0x20
#define     FAMILY_TYPE_RF 0x40
#define     FAMILY_TYPE_PLC 0x60
#define     FAMILY_TYPE_IR 0x80
#define     FAMILY_TYPE_RESERVED_1 0xA0
#define     FAMILY_TYPE_RESERVED_2 0xC0
#define     FAMILY_TYPE_RESERVED_3 0xE0
#define ADDR_MODE_MASK 0x18
#define     ADDR_MODE_BCAST 0x00
#define     ADDR_MODE_ANONMCAST 0x08
#define     ADDR_MODE_UCAST 0x10
#define     ADDR_MODE_ANONUCAST 0x18
#define ROUTING_INFO_MASK 0x07
#define     ROUTING_INFO_REPEAT 0x04
#define     ROUTING_INFO_NOREPEAT 0x00
#define     ROUTING_INFO_FRAMENUMBER_MASK 0x03

//ADDRESS
#define ADDR_MASK 0xFFFFFF
#define     ADDR_ID_MASK 0xFFFFF
#define     ADDR_ID_POSITION 4
#define     ADDR_CHANNEL_MASK 0x0F
#define     ADDR_CHANNEL_0 0x00     //node
#define     ADDR_CHANNEL_1 0x01
#define     ADDR_CHANNEL_2 0x02
#define     ADDR_CHANNEL_3 0x03
#define     ADDR_CHANNEL_4 0x04
#define     ADDR_CHANNEL_5 0x05
#define     ADDR_CHANNEL_6 0x06
#define     ADDR_CHANNEL_7 0x07
#define     ADDR_CHANNEL_8 0x08
#define     ADDR_CHANNEL_9 0x09
#define     ADDR_CHANNEL_A 0x0A
#define     ADDR_CHANNEL_B 0x0B
#define     ADDR_CHANNEL_C 0x0C
#define     ADDR_CHANNEL_D 0x0D
#define     ADDR_CHANNEL_E 0x0E
#define     ADDR_CHANNEL_F 0x0F

//           (CODE_...)         (CMD_...)
//PAYLOAD == PL+FUNCTION_CODE - EXT_CMD_CODE - DATA2 - DATA3 - ...
#define PL_MASK 0xC0
#define PL_1_BYTE 0x00
#define PL_2_BYTE 0x40
#define PL_4_BYTE 0x80
#define PL_8_BYTE 0xC0

//Sensor Parameters Manipulation Code
#define CODE_SET_MOTION_SENSOR_PARAMETER 0xF1
#define CMD_SET_MOTION_SENSOR_PARAMETER 0x03
//#define LENGTH_SET_MOTION_SENSOR_PARAMETER 8
#define CODE_READ_MOTION_SENSOR_PARAMETER 0xB1
#define CMD_READ_MOTION_SENSOR_PARAMETER 0x04
//#define LENGTH_READ_MOTION_SENSOR_PARAMETER 4
#define CODE_INDICATION_MOTION_SENSOR_PARAMETER 0xF1
#define CMD_INDICATION_MOTION_SENSOR_PARAMETER 0x05
//#define LENGTH_INDICATION_MOTION_SENSOR_PARAMETER 8

//Remote PnL Group Code
#define CODE_START_GROUP_PNL 0x70
#define CMD_START_GROUP_PNL 0x2C
#define CODE_NEXT_ACTUATOR 0x70
#define CMD_NEXT_ACTUATOR 0x2D
#define CODE_INCLUDE 0x70
#define CMD_INCLUDE 0x2E
#define CODE_EXCLUDE 0x70
#define CMD_EXCLUDE 0x2F
//#define CODE_SET_ON 0x70
//#define CMD_SET_ON 0x30
//#define CODE_SET_OFF 0x70
//#define CMD_SET_OFF 0x31
//#define CODE_SET_DIMMING_UP 0x70
//#define CMD_SET_DIMMING_UP 0x32
//#define CODE_SET_DIMMING_DOWN 0x70
//#define CMD_SET_DIMMING_DOWN 0x33
#define CODE_END_GROUP_PNL 0x70
#define CMD_END_GROUP_PNL 0x34
#define CODE_ERASE_GROUP_PNL 0x70
#define CMD_ERASE_GROUP_PNL 0x35
#define CODE_ADD_PERIPHERAL 0x70
#define CMD_ADD_PERIPHERAL 0x36

//Other Code
#define CODE_MOTION_SENSOR_RETURN_TO_FACTORY_CONFIG 0x70
#define CMD_MOTION_SENSOR_RETURN_TO_FACTORY_CONFIG 0x37
#define CODE_RESET_ROOM_CONTROL 0x70
#define CMD_RESET_ROOM_CONTROL 0x38

//Short Command
#define CODE_ON 0x01    //short command
//#define CMD_ON 0x00     //dummy
#define CODE_OFF 0x02   //short command
//#define CMD_OFF 0x00    //dummy
#define CODE_DIM_STEP 0x83  //short command, this command has 2 more parameters as the time value, here uses 0
//#define CMD_DIM_STEP 1  //dimming step
#define CODE_ACTION 0x07    //short command
//#define CMD_ACTION 0x00 //dummy
#define CODE_STOPACTION 0x09    //short command
//#define CMD_STOPACTION 0x00 //dummy
#define CODE_VALID_ACTION 0x11  //short command
//#define CMD_VALID_ACTION 0x00    //dummy
#define CODE_INVALID_ACTION 0x12    //short command
//#define CMD_INVALID_ACTION 0x00    //dummy

//Management Code
#define CODE_ANNOUNCE_ID 0x70
#define CMD_ANNOUNCE_ID 0x00
#define CODE_OPEN_LEARNING 0xB0
#define CMD_OPEN_LEARNING 0x06
#define CODE_CLOSE_LEARNING 0x70
#define CMD_CLOSE_LEARNING 0x07
#define CODE_ADDRESS_ERASE 0x70
#define CMD_ADDRESS_ERASE 0x0E

#define CMD_DELETE_ALL_CONTROL_GEAR    0X49
#define CMD_CONTROL_GEAR_CREATE_GROUP  0X3A
#define CMD_CONTROL_SENSOR_CREATE_GROUP 0X43

#define CMD_CANCEL_LEARNING            0X08
#define CMD_QUERY_DEVICE_SHORT_ADDRESS 0X4B
#define CMD_QUERY_NEXT_CONTROL_GEAR    0x3D
#define CMD_QUERY_PREVIOUS_CONTROL_GEAR 0x3E
#define CMD_NODE_INDEX                  0x3F
#define CMD_UNIT_DESCRIPTION_REQUEST    0X0C



//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

unsigned char NITOO_CreateSetParameterFrame(unsigned char *pFrame, SENSOR_PARAMS *pParams, unsigned char ParamIndex);
unsigned char NITOO_CreateReadParameterFrame(unsigned char *pFrame, SENSOR_PARAMS *pParams, unsigned char ParamIndex);
unsigned char NITOO_ValidateIndicationParameterFrame(unsigned char *pFrame, SENSOR_PARAMS *pParams, unsigned char ParamIndex);
unsigned short NITOO_GetParameterValueFromIndicationFrame(unsigned char *pFrame); //the frame must be validated prior to calling this function
unsigned char NITOO_CreateExtendedCommandFrame(unsigned char *pFrame, unsigned char Channel, unsigned char Code, unsigned char Command);            // 2.3.0
unsigned char NITOO_CreateDirectlySetParameterFrame(unsigned char *pFrame, const PARAMETER *pParameter);
unsigned char NITOO_CreateReadNBofControlGearFrame(unsigned char *pFrame,unsigned char Channel,unsigned char Code,unsigned char Command,unsigned char MaskL,unsigned char MaskH);
#endif
