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
    this._canvasElement = document.getElementById(canvasElement);
    this._canvasContext = this._canvasElement.getContext("2d");
}

/**
 * showRadar
 * ---------
 * Shows the radar.
 */
RadarCanvas.prototype.showRadar = function(compassHeadingRadians) {
    try {
        // We store the context and restore it later, as we will
        // be applying various transformations to it, such as rotations.
        // (If we don't save and restore, these will be cumulative in
        // later updates.)
        this._canvasContext.save();



    } finally {
        this._canvasContext.restore();
    }
};
