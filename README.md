# Particle-Pozyx

A wrapper library for building [Pozyx](https://www.pozyx.io/) apps for
the [Particle Photon](https://www.particle.io/products/hardware/photon-wifi-dev-kit).

This library is a wrapper around the PozyxLabs' 
[Pozyx-Arduino-library](https://github.com/pozyxLabs/Pozyx-Arduino-library),
albeit slightly modified to handle the specifics of the Particle platforms.
This example enabling the building of apps for the Particle Photons that communicate
with the Pozyx indoor tracking shield.

See the [Pozyx official documentation](https://www.pozyx.io/Documentation)
for implementation details regarding the examples included. 

The code and solutions herein are tested with a 
[Particle Photon with firmware 0.5.3](https://docs.particle.io/datasheets/photon-datasheet/),
 a [Shield Shield](https://docs.particle.io/datasheets/particle-shields/#shield-shield)
 and a [Pozyx shield v1.4](https://www.pozyx.io/store/detail/4).

## Usage

### Use from Particle Build Web IDE

- [Particle Build](https://build.particle.io/)

Create a new app, include the `Pozyx` library from list
of Community Libraries and start writing you app.

### Use from terminal and local IDE

First, create folder, e.g. `myapp`, for your app in the `apps` folder 
of this project. In it, create a file called `particle.include`
with the following content:

```
myapp.ino
../../firmware/Pozyx.h
../../firmware/Pozyx_definitions.h
../../firmware/Pozyx_core.cpp
../../firmware/Pozyx_lib.cpp
```

Create the `myapp.ino` file and write your application using the editor
of your choice.

Install the [particle-cli](https://github.com/spark/particle-cli), login
and register your Photon.

To compile your app for the Photon system, run
 
```sh
./particle-pozyx.sh compile photon myapp 
```

and to flash your system (remotely) 

```sh
./particle-pozyx.sh flash DEVICE_ID myapp 
```
