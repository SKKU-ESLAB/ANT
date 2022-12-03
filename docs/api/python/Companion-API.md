
# ANT Companion -API

ANT Python is now available on any ARM Cortex-A devices. In future, we will add support for Cortex-M or Cortex-R devices.

## Class
ANTCompanion()

**Kind**: class

## Method

### getCompanionHost()

get Companion Host object

**Kind**: class method

### _setCompanionAddress(companionHost, companionPort, companionPath)

set Companion's Port and path for Host object

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|companionHost|class|get device type for application's initialization|
|companionPort|int(static const)|get device type for application's initialization|
|companionPath|int(static const)|get device type for application's initialization|

### _onReceiveMessageFromCompanion(message)

handle messages from other companions

**Kind**: class method

### sendMessage(sourcePath)

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|sourcePath|string|get source code path for application's initialization|

### registerOnReceiveMessage(handler)

add new handler for message from companions

**Kind**: class method

###  unregisterOnReceiveMessage(handler)

remove specific handler from handler list

**Kind**: class method

### getMyIPAddress(name)

get specific companion's interface name

**Kind**: class method




