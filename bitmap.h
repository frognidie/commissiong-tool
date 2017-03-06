#ifndef BITMAP_H
#define BITMAP_H


//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

#define BITMAP_BATTERY_WIDTH        32
#define BITMAP_ARROW_NAV_WIDTH      13
#define BITMAP_ARROW_NAV_HEIGHT     16

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

typedef struct {
    unsigned char Width;
    unsigned char Height;
    unsigned char BytesPerLine;
    const unsigned char *paData;
} BitmapInfo;

//------------------------------------------------------------------------------
/// Exported Variables
//------------------------------------------------------------------------------
extern const BitmapInfo BitmapInfo_BackHome;
extern const BitmapInfo BitmapInfo_Battery[4];
extern const BitmapInfo BitmapInfo_ArrowNav[2];
extern const BitmapInfo BitmapInfo_ArrowReturn;
extern const unsigned char Bitmap_Exclamation[440];
#ifndef APP_BAES
extern const BitmapInfo BitmapInfo_Validation[2];
extern const BitmapInfo BitmapInfo_ReadSensor;
extern const BitmapInfo BitmapInfo_WriteSensor;
extern const BitmapInfo BitmapInfo_ReadFile;
extern const BitmapInfo BitmapInfo_SaveFile;
extern const BitmapInfo BitmapInfo_DeleteFile;
extern const BitmapInfo BitmapInfo_QuestionMark;
extern const unsigned char Bitmap_Rectangular[540];
extern const BitmapInfo BitmapInfo_Pulse;   // 2.3.0
extern const BitmapInfo BitmapInfo_Pulse_S;     // 2.3.0
extern const unsigned char Bitmap_Eye[384];     // 2.3.0
extern const BitmapInfo BitmapInfo_ReadType;    // 2.4.0
#else
extern const BitmapInfo BitmapInfo_DeleteData;
extern const BitmapInfo BitmapInfo_ResetData;
extern const BitmapInfo BitmapInfo_SendData;
extern const BitmapInfo BitmapInfo_tool;
extern const BitmapInfo BitmapInfo_SendData_big[3];
extern const BitmapInfo BitmapInfo_DeleteData_big[3];
extern const BitmapInfo BitmapInfo_Digit[15];


#endif  // #ifndef APP_BAES

//------------------------------------------------------------------------------
/// Exported Functions
//------------------------------------------------------------------------------


#endif
