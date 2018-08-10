#!/bin/bash

sudo wpa_cli -i wlan0 p2p_group_remove p2p-wlan0-$1
sudo ifconfig wlan0 down
