# WhoAmI app

Flash the Photon with this app and call

```
particle get <DEVICE_ID> whoami
```

to see if your Photon can talk to your Pozyx shield. The
response, if successful, should be something like

```
0x43, FW ver.: v1.0, HW ver.: 35, Selftest: 00111111
```

You can also check this [Photon variable](https://docs.particle.io/reference/firmware/photon/?fw_ver=0.5.3&cli_ver=1.17.0&electron_parts=2#particle-variable-)
directly via a HTTP call, e.g. from `curl`:

```sh
curl "https://api.particle.io/v1/devices/<DEVICE_ID>/whoami?access_token=<YOUR_ACCESS_TOKEN>"
```

which yield a JSON response:

```json
{
  "cmd": "VarReturn",
  "name": "whoami",
  "error": null,
  "result": "0x43, FW ver.: v1.0, HW ver.: 35, Selftest: 00111111",
  "coreInfo": {
    "last_app": "",
    "last_heard": "2016-11-07T13:27:04.325Z",
    "connected": true,
    "last_handshake_at": "2016-11-07T13:18:22.461Z",
    "deviceID": "<YOUR_DEVICE_ID>",
    "product_id": 6
  }
}
```
