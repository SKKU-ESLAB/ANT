try:
    import antstream
    StreamAPI = antstream
except ImportError:
    raise Exception('Gateway API Dependency Error: not found Stream API')

deviceTypeToCameraType = {
    'rpi2': 'rpi',
    'rpi3': 'rpi',
    'xu3': 'v4l2',
    'xu4': 'v4l2',
    'tx1': 'nvidia',
    'tx2': 'nvidia'
}


class ANTCamera:
    def __init__(self):
        pass

    def createCameraElement(self, deviceType):
        cameraType = deviceTypeToCameraType[deviceType]
        if cameraType == 'rpi':
            source = self.createRPiCameraElement()
        elif cameraType == 'test':
            source = self.createTestCameraElement()
        elif cameraType == 'v4l2':
            source = self.createV4L2CameraElement()
        elif cameraType == 'nvidia':
            source = self.createNvidiaCameraElement()
        return source

    def createRPiCameraElement(self):
        source = StreamAPI.createElement('rpicamsrc')
        return source

    def createTestCameraElement(self):
        source = StreamAPI.createElement('videotestsrc')
        source.setProperty('pattern', 1)
        return source

    def createV4L2CameraElement(self, sourcePath):
        source = StreamAPI.createElement('v4l2src')
        if sourcePath is None:
            sourcePath = '/dev/video0'
        source.setProperty('device', sourcePath)
        return source

    def createNvidiaCameraElement(self):
        source = StreamAPI.createElement('nvcamsrc')
        source.setProperty('fpsRange', '30.0 30.0')
        return source

    
