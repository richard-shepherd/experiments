/**
 * Camera
 * ------
 * Manages video input from a camera.
 *
 * The options passed in look like this:
 *     var options = {
 *         width: (default=640),
 *         height: (default=480),
 *         facingDirection: Camera.FacingDirection enum (default=DEFAULT),
 *         videoElementID: videoElementID (default=hidden video element),
 *         showCanvas: {
 *             canvasElementID: canvasElementID (default=no canvas element),
 *             sampleIntervalMilliseconds: interval (default=33),
 *             imageDataCallback: (default=null)
 *         }
 *     }
 *
 * Note: imageDataCallback(data, canvasContext)
 *
 * @constructor
 */
function Camera(options) {
    Logger.log("Starting camera");

    this._parseOptions(options);

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
 * snapshotToCanvas
 * ----------------
 * Snapshots the video to the canvas passed in.
 */
Camera.prototype.snapshotToCanvas = function(canvas) {
    try {
        var canvasWidth = canvas.width;
        var canvasHeight = this.videoElement.videoHeight / (this.videoElement.videoWidth / canvasWidth);
        canvas.setAttribute('height', canvasHeight);

        var canvasContext = canvas.getContext('2d');
        canvasContext.drawImage(this.videoElement, 0, 0, canvasWidth, canvasHeight);
    } catch(ex) {
        Logger.log(ex.message);
    }
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
 * _parseOptions
 * -------------
 * Parses the options passed to the constructor.
 */
Camera.prototype._parseOptions = function(options) {
    if(options) {
        // Some options were passed in, so we parse them...
        this._parseOptions_Options(options);
    } else {
        // There were no options, so we set all the defaults...
        this._parseOptions_AllDefaults();

    }
};

/**
 * _parseOptions_Options
 * ---------------------
 * Parses options from the options passed in.
 */
Camera.prototype._parseOptions_Options = function(options) {
    this.width = options.width || 640;
    this.height = options.height || 480;
    this.facingDirection = options.facingDirection || Camera.FacingDirection.DEFAULT;

    // If there is no video element specified, we create a hidden one...
    if(options.videoElementID) {
        this.videoElement = document.getElementById(options.videoElementID);
    } else {
        this.videoElement = this._createVideoElement();
    }

    // We check if we are sampling the video to a canvas...
    this.canvas = null;
    if(options.showCanvas && options.showCanvas.canvasElementID) {
        this.canvas = document.getElementById(options.showCanvas.canvasElementID);
        var sampleIntervalMS = options.showCanvas.sampleIntervalMilliseconds || 33;
        this.imageDataCallback = options.showCanvas.imageDataCallback || null;

        // We get the context for the canvas...
        this.canvasContext = this.canvas.getContext('2d');

        // We create a time to sample the video...
        var that = this;
        this.canvasSampleTimer = setInterval(function() { that._onCanvasSampleTimer(); }, sampleIntervalMS);
    }
};

/**
 * _parseOptions_AllDefaults
 * -------------------------
 * Sets all the default options.
 */
Camera.prototype._parseOptions_AllDefaults = function() {
    this.width = 640;
    this.height = 480;
    this.facingDirection = Camera.FacingDirection.DEFAULT;
    this.videoElement = this._createVideoElement();
    this.canvas = null;
};

/**
 * _onCanvasSampleTimer
 * --------------------
 * Called when the canvas sample timer ticks.
 */
Camera.prototype._onCanvasSampleTimer = function() {
    try {
        // We copy the video to the canvas...
        var canvasWidth = this.canvas.width;
        var canvasHeight = this.videoElement.videoHeight / (this.videoElement.videoWidth / canvasWidth);
        this.canvas.setAttribute('height', canvasHeight);
        this.canvasContext.drawImage(this.videoElement, 0, 0, canvasWidth, canvasHeight);

        if(this.imageDataCallback) {
            // A callback is set up for the image data...
            var data = this.canvasContext.getImageData(0, 0, canvasWidth, canvasHeight).data;
            this.imageDataCallback(data, this.canvasContext);
        }
    } catch(ex) {
        Logger.log(ex.message);
    }

};

/**
 * _createVideoElement
 * -------------------
 * Creates a hidden video element (when one is not supplied).
 */
Camera.prototype._createVideoElement = function() {
    var videoElement = document.createElement("video");
    videoElement.autoplay = true;
    videoElement.style.display = "none";
    document.body.prepend(videoElement);
    return videoElement;
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
