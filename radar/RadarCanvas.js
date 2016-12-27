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
    this._radarRadius = 0.5;

    // The most recent compass heading...
    this._compassHeadingRadians = 0.0;

    // The angle of the radar line...
    this._radarLineAngleRadians = 0.0;

    // We set the background color of the canvas...
    this._canvasElement.style.backgroundColor = "#000000";
}

/**
 * showRadar
 * ---------
 * Shows the radar.
 */
RadarCanvas.prototype.showRadar = function(compassHeadingRadians) {
    try {
        var ctx = this._canvasContext;

        // We find the current size of the canvas...
        this._canvasWidth = ctx.canvas.width;
        this._canvasHeight = ctx.canvas.height;
        this._radarRadius = Math.min(this._canvasWidth, this._canvasHeight);
        this._radarRadius /= 2.0;
        this._radarRadius *= 0.85;

        // We store the context and restore it later, as we will
        // be applying various transformations to it, such as rotations.
        // (If we don't save and restore, these will be cumulative in
        // later updates.)this._radarRadius
        ctx.save();

        // We clear the canvas, before redrawing...
        ctx.clearRect(0, 0, this._canvasWidth, this._canvasHeight);

        // We set the origin to the center of the canvas...
        ctx.translate(this._canvasWidth/2, this._canvasWidth/2);

        // We rotate the canvas, so that north points north...
        ctx.rotate(-1.0 * this._compassHeadingRadians);

        // We show the sweeping, green radar line...
        this._drawRadarLine();

        // We show the compass...
        this._compassHeadingRadians = compassHeadingRadians;
        this._drawCompass();

        this._radarLineAngleRadians += 0.02;
        if(this._radarLineAngleRadians > 2.0 * Math.PI) {
            this._radarLineAngleRadians = 0.0;
        }

    } finally {
        this._canvasContext.restore();
    }
};

/**
 * _drawRadarLine
 * --------------
 * Draws the sweeping, green radar line.
 */
RadarCanvas.prototype._drawRadarLine = function() {
    var ctx = this._canvasContext;

    var numBands = 60;
    var green = 150;
    var greenOffset = green / (numBands + 1);
    var bandWidthRadians = 0.09;
    var angle = this._radarLineAngleRadians;
    for(var i=0; i<numBands; ++i) {
        ctx.fillStyle = Utils.rgbToString(0, green, 0);
        ctx.beginPath();
        ctx.arc(0, 0, this._radarRadius, angle, angle-bandWidthRadians, true);
        ctx.lineTo(0, 0);
        ctx.closePath();
        ctx.fill();

        green -= greenOffset;
        angle -= (bandWidthRadians * 0.5);
    }
};

/**
 * _drawCompass
 * ------------
 * Draws the compass.
 */
RadarCanvas.prototype._drawCompass = function() {
    var ctx = this._canvasContext;

    // We draw the compass lines...
    ctx.beginPath();
    ctx.strokeStyle = 'white';
    ctx.lineWidth = 2;
    ctx.moveTo(0, -1.0 * this._radarRadius);
    ctx.lineTo(0, this._radarRadius);
    ctx.stroke();

    ctx.beginPath();
    ctx.strokeStyle = 'white';
    ctx.lineWidth = 2;
    ctx.moveTo(-1.0 * this._radarRadius, 0);
    ctx.lineTo(this._radarRadius, 0);
    ctx.stroke();

    // We show the N, E, W, S labels...
    var fontSize = Math.floor(this._canvasWidth / 14.0);
    var fontOffset = fontSize / 2.0 * 0.8;
    ctx.font =  fontSize +  "px Arial";
    ctx.textAlign = "center";

    ctx.fillStyle = "red";
    ctx.fillText("N", 0, -1.0 * this._radarRadius);

    ctx.fillStyle = "white";
    ctx.fillText("S", 0, this._radarRadius + fontOffset*2);
    ctx.fillText("W", -1.0 * this._radarRadius - fontOffset, fontOffset);
    ctx.fillText("E", this._radarRadius + fontOffset, fontOffset);

    var shotgun = imageManager.getImage(ImageManager.Image.RADAR_SHOTGUN);
    var imageWidth = 70;
    var imageScale = imageWidth / shotgun.width;
    var imageHeight = shotgun.height * imageScale;
    ctx.drawImage(shotgun, 30, 40, imageWidth, imageHeight);
};