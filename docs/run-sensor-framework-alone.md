# Run Sensor Framework Alone
This is a manual to explain how to run sensor framework alone.

## ANT Path
After install ANT framework successfully, ANT files are in the /etc/ant/ dir.

## Sensor Framework
The binary of sensor framework is in /etc/ant/bin/ and named "ant-sensor"
you can run with sudo.

```
$ sudo ./ant-sensor
```

### Environent Variable
In sensor framework, it uses environment variable ${ANT_SENSOR_DRIVER_DIR}.
You need to specify the path of sensor driver (where libsensor.so exists).

You can set the path with export in bash
(The sensor drivers are usually installed in /etc/ant/sensor-drivers/)

```
$ export ANT_SENSOR_DRIVER_DIR=/etc/ant/sensor-drivers
```

You can run with the new environment variables
```
$ sudo -E /etc/ant/bin/ant-sensor
```

