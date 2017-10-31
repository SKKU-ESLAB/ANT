# Notes
## Bluetooth Issues
### Bluetooth Version Compatibility
Check your bluetooth version using
```
$ sudo vi /etc/systemd/system/dbus-org.bluez.service

ExecStart=/usr/lib/bluetooth/bluetoothd --compat

$ sudo systemctl daemon-reload
$ sudo systemctl restart bluetooth
$ sudo chmod 777 /var/run/sdp
```

### Check the bluetooth HCI interface
```
$ hciconfig -a
```

### Change hci0 interface to Scan Mode 
Set page scan and inquiry can (Discoverable)
  ```
  $ sudo hciconfig hci0 piscan
  ```

### Change the hci0 interface name
  e.g., change the name of the bluetooth interface to 'pi'
  ```
  $ sudo hciconfig hci0 name pi
  ```

### Turn on the interface
  ```
  $ sudo hciconfig hci0 up
  ```

### Turn on the bluetoothd (For the case, it's off)
  ```
  sudo service bluetooth start
  ```
  When the bluetooth.target is masked, unmask it
  ```
  $ sudo systemctl unmask bluetooth
  ```
  Check the bluetoothd exists
  ```
  $ ps -ef | grep bluetooth
  ```
