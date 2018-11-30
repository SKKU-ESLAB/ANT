#!/bin/bash

sudo wpa_cli -i wlan0 p2p_group_remove p2p-wlan0-$1
sudo wpa_cli -i wlan1 p2p_group_remove wlan1
sudo ifconfig wlan0 down
sudo ifconfig wlan1 down
sudo killall udhcpd