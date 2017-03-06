
#include "parameters.h"
#include "nitoo.h"

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/// Internal Functions
//------------------------------------------------------------------------------

unsigned char _calculate_crc(unsigned char byte, unsigned char crc)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        if ((byte & 0x01) ^ (crc & 0x01))
        {
            crc ^= 0x18;
            crc >>= 1;
            crc |= 0x80;
        }
        else
        {
            crc >>= 1;
        }
        byte >>= 1;
    }
    return crc;
}

//return the length of created frame
unsigned char _create_frame(
                            unsigned char *pFrame,  //address of the buffer for holding the created frame
                            unsigned char addr_mode,  //can be one of ADDR_MODE_BCAST, ADDR_MODE_ANONMCAST, ADDR_MODE_UCAST, ADDR_MODE_ANONUCAST
                            unsigned int id,  //node id of IR tool, 20bits
                            unsigned char channel,    //unit number of IR tool buttons, 4bits
                            unsigned char code,   //PL+Function code, see above definitions
                            unsigned char data1,  //maybe Extended Command code, see above definitions, put 0 if not
                            unsigned char data2,  //data2 if any, 0 if not
                            unsigned char data3,  //data3 if any, 0 if not
                            unsigned char data4,  //data4 if any, 0 if not
                            unsigned char data5,  //data5 if any, 0 if not
                            unsigned char data6,  //data6 if any, 0 if not
                            unsigned char data7,  //data7 if any, 0 if not
                            unsigned int second_address     //optional second address, use 0 if don't know
                            )
{
    static unsigned char FrameNumber = 0;
    unsigned char result = 0;
    unsigned char CRC = 0;

    pFrame[result] = PREAMBLE;
    result++;   //1
    pFrame[result] = FAMILY_TYPE_IR | addr_mode | ROUTING_INFO_NOREPEAT | (FrameNumber & ROUTING_INFO_FRAMENUMBER_MASK);
    CRC = _calculate_crc(pFrame[result], CRC);
    result++;   //2
    id = ((id & ADDR_ID_MASK) << ADDR_ID_POSITION);
    id |= (channel & 0x0F);
    pFrame[result] = ((id & 0xFF0000) >> 16);
    CRC = _calculate_crc(pFrame[result], CRC);
    result++;   //3
    pFrame[result] = ((id & 0xFF00) >> 8);
    CRC = _calculate_crc(pFrame[result], CRC);
    result++;   //4
    pFrame[result] = (id & 0xFF);
    CRC = _calculate_crc(pFrame[result], CRC);
    result++;   //5
    pFrame[result] = code;
    CRC = _calculate_crc(pFrame[result], CRC);
    result++;   //6
    switch (code & PL_MASK)
    {
    case PL_1_BYTE:
    default:
        break;
    case PL_2_BYTE:
        pFrame[result] = data1;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //7
        break;
    case PL_4_BYTE:
        pFrame[result] = data1;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //7
        pFrame[result] = data2;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //8
        pFrame[result] = data3;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //9
        break;
    case PL_8_BYTE:
        pFrame[result] = data1;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //7
        pFrame[result] = data2;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //8
        pFrame[result] = data3;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //9
        pFrame[result] = data4;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //10
        pFrame[result] = data5;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //11
        pFrame[result] = data6;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //12
        pFrame[result] = data7;
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //13
        break;
    }
    if (second_address)
    {
        pFrame[result] = ((second_address & 0xFF0000) >> 16);
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //??
        pFrame[result] = ((second_address & 0xFF00) >> 8);
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //??
        pFrame[result] = (second_address & 0xFF);
        CRC = _calculate_crc(pFrame[result], CRC);
        result++;   //??
    }
    //put CRC at the last byte
    pFrame[result] = CRC;
    result++;   //now result indicates the length of Tx buffer

    FrameNumber++;    //increase FrameNumber by 1 for next frame

    return result;
}

unsigned char _verify_frame_crc(unsigned char *pFrame, unsigned char Size)
{
    unsigned char crc = 0;

    for (unsigned char i=1; i<Size-1; i++)
    {
        crc = _calculate_crc(*(pFrame+i), crc);
    }

    return (crc == *(pFrame+Size-1));
}

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------

unsigned char NITOO_CreateSetParameterFrame(unsigned char *pFrame, SENSOR_PARAMS *pParams, unsigned char ParamIndex)
{
    return _create_frame(
                    pFrame,
                    ADDR_MODE_BCAST,
                    NITOO_LM058A_ID,
                    NITOO_LM058A_NODE,      // 2.3.0
                    CODE_SET_MOTION_SENSOR_PARAMETER,
                    CMD_SET_MOTION_SENSOR_PARAMETER,
                    (PARAMS_GetParameterNumber(pParams, ParamIndex) & 0xFF),
                    ((PARAMS_GetParameterNumber(pParams, ParamIndex) & 0xFF00) >> 8),
                    (PARAMS_GetParameterValue(pParams, ParamIndex) & 0xFF),
                    ((PARAMS_GetParameterValue(pParams, ParamIndex) & 0xFF00) >> 8),
                    0,
                    0,
                    (unsigned int)0);
}

unsigned char NITOO_CreateReadParameterFrame(unsigned char *pFrame, SENSOR_PARAMS *pParams, unsigned char ParamIndex)
{
    return _create_frame(
                    pFrame,
                    ADDR_MODE_BCAST,
                    NITOO_LM058A_ID,
                    NITOO_LM058A_NODE,      // 2.3.0
                    CODE_READ_MOTION_SENSOR_PARAMETER,
                    CMD_READ_MOTION_SENSOR_PARAMETER,
                    (PARAMS_GetParameterNumber(pParams, ParamIndex) & 0xFF),
                    ((PARAMS_GetParameterNumber(pParams, ParamIndex) & 0xFF00) >> 8),
                    0,
                    0,
                    0,
                    0,
                    (unsigned int)0);
}

// return 0 if crc error or unexpected frame received, 2 if "Parameter error" frame received, 1 if everything is good
unsigned char NITOO_ValidateIndicationParameterFrame(unsigned char *pFrame, SENSOR_PARAMS *pParams, unsigned char ParamIndex)
{
    unsigned short temp_param_number = PARAMS_GetParameterNumber(pParams, ParamIndex);

    if (_verify_frame_crc(pFrame, NITOO_INDICATION_PARAMETER_FRAME_LENGTH) == 0)
    {
        return 0;
    }
    else
    {
        if ((pFrame[5] == CODE_INDICATION_MOTION_SENSOR_PARAMETER) && \
            (pFrame[6] == CMD_INDICATION_MOTION_SENSOR_PARAMETER))
        {
            if ((pFrame[7] == (unsigned char)(temp_param_number & 0xFF)) && \
                (pFrame[8] == (unsigned char)((temp_param_number & 0xFF00) >> 8)))
            {
                return 1;
            }
            else if ((pFrame[7] == (unsigned char)(PARAM_NUMBER_ERROR & 0xFF)) && \
                     (pFrame[8] == (unsigned char)((PARAM_NUMBER_ERROR & 0xFF00) >> 8)))
            {
                return 2;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
}

unsigned short NITOO_GetParameterValueFromIndicationFrame(unsigned char *pFrame)
{
    return (((unsigned short)pFrame[10] << 8) | (unsigned short)pFrame[9]);
}

// 2.3.0
unsigned char NITOO_CreateExtendedCommandFrame(unsigned char *pFrame, \
                                               unsigned char Channel, \
                                               unsigned char Code, \
                                               unsigned char Command)
{
    return _create_frame(
                    pFrame,
                    ADDR_MODE_BCAST,
                    NITOO_LM058A_ID,
                    Channel,            // 2.3.0
                    Code,
                    Command,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    (unsigned int)0);
}

unsigned char NITOO_CreateDirectlySetParameterFrame(unsigned char *pFrame, const PARAMETER *pParameter)
{
    return _create_frame(
                    pFrame,
                    ADDR_MODE_BCAST,
                    NITOO_LM058A_ID,
                    NITOO_LM058A_NODE,      // 2.3.0
                    CODE_SET_MOTION_SENSOR_PARAMETER,
                    CMD_SET_MOTION_SENSOR_PARAMETER,
                    (pParameter->Number & 0xFF),
                    ((pParameter->Number & 0xFF00) >> 8),
                    (pParameter->Value & 0xFF),
                    ((pParameter->Value & 0xFF00) >> 8),
                    0,
                    0,
                    (unsigned int)0);
}

unsigned char NITOO_CreateDirectlySetAreaOnOFF(unsigned char *pFrame,unsigned char Channel,unsigned char Code)
{
    return _create_frame(
                    pFrame,
                    ADDR_MODE_BCAST,
                    NITOO_LM058A_ID,
                    Channel,      // 2.3.0
                    Code,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    (unsigned int)0);
}


unsigned char NITOO_CreateReadNBofControlGearFrame(unsigned char *pFrame,unsigned char Channel,unsigned char Code,unsigned char Command,unsigned char MaskL,unsigned char MaskH)
{
    return _create_frame(
                    pFrame,
                    ADDR_MODE_BCAST,
                    NITOO_LM058A_ID,
                    Channel,            // 2.3.0
                    Code,
                    Command,
                    MaskL,
                    MaskH,
                    0,
                    0,
                    0,
                    0,
                   (unsigned int)0);
}



unsigned char NITOO_CreateSetDeviceGroupNum(unsigned char *pFrame,unsigned char Channel,unsigned char Code,unsigned char Command,unsigned char MaskL,unsigned char MaskLH,unsigned char MaskHL,unsigned char MaskH,unsigned char data6)
{
    return _create_frame(
                    pFrame,
                    ADDR_MODE_BCAST,
                    NITOO_LM058A_ID,
                    Channel,            // 2.3.0
                    Code,
                    Command,
                    MaskL,
                    MaskLH,
                    MaskHL,
                    MaskH,
                    data6,
                    0,
                   (unsigned int)0);
}