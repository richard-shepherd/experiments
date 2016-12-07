/**
 * Camera
 * ------
 * Manages video input from a camera.
 *
 * @constructor
 */
function Camera(width, height, facingDirection, videoElement) {

    // We keep hold of the construction parameters...
    this.width = width;
    this.height = height;
    this.facingDirection = facingDirection;
    this.videoElement = videoElement;

    // The collection of available cameras.
    // This is a collection of objects with 'label' and 'deviceID' fields.
    this.cameras = [];

    // The index into the cameras collection for the preferred camera
    // (which matches the facingDirection requested)...
    this.preferredCameraIndex = 0;

    Logger.log("Starting camera");

    // We find the available cameras...
    this.findCameras();

    // // We create the video part of the constraints...
    // var videoConstraints = {
    //     width : width,
    //     height: height,
    //     facingMode: "environment"
    // };
    // if(facingDirection === Camera.FacingDirection.FRONT_FACING) {
    //     videoConstraints.facingMode = "user";
    // }
    // if(facingDirection === Camera.FacingDirection.BACK_FACING) {
    //     videoConstraints.facingMode = "environment";
    // }

    // // We create the constraints...
    // var constraints = {audio: false, video: videoConstraints };
    //
    // // We connect to the video stream from the camera...
    // var that = this;
    // navigator.mediaDevices.getUserMedia(constraints)
    //     .then(function(stream) { that.gotVideoStream(stream); });
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
 * findCameras
 * -----------
 * Finds the collection of available cameras.
 */
Camera.prototype.findCameras = function() {
    Logger.log("Finding available cameras");

    // We clear any existing camera info we hold...
    this.cameras = [];

    // We find the collection of cameras...
    var that = this;
    navigator.mediaDevices.enumerateDevices()
        .then(function(deviceInfos) { that.gotCameras(deviceInfos); })
        .catch(Logger.log);
};

/**
 * gotCameras
 * ----------
 * Called when we have found the collection of cameras (and other devices).
 */
Camera.prototype.gotCameras = function(deviceInfos) {
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

        }
    }
};

/**
 * Called when we have got a video stream.
 */
Camera.prototype.gotVideoStream = function (stream) {
    Logger.log("Got video stream");
    this.videoElement.src = window.URL.createObjectURL(stream);
};
