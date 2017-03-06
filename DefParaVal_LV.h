
//------------------------------------------------------------------------------
//         type = 0x11
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_PIR= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {25, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {30, 1},        //Initial
    {31, 1},        //Maintain
    {32, 1},        //Retrigger
    {29, 2},        //functional Mode
    {34, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {28, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 12},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 3},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 30},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 31},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 8},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 2},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 49},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x11},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
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
    {268, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

//------------------------------------------------------------------------------
//         type = 0x21
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_US= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {21, 2},        //US sensitivity
    {25, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {30, 2},        //Initial
    {31, 2},        //Maintain
    {32, 2},        //Retrigger
    {29, 2},        //functional Mode
    {34, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {28, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 12},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 3},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 30},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 31},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 8},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 2},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 49},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x21},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
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
    {268, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

//------------------------------------------------------------------------------
//         type = 0x31
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_PIR_US= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 0},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {21, 2},        //US sensitivity
    {25, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {30, 3},        //Initial
    {31, 4},        //Maintain
    {32, 4},        //Retrigger
    {29, 2},        //functional Mode
    {34, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {28, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 12},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 3},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 30},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 31},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 8},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 2},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 49},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x31},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
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
    {268, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

//------------------------------------------------------------------------------
//         type = 0x9
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_HF= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {19, 3},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {25, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {30, 5},        //Initial
    {31, 5},        //Maintain
    {32, 5},        //Retrigger
    {29, 2},        //functional Mode
    {34, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {28, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 12},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 3},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 30},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 31},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 8},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 2},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 49},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 9},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
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
    {PARAM_NUMBER_LOADTYPE, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_POWERLINEFILTER, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {268, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

//------------------------------------------------------------------------------
//         type = 0x19
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_HF_PIR= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 15},       //Minute
    {12, 0},        //Second
    {19, 2},        //HF sensitivity        // 1.0.24
    {20, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {25, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {30, 6},        //Initial
    {31, 7},        //Maintain
    {32, 7},        //Retrigger
    {29, 2},        //functional Mode
    {34, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {28, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {PARAM_NUMBER_ERROR, 12},       //Year                  // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Month                 // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Day                   // 2.3.1
    {PARAM_NUMBER_ERROR, 1},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_ERROR, 3},        //Start month           // 2.3.1
    {PARAM_NUMBER_ERROR, 30},       //Start day             // 2.3.1
    {PARAM_NUMBER_ERROR, 10},       //End month             // 2.3.1
    {PARAM_NUMBER_ERROR, 31},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 8},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 2},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 49},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x19},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
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
    {268, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};


