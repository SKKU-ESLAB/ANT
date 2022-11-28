try:
  import antcompanion as CompanionAPI
except ModuleNotFoundError as e:
  raise ModuleNotFoundError('Resource API Dependency Error: not found Companion API')


class ANTResource:
    def __init__(self):
        self._mIsInitialized = False
        self._mRequestId = 0
        self._mOnResourceResponseDict = {}

    def _initialize(self):
        antresource = ANTResource()
        antresource._mIsInitialized = True
        CompanionAPI.registerOnReceiveMessage(ANTResource._onReceiveRawMessage)

    def _onReceiveRawMessage(rawMessage):
        antresource = ANTResource()
        firstLineEnd = rawMessage.index('\n')
        firstLine = rawMessage[:firstLineEnd]
        if firstLine!='ResourceResponse':
            return
        secondLineEnd = rawMessage.index('\n', firstLine+1)
        secondLine = rawMessage[firstLineEnd+1:secondLineEnd]
        thirdLineEnd = rawMessage.index('\n', secondLine+1)
        thirdLine = rawMessage[secondLineEnd+1:thirdLineEnd]
        fourthLineEnd = rawMessage.index('\n', thirdLine+1)
        fourthLine = rawMessage[thirdLine+1, fourthLineEnd]
        otherLines = rawMessage[fourthLineEnd+1]

        requestId = float(secondLine)
        method = thirdLine
        targetUri = fourthLine
        message = otherLines

        onResourceResponse = antresource._mOnResourceResponseDict[requestId]
        if onResourceResponse:
            antresource._mOnResourceResponseDict['onResourceResponse'](method, targetUri, message)
            del antresource._mOnResourceResponseDict['onResourceResponse']


    def _sendRequest(method, targetUri, message, onResourceResponse):
        antresource = ANTResource()
        if antresource._mIsInitialized:
            antresource._initialize()
        rawMessage = 'ResourceRequest\n' + \
                    ANTResource._mRequestId + '\n' \
                    + method + '\n' \
                    + targetUri + '\n' \
                    + message
        antresource._mIsInitialized+=1

        CompanionAPI.sendMessage(rawMessage)
        if onResourceResponse:
            antresource._mOnResourceResponseDict[antresource._mRequestId] = onResourceResponse
        

    def requestGet(method, targetUri, message, onResourceResponse):
        antresource = ANTResource()
        antresource._sendRequest('GET', targetUri, message, onResourceResponse)

    def requestPost(targetUri, message, onResourceResponse):
        antresource = ANTResource()
        antresource._sendRequest('POST', targetUri, message, onResourceResponse)

    def requestPut(targetUri, message, onResourceResponse):
        antresource = ANTResource()
        antresource._sendRequest('PUT', targetUri, message, onResourceResponse)

    def requestDelete(targetUri, message, onResourceResponse):
        antresource = ANTResource()
        antresource._sendRequest('DELETE', targetUri, message, onResourceResponse)