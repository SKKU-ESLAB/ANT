# ANT-Resource API

# Class

ANTResource()

define ANT Resource Class

## Method

### __init__()

initialize ANT Resource Element

**Kind**:class method

### _initialize()

virtual initialize ANT Resource Element

**Kind**:class method

### _onReceiveRawMessage(rawMessage)

Receive Raw Message

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|rawMessage|string|raw type message received|

### _sendRequest(method, targetUri, message, onResourceResponse)

send Request

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|method|function|function to send target Uri|
|targetUri|string|target uri to send request|
|message|string|raw type message to send|
|onResourceResponse|ANTResource|

### requestGet(method, targetUri, message, onResourceResponse)

get request

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|method|function|function to send target Uri|
|targetUri|string|target uri to send request|
|message|string|raw type message to send|
|onResourceResponse|ANTResource|


### requestPost(targetUri, message, onResourceResponse)

request Post

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|method|function|function to send target Uri|
|message|string|raw type message to send|
|onResourceResponse|ANTResource|

### requestPut(targetUri, message, onResourceResponse)

request Put

|Param|Type|Description|
|------|---|---|
|method|function|function to send target Uri|
|message|string|raw type message to send|
|onResourceResponse|ANTResource|

### requestDelete(targetUri, message, onResourceResponse)

request Delete

|Param|Type|Description|
|------|---|---|
|method|function|function to send target Uri|
|message|string|raw type message to send|
|onResourceResponse|ANTResource|