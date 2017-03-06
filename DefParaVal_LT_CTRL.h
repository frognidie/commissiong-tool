

//------------------------------------------------------------------------------
//         type = 0x201
//------------------------------------------------------------------------------
static const SENSOR_PARAMS DefParams_LT_CTR = {
    {10, 0},        //Hour
    {11, 0},       //Minute
    {12, 0},        //Second
    {PARAM_NUMBER_ERROR, 2},        //HF sensitivity        // 1.0.24
    {PARAM_NUMBER_ERROR, 3},        //PIR sensitivity
    {PARAM_NUMBER_ERROR, 2},        //US sensitivity
    {25, 100},      //Daylight setpoint
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
    {28, 1},        //Light regulation      default value is 0, here uses 1 for testing
    {PARAM_NUMBER_ERROR, 0},        //Loop type
    {26, 0},        //Provision of light
    {270, 12},       //Year                  // 2.3.1
    {271, 1},        //Month                 // 2.3.1
    {272, 1},        //Day                   // 2.3.1
    {273, 1},        //Summer hour           // 2.3.1
    {274, 3},        //Start month           // 2.3.1
    {275, 30},       //Start day             // 2.3.1
    {276, 10},       //End month             // 2.3.1
    {277, 31},       //End day               // 2.3.1
    {278, 8},        //Country               // 2.3.1
    {279, 2},        //Longitude             // 2.3.1
    {280, 49},       //Latitude              // 2.3.1
    {22, 10},       //Daylight factor       read write only in hidden page
    {PARAM_NUMBER_ERROR, 10},       //Daylight factor cen   read write only in hidden page
    {24, 10},       //Natural light factor  read write only in hidden page
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee      read only, writable in hidden page   // 1.0.24
    {PARAM_NUMBER_ERROR, 0x0103},   //Device ID Zigbee 2    read only, writable in hidden page   // 2.3.1
    {256, 0x201},         //Type                  read only
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
    {287, 1},        //2.4.1 gmt timezone sign
    {288, 0},        //2.4.1 gmt timezone hours
    {289, 0},        //2.4.1 gmt timezone minutes

};

