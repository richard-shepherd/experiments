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
    this._radarLineAngleRadians = 1.5 * Math.PI;

    // The number of seconds it takes to sweep the entire circle
    // with the radar...
    this._radarSweepTimeMilliseconds = 5000.0;
    this._lastUpdateTime = Date.now();

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

        // We find the time delta since the last time we drew...
        var now = Date.now();
        var deltaMilliseconds = now - this._lastUpdateTime;
        this._lastUpdateTime = now;

        // We find the current size of the canvas...
        this._canvasWidth = ctx.canvas.width;
        this._canvasHeight = ctx.canvas.height;
        this._radarRadius = Math.min(this._canvasWidth, this._canvasHeight);
        this._radarRadius /= 2.0;
        this._radarRadius *= 0.85;

        // We clear the canvas, before redrawing...
        ctx.clearRect(0, 0, this._canvasWidth, this._canvasHeight);

        // We show the sweeping, green radar line...
        this._drawRadarLine(deltaMilliseconds);

        // We show the compass...
        this._compassHeadingRadians = compassHeadingRadians;
        this._drawCompass();

        // We draw the radar grid (unrotated)...
        this._drawGrid();
    } catch(ex) {
        Logger.log(ex.message);
    }
};

/**
 * _drawGrid
 * ---------
 * Draws the radar grid.
 */
RadarCanvas.prototype._drawGrid = function() {
    var ctx = this._canvasContext;
    try {
        // We set the origin to the center of the canvas (with no rotation)...
        ctx.save();
        ctx.translate(this._canvasWidth/2, this._canvasWidth/2);

        // We draw crosshair lines...
        var lineColor = "#008000"
        var textColor = "#405000"
        var circleColor = "#405000"

        // Vertical line...
        ctx.beginPath();
        ctx.strokeStyle = lineColor;
        ctx.lineWidth = 1;
        ctx.moveTo(0, -1.0 * this._radarRadius);
        ctx.lineTo(0, this._radarRadius);
        ctx.stroke();

        // Horizontal line...
        ctx.beginPath();
        ctx.strokeStyle = lineColor;
        ctx.lineWidth = 1;
        ctx.moveTo(-1.0 * this._radarRadius, 0);
        ctx.lineTo(this._radarRadius, 0);
        ctx.stroke();

        // Circles with labels at set distances...
        var fontSize = Math.floor(this._canvasWidth / 40.0);
        var fontOffset = this._canvasWidth / 150.0;
        ctx.font =  fontSize +  "px Arial";
        ctx.fillStyle = textColor;
        ctx.textAlign = "left";

        // Circle color and style...
        ctx.lineWidth = 1;
        ctx.strokeStyle = circleColor;
        ctx.setLineDash([2, 3]);

        // 50m line...
        ctx.beginPath();
        ctx.arc(0, 0, 0.25 * this._radarRadius, 0, 2.0 * Math.PI);
        ctx.stroke();
        ctx.fillText("50m", fontOffset, -0.25 * this._radarRadius - fontOffset);

        // 100m line...
        ctx.beginPath();
        ctx.arc(0, 0, 0.5 * this._radarRadius, 0, 2.0 * Math.PI);
        ctx.stroke();
        ctx.fillText("100m", fontOffset, -0.5 * this._radarRadius - fontOffset);

        // 150m line...
        ctx.beginPath();
        ctx.arc(0, 0, 0.75 * this._radarRadius, 0, 2.0 * Math.PI);
        ctx.stroke();
        ctx.fillText("150m", fontOffset, -0.75 * this._radarRadius - fontOffset);

        // 200m line...
        ctx.beginPath();
        ctx.arc(0, 0, 1.0 * this._radarRadius, 0, 2.0 * Math.PI);
        ctx.stroke();
        ctx.fillText("200m", fontOffset, -1.0 * this._radarRadius - fontOffset);
    } finally {
        ctx.restore();
    }
};

/**
 * _drawRadarLine
 * --------------
 * Draws the sweeping, green radar line.
 */
RadarCanvas.prototype._drawRadarLine = function(deltaMilliseconds) {
    var ctx = this._canvasContext;
    try {
        // We set the origin to the center of the canvas, and with
        // rotation...
        ctx.save();
        ctx.translate(this._canvasWidth/2, this._canvasWidth/2);
        ctx.rotate(-1.0 * this._compassHeadingRadians);

        // We update the angle...
        var offsetRadians = deltaMilliseconds / this._radarSweepTimeMilliseconds * 2.0 * Math.PI;
        this._radarLineAngleRadians += offsetRadians;
        if(this._radarLineAngleRadians > 2.0 * Math.PI) {
            this._radarLineAngleRadians = 0.0;
        }

        // We show the radar as a number of bands fading from
        // green to black...
        var numBands = 100;
        var green = 60;
        var greenOffset = green / (numBands + 1);
        var bandWidthRadians = 0.08;
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
    } finally {
        ctx.restore();
    }
};

/**
 * _drawCompass
 * ------------
 * Draws the compass.
 */
RadarCanvas.prototype._drawCompass = function() {
    var ctx = this._canvasContext;
    try {
        // We set the origin to the center of the canvas, and with
        // rotation...
        ctx.save();
        ctx.translate(this._canvasWidth/2, this._canvasWidth/2);
        ctx.rotate(-1.0 * this._compassHeadingRadians);

        var numLines = 72;
        var angleBetweenLines = 2.0 * Math.PI / numLines;
        for(var i=0; i<numLines; ++i) {
            ctx.beginPath();
            ctx.strokeStyle = '#dddddd';
            ctx.lineWidth = 1;
            if(i%2 === 0) {
                ctx.moveTo(0, 0.93 * this._radarRadius);
            } else {
                ctx.moveTo(0, 0.96 * this._radarRadius);
            }
            ctx.lineTo(0, this._radarRadius);
            ctx.stroke();

            ctx.rotate(angleBetweenLines);
        }

        // We show the N, E, W, S labels...
        var fontSize = Math.floor(this._canvasWidth / 22.0);
        var fontOffset = fontSize / 2.0 * 0.8;
        ctx.font =  fontSize +  "px Arial";
        ctx.textAlign = "center";

        ctx.fillStyle = "red";
        ctx.fillText("N", 0, -1.0 * this._radarRadius);
        ctx.fillStyle = "white";

        ctx.font =  fontSize/1.3 +  "px Arial";
        ctx.textAlign = "center";
        ctx.fillText("S", 0, this._radarRadius + fontOffset*2);
        ctx.fillText("W", -1.0 * this._radarRadius - fontOffset, fontOffset);
        ctx.fillText("E", this._radarRadius + fontOffset, fontOffset);
    } finally {
        ctx.restore();
    }
};