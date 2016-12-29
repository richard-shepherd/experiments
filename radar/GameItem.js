/**
 * GameItem
 * --------
 * A 'base class' for items in the game, including players as
 * well as items in the environment.
 * @constructor
 */
function GameItem() {
    // The item's position...
    this.latitude = 0.0;
    this.longitude = 0.0;

    // The item's position relative to us as polar coordinates.
    // The angle is radians clockwise from north.
    this.distanceMeters = 1000.0;
    this.angleRadians = Math.PI / 4.0;
}

/**
 * radarInfo
 * ---------
 * Returns info about how to show this item on the radar.
 *
 * Returned object = {
 *      showAsCircle : true / false,
 *      circleColor: hex-string,
 *      label: (string)
 * }
 */
GameItem.prototype.radarInfo = function() {
    throw new Error("radarInfo must be implemented in derived classes");
};


