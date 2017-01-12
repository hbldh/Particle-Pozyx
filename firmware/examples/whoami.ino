/*
 * # WhoAmI app
 *
 * Flash the Photon with this app and call
 *
 * ```
 * particle get <DEVICE_ID> whoami
 * ```
 *
 * to see if your Photon can talk to your Pozyx shield. The
 * response, if successful, should be something like
 *
 * ```
 * 0x43, FW ver.: v1.0, HW ver.: 35, Selftest: 00111111
 * ```
 *
 * You can also check this [Photon variable](https://docs.particle.io/reference/firmware/photon/?fw_ver=0.5.3&cli_ver=1.17.0&electron_parts=2#particle-variable-)
 * directly via a HTTP call, e.g. from `curl`:
 *
 * ```sh
 * curl "https://api.particle.io/v1/devices/<DEVICE_ID>/whoami?access_token=<YOUR_ACCESS_TOKEN>"
 * ```
 *
 * which yield a JSON response:
 *
 * ```json
 * {
 *   "cmd": "VarReturn",
 *   "name": "whoami",
 *   "error": null,
 *   "result": "0x43, FW ver.: v1.0, HW ver.: 35, Selftest: 00111111",
 *   "coreInfo": {
 *     "last_app": "",
 *     "last_heard": "2016-11-07T13:27:04.325Z",
 *     "connected": true,
 *     "last_handshake_at": "2016-11-07T13:18:22.461Z",
 *     "deviceID": "<YOUR_DEVICE_ID>",
 *     "product_id": 6
 *   }
 * }
 * ```
 */

#include "Pozyx/Pozyx.h"
#include "Pozyx/Pozyx_definitions.h"

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
