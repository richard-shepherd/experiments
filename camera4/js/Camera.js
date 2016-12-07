/**
 * Camera
 * ------
 * Manages video input from a camera.
 *
 * @constructor
 */
function Camera(width, height, facingDirection, videoElement) {
    Logger.log("Starting camera");

    // We keep hold of the construction parameters...
    this.width = width;
    this.height = height;
    this.facingDirection = facingDirection;
    this.videoElement = videoElement;

    // The collection of available cameras.
    // This is a collection of objects with 'label' and 'deviceID' fields.
    this.cameras = [];

    // The index into the cameras collection for the preferred camera (which matches the facingDirection requested)...
    this.preferredCameraIndex = 0;

    // The current camera index...
    this.currentCameraIndex = 0;

    // The stream from the currently active camera...
    this.activeStream = null;

    // We find the available cameras...
    this._findCameras();
}

/**
 * An enum for the direction cameras can face (particularly in phones).
 */
Camera.FacingDirection = {
    DEFAULT : 0,
    FRONT_FACING : 1,
    BACK_FACING : 2
};

/**
 * toggleCamera
 * ------------
 * Switches between the available cameras.
 */
Camera.prototype.toggleCamera = function() {
    try {
        Logger.log("Toggling camera");
        this.currentCameraIndex++;
        if(this.currentCameraIndex >= this.cameras.length) {
            this.currentCameraIndex = 0;
        }
        this._selectCamera();
    } catch (ex) {
        Logger.log(ex.message);
    }
};

/**
 * _findCameras
 * ------------
 * Finds the collection of available cameras.
 */
Camera.prototype._findCameras = function() {
    try {
        Logger.log("Finding available cameras");

        // We clear any existing camera info we hold...
        this.cameras = [];

        // We find the collection of cameras...
        var that = this;
        navigator.mediaDevices.enumerateDevices()
            .then(function(deviceInfos) { that._gotCameras(deviceInfos); })
            .catch(Logger.log);
    } catch (ex) {
        Logger.log(ex.message);
    }
};

/**
 * _gotCameras
 * -----------
 * Called when we have found the collection of cameras (and other devices).
 */
Camera.prototype._gotCameras = function(deviceInfos) {
    try {
        Logger.log("Got camera infos");

        // We find the cameras, and add info about them
        // to our collection...
        for (var i = 0; i !== deviceInfos.length; ++i) {
            var deviceInfo = deviceInfos[i];

            if (deviceInfo.kind === 'videoinput') {
                // We've found a camera...
                Logger.log("Found camera: label=" + deviceInfo.label + ", deviceID=" + deviceInfo.deviceId);
                this.cameras.push( {label: deviceInfo.label, deviceID: deviceInfo.deviceId} );
            }
        }

        // We check to see if our preferred camera can be found...
        for(var i=0; i<this.cameras.length; ++i) {
            var label = this.cameras[i].label;
            if(this.facingDirection === Camera.FacingDirection.FRONT_FACING && label.includes("front")) {
                this.preferredCameraIndex = i;
                break;
            }
            if(this.facingDirection === Camera.FacingDirection.BACK_FACING && label.includes("back")) {
                this.preferredCameraIndex = i;
                break;
            }
        }

        // We select the camera...
        this.currentCameraIndex = this.preferredCameraIndex;
        this._selectCamera();
    } catch (ex) {
        Logger.log(ex.message);
    }
};

/**
 * _selectCamera
 * -------------
 * Selects the camera with the current camera index.
 */
Camera.prototype._selectCamera = function() {
    try {
        // We stop the current stream before starting a new one...
        this._stopActiveStream();

        // We find the device ID of the current camera...
        var cameraInfo = this.cameras[this.currentCameraIndex];
        Logger.log("Connecting to camera: " + cameraInfo.label);

        // We create the constraints...
        var videoConstraints = {
            width : this.width,
            height: this.height,
            deviceId: {exact: cameraInfo.deviceID }
        };
        var constraints = {audio: false, video: videoConstraints };

        // We connect to the video stream from the camera...
        var that = this;
        navigator.mediaDevices.getUserMedia(constraints)
            .then(function(stream) { that._gotVideoStream(stream); });
    } catch (ex) {
        Logger.log(ex.message);
    }
};

/**
 * _gotVideoStream
 * ---------------
 * Called when we have got a video stream.
 */
Camera.prototype._gotVideoStream = function (stream) {
    try {
        Logger.log("Got video stream");
        this.activeStream = stream;
        this.videoElement.src = window.URL.createObjectURL(stream);
    } catch (ex) {
        Logger.log(ex.message);
    }
};

/**
 * _stopActiveStream
 * -----------------
 * Stops the active stream, if there is one.
 */
Camera.prototype._stopActiveStream = function() {
    try {
        Logger.log("Stopping active stream");
        if (this.activeStream) {
            this.activeStream.getTracks().forEach(function(track) {
                track.stop();
            });
        }
        this.activeStream = null;
    } catch (ex) {
        Logger.log(ex.message);
    }
};
