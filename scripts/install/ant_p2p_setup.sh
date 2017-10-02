#!/bin/bash

WFD_DIR=/tmp/wifi/wifi-direct/
INIT_PATH=/tmp/wifi/wifi-direct/init
WFD_STAT_PATH=/tmp/wifi/wifi-direct/wfd_stat
DHCP_STAT_PATH=/tmp/wifi/wifi-direct/dhcp_stat
IFACE_PATH=/tmp/wifi/wifi-direct/iface
DEV_ADDR_PATH=/tmp/wifi/wifi-direct/dev_addr

P2P_CONF_PATH="${ANT_CONFIG_DIR}/p2p.conf"
DHCP_CONF_PATH="${ANT_CONFIG_DIR}/dhcpd.conf"

init_wf()
{
	if [ ! -e "${INIT_PATH}/self" ]
	then
		sudo pkill -x wpa_supplicant
	fi

	IS_INIT=`cat ${INIT_PATH}/self | awk '{print $1}'`
	echo $IS_INIT
	if [ "${IS_INIT}" = "1" ];
	then
		exit 1
	fi
		
  sudo rfkill unblock all
	sudo mkdir -p ${WFD_DIR}
	sudo mkdir -p ${INIT_PATH}
	sudo mkdir -p ${WFD_STAT_PATH}
	sudo mkdir -p ${DHCP_STAT_PATH}
	sudo mkdir -p ${IFACE_PATH}
	sudo mkdir -p ${DEV_ADDR_PATH}
	
	sudo touch ${INIT_PATH}/self
	sudo chmod 666 ${INIT_PATH}/self
	sudo echo 0 > ${INIT_PATH}/self
	sudo touch ${WFD_STAT_PATH}/self
	sudo chmod 666 ${WFD_STAT_PATH}/self
	sudo touch ${IFACE_PATH}/self
	sudo chmod 666 ${IFACE_PATH}/self
	sudo touch ${DHCP_STAT_PATH}/self
	sudo chmod 666 ${DHCP_STAT_PATH}/self
	sudo touch ${DEV_ADDR_PATH}/self
	sudo chmod 666 ${DEV_ADDR_PATH}/self

	sudo echo 0 > ${WFD_STAT_PATH}/self
	sudo echo 0 > ${DHCP_STAT_PATH}/self
	sudo echo 0 > ${INIT_PATH}/self

	sudo chown -R $USER:$USER /tmp/wifi
	sudo ifconfig wlan0 up
	sudo ${ANT_WPA_SUPPLICANT_PATH} -Dnl80211 -iwlan0 -c${P2P_CONF_PATH} -Bd
	sudo ${ANT_WPA_CLI_PATH} p2p_group_add persistent=0
	P2P_IFNAME=`ifconfig | awk '/p2p/ {print $1}'`
	if [ -z ${P2P_IFNAME} ]
	then
	echo "Failed to turn wfd on"
	sudo echo 0 > ${INIT_PATH}/self
	exit 1
	fi

	echo ${P2P_IFNAME} > ${IFACE_PATH}/self
	sudo ip addr add 192.168.49.1/24 brd 192.168.49.255 dev ${P2P_IFNAME}
	sudo ifconfig ${P2P_IFNAME} 192.168.49.1 up
	DEV_ADDR=`cat ${DEV_ADDR_PATH}/self`
	
	sudo ${ANT_WPA_CLI_PATH} wps_pin any 12345670
	sudo udhcpd ${DHCP_CONF_PATH} -f &
	echo 1 > ${INIT_PATH}/self

	echo Started
}
deinit_wfd()
{
	IS_INIT=`cat ${INIT_PATH}/self | awk '{print $1}'`
	if [ -z ${IS_INIT} ] || [ ${IS_INIT} -eq 0 ] 
	then
	echo "WFD is not initialized yet"
	exit 1
	fi
	P2P_IFNAME=`ifconfig | awk '/p2p/ {print $1}'`
	sudo ${ANT_WPA_CLI_PATH} p2p_flush
	sudo ${ANT_WPA_CLI_PATH} p2p_group_remove ${P2P_IFNAME}
	sudo pkill -x udhcpd
	sudo echo 0 > ${DHCP_STAT_PATH}/self
	sudo rm /var/lib/dhcpcd5/dhcpcd-p2p-wlan0-* -f

	sudo pkill -x wpa_supplicant
	sudo ifconfig wlan0 down
	sudo echo 0 > ${WFD_STAT_PATH}/self
	sudo echo 0 > ${INIT_PATH}/self
}

start_wfd()
{
	IS_INIT=`cat ${INIT_PATH}/self | awk '{print $1}'`
	if [ -z ${IS_INIT} ] || [ ${IS_INIT} -eq 0 ]
	then
	exit 1
	fi
	IS_START=`sudo ${ANT_WPA_CLI_PATH} wps_pin any 12345670 | grep 12345670`
	if [ -z ${IS_START} ]
	then
	echo "Failed to start WFD"
	fi
	
	sudo echo 1 > ${WFD_STAT_PATH}/self

	echo started
}

stop_wfd()
{
	sudo ${ANT_WPA_CLI_PATH} p2p_flush
	
	sudo echo 0 > ${WFD_STAT_PATH}/self
	echo stopped
}

# Check required environment variables
CHECK_ENV_LIST=("ANT_CONFIG_DIR" "ANT_WPA_SUPPLICANT_PATH"
                "ANT_WPA_CLI_PATH")
for CHECK_ENV_ITEM in "${CHECK_ENV_LIST[@]}";
do
  if [ -z ${!CHECK_ENV_ITEM} ]
  then
    echo "$0: ${CHECK_ENV_ITEM} is unset. Set it first."
    exit 1
  fi
done

case $1 in
"start")
start_wfd
;;
"stop")
stop_wfd
;;
"init")
init_wf
;;
"deinit")
deinit_wfd
;;
*)
/bin/echo $0 [init/start/stop]
exit 1
;;
esac
