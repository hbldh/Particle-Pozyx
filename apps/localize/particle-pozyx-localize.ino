// This #include statement was automatically added by the Particle IDE.
#include "Pozyx.h"
#include "Pozyx_definitions.h"

#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')



////////////////////////////////////////////////
/////////////// POZYX PARAMETERS ///////////////
////////////////////////////////////////////////
uint8_t num_anchors = 4;
uint16_t anchors[4] = {
    0x6819,
    0x6820,
    0x683E,
    0x6847
};
// only required for manual anchor calibration. Please change this to the coordinates measured for the anchors
int32_t anchors_x[4] = {0, 1000, 0, 1000};              // anchor x-coorindates in mm
int32_t anchors_y[4] = {0, 0, 1000, 1000};                  // anchor y-coordinates in mm
int32_t heights[4]  = {0, 0, 0, 0};

char *version = "v0.1.2, localize";
char whoami_str[80];
char error_msg[80];
char error_code[4];
char position[80];
int run = 0;


void setup() {

    Particle.variable("version", version, STRING);
    Particle.variable("whoami", whoami_str, STRING);
    Particle.variable("errormsg", error_msg, STRING);
    Particle.variable("errorcode", error_code, STRING);

    // Register the cloud functions
    Particle.function("mancalib", manual_calibration);
    Particle.function("autocalib", auto_calibration);
    Particle.function("clear_error", clear_error);
    Particle.function("toggle", toggle);

    Serial.begin(115200);
    int status = Pozyx.begin();

    if(status == POZYX_FAILURE){
        String s = Pozyx.getSystemError();
        sprintf(error_msg, s);
        set_errorcode_variable();
        //delay(100);
        //abort();
    }
    else {
        set_whoami_variable();
        sprintf(error_msg, "");
    }

}

void loop() {
    coordinates_t pos;
    int status = 0;

    if (run == 1) {

        status = Pozyx.doPositioning(&pos, POZYX_2_5D, 0, POZYX_POS_ALG_UWB_ONLY);
        //status = Pozyx.doPositioning(&pos, POZYX_2_5D, 0, POZYX_POS_ALG_LS);
        //pos_error_t pos_error;
        //Pozyx.getPositionError(&pos_error);

        if (status == POZYX_SUCCESS)
        {
            sprintf(position, "(%d, %d, %d)", pos.x, pos.y, pos.z);
            Particle.publish("position", position, 2);
        }
        else
        {
            String s = Pozyx.getSystemError();
            sprintf(error_msg, "Positioning error: %s", s.c_str());
            set_errorcode_variable();
        }
    }
    delay(500);
}

/*
 * Set the Particle `whoami` variable string.
 */
void set_whoami_variable() {
    // Get board details.
    int whoami;
    int fw_version;
    int hw_version;
    int selftest;
    uint8_t tmp;

    Pozyx.getWhoAmI(&tmp, NULL);
    whoami = (int) tmp;

    Pozyx.getFirmwareVersion(&tmp, NULL);
    fw_version = (int) tmp;

    Pozyx.getHardwareVersion(&tmp, NULL);
    hw_version = (int) tmp;

    Pozyx.getSelftest(&tmp, NULL);
    selftest = (int) tmp;

    sprintf(whoami_str, "ParticlePozyx - 0x%02X, FW ver.: v%d.%d, HW ver.: %d, HW type: %d, Selftest: %c%c%c%c%c%c%c%c",
       whoami, (fw_version & 0xf0) >> 4, fw_version & 0x0f, hw_version & 0x1f, (hw_version & 0xe0) >> 5, BYTE_TO_BINARY(selftest));
}

void set_errorcode_variable() {
    uint8_t ecode;
    Pozyx.regRead(POZYX_ERRORCODE, &ecode, 1);
    sprintf(error_code, "0x%02X", (int) ecode);
}


/*
 * Run manual calibration method.
 */
int manual_calibration(String instr)
{
    // function to manually set the anchor coordinates
    int i=0;

    for(i=0; i < num_anchors; i++) {
        device_coordinates_t anchor;
        anchor.network_id = anchors[i];
        anchor.flag = 0x1;
        anchor.pos.x = anchors_x[i];
        anchor.pos.y = anchors_y[i];
        anchor.pos.z = heights[i];
        Pozyx.addDevice(anchor);
    }

    return POZYX_SUCCESS;
}

/*
 * Run automatic calibration method.
 */
int auto_calibration(String instr)
{
    // clear all previous devices in the device list.
    Pozyx.clearDevices();

    int status = Pozyx.doAnchorCalibration(POZYX_2_5D, 20, num_anchors, anchors, heights);

    if (status != POZYX_SUCCESS){
        String s = Pozyx.getSystemError();
        sprintf(error_msg, s);
        set_errorcode_variable();

    }
    return status;
}


int clear_error(String instr) {
    sprintf(error_msg, "");
    return 1;
}

int toggle(String state) {
    if (state.startsWith("0")) {
        run = 0;
    }
    else {
        run = 1;
    }
    return run;
}
