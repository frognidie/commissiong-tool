
//------------------------------------------------------------------------------
//         type = 0x107
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_HF_PRECIR= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 1},       //Minute
    {12, 0},        //Second
    {19, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {25, 200},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_ERROR, 5},        //Initial
    {PARAM_NUMBER_ERROR, 5},        //Maintain
    {PARAM_NUMBER_ERROR, 5},        //Retrigger
    {29, 1},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
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
    {PARAM_NUMBER_TYPE, 0x107},         //Type                  read only
    {PARAM_NUMBER_TYPE2, 1},        //Type 2                read only       // 2.3.1
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
    {PARAM_NUMBER_ERROR, 0},
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_DIM_R , 1},        //
    {PARAM_NUMBER_DIM_F , 12},        //
    {PARAM_NUMBER_AUX_MODE, 0},        //
    {PARAM_NUMBER_AUX_PULSE_TIME, 2},        //
    {PARAM_NUMBER_ON_LEVEL,100},        //
    {PARAM_NUMBER_EXT_MODE_STATE, 1},        //
    {PARAM_NUMBER_EXT_FUNC_MODE , 1},        //
    {PARAM_NUMBER_EXT_STANDBY_LEVEL, 10},        //
    {PARAM_NUMBER_EXT_STANDBY_DELAY , 255},        //
    {PARAM_NUMBER_EXT_ON_LEVEL  , 50},        //
    {PARAM_NUMBER_OP_TIME  , 0},        //
    {PARAM_NUMBER_ERROR, 0}, 
    {PARAM_NUMBER_ERROR, 0},
    {PARAM_NUMBER_ERROR, 0}, 
    {PARAM_NUMBER_ERROR, 0}, 
    {PARAM_NUMBER_ERROR, 0}, 
    {PARAM_NUMBER_ERROR, 0}, 
    {PARAM_NUMBER_ERROR, 0}, 
    {PARAM_NUMBER_ERROR, 0}, 
    {PARAM_NUMBER_ERROR, 0}, 

};





//------------------------------------------------------------------------------
//         type = 0x111
//------------------------------------------------------------------------------
//static const SENSOR_PARAMS  DefParams_LV_HF_PRECIR_clock= {
//    {PARAM_NUMBER_ERROR, 0},        //Hour
//    {11, 1},       //Minute
//    {12, 0},        //Second
//    {19, 2},        //HF sensitivity        // 1.0.24
//    {PARAM_NUMBER_PIR_SENS, 3},        //PIR sensitivity
//    {PARAM_NUMBER_US_SENS, 2},        //US sensitivity
//    {25, 200},      //Daylight setpoint
//    {PARAM_NUMBER_DETECTIONSCHEME, 3},        //Detection scheme
//    {PARAM_NUMBER_INITIAL, 5},        //Initial
//    {PARAM_NUMBER_MAINTAIN, 5},        //Maintain
//    {PARAM_NUMBER_RETRIGGER, 5},        //Retrigger
//    {29, 1},        //functional Mode
//    {PARAM_NUMBER_ALERT, 0},        //Alert
//    {35, 10},       //Standby level         // 1.0.24
//    {36, 0},        //Standby delay         // 1.0.24
//    {PARAM_NUMBER_SENDCONDITION, 3},        //Send condition        // 1.0.24
//    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
//    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
//    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
//    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
//    {PARAM_NUMBER_ERROR, 0},        //Loop type
//    {PARAM_NUMBER_ERROR, 0},        //Provision of light
//    {PARAM_NUMBER_YEAR, 12},       //Year                  // 2.3.1
//    {PARAM_NUMBER_MONTH, 1},        //Month                 // 2.3.1
//    {PARAM_NUMBER_DAY, 1},        //Day                   // 2.3.1
//    {PARAM_NUMBER_SUMMERHOUR, 1},        //Summer hour           // 2.3.1
//    {PARAM_NUMBER_STARTMONTH, 3},        //Start month           // 2.3.1
//    {PARAM_NUMBER_STARTDAY, 30},       //Start day             // 2.3.1
//    {PARAM_NUMBER_ENDMONTH, 10},       //End month             // 2.3.1
//    {PARAM_NUMBER_ENDDAY, 31},       //End day               // 2.3.1
//    {PARAM_NUMBER_ERROR, 8},        //Country               // 2.3.1
//    {PARAM_NUMBER_ERROR, 2},        //Longitude             // 2.3.1
//    {PARAM_NUMBER_ERROR, 49},       //Latitude              // 2.3.1
//    {22, 10},       //Daylight factor       read write only in hidden page
//    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
//    {24, 10},       //Natural light factor  read write only in hidden page
//    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
//    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
//    {PARAM_NUMBER_TYPE, 0x111},         //Type                  read only
//    {PARAM_NUMBER_TYPE2, 1},        //Type 2                read only       // 2.3.1
//    {259, 0},        //Firmware version      read only
//    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
//    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
//    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
//    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
//    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
//    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
//    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
//    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
//    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
//    {27, 0},        //Daylight level        read only, writable when used for calibration
//    {260, 0},        //Sensor status         read only in hidden page
//    {262, 0},        //Current provision of light    read only in hidden page    // 1.0.24
//    {PARAM_NUMBER_LOADTYPE, 0},        //Load type             read/write only in hidden page  // 2.2.0
//    {PARAM_NUMBER_POWERLINEFILTER, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
//    {268, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
//    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
//    {PARAM_NUMBER_ERROR, 0},        //2.4.1
//    {PARAM_NUMBER_ERROR, 0},        //2.4.1
//    {PARAM_NUMBER_ERROR, 0},        //2.4.1
//    {PARAM_NUMBER_ERROR, 0},
//        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
//    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
//    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
//        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
//    {PARAM_NUMBER_DIM_R , 1},        //
//    {PARAM_NUMBER_DIM_F , 12},        //
//    {PARAM_NUMBER_AUX_MODE, 0},        //
//    {PARAM_NUMBER_AUX_PULSE_TIME, 1},        //
//    {PARAM_NUMBER_ON_LEVEL,100},        //
//    {PARAM_NUMBER_EXT_MODE_STATE, 1},        //
//    {PARAM_NUMBER_EXT_FUNC_MODE , 1},        //
//    {PARAM_NUMBER_EXT_STANDBY_LEVEL, 10},        //
//    {PARAM_NUMBER_EXT_STANDBY_DELAY , 255},        //
//    {PARAM_NUMBER_EXT_ON_LEVEL  , 50},        //
//    {PARAM_NUMBER_OP_TIME  , 65535},        //
//    {320,3},
//    {311,20},
//    {312,0},
//    {313,8},
//    {314,0},
//    {310,0},
//    {306,8},
//    {307,0},
//    {308,0},
//
//};


static const SENSOR_PARAMS  DefParams_LV_HF_PRECIR_clock= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 1},       //Minute
    {12, 0},        //Second
    {19, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {25, 200},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_ERROR, 5},        //Initial
    {PARAM_NUMBER_ERROR, 5},        //Maintain
    {PARAM_NUMBER_ERROR, 5},        //Retrigger
    {29, 1},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
    {PARAM_NUMBER_YEAR, 12},       //Year                  // 2.3.1
    {PARAM_NUMBER_MONTH, 1},        //Month                 // 2.3.1
    {PARAM_NUMBER_DAY, 1},        //Day                   // 2.3.1
    {PARAM_NUMBER_SUMMERHOUR, 1},        //Summer hour           // 2.3.1
    {PARAM_NUMBER_STARTMONTH, 3},        //Start month           // 2.3.1
    {PARAM_NUMBER_STARTDAY, 30},       //Start day             // 2.3.1
    {PARAM_NUMBER_ENDMONTH, 10},       //End month             // 2.3.1
    {PARAM_NUMBER_ENDDAY, 31},       //End day               // 2.3.1
    {PARAM_NUMBER_ERROR, 8},        //Country               // 2.3.1
    {PARAM_NUMBER_ERROR, 2},        //Longitude             // 2.3.1
    {PARAM_NUMBER_ERROR, 49},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x111},         //Type                  read only
    {PARAM_NUMBER_TYPE2, 1},        //Type 2                read only       // 2.3.1
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
    {PARAM_NUMBER_ERROR, 0},
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_DIM_R , 1},        //
    {PARAM_NUMBER_DIM_F , 12},        //
    {PARAM_NUMBER_AUX_MODE, 0},        //
    {PARAM_NUMBER_AUX_PULSE_TIME, 2},        //
    {PARAM_NUMBER_ON_LEVEL,100},        //
    {PARAM_NUMBER_EXT_MODE_STATE, 1},        //
    {PARAM_NUMBER_EXT_FUNC_MODE , 1},        //
    {PARAM_NUMBER_EXT_STANDBY_LEVEL, 10},        //
    {PARAM_NUMBER_EXT_STANDBY_DELAY , 255},        //
    {PARAM_NUMBER_EXT_ON_LEVEL  , 50},        //
    {PARAM_NUMBER_OP_TIME  , 0},        //
    {320,3},
    {311,20},
    {312,0},
    {313,8},
    {314,0},
    {310,0},
    {306,8},
    {307,0},
    {308,0},

};

//------------------------------------------------------------------------------
//         type = 0x112
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_HF_PRECIR_bilevel= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 1},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {PARAM_NUMBER_ERROR, 200},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_ERROR, 5},        //Initial
    {PARAM_NUMBER_ERROR, 5},        //Maintain
    {PARAM_NUMBER_ERROR, 5},        //Retrigger
    {29, 7},        //functional Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {35, 10},       //Standby level         // 1.0.24
    {36, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
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
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x112},         //Type                  read only
    {PARAM_NUMBER_TYPE2, 1},        //Type 2                read only       // 2.3.1
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
    {PARAM_NUMBER_ERROR, 0},
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_DIM_R , 1},        //
    {PARAM_NUMBER_DIM_F , 12},        //
    {PARAM_NUMBER_AUX_MODE, 0},        //
    {PARAM_NUMBER_AUX_PULSE_TIME, 2},        //
    {PARAM_NUMBER_ON_LEVEL,100},        //
    {PARAM_NUMBER_ERROR, 1},        //
    {PARAM_NUMBER_ERROR , 1},        //
    {PARAM_NUMBER_ERROR, 10},        //
    {PARAM_NUMBER_ERROR , 255},        //
    {PARAM_NUMBER_ERROR  , 50},        //
    {PARAM_NUMBER_OP_TIME  , 0},        //
    {PARAM_NUMBER_ERROR,3},
    {PARAM_NUMBER_ERROR,20},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,8},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,8},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,0},

};


//------------------------------------------------------------------------------
//         type = 0x113
//------------------------------------------------------------------------------
static const SENSOR_PARAMS  DefParams_LV_HF_PRECIR_essential= {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {11, 1},       //Minute
    {12, 0},        //Second
    {19, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {25, 200},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_ERROR, 5},        //Initial
    {PARAM_NUMBER_ERROR, 5},        //Maintain
    {PARAM_NUMBER_ERROR, 5},        //Retrigger
    {PARAM_NUMBER_ERROR, 1},        //functional Mode
    {PARAM_NUMBER_ALERT, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {PARAM_NUMBER_ERROR, 0},        //Provision of light
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
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0x113},         //Type                  read only
    {PARAM_NUMBER_TYPE2, 1},        //Type 2                read only       // 2.3.1
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
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_LOADTYPE, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_POWERLINEFILTER, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {268, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {269, 10},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
        {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR , 1},        //
    {PARAM_NUMBER_ERROR , 12},        //
    {PARAM_NUMBER_ERROR, 0},        //
    {PARAM_NUMBER_ERROR, 1},        //
    {PARAM_NUMBER_ERROR,100},        //
    {PARAM_NUMBER_ERROR, 1},        //
    {PARAM_NUMBER_ERROR , 1},        //
    {PARAM_NUMBER_ERROR, 10},        //
    {PARAM_NUMBER_ERROR , 255},        //
    {PARAM_NUMBER_ERROR  , 50},        //
    {PARAM_NUMBER_OP_TIME  , 0},        //
    {PARAM_NUMBER_ERROR,3},
    {PARAM_NUMBER_ERROR,20},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,8},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,8},
    {PARAM_NUMBER_ERROR,0},
    {PARAM_NUMBER_ERROR,0},

};