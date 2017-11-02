[33mcommit 9ba0dd2ed737283ce2185966ebde12aa6e2ac39a[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Thu Nov 2 07:26:17 2017 +0000

    Bug fix: sensor driver code

[33mcommit 6364930de23b613275f51939a56d2c1c94cdbe20[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Wed Nov 1 13:47:01 2017 +0000

    Runtime dynamic library linking bug fix

[33mcommit 4e7b32bd34ee37de05a611efa860e643d3929d28[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Wed Nov 1 13:06:00 2017 +0000

    Bug fix in MLDaemon which was from merging

[33mcommit 30c46b9017f3ed1b6d4fc38ac0bf96da02cd6f10[m
Merge: d528b70 5b887ce
Author: Injung Hwang <sinban04@gmail.com>
Date:   Wed Nov 1 07:19:38 2017 +0000

    Merge branch 'developing'

[33mcommit 5b887ceff27ef64b555eb200bd4ceaf61ab33992[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Wed Nov 1 07:08:38 2017 +0000

    Make the relative path dependent on  variable (#32)

[33mcommit 83e85b0a8bd1e44264c47125b188e2040357c837[m
Author: Jongwon Park <jong1prk@gmail.com>
Date:   Tue Oct 31 12:20:35 2017 +0000

    accelerometer network update
    
    aaa

[33mcommit 735d111773bf7bfd4dfc98fdca0ae73b7e5a4203[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Oct 12 07:47:03 2017 +0000

    Implement test system of machine learning framework and system apps, bug fixes
    * Major changes on machine learning framework
      * Test code of machine learning framework
      * System server loads machine learning daemon
      * Add automatic update on system app list database
      * Add prototype of ImageClassifier & MotionClassifier system apps
    * Major changes on API
      * Add prototype of machine learning API
      * Add mlApi.runModel() (simple machine learning API for built-in models)
      * Combine app API and machine learning API into new API (Issue #22)
      * Modularize JS Async callback as a C++ class (but not clear still)
    * Code readability improvements
      * Detach internal functions from MLDaemon's message handling functions
    * Bug fixes
      * Add missing senderUri on message handling layer
      * Add automatic update on system app list database
      * Initialize pthread-related member attributes at declaration phase
      * Fix dangling pointer hazard of MLTensorLayout

[33mcommit ab4af8d93443cd91d19922d2b2194bedf6b980a1[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Tue Oct 31 13:22:02 2017 +0000

    Complete to implementation of MLAPI.runModel()

[33mcommit da0e77aa318af69ac3b093a3a52cf5f66e4face1[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Oct 12 07:47:03 2017 +0000

    Implement test system of machine learning framework and system apps, bug fixes
    * Major changes on machine learning framework
      * Test code of machine learning framework
      * System server loads machine learning daemon
      * Add automatic update on system app list database
      * Add prototype of ImageClassifier & MotionClassifier system apps
    * Major changes on API
      * Add mlApi.runModel() (API for built-in models, not fully implemented)
      * Combine app API and machine learning API into new API (Issue #22)
      * Modularize JS Async callback as a C++ class (but not clear still)
    * Code readability improvements
      * Detach internal functions from MLDaemon's message handling functions
    * Bug fixes
      * Add missing senderUri on message handling layer
      * Add automatic update on system app list database
      * Initialize pthread-related member attributes at declaration phase
      * Fix dangling pointer hazard of MLTensorLayout

[33mcommit f44f75b86e07235e7294a21940b0cd9769e509f9[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Oct 12 01:55:41 2017 +0000

    Complete command handling in MLDaemon and InferenceUnitDirectory

[33mcommit c1f219c31093fcfe087d971942d5055c5fb4a12a[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Oct 11 12:03:51 2017 +0000

    Apply MLMessage into MLDaemon

[33mcommit af80cb93380ad38e8dc767eb94ec48cd6460f184[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Oct 11 07:30:42 2017 +0000

    Implement MLMessage and JSON-encoding code of MLDataUnit/MLTensor

[33mcommit 51afb2d936b2d4f574dd7b488f4a6c2743db9914[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Oct 11 05:34:54 2017 +0000

    Remove InferenceUnitParams and replace it with MLDataUnit

[33mcommit 89d2e0eb5f603f3aa572d33e7c1f6a23ca496e9e[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Wed Nov 1 07:09:48 2017 +0000

    Introduce MLDataUnit/MLTensor and integrate them to InferenceUnit

[33mcommit f7f34f3fca67f1c9ce41f961641fa39416c83688[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Oct 9 02:58:01 2017 +0000

    Rename ML command's name and parameters
    * Rename model id to inference unit id(iuid)
    * Rename ML command's name to clarify that the command handles each inference unit

[33mcommit ceca7ca15bd1791ba64e5a88cd48445f738b5a29[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 28 12:57:36 2017 +0000

    Skeleton code for DNN/ANN inference runner are added.

[33mcommit 326546586e06d1d15844343056cefcec919fb0db[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 28 12:44:00 2017 +0000

    Add mutex locks to InferenceUnit of Machine Learning FW

[33mcommit cf5adf6ffc51bd69e2a634f88ff3a9edaaa347de[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 28 07:05:06 2017 +0000

    Prototype of Inference Unit for Machine Learning Framework is added.

[33mcommit 2aa5d359561fb79092dd0d76292bdb74bc94890c[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 21 13:28:44 2017 +0000

    Initial prototype of machine learning daemon message

[33mcommit 11c5a52e3f17a92b011f02d043713dc766375586[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 21 13:01:27 2017 +0000

    Initial prototype of machine learning daemon

[33mcommit 5cf24adc30a348dfc9f04f691619b0e3fee811a2[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 12:44:30 2017 +0000

    CMake Directory Path Correction

[33mcommit 3cbe486b45dbd682e5bfdd654dbf957e99fef9de[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 12:20:13 2017 +0000

    Add docs/ dir with bluetooth.md

[33mcommit db708aa348f061e222d65e3a73a13f09b42f78a4[m
Author: InJung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 21:11:58 2017 +0900

    Update README.md
    
    Add some comment

[33mcommit 33e0fa0c03a0e86dd74512ecd87f19fe2fcb4839[m
Author: InJung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 21:08:51 2017 +0900

    Update README.md
    
    Add how-to-run section and FYI section.

[33mcommit 54c4237afd1973e1ca1f68d9dfc6d0cf126a4556[m
Author: InJung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 20:32:35 2017 +0900

    Update README.md
    
    Modify some build processes and directory names

[33mcommit d528b70977fc2082cb23aa498ec21ad148fbdf31[m
Merge: 8a01791 c5850dc
Author: Injung Hwang <sinban04@gmail.com>
Date:   Wed Nov 1 02:20:59 2017 +0000

    Merge branch 'machine-learning'

[33mcommit c5850dcc970a8f8672d94b9bc28490d6ae29a423[m
Merge: caa20b2 fa1726a
Author: Hayun Lee <lhy920806@gmail.com>
Date:   Tue Oct 31 22:40:44 2017 +0900

    Merge pull request #36 from RedCarrottt/machine-learning
    
    Complete to implementation of MLAPI.runModel()

[33mcommit fa1726ac8e7a07ca2a30edfc280ebcd36c740433[m
Merge: 52ce2e9 caa20b2
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Tue Oct 31 13:33:15 2017 +0000

    Merge branch upstream/machine-learning

[33mcommit 52ce2e930f4bd999f19a499d8713d09c15593ee6[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Tue Oct 31 13:22:02 2017 +0000

    Complete to implementation of MLAPI.runModel()

[33mcommit 8a01791d8b8a25025814619d660b2705526a1cc7[m
Merge: 292a665 9ac1b8d
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Tue Oct 31 22:10:45 2017 +0900

    Merge pull request #35 from sinban04/master
    
    CMakeList directory issue & New docs/ directory

[33mcommit caa20b28fa4f110acaa6a24c6d3ec97e6bfeedcd[m
Merge: f1d5007 a8230cd
Author: Hayun Lee <lhy920806@gmail.com>
Date:   Tue Oct 31 22:10:44 2017 +0900

    Merge pull request #34 from jong1prk/machine-learning
    
    Accelerometer Network Update

[33mcommit 9ac1b8d03d11735a6e248be3374d535156600021[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 12:44:30 2017 +0000

    CMake Directory Path Correction

[33mcommit a8230cd642275710c534995ac1b33903c74e1a86[m
Author: Jongwon Park <jong1prk@gmail.com>
Date:   Tue Oct 31 12:20:35 2017 +0000

    accelerometer network update

[33mcommit 22afa9cebea8e00a0b5dd964331076af4a87c867[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 12:20:13 2017 +0000

    Add docs/ dir with bluetooth.md

[33mcommit 292a6656ea70a4a763da9613f864ad3d7fee142f[m
Author: InJung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 21:11:58 2017 +0900

    Update README.md
    
    Add some comment

[33mcommit 6552940782fab1d53b416b28ca9bf9f4bbd3331c[m
Author: InJung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 21:08:51 2017 +0900

    Update README.md
    
    Add how-to-run section and FYI section.

[33mcommit d3c046f4c370634a9e4f8d56fb063665aa43ee8f[m
Author: InJung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 20:32:35 2017 +0900

    Update README.md
    
    Modify some build processes and directory names

[33mcommit f1d50075525bfdea554c15c02518cda659620215[m
Merge: 7f1f925 d89d5c7
Author: InJung Hwang <sinban04@gmail.com>
Date:   Tue Oct 31 16:53:44 2017 +0900

    Merge pull request #30 from RedCarrottt/machine-learning
    
    Implement test system of machine learning framework and system apps, bug fixes

[33mcommit fca1ac7dbd720a1179b5f0586d8824c3d0b337da[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Oct 12 07:47:03 2017 +0000

    Implement test system of machine learning framework and system apps, bug fixes
    * Major changes on machine learning framework
      * Test code of machine learning framework
      * System server loads machine learning daemon
      * Add automatic update on system app list database
      * Add prototype of ImageClassifier & MotionClassifier system apps
    * Major changes on API
      * Add mlApi.runModel() (API for built-in models, not fully implemented)
      * Combine app API and machine learning API into new API (Issue #22)
      * Modularize JS Async callback as a C++ class (but not clear still)
    * Code readability improvements
      * Detach internal functions from MLDaemon's message handling functions
    * Bug fixes
      * Add missing senderUri on message handling layer
      * Add automatic update on system app list database
      * Initialize pthread-related member attributes at declaration phase
      * Fix dangling pointer hazard of MLTensorLayout

[33mcommit d89d5c7585d5ba5a411ce1cc7c11f648baca8fba[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Oct 12 07:47:03 2017 +0000

    Implement test system of machine learning framework and system apps, bug fixes
    * Major changes on machine learning framework
      * Test code of machine learning framework
      * System server loads machine learning daemon
      * Add automatic update on system app list database
      * Add prototype of ImageClassifier & MotionClassifier system apps
    * Major changes on API
      * Add prototype of machine learning API
      * Add mlApi.runModel() (simple machine learning API for built-in models)
      * Combine app API and machine learning API into new API (Issue #22)
      * Modularize JS Async callback as a C++ class (but not clear still)
    * Code readability improvements
      * Detach internal functions from MLDaemon's message handling functions
    * Bug fixes
      * Add missing senderUri on message handling layer
      * Add automatic update on system app list database
      * Initialize pthread-related member attributes at declaration phase
      * Fix dangling pointer hazard of MLTensorLayout

[33mcommit a3ee300b2fc58f55a7a199f6a96431a311679718[m
Author: Injung <sinban04@gmail.com>
Date:   Tue Oct 31 16:24:01 2017 +0900

    travis script - modify ANT_DIR

[33mcommit 4853601839ce900be0ba6b5b3a09258681248a29[m
Merge: 3f4c50f a305a2f
Author: Jongwon Park <jong1prk@users.noreply.github.com>
Date:   Mon Oct 30 16:30:20 2017 +0900

    Merge pull request #26 from LeeHayun/master
    
    Minimizing install-deps-*.sh execution time

[33mcommit a305a2f5e08118375bb853f48d9e25681c8bb6a7[m
Author: Hayun Lee <lhy920806@gmail.com>
Date:   Sat Oct 28 07:52:30 2017 +0000

    Minimize the install-deps-*.sh execution time
    
    * Add Apache LICENSE for Camera Framework
    * Remove face detection API (temporary)
    * Fix raspberry pi camera-config.json
    * Remove unused GStreamer packages
    * Utilize multi-core (in prerequisite installation time)

[33mcommit 3f4c50f6372a58db190bd24902db541265df30a1[m
Merge: 3d55272 84dccd5
Author: Jongwon Park <jong1prk@users.noreply.github.com>
Date:   Tue Oct 17 15:47:30 2017 +0900

    Merge pull request #24 from sinban04/master
    
    Complete Travis CI test system

[33mcommit 84dccd52777b3ed14c3df0c4b316b1b90dfde2ca[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Tue Oct 17 04:03:20 2017 +0000

    Update License file about Samsung/iotjs

[33mcommit fc71f8e56416ccaed1edc0c80fd548fea8892913[m
Merge: 3d55272 9582187
Author: Injung Hwang <sinban04@gmail.com>
Date:   Mon Oct 16 08:47:00 2017 +0000

    Merge branch 'developing'
    
    Conflicts:
            .travis.yml

[33mcommit 9582187e2b40051c08fc2b8b901a680c07d0f3d8[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Mon Oct 16 01:50:48 2017 +0000

    Add travis script
    
    add common_py code from Samsung/iotjs

[33mcommit 1f432d0336f8e4a4e3cc42de4788d30a5dbff6a9[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Sat Oct 14 02:20:18 2017 +0000

    update the install-deps script for rpi

[33mcommit a6488f8e844d9e96d491a4f849d4626894d2d409[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Sat Oct 14 01:58:56 2017 +0000

    fix dir problem

[33mcommit 9499aebd7d9f2f39806b6c1a79cd47ba079c1704[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Sat Oct 14 01:36:53 2017 +0000

    install deps and build test on docker from Travis

[33mcommit c1b92504d9e6e98d7448a26f1a49aed3a0ea9260[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Fri Oct 13 11:52:28 2017 +0000

    Travis CI test environment based on Docker for raspbian

[33mcommit e06f72380d1dcea1d29bc68635b80a6e1a2306d6[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Oct 2 06:29:59 2017 +0000

    Fix mistypes on ant_p2p_setup.sh and update .gitignore of bluez-4.101

[33mcommit 06f2879d2923400036348e969c1a96265c80bc4e[m
Author: Hayun Lee <lhy920806@gmail.com>
Date:   Sat Sep 30 11:09:33 2017 +0000

    Add RPI camera-config.json

[33mcommit 3d5527224ea955a2cf8227e2813ab86d94a6062b[m
Merge: a53a8ba 831eb12
Author: Jongwon Park <jong1prk@users.noreply.github.com>
Date:   Sat Oct 14 12:13:47 2017 +0900

    Merge pull request #23 from sinban04/master
    
    Travis CI test environment based on Docker for raspbian

[33mcommit 831eb125ea7cea4bbc37cd4800a0bea25704e673[m
Author: Injung Hwang <sinban04@gmail.com>
Date:   Fri Oct 13 11:52:28 2017 +0000

    Travis CI test environment based on Docker for raspbian

[33mcommit 2debde4ddf35c91a8a43c21a1d1524e9b7f479c7[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Oct 2 06:29:59 2017 +0000

    Fix mistypes on ant_p2p_setup.sh and update .gitignore of bluez-4.101

[33mcommit 7f1f9258a8812abe64c1ca302eae8400740c7dcf[m
Merge: 9c3f2b9 6a06714
Author: Jongwon Park <jong1prk@users.noreply.github.com>
Date:   Fri Oct 13 21:16:57 2017 +0900

    Merge pull request #21 from RedCarrottt/machine-learning2
    
    Complete MLDaemon and MLMessage

[33mcommit 6a0671490e34f7360abb5f93a02117ee1c74c29a[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Oct 12 01:55:41 2017 +0000

    Complete command handling in MLDaemon and InferenceUnitDirectory

[33mcommit bcb3195d2173d75ce66e7071d46a6efc545ffd1a[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Oct 11 12:03:51 2017 +0000

    Apply MLMessage into MLDaemon

[33mcommit 14fb3b30be96d7a488d6b11252986ee311039768[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Oct 11 07:30:42 2017 +0000

    Implement MLMessage and JSON-encoding code of MLDataUnit/MLTensor

[33mcommit 9c3f2b9e80104dce8a74b2a6208174a2620705ca[m
Merge: 178679e 0c882a2
Author: InJung Hwang <sinban04@gmail.com>
Date:   Wed Oct 11 19:46:02 2017 +0900

    Merge pull request #20 from RedCarrottt/machine-learning
    
    Update Machine Learning Framework Prototype

[33mcommit 0c882a2b5123e6484b66357d4a169939b1509038[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Oct 11 05:34:54 2017 +0000

    Remove InferenceUnitParams and replace it with MLDataUnit

[33mcommit bb3e3cd0844f579fdfd6b53b27f810cc1da6e658[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Oct 9 13:25:57 2017 +0000

    Introduce MLDataUnit/MLTensor and integrate them to InferenceUnit

[33mcommit 5d5747933d4d7cb0d05faa9e9263d4eb8dad4e37[m
Merge: 07ac9d4 a53a8ba
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Oct 9 02:58:22 2017 +0000

    Merge remote-tracking branch 'upstream/master' into machine-learning

[33mcommit 07ac9d44429233c36fe8b56f9fa75f424e9832e5[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Oct 9 02:58:01 2017 +0000

    Rename ML command's name and parameters
    * Rename model id to inference unit id(iuid)
    * Rename ML command's name to clarify that the command handles each inference unit

[33mcommit a53a8ba67fd53ad64916cdf46d50eb5c0bf025d3[m
Merge: 679080f daa6529
Author: InJung Hwang <sinban04@gmail.com>
Date:   Mon Oct 2 21:45:39 2017 +0900

    Merge pull request #17 from RedCarrottt/master
    
    Fix grammar error on ant_p2p_setup.sh and update .gitignore of bluez-4.101

[33mcommit daa65290cc8a2380ac2dfb617aad49898a144409[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Oct 2 06:29:59 2017 +0000

    Fix mistypes on ant_p2p_setup.sh and update .gitignore of bluez-4.101

[33mcommit 679080fb05fab11abbd7d5e6dac310427a3b16da[m
Merge: bddeb32 d10729d
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Sat Sep 30 20:32:05 2017 +0900

    Merge pull request #15 from LeeHayun/master
    
    Add RPI camera-config.json

[33mcommit d10729d5135aae48c8c3dfaaae12d272946cef2c[m
Author: Hayun Lee <lhy920806@gmail.com>
Date:   Sat Sep 30 11:09:33 2017 +0000

    Add RPI camera-config.json

[33mcommit bddeb3225aa1418bbd28f7cc493ad69c2a64d5ca[m
Merge: b8c0112 eb5d5a8
Author: Hayun Lee <lhy920806@gmail.com>
Date:   Sat Sep 30 18:13:43 2017 +0900

    Merge pull request #13 from RedCarrottt/master
    
    Fix bugs on ant_p2p_setup.sh and install-deps-*.sh

[33mcommit eb5d5a80ee88b5ee3d50c246dbb95b8014556c5a[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Sat Sep 30 07:21:31 2017 +0000

    Fix bugs on ant_p2p_setup.sh and install-deps*.sh
    * Fix a crash when running 'ant_p2p_setup.sh init'
    * Add bison and yacc on install-deps*.sh that are required at building bluz-4.101

[33mcommit b8c01122ecd31e2e51df9deb62b43f735871d1dc[m
Merge: 084719e 0c75801
Author: Hayun Lee <lhy920806@gmail.com>
Date:   Sat Sep 30 13:48:07 2017 +0900

    Merge pull request #11 from RedCarrottt/master
    
    Update dependent packages

[33mcommit 0c75801d3921295dc00bb37591b4c4f673634a9c[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Sat Sep 30 04:21:13 2017 +0000

    Update dependent packages
    * Add .gitignore to ant-bluez-4.101, ant-deletesem, ant-opencv3
    * Fix omitted 'sudo' in install-deps-raspberry-pi2_3.sh

[33mcommit 178679eedbe13584ddd1089a5f0d45d9526e6556[m
Merge: a6fa834 e9d8818
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Thu Sep 28 21:59:29 2017 +0900

    Merge pull request #9 from RedCarrottt/machine-learning
    
    Supplement Skeleton Code of Machine Learning FW

[33mcommit e9d881815c1de7ae818dcfb4032f056180405eee[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 28 12:57:36 2017 +0000

    Skeleton code for DNN/ANN inference runner are added.

[33mcommit 2dd4283a92efa80cd23b4aca4ccba2a9e637f101[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 28 12:44:00 2017 +0000

    Add mutex locks to InferenceUnit of Machine Learning FW

[33mcommit 084719e480754985dc31f66a6b2a273ef17c699c[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 28 11:17:30 2017 +0000

    Rollback gh-pages config

[33mcommit 62d05037772d3b6a65659bbba5e9443919e4dd1f[m
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Thu Sep 28 20:12:21 2017 +0900

    Set theme jekyll-theme-tactile

[33mcommit a6fa83459e963b580f93d94caa8453c73193e1e0[m
Merge: 946f4d6 5c85f36
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Thu Sep 28 16:21:39 2017 +0900

    Merge pull request #8 from RedCarrottt/machine-learning
    
    Prototype of Inference Unit for Machine Learning Framework is added.

[33mcommit 5c85f3616384b2e62f540687c49e4e92fd656c07[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 28 07:05:06 2017 +0000

    Prototype of Inference Unit for Machine Learning Framework is added.

[33mcommit 45142391a0b75df1fe9a45e98dfb8c822de5ba4a[m
Merge: 946f4d6 86b98f0
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Sep 27 12:47:59 2017 +0000

    Merge remote-tracking branch 'origin/master' into machine-learning

[33mcommit 86b98f034902ae31e74286c5375b4a38b2dc3f82[m
Merge: 300f532 b1eb6e7
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Tue Sep 26 11:49:46 2017 +0900

    Merge pull request #5 from RedCarrottt/master
    
    Bug fix on install-deps-raspberry-pi2_3.sh

[33mcommit b1eb6e7046c4e8bc25c91d3a8dbe298bbbe90bc4[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Tue Sep 26 01:50:24 2017 +0000

    Bug fix on install-deps-raspberry-pi2_3.sh
    * Mistypo at a command copying ant-dbus-config to dbus config directory

[33mcommit 300f532c21cb3ec8b2b9aa46b673005c023b2e1c[m
Merge: 0dd4e82 016479d
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Sep 25 07:31:53 2017 +0000

    Merge remote-tracking branch 'origin/master'

[33mcommit 0dd4e8231ebf8037b0d80381a0935c0d7f65f36f[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Mon Sep 25 07:31:21 2017 +0000

    Use nodejs 4.0 instead of nodejs 9.0

[33mcommit 016479d5116d05d75b997af065dae910726c0a29[m
Merge: f600ac4 33e0e21
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Sat Sep 23 19:18:11 2017 +0900

    Merge pull request #4 from RedCarrottt/master
    
    Fix bug on install-deps-target.sh script.

[33mcommit 33e0e2140498e728d37899963a144b4d4809cf65[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Sat Sep 23 10:16:46 2017 +0000

    Fix bug on install-deps-target.sh script.
    * Add executing autogen.sh before building libxml2.

[33mcommit 946f4d68500de4c6132853ba6ca7aef8480df7a9[m
Merge: 1993519 087255d
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Sat Sep 23 09:38:39 2017 +0000

    Merge remote-tracking branch 'upstream/machine-learning' into machine-learning

[33mcommit 1993519441e9bec29de8e68304053c296aa188e3[m
Merge: 7d05bcc f600ac4
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Sat Sep 23 09:34:16 2017 +0000

    Merge remote-tracking branch 'upstream/master' into machine-learning

[33mcommit 087255dbf614f383936c61f141377e157dc62adf[m
Merge: 56d7c41 7d05bcc
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Sat Sep 23 18:24:55 2017 +0900

    Merge pull request #2 from RedCarrottt/machine-learning
    
    Initial prototype of machine learning framework

[33mcommit f600ac4c99cc5f34cecef296be5f9010bb31844a[m
Merge: 56d7c41 598a300
Author: Gyeonghwan Hong <RedCarrottt@gmail.com>
Date:   Sat Sep 23 18:24:45 2017 +0900

    Merge pull request #3 from RedCarrottt/master
    
    Add dependent packages as git submodules

[33mcommit 598a300d5d3d9e36659b34f7f723c7612b4175ef[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Sat Sep 23 08:55:25 2017 +0000

    Add dependent packages as git submodules

[33mcommit 7d05bccd7a4bde76b6e7add0c018c4f1a334d74d[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 21 13:28:44 2017 +0000

    Initial prototype of machine learning daemon message

[33mcommit 8e70cd1778d1cc0f7d5d2ed4848f8897a524bd7d[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 21 13:01:27 2017 +0000

    Initial prototype of machine learning daemon

[33mcommit 56d7c415d44c6d9503961a0c1e67713e9717907c[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Thu Sep 21 10:32:17 2017 +0000

    Update README.md

[33mcommit 9e129347aea9ab65c7b143c4589835995a22141f[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Sep 20 13:22:49 2017 +0000

    Initial commit for ANT master branch

[33mcommit 2d5d80071acba2f11ca11255ef627c0c597ce6fb[m
Author: Gyeonghwan Hong <redcarrottt@gmail.com>
Date:   Wed Sep 20 13:22:08 2017 +0000

    Initial commit
