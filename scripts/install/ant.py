#!/usr/bin/python3
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
# ant.py: ANT SystemServer
##########################################################################

import argparse, signal, os, time, sys


# Check environment variables
try:
    # Environment variables that is used in this script
    gAntBinDir = os.environ['ANT_BIN_DIR']
    gAntDataDir = os.environ['ANT_DATA_DIR']
    gAntDeleteSemPath = os.environ['ANT_DELETESEM_PATH']

    # Environment variables that will be used inside of ANT frameworks
    gAntTargetName = os.environ['ANT_TARGET_NAME']
    gAntWpaSupplicantPath = os.environ['ANT_WPA_SUPPLICANT_PATH']
    gAntWpaCliPath = os.environ['ANT_WPA_CLI_PATH']
    gAntConfigDir = os.environ['ANT_CONFIG_DIR']
    gAntDeviceDriverDir = os.environ['ANT_DEVICE_DRIVER_DIR']
    gAntAppsDir = os.environ['ANT_APPS_DIR']
except KeyError as e:
    print("Not found environment variable. {}".format(str(e)))
    exit()

def signal_handler(signum, frame):
    return

def run_command(command, baseDir=gAntBinDir):
    # Run command on this system server
    # Change directory
    os.chdir(baseDir)

    # Execute the daemon
    os.system(command)
    return

def run_on_daemon(command, name, uid=0, gid=0, baseDir=gAntBinDir):
    # Run a command on child daemon process
    pid = os.fork()
    if pid:
        log("{} process is executed (pid {})".format(name, pid))
    else:
        # Set uid & gid
        os.setuid(uid)
        os.setgid(uid)

        # Change directory
        os.chdir(baseDir)

        # Execute the daemon
        os.execv(command[0], command)
    return

def log(message):
    print("[SystemServer] {}".format(message))
    return

############################## EDIT HERE!!! ##############################

# on_did_initialize: (event handler) initialization process is completed
# initialization process includes setting options and signal handlers.
def on_did_initialize(args):
    log("Initializing ANT daemons...")

    # Execute prerequisites on system server
    run_command("mkdir -p " + gAntDataDir)
    run_command("hciconfig hci0 piscan")
    run_command(gAntDeleteSemPath)

    # Execute daemons
    if args.debugappcore:
        run_on_daemon(command=["/usr/bin/gdb", "./ant-appcore"],
                name="App-core Framework Daemon")
    elif args.debugapps:
        run_on_daemon(command=["./ant-appcore", "-d"],
                name="App-core Framework Daemon")
    else:
        run_on_daemon(command=["./ant-appcore"],
                name="App-core Framework Daemon")
    run_on_daemon(command=["./ant-camera"],
            name="Camera Framework Daemon")
    if args.debugsensor:
        # TODO: merging new sensor daemon is postponed.
        # run_on_daemon(command=["/usr/bin/gdb", "./ant-sensor"],
        #         name="Sensor Framework Daemon")
        run_on_daemon(command=["/usr/bin/gdb", "./ant-legacy-sensor"],
                name="Legacy Sensor Framework Daemon")
    else:
        # TODO: merging new sensor daemon is postponed.
        # run_on_daemon(command=["./ant-sensor"],
        #         name="Sensor Framework Daemon")
        run_on_daemon(command=["./ant-legacy-sensor"],
                name="Legacy Sensor Framework Daemon")
    if args.debugml:
        run_on_daemon(command=["/usr/bin/gdb", "./ant-ml"],
                name="Machine Learning Framework Daemon")
    else:
        run_on_daemon(command=["./ant-ml"],
                name="Machine Learning Framework Daemon")
    return

# on_did_halt_by_user: (event handler) user killed ANT manager manually
def on_did_halt_by_user():
    log("User key interrupt is detected.")
    log("Terminating ANT...")
    run_command("./ant_p2p_setup.sh stop")
    run_command("./ant_p2p_setup.sh deinit")
    return

# on_did_dead_daemon: (event handler) daemon process is dead
def on_did_dead_deamon(pid):
    log("An ANT process (pid {}) is dead.".format(pid))
    return
##########################################################################

def main():
    # Check python version
    if (sys.version_info[0] < 3):
        log("You are trying to use Python version {}.{}.{}"
            .format(sys.version_info[0], sys.version_info[1],
                    sys.version_info[2]))
        log("ANT requires Python 3.0+.")
        return

    # Check if this is root user 
    thisUid = os.getuid()
    if thisUid != 0:
        log("You(uid={}) are not root user. Please try it again on root."
            .format(thisUid))
        return

    # Parse option arguments
    parser = argparse.ArgumentParser(description="ANT options.")
    parser.add_argument("--debug-appcore", "-dac", dest="debugappcore",
            action="store_true",
            help="Debug Appcore Manager")
    parser.add_argument("--debug-apps", "-da", dest="debugapps",
            action="store_true",
            help="Debug Apps")
    parser.add_argument("--debug-ml", "-dm", dest="debugml",
            action="store_true",
            help="Debug ML Daemon")
    parser.add_argument("--debug-sensor", "-ds", dest="debugsensor",
            action="store_true",
            help="Debug Sensor Daemon")
    args = parser.parse_args()

    # Register signal handlers
    signal.signal(signal.SIGCHLD, signal_handler)
    signal.signal(signal.SIGINT, signal_handler)

    # Initialize Completed
    log("args: {}".format(args))
    on_did_initialize(args)

    # Wait for signals
    while True:
        siginfo = signal.sigwaitinfo({signal.SIGCHLD, signal.SIGINT})
        if siginfo.si_signo == signal.SIGCHLD:
            # Child daemon process is dead
            on_did_dead_deamon(siginfo.si_pid)
        elif siginfo.si_signo == signal.SIGINT:
            # Halt ANT triggered by user
            on_did_halt_by_user()
            return

if __name__ == "__main__":
    main()
