/**
 * @constructor
 */
function Camera(width, height, facingDirection, videoElement) {

    // The video element we show video in...
    this.videoElement = videoElement;

    // We create the video part of the constraints...
    var videoConstraints = {
        width : width,
        height: height,
        facingMode: "environment"
    };
    // if(facingDirection === Camera.Direction.FRONT_FACING) {
    //     videoConstraints.facingMode = "user";
    // }
    // if(facingDirection === Camera.Direction.BACK_FACING) {
    //     videoConstraints.facingMode = "environment";
    // }

    // We create the constraints...
    var constraints = {audio: false, video: videoConstraints };

    // We connect to the video stream from the camera...
    var that = this;
    navigator.mediaDevices.getUserMedia(constraints)
        .then(function(stream) { that.gotVideoStream(stream); });
}

/**
 * An enum for the direction cameras can face (particularly in phones).
 */
Camera.Direction = {
    DEFAULT : 0,
    FRONT_FACING : 1,
    BACK_FACING : 2
};

/**
 * Called when we have got a video stream.
 */
Camera.prototype.gotVideoStream = function (stream) {
    this.videoElement.src = window.URL.createObjectURL(stream);
};
