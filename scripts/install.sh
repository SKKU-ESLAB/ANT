#!/bin/bash
#
##########################################################################
#
# Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
#
# Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##########################################################################
# install.sh: ANT Install Script
##########################################################################

## Available target boards
AVAILABLE_TARGET_LIST=("tegraTX1" "raspberry-pi2_3")

## Functions
# show_usage(): Show the usage of install script
#  - no arguments
show_usage() {
  echo \
"$0: ANT install script
usage: $0 --target=<TARGET_BOARD> [-h] [--help]

Available target boards(--target):"
  AVAILABLE_TARGET_STR=""
  for AVAILABLE_TARGET_ITEM in "${AVAILABLE_TARGET_LIST[@]}";
  do
    AVAILABLE_TARGET_STR="${AVAILABLE_TARGET_STR} ${AVAILABLE_TARGET_ITEM}"
  done
  echo "${AVAILABLE_TARGET_STR}"
}

## check_var(): Check if an environment variable is vaild
## - $1(ENV_NAME): (string) Environment variable's name to be determined
#check_var() {
#  ENV_NAME=$1
#  if [ ! ${!ENV_NAME} ]
#  then
#  fi
#}

## determine_var(): Determine an environment variable. If the environment
#                   variable's value has already determined, use that as it is.
#                   If not, user should input its value.
#  - $1(ENV_NAME): (string) Environment variable's name to be determined
#  - $2(ENV_DESC): (string) The environment variable's description
#  - $3(IS_DIRECTORY): (boolean) If the environment variable's value should be
#                                the path of directory, it is set as "y".
#                                If not, it is set as "n".
#                                If "y" and the directory path indicated by the
#                                value of the environment name does not exist,
#                                new directory can be created.
#  - $4(DEFAULT_VALUE): (string; optional) If the environment variable's value
#                                          does not exist, the given default
#                                          value is proposed to user.
determine_var() {
  ENV_NAME=$1
  ENV_DESC=$2
  IS_DIRECTORY=$3
  DEFAULT_VALUE=$4

  RESU_COLO="\033[33m"
  INIT_COLO="\033[0m"

  # If the environment variable's value is determined, use that as it is.
  if [ ${!ENV_NAME} ]
  then
    echo -e "${RESU_COLO}$ENV_NAME($ENV_DESC) = ${!ENV_NAME}${INIT_COLO}"

    if [ $IS_DIRECTORY = y ]
    then
      if [ -e ${!ENV_NAME} ]
      then
        return
      fi

      IS_CREATE_NEW=y

      # If the environment variable's value should be directory path,
      # user can create a new directory.
      while [ : ];
      do
        echo -n "Do you want to create a new directory? (y/n; default=y): "
        read IS_CREATE_NEW
        if [ -z $IS_CREATE_NEW ]
        then
          break;
        fi
        if [ $IS_CREATE_NEW = y ] || [ $IS_CREATE_NEW = n];
        then
          break;
        fi
      done
      if [ -z $IS_CREATE_NEW ] || [ $IS_CREATE_NEW = y ]
      then
        mkdir -p ${!ENV_NAME}
      fi
    fi
    return
  fi

  # If default value is given, transform it into absolute path
  if [ ${DEFAULT_VALUE} ]
  then
    eval DEFAULT_VALUE=`readlink --canonicalize ${DEFAULT_VALUE}`
  fi

  # If the value is not determined, user should its value.
  LOOP_CONTINUE=y
  while [ $LOOP_CONTINUE = y ]
  do
    # If default value is given, user can use it without any key input.
    if [ ${DEFAULT_VALUE} ]
    then
      echo -n "Enter $ENV_DESC(${ENV_NAME}; default=${DEFAULT_VALUE}): "
    else
      echo -n "Enter $ENV_DESC(${ENV_NAME}): "
    fi
    read INPUT_VALUE

    # If user did not gave any input, use default value.
    # If default value is not given, require input of user again.
    if [ -z ${INPUT_VALUE} ]
    then
      if [ ${DEFAULT_VALUE} ]
      then
        eval ${ENV_NAME}=${DEFAULT_VALUE}
      else
        continue
      fi
    else
      eval ${ENV_NAME}=${INPUT_VALUE}
    fi

    # Transform relative path into absolute path
    eval ${ENV_NAME}=`readlink --canonicalize ${!ENV_NAME}`

    # Check if the path indicated by environment variable exists
    if [ ${!ENV_NAME} ]
    then
      if [ ! -e ${!ENV_NAME} ]
      then
        echo "${!ENV_NAME} does not exist."

        if [ $IS_DIRECTORY = y ]
        then
          IS_CREATE_NEW=y

          # If the environment variable's value should be directory path,
          # user can create a new directory.
          while [ : ];
          do
            echo -n "Do you want to create a new directory? (y/n; default=y): "
            read IS_CREATE_NEW
            if [ -z $IS_CREATE_NEW ]
            then
              break;
            fi
            if [ $IS_CREATE_NEW = y ] || [ $IS_CREATE_NEW = n];
            then
              break;
            fi
          done
          if [ -z $IS_CREATE_NEW ] || [ $IS_CREATE_NEW = y ]
          then
            mkdir -p ${!ENV_NAME}
          fi
        fi
      else
        # Check if the environment variable indicates directory or regular file.
        if [ $IS_DIRECTORY = y ]
        then
          if [ ! -d ${!ENV_NAME} ]
          then
            echo "${!ENV_NAME} is not directory."
          else
            LOOP_CONTINUE=n
          fi
        else
          if [ ! -f ${!ENV_NAME} ]
          then
            echo "${!ENV_NAME} is not regular file."
          else
            LOOP_CONTINUE=n
          fi
        fi
      fi
    fi
  done

  echo -e "${RESU_COLO}$ENV_NAME($ENV_DESC) = ${!ENV_NAME}${INIT_COLO}"
}

# print_progress(): Print the progress
#  - $1(STEP_NUM): (int) Progress step's number
#  - $2(STEP_DESC): (string) Progress step's description
print_progress() {
  STEP_NUM=$1
  STEP_DESC=$2

  WARN_COLO="\033[31;47m"
  INFO_COLO="\033[36m"
  INIT_COLO="\033[0m"

  echo -e "${INFO_COLO}Step ${STEP_NUM}. ${STEP_DESC} ${INIT_COLO}"
}

## Main Code
## Step 0. Check permission and arguments

# Check permission
if ! [ $(id -u) = 0 ]; then
  echo "This script should run with root permission."
  exit 1
fi

# Test getopt
getopt --test > /dev/null
if [[ $? -ne 4 ]]; then
  echo "This system has failed to test getopt."
  exit 1
fi

# Variables set by parameters
ARG_HELP=n

# Parse arguments
SHORT=th
LONG=target:,help
PARSED=`getopt --options $SHORT --longoptions $LONG --name "$0" -- "$@"`
if [[ $? -ne 0 ]]; then
  exit 2
fi
eval set -- "$PARSED"
while true; do
  case "$1" in
    -t|--target)
      ARG_TARGET_BOARD="$2"
      shift 2
      ;;
    -h|--help)
      ARG_HELP=y
      shift
      ;;
    --)
      shift
      break
      ;;
    *)
      echo "Invalid arguments"
      exit 3
      ;;
  esac
done

# Print usage message
if [ $ARG_HELP = "y" ]
then
  show_usage
  exit 0
fi

# Check target board
VALID_TARGET=n
if [ -z $ARG_TARGET_BOARD ]
then
  echo "No target board specified."
  show_usage
  exit 3
fi
for AVAILABLE_TARGET_ITEM in "${AVAILABLE_TARGET_LIST[@]}";
do
  if [ $ARG_TARGET_BOARD = ${AVAILABLE_TARGET_ITEM} ]
  then
    echo "Target board $ARG_TARGET_BOARD is selected."
    TARGET_DIR=target/$ARG_TARGET_BOARD
    if [ ! -e $TARGET_DIR ]
    then
      echo "Target board directory does not exist: $TARGET_DIR"
      exit 3
    fi
    VALID_TARGET=y
    break
  fi
done
if [ ${VALID_TARGET} = "n" ]
then
  echo "Invalid target board: $ARG_TARGET_BOARD"
  exit 3
fi

## Step 1. Import target profile
print_progress 1 "Import target profile..."
TARGET_PROFILE_PATH=$TARGET_DIR/profile.env
if [ ! -e $TARGET_PROFILE_PATH ]
then
  echo "Target profile does not exist: $TARGET_PROFILE_PATH"
  exit 3
fi
source $TARGET_PROFILE_PATH

## Step 2. Determine environment variables to be used by install process
print_progress 2 "ANT Environment variable setting..."
# Determine environment variables
determine_var "ANT_OUT_DIR" "ANT output directory" y "./out"

# Runtime directory path
determine_var "ANT_BIN_DIR" "ANT binary directory" y
determine_var "ANT_CONFIG_DIR" "ANT config directory" y
determine_var "ANT_SENSOR_DRIVER_DIR" "ANT sensor driver directory" y
determine_var "ANT_APPS_DIR" "ANT sensor driver directory" y
determine_var "ANT_DATA_DIR" "ANT data directory" y

# Dependent binary path
determine_var "ANT_WPA_SUPPLICANT_PATH" \
              "wpa_supplicant's path to be used by ANT" n \
              `which wpa_supplicant`
determine_var "ANT_WPA_CLI_PATH" \
              "wpa_cli's path to be used by ANT" n \
              `which wpa_cli`
determine_var "ANT_DELETESEM_PATH" \
              "deletesem's path to be used by ANT" n \
              `which deletesem`

### Step 3. Check the contents of ANT_OUT_DIR
print_progress 3 "Check the contents of ANT out directory..."
check_ant_out_dir() {
  # TODO
  return
}
check_ant_out_dir

## Step 4. Install ANT Libraries
ANT_LIB_DIR=${ANT_BIN_DIR}/libs
if [ ! -e ${ANT_LIB_DIR} ]
then
  mkdir -p ${ANT_LIB_DIR}
fi
cp -R ${ANT_OUT_DIR}/libs/* ${ANT_LIB_DIR}/

## Step 4+. Install ANT
# Install ANT_BIN_DIR
print_progress 4 "Install ANT binaries..."
THIS_SCRIPT_DIR=$(dirname "$0")
cp -R ${ANT_OUT_DIR}/bin/* ${ANT_BIN_DIR}
cp ${THIS_SCRIPT_DIR}/install/ant.py ${ANT_BIN_DIR}
cp ${THIS_SCRIPT_DIR}/install/ant_p2p_setup.sh ${ANT_BIN_DIR}

# Make target profile
echo "#!/bin/bash
export ANT_TARGET_NAME=\"${ANT_TARGET_NAME}\";
export ANT_BIN_DIR=\"${ANT_BIN_DIR}\";
export ANT_CONFIG_DIR=\"${ANT_CONFIG_DIR}\";
export ANT_SENSOR_DRIVER_DIR=\"${ANT_SENSOR_DRIVER_DIR}\";
export ANT_APPS_DIR=\"${ANT_APPS_DIR}\";
export ANT_DATA_DIR=\"${ANT_DATA_DIR}\";
export ANT_WPA_SUPPLICANT_PATH=\"${ANT_WPA_SUPPLICANT_PATH}\";
export ANT_WPA_CLI_PATH=\"${ANT_WPA_CLI_PATH}\";
export ANT_DELETESEM_PATH=\"${ANT_DELETESEM_PATH}\";
export ANT_WFD_NAME=\"${ANT_WFD_NAME}\";" \
  > ${ANT_CONFIG_DIR}/profile.env

# Make ANT launcher
cat ${THIS_SCRIPT_DIR}/install/run_ant.head > ${ANT_BIN_DIR}/run_ant
echo "# ANT config directory
export ANT_CONFIG_DIR=\"${ANT_CONFIG_DIR}\";" >> ${ANT_BIN_DIR}/run_ant
cat ${THIS_SCRIPT_DIR}/install/run_ant.tail >> ${ANT_BIN_DIR}/run_ant

chmod +x ${ANT_BIN_DIR}/*
rm ${ANT_SYSTEM_BIN_DIR}/run_ant
ln -s ${ANT_BIN_DIR}/run_ant ${ANT_SYSTEM_BIN_DIR}/run_ant

# Install ANT_CONFIG_DIR
print_progress 5 "Install ANT configs..."
cp -R ${ANT_OUT_DIR}/config/* ${ANT_CONFIG_DIR}/
cp ${TARGET_DIR}/camera-config.json ${ANT_CONFIG_DIR}/
cp ${TARGET_DIR}/dhcpd.conf ${ANT_CONFIG_DIR}/
cp ${TARGET_DIR}/p2p.conf ${ANT_CONFIG_DIR}/
cp ${TARGET_DIR}/haarcascade_frontalface_alt.xml ${ANT_CONFIG_DIR}/

# Install ANT_SENSOR_DRIVER_DIR
print_progress 6 "Install ANT sensor drivers..."
cp -R ${ANT_OUT_DIR}/sensor-drivers/* ${ANT_SENSOR_DRIVER_DIR}/

# Install ANT_APPS_DIR
print_progress 7 "Install ANT system applications..."
mkdir -p ${ANT_APPS_DIR}/system
mkdir -p ${ANT_APPS_DIR}/user
cp -R ${ANT_OUT_DIR}/system-apps/* ${ANT_APPS_DIR}/system/

WARN_COLO="\033[31;47m"
INFO_COLO="\033[36m"
INIT_COLO="\033[0m"

echo -e "${WARN_COLO}ANT install completed${INIT_COLO}"
echo -e "Input ${INFO_COLO}run_ant${INIT_COLO} to run ANT."
