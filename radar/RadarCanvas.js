/**
 * RadarCanvas
 * -----------
 * Manages the radar, showing the positions of players
 * and items relative to us.
 *
 * @constructor
 */
function RadarCanvas(canvasElementID) {
    // We find the canvas element and its drawing context...
    this._canvasElement = document.getElementById(canvasElementID);
    this._canvasContext = this._canvasElement.getContext("2d");

    // The canvas size...
    this._canvasWidth = 1.0;
    this._canvasHeight = 1.0;

    // The most recent compass heading...
    this._compassHeadingRadians = 0.0;

    // We set the background color of the canvas...
    this._canvasElement.style.backgroundColor = "#002000";
}

/**
 * showRadar
 * ---------
 * Shows the radar.
 */
RadarCanvas.prototype.showRadar = function(compassHeadingRadians) {
    try {
        // We find the current size of the canvas...
        this._canvasWidth = this._canvasContext.canvas.width;
        this._canvasHeight = this._canvasContext.canvas.height;

        // We store the context and restore it later, as we will
        // be applying various transformations to it, such as rotations.
        // (If we don't save and restore, these will be cumulative in
        // later updates.)
        this._canvasContext.save();

        // We clear the canvas, before redrawing...
        this._canvasContext.clearRect(0, 0, this._canvasWidth, this._canvasHeight);

        // We show the compass...
        this._compassHeadingRadians = compassHeadingRadians;
        this._drawCompass();

    } finally {
        this._canvasContext.restore();
    }
};

/**
 * _drawCompass
 * ------------
 * Draws the compass.
 */
RadarCanvas.prototype._drawCompass = function() {
    // We set the origin to the center of the canvas...
    this._canvasContext.translate(this._canvasWidth/2, this._canvasWidth/2);

    // We rotate the canvas, so that north points north...
    this._canvasContext.rotate(-1.0 * this._compassHeadingRadians);

    // We work out the offset of the compass lines from the center...
    var compassLineOffset = Math.min(this._canvasWidth, this._canvasHeight);
    compassLineOffset /= 2.0;
    compassLineOffset *= 0.85;

    // We draw the compass lines...
    this._canvasContext.beginPath();
    this._canvasContext.strokeStyle = 'white';
    this._canvasContext.lineWidth = 2;
    this._canvasContext.moveTo(0, -compassLineOffset);
    this._canvasContext.lineTo(0, compassLineOffset);
    this._canvasContext.stroke();

    this._canvasContext.beginPath();
    this._canvasContext.strokeStyle = 'white';
    this._canvasContext.lineWidth = 2;
    this._canvasContext.moveTo(-compassLineOffset, 0);
    this._canvasContext.lineTo(compassLineOffset, 0);
    this._canvasContext.stroke();

    // We show the N, E, W, S labels...
    var fontSize = Math.floor(this._canvasWidth / 14.0);
    var fontOffset = fontSize / 2.0 * 0.8;
    this._canvasContext.font =  fontSize +  "px Arial";
    this._canvasContext.textAlign = "center";

    this._canvasContext.fillStyle = "red";
    this._canvasContext.fillText("N", 0, -compassLineOffset);

    this._canvasContext.fillStyle = "white";
    this._canvasContext.fillText("S", 0, compassLineOffset + fontOffset*2);
    this._canvasContext.fillText("W", -compassLineOffset - fontOffset, fontOffset);
    this._canvasContext.fillText("E", compassLineOffset + fontOffset, fontOffset);

    var shotgun = imageManager.getImage(ImageManager.Image.RADAR_SHOTGUN);
    var imageWidth = 70;
    var imageScale = imageWidth / shotgun.width;
    var imageHeight = shotgun.height * imageScale;
    this._canvasContext.drawImage(shotgun, 30, 40, imageWidth, imageHeight);
};