#include  "KNX_sensor_name_string.h"
//------------------------------------------------------------------------------
//         type = 0x0100  => KNX sensor daylight only
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_KNX_DAYLIGHT_ONLY= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {PARAM_NUMBER_ERROR, 0},       //Minute
    {PARAM_NUMBER_ERROR, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 0},        //US sensitivity
    {25, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 0},        //Detection scheme
    {PARAM_NUMBER_ERROR, 0},        //Initial
    {PARAM_NUMBER_ERROR, 0},        //Maintain
    {PARAM_NUMBER_ERROR, 0},        //Retrigger
    {PARAM_NUMBER_ERROR, 0},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 0},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x100},         //Type                  read only
    {512, 0},        //Type 2                read only       // 2.3.1
    {259, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {27, 0},        //Daylight level        read only, writable when used for calibration
    {260, 0},        //Sensor status         read only in hidden page
    {262, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTSIGN
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTHOURS
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTMINUTES
    { 290, 0 }  // PARAM_NUMBER_KNX_MASTER_SLAVE
};


//------------------------------------------------------------------------------
//         type = 0x0101
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_KNX_PIR_ONLY_OCCUPANCY_ONLY = {
    {10, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 0},        //US sensitivity
    {PARAM_NUMBER_ERROR, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 0},        //Detection scheme
    {30, 1},        //Initial
    {31, 1},        //Maintain
    {PARAM_NUMBER_ERROR, 0},        //Retrigger
    {PARAM_NUMBER_ERROR, 0},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 0},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Latitude              // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {256, 0x101},         //Type                  read only
    {512, 0},        //Type 2                read only       // 2.3.1
    {259, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Daylight level        read only, writable when used for calibration
    {260, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTSIGN
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTHOURS
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTMINUTES
    { 290, 0 }  // PARAM_NUMBER_KNX_MASTER_SLAVE
}; 
 

//------------------------------------------------------------------------------
//         type = 0x0102
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_KNX_PIR_ONLY_OCCUPANCY_DAYLIGHT = {
    {10, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 0},        //US sensitivity
    {25, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 0},        //Detection scheme
    {30, 1},        //Initial
    {31, 1},        //Maintain
    {PARAM_NUMBER_ERROR, 0},        //Retrigger
    {PARAM_NUMBER_ERROR, 0},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 0},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {256, 0x102},         //Type                  read only
    {512, 0},        //Type 2                read only       // 2.3.1
    {259, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {27, 0},        //Daylight level        read only, writable when used for calibration
    {260, 0},        //Sensor status         read only in hidden page
    {262, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTSIGN
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTHOURS
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTMINUTES
    { 290, 0 }  // PARAM_NUMBER_KNX_MASTER_SLAVE
};



//------------------------------------------------------------------------------
//         type = 0x0103
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_SLAVE_KNX_PIR_ONLY_OCCUPANCY_ONLY = {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {PARAM_NUMBER_ERROR, 0},       //Minute
    {PARAM_NUMBER_ERROR, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 0},        //US sensitivity
    {PARAM_NUMBER_ERROR, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 0},        //Detection scheme
    {30, 1},        //Initial
    {31, 1},        //Maintain
    {PARAM_NUMBER_ERROR, 0},        //Retrigger
    {PARAM_NUMBER_ERROR, 0},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 0},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Latitude              // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {256, 0x103},         //Type                  read only
    {512, 0},        //Type 2                read only       // 2.3.1
    {259, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Daylight level        read only, writable when used for calibration
    {260, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTSIGN
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTHOURS
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTMINUTES
    { 290, 0 }  // PARAM_NUMBER_KNX_MASTER_SLAVE
};


//------------------------------------------------------------------------------
//         type = 0x0104
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_KNX_PIR_US_OCCUPANCY_ONLY = {
    {10, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {21, 2},        //US sensitivity
    {PARAM_NUMBER_ERROR, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 0},        //Detection scheme
    {30, 3},        //Initial
    {31, 4},        //Maintain
    {PARAM_NUMBER_ERROR, 0},        //Retrigger
    {PARAM_NUMBER_ERROR, 0},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 0},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Latitude              // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {256, 0x104},         //Type                  read only
    {512, 0},        //Type 2                read only       // 2.3.1
    {259, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Daylight level        read only, writable when used for calibration
    {260, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTSIGN
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTHOURS
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTMINUTES
    { 290, 0 }  // PARAM_NUMBER_KNX_MASTER_SLAVE
};

//------------------------------------------------------------------------------
//         type = 0x0105
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_KNX_PIR_US_OCCUPANCY_DAYLIGHT = {
    {10, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {21, 2},        //US sensitivity
    {25, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 0},        //Detection scheme
    {30, 3},        //Initial
    {31, 4},        //Maintain
    {PARAM_NUMBER_ERROR, 0},        //Retrigger
    {PARAM_NUMBER_ERROR, 0},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 0},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {256, 0x105},         //Type                  read only
    {512, 0},        //Type 2                read only       // 2.3.1
    {259, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {27, 0},        //Daylight level        read only, writable when used for calibration
    {260, 0},        //Sensor status         read only in hidden page
    {262, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTSIGN
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTHOURS
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTMINUTES
    { 290, 0 }  // PARAM_NUMBER_KNX_MASTER_SLAVE
};


//------------------------------------------------------------------------------
//         type = 0x0106
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_SLAVE_KNX_PIR_US_OCCUPANCY_ONLY = {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {PARAM_NUMBER_ERROR, 0},       //Minute
    {PARAM_NUMBER_ERROR, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {21, 2},        //US sensitivity
    {PARAM_NUMBER_ERROR, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 0},        //Detection scheme
    {30, 3},        //Initial
    {31, 4},        //Maintain
    {PARAM_NUMBER_ERROR, 0},        //Retrigger
    {PARAM_NUMBER_ERROR, 0},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 0},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 0},       //Latitude              // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {256, 0x106},         //Type                  read only
    {512, 0},        //Type 2                read only       // 2.3.1
    {259, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Daylight level        read only, writable when used for calibration
    {260, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTSIGN
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTHOURS
    {PARAM_NUMBER_ERROR, 0},        // PARAM_INDEX_GMTMINUTES
    { 290, 0 }  // PARAM_NUMBER_KNX_MASTER_SLAVE
};



