

//------------------------------------------------------------------------------
//         Local Variables
//------------------------------------------------------------------------------

static const SENSOR_PARAMS defaultParams = {
    {PARAM_NUMBER_ERROR, 0},        //Hour
    {PARAM_NUMBER_ERROR, 15},       //Minute
    {PARAM_NUMBER_ERROR, 0},        //Second
    {PARAM_NUMBER_ERROR, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {PARAM_NUMBER_ERROR, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_ERROR, 3},        //Initial
    {PARAM_NUMBER_ERROR, 4},        //Maintain
    {PARAM_NUMBER_ERROR, 4},        //Retrigger
    {PARAM_NUMBER_ERROR, 2},        //Mode
    {PARAM_NUMBER_ERROR, 0},        //Alert
    {PARAM_NUMBER_ERROR, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_ERROR, 1},        //Light regulation      default value is 0, here uses 1 for testing
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
    {PARAM_NUMBER_TYPE, 0},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
    {PARAM_NUMBER_FIRMWAREVERSION, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_DAYLIGHTLEVEL, 0},        //Daylight level        read only, writable when used for calibration
    {PARAM_NUMBER_ERROR, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_BUZZERFEEDBACK, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_FACTORYLIGHTFACTOR, 1},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtHour
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtMin
    {PARAM_NUMBER_ERROR, 0},        //2.4.1  GmtSign
};

static const SENSOR_PARAMS defaultParamsCorridorHall = {
    {PARAM_NUMBER_HOUR, 0},        //Hour
    {PARAM_NUMBER_MINUTE, 10},       //Minute
    {PARAM_NUMBER_SECOND, 0},        //Second
    {PARAM_NUMBER_HF_SENS, 3},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_PIR_SENS, 2},        //PIR sensitivity
    {PARAM_NUMBER_US_SENS, 2},        //US sensitivity
    {PARAM_NUMBER_DAYLIGHTSETPOINT, 20},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_INITIAL, 3},        //Initial
    {PARAM_NUMBER_MAINTAIN, 4},        //Maintain
    {PARAM_NUMBER_RETRIGGER, 0},        //Retrigger
    {PARAM_NUMBER_MODE, 2},        //Mode
    {PARAM_NUMBER_ALERT, 0},        //Alert
    {PARAM_NUMBER_STANDBYLEVEL, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_STANDBYDELAY, 61},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_LIGHTREGULATION, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_LOOPTYPE, 0},        //Loop type
    {PARAM_NUMBER_PROVISIONOFLIGHT, 0},      //Provision of light
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
    {PARAM_NUMBER_DAYLIGHTFACTOR, 35},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_NATURALLIGHTFACTOR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
    {PARAM_NUMBER_FIRMWAREVERSION, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_DAYLIGHTLEVEL, 0},        //Daylight level        read only, writable when used for calibration
    {PARAM_NUMBER_ERROR, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_BUZZERFEEDBACK, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_FACTORYLIGHTFACTOR, 1},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

static const SENSOR_PARAMS defaultParamsSmallOfficeCell = {
    {PARAM_NUMBER_HOUR, 0},        //Hour
    {PARAM_NUMBER_MINUTE, 15},       //Minute
    {PARAM_NUMBER_SECOND, 0},        //Second
    {PARAM_NUMBER_HF_SENS, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_PIR_SENS, 0},        //PIR sensitivity
    {PARAM_NUMBER_US_SENS, 1},        //US sensitivity
    {PARAM_NUMBER_DAYLIGHTSETPOINT, 60},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_INITIAL, 3},        //Initial
    {PARAM_NUMBER_MAINTAIN, 4},        //Maintain
    {PARAM_NUMBER_RETRIGGER, 2},        //Retrigger
    {PARAM_NUMBER_MODE, 1},        //Mode
    {PARAM_NUMBER_ALERT, 0},        //Alert
    {PARAM_NUMBER_STANDBYLEVEL, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_STANDBYDELAY, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_LIGHTREGULATION, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_LOOPTYPE, 0},        //Loop type
    {PARAM_NUMBER_PROVISIONOFLIGHT, 0},      //Provision of light
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
    {PARAM_NUMBER_DAYLIGHTFACTOR, 35},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_NATURALLIGHTFACTOR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
    {PARAM_NUMBER_FIRMWAREVERSION, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_DAYLIGHTLEVEL, 0},        //Daylight level        read only, writable when used for calibration
    {PARAM_NUMBER_ERROR, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_BUZZERFEEDBACK, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_FACTORYLIGHTFACTOR, 1},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

static const SENSOR_PARAMS defaultParamsMeetingRoom = {
    {PARAM_NUMBER_HOUR, 0},        //Hour
    {PARAM_NUMBER_MINUTE, 15},       //Minute
    {PARAM_NUMBER_SECOND, 0},        //Second
    {PARAM_NUMBER_HF_SENS, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_PIR_SENS, 2},        //PIR sensitivity
    {PARAM_NUMBER_US_SENS, 1},        //US sensitivity
    {PARAM_NUMBER_DAYLIGHTSETPOINT, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_INITIAL, 3},        //Initial
    {PARAM_NUMBER_MAINTAIN, 2},        //Maintain
    {PARAM_NUMBER_RETRIGGER, 2},        //Retrigger
    {PARAM_NUMBER_MODE, 1},        //Mode
    {PARAM_NUMBER_ALERT, 0},        //Alert
    {PARAM_NUMBER_STANDBYLEVEL, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_STANDBYDELAY, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_LIGHTREGULATION, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_LOOPTYPE, 0},        //Loop type
    {PARAM_NUMBER_PROVISIONOFLIGHT, 0},      //Provision of light
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
    {PARAM_NUMBER_DAYLIGHTFACTOR, 35},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_NATURALLIGHTFACTOR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
    {PARAM_NUMBER_FIRMWAREVERSION, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_DAYLIGHTLEVEL, 0},        //Daylight level        read only, writable when used for calibration
    {PARAM_NUMBER_ERROR, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_BUZZERFEEDBACK, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_FACTORYLIGHTFACTOR, 1},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

static const SENSOR_PARAMS defaultParamsClassRoom = {
    {PARAM_NUMBER_HOUR, 0},        //Hour
    {PARAM_NUMBER_MINUTE, 20},       //Minute
    {PARAM_NUMBER_SECOND, 0},        //Second
    {PARAM_NUMBER_HF_SENS, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_PIR_SENS, 2},        //PIR sensitivity
    {PARAM_NUMBER_US_SENS, 2},        //US sensitivity
    {PARAM_NUMBER_DAYLIGHTSETPOINT, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_INITIAL, 3},        //Initial
    {PARAM_NUMBER_MAINTAIN, 4},        //Maintain
    {PARAM_NUMBER_RETRIGGER, 4},        //Retrigger
    {PARAM_NUMBER_MODE, 1},        //Mode
    {PARAM_NUMBER_ALERT, 0},        //Alert
    {PARAM_NUMBER_STANDBYLEVEL, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_STANDBYDELAY, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_LIGHTREGULATION, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_LOOPTYPE, 0},        //Loop type
    {PARAM_NUMBER_PROVISIONOFLIGHT, 0},      //Provision of light
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
    {PARAM_NUMBER_DAYLIGHTFACTOR, 35},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_NATURALLIGHTFACTOR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
    {PARAM_NUMBER_FIRMWAREVERSION, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_DAYLIGHTLEVEL, 0},        //Daylight level        read only, writable when used for calibration
    {PARAM_NUMBER_ERROR, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_BUZZERFEEDBACK, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_FACTORYLIGHTFACTOR, 1},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

static const SENSOR_PARAMS defaultParamsOpenSpace = {
    {PARAM_NUMBER_HOUR, 0},        //Hour
    {PARAM_NUMBER_MINUTE, 15},       //Minute
    {PARAM_NUMBER_SECOND, 0},        //Second
    {PARAM_NUMBER_HF_SENS, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_PIR_SENS, 2},        //PIR sensitivity
    {PARAM_NUMBER_US_SENS, 2},        //US sensitivity
    {PARAM_NUMBER_DAYLIGHTSETPOINT, 100},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_INITIAL, 3},        //Initial
    {PARAM_NUMBER_MAINTAIN, 4},        //Maintain
    {PARAM_NUMBER_RETRIGGER, 0},        //Retrigger
    {PARAM_NUMBER_MODE, 1},        //Mode
    {PARAM_NUMBER_ALERT, 0},        //Alert
    {PARAM_NUMBER_STANDBYLEVEL, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_STANDBYDELAY, 0},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_LIGHTREGULATION, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_LOOPTYPE, 0},        //Loop type
    {PARAM_NUMBER_PROVISIONOFLIGHT, 0},      //Provision of light
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
    {PARAM_NUMBER_DAYLIGHTFACTOR, 35},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_NATURALLIGHTFACTOR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
    {PARAM_NUMBER_FIRMWAREVERSION, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_DAYLIGHTLEVEL, 0},        //Daylight level        read only, writable when used for calibration
    {PARAM_NUMBER_ERROR, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_BUZZERFEEDBACK, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_FACTORYLIGHTFACTOR, 1},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};

static const SENSOR_PARAMS defaultParamsToilet = {
    {PARAM_NUMBER_HOUR, 0},        //Hour
    {PARAM_NUMBER_MINUTE, 10},       //Minute
    {PARAM_NUMBER_SECOND, 0},        //Second
    {PARAM_NUMBER_HF_SENS, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_PIR_SENS, 2},        //PIR sensitivity
    {PARAM_NUMBER_US_SENS, 0},        //US sensitivity
    {PARAM_NUMBER_DAYLIGHTSETPOINT, 40},      //Daylight setpoint
    {PARAM_NUMBER_ERROR, 3},        //Detection scheme
    {PARAM_NUMBER_INITIAL, 3},        //Initial
    {PARAM_NUMBER_MAINTAIN, 4},        //Maintain
    {PARAM_NUMBER_RETRIGGER, 4},        //Retrigger
    {PARAM_NUMBER_MODE, 1},        //Mode
    {PARAM_NUMBER_ALERT, 0},        //Alert
    {PARAM_NUMBER_STANDBYLEVEL, 10},       //Standby level         // 1.0.24
    {PARAM_NUMBER_STANDBYDELAY, 61},        //Standby delay         // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Send condition        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //Dead band             // 1.0.24
    {PARAM_NUMBER_ERROR, 5},        //Time base             // 1.0.24
    {PARAM_NUMBER_ERROR, 60},       //Bandwidth             // 1.0.24
    {PARAM_NUMBER_LIGHTREGULATION, 0},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_LOOPTYPE, 0},        //Loop type
    {PARAM_NUMBER_PROVISIONOFLIGHT, 0},      //Provision of light
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
    {PARAM_NUMBER_DAYLIGHTFACTOR, 35},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {PARAM_NUMBER_NATURALLIGHTFACTOR, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {PARAM_NUMBER_TYPE, 0},         //Type                  read only
    {PARAM_NUMBER_ERROR, 0},        //Type 2                read only       // 2.3.1
    {PARAM_NUMBER_FIRMWAREVERSION, 0},        //Firmware version      read only
    {PARAM_NUMBER_ERROR, 0},        //ID1                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID2                   read only
    {PARAM_NUMBER_ERROR, 0},        //ID3                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //ID4                   read only       // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //Referent              read only
    {PARAM_NUMBER_ERROR, 0},        //Main group            read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Battery information   read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version     read only       // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Daughter firmware version 2   read only       // 2.3.1
    {PARAM_NUMBER_DAYLIGHTLEVEL, 0},        //Daylight level        read only, writable when used for calibration
    {PARAM_NUMBER_ERROR, 0},        //Sensor status         read only in hidden page
    {PARAM_NUMBER_ERROR, 0},        //Current provision of light    read only in hidden page    // 1.0.24
    {PARAM_NUMBER_ERROR, 0},        //Load type             read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_ERROR, 0},        //Power line filter     read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_BUZZERFEEDBACK, 1},        //Buzzer feedback       read/write only in hidden page  // 2.2.0
    {PARAM_NUMBER_FACTORYLIGHTFACTOR, 1},   //Factory light factor      not implemented in menu     // 2.3.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1
    {PARAM_NUMBER_ERROR, 0},        //2.4.1

};
