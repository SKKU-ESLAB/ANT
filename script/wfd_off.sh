#!/bin/bash
if [ "$EUID" -ne 0 ]
then echo "$0: it needs privilege permission."
  exit
fi

wpa_cli -i wlan0 p2p_group_remove p2p-wlan0-$1
