#!/usr/bin/env python


import os

from common_py.system.filesystem import FileSystem as fs
from common_py.system.executor import Executor as ex

DOCKER_ROOT_PATH = fs.join('/')

# ANT path in travis
TRAVIS_BUILD_PATH = fs.join(os.environ['TRAVIS_BUILD_DIR'])

# ANT path in docker
DOCKER_ANT_PATH = fs.join('/home/pi/ANT')

#ANT_BUILD_MODULE_PATH = fs.join(TRAVIS_BUILD_PATH, 'build.module')

DOCKER_NAME = 'ant_docker'


# Functions 
def run_docker():
    ex.check_run_cmd('docker', ['run', '-dit', '--name', DOCKER_NAME, '-v',
        '%s:%s' % (TRAVIS_BUILD_PATH, DOCKER_ANT_PATH),
        'sedden/rpi-raspbian-qemu:wheezy'])

def exec_docker(cwd, cmd):
    exec_cmd = 'cd %s && ' % cwd + ' '.join(cmd)
    ex.check_run_cmd('docker', ['exec','-it', DOCKER_NAME,
        'bash', '-c', exec_cmd])


# Main function
if __name__ == '__main__':
    
    # Execute docker
    run_docker()

    exec_docker(DOCKER_ANT_PATH, ['./tools/install-deps-rpi.sh'])
    exec_docker(DOCKER_ANT_PATH, ['mkdir build'])
    exec_docker(DOCKER_ANT_PATH, ['cd build'])
    exec_docker(DOCKER_ANT_PATH, ['cmake ..'])
    exec_docker(DOCKER_ANT_PATH, ['make'])


