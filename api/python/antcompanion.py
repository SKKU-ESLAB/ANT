import requests, flask
import native

class ANTCompanion:
    def __init__(self):
        self._mCompanionHost = None
        self._mCompanionPort = None
        self._mCompanionPath = None
        self._mHandlers = []

    def getCompanionHost(self):
        return self._mCompanionHost

    def _setCompanionAddress(self, companionHost, companionPort, companionPath):
        self._mCompanionHost = companionHost
        self._mCompanionPort = companionPort
        self._mCompanionPath = companionPath
        return True

    def _onReceiveMessageFromCompanion(self, message):
        for handler in self._mHandlers:
            handler(message)

    def sendMessage(self, message): # http 모듈을 python에서 네트워크 기능을 하는 모듈로 변경(requests?, flask?) & 기능 파악 필요(질문)
        if self._mCompanionPath is None:
            print('Error: failed to send message due to no companion address')
            return False

        options = {
            'method': 'POST',
            'host': self._mCompanionHost,
            'port': self._mCompanionPort,
            'path': self._mCompanionPath,
            'headers': {'Content-Length': len(message)}
        }
        client_request =  requests.request(options)
        client_request.write(message)
        client_request.end()
        return True

    def registerOnReceiveMessage(self, handler):
        self._mHandlers.append(handler)

    def unregisterOnReceiveMessage(self, handler):
        if handler is None:
            return False
        for i in range(len(self._mHandlers)):
            if self._mHandlers[i] == handler:
                self._mHandlers.pop(i)
                return True
        return False

    def getMyIPAddress(self, interfaceName):
        return native.ant_companion_getMyIPAddress(interfaceName)