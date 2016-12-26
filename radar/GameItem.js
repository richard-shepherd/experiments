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
}

/**
 * radarInfo
 * ---------
 * Returns info about how to show this item on the radar.
 *
 * Returned object = {
 *      showAsCircle : true / false,
 *      circleColor: hex-string,
 *      showAsImage: true / false,
 *      image: ImageManager.Image enum
 * }
 */
GameItem.prototype.radarInfo = function() {
    throw new Error("radarInfo must be implemented in derived classes");
};


