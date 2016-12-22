/**
 * LocationProvider
 * ----------------
 * Subscribes to GPS position and calls back with the position.
 *
 * @constructor
 */
function LocationProvider(callback) {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(callback);
    } else {
        //Logger.log("GPS not available");
    }
}
