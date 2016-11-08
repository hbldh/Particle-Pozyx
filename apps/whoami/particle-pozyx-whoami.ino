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

char *version = "whoami - version 1.0.0";
char whoami_str[80];
char *error_msg = "";
uint8_t tmp;

int whoami;
int fw_version;
int hw_version;
int selftest;
int status;

void setup() {

    Particle.variable("version", version, STRING);
    Particle.variable("whoami", whoami_str, STRING);

    Serial.begin(115200);
    status = Pozyx.begin();

    if(status == POZYX_FAILURE){
        error_msg = F("ERROR: Unable to connect to POZYX shield");
        delay(100);
        abort();
    }
    set_whoami_variable();
}

void loop() {

}

/*
 * Set the Particle `whoami` variable string.
 */
void set_whoami_variable() {
    // Get board details.
    Pozyx.getWhoAmI(&tmp, NULL);
    whoami = (int) tmp;

    Pozyx.getFirmwareVersion(&tmp, NULL);
    fw_version = (int) tmp;

    Pozyx.getHardwareVersion(&tmp, NULL);
    hw_version = (int) tmp;

    Pozyx.getSelftest(&tmp, NULL);
    selftest = (int) tmp;

    sprintf(whoami_str, "0x%02X, FW ver.: v%d.%d, HW ver.: %d, Selftest: %c%c%c%c%c%c%c%c",
       whoami, (fw_version & 0xf0) >> 4, fw_version & 0x0f, hw_version, BYTE_TO_BINARY(selftest));
}
