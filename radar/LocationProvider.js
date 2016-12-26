/**
 * LocationProvider
 * ----------------
 * Subscribes to GPS position and calls back with the position.
 * Also calls back with the compass heading.
 *
 * locationCallback(position-object)
 * compassCallback(headingRadians)
 *
 * @constructor
 */
function LocationProvider(locationCallback, compassCallback) {
    this._locationCallback = locationCallback;
    this._compassCallback = compassCallback;

    this.position = null;
    this.compassHeadingRadians = 0.0;

    this._subscribeLocation();
    this._subscribeCompass();
}

/**
 * _subscribeLocation
 * ------------------
 * Subscribes to the GPS location.
 */
LocationProvider.prototype._subscribeLocation =function() {
    if (!navigator.geolocation) {
        Logger.log("GPS not available");
    }

    var that = this;
    navigator.geolocation.getCurrentPosition(function(position) {
        try {
            // We hold the latest position...
            that.position = position;

            // And call back, if the callback is set up...
            if(that._locationCallback !== null) {
                that._locationCallback(position);
            }
        } catch(ex) {
            Logger.log(ex.message);
        }
    });
};

/**
 * _subscribeCompass
 * -----------------
 * Subscribes to the compass heading.
 */
LocationProvider.prototype._subscribeCompass = function() {
    var that = this;
    window.addEventListener('deviceorientationabsolute', function(orientationInfo) {
        try {
            // We store the latest compass heading...
            that.compassHeadingRadians = LocationProvider.compassHeading(orientationInfo.alpha, orientationInfo.beta, orientationInfo.gamma);

            // And call back , if a callback is set up...
            if(that._compassCallback !== null) {
                that._compassCallback(that.compassHeadingRadians);
            }
        } catch(ex) {
            Logger.log(ex.message);
        }
    }, false);
};

/**
 * compassHeading
 * --------------
 * Static method to convert phone orientation into a compass heading
 * in radians. (When the phone is held upright.)
 */
LocationProvider.compassHeading = function(alpha, beta, gamma) {

    // Convert degrees to radians
    var alphaRad = alpha * (Math.PI / 180);
    var betaRad = beta * (Math.PI / 180);
    var gammaRad = gamma * (Math.PI / 180);

    // Calculate equation components
    var cA = Math.cos(alphaRad);
    var sA = Math.sin(alphaRad);
    var cB = Math.cos(betaRad);
    var sB = Math.sin(betaRad);
    var cG = Math.cos(gammaRad);
    var sG = Math.sin(gammaRad);

    // Calculate A, B, C rotation components
    var rA = - cA * sG - sA * sB * cG;
    var rB = - sA * sG + cA * sB * cG;
    var rC = - cB * cG;

    // Calculate compass heading
    var compassHeading = 0.0;
    if(rB !== 0.0) {
        compassHeading = Math.atan(rA / rB);
    }

    // Convert from half unit circle to whole unit circle
    if(rB < 0) {
        compassHeading += Math.PI;
    }else if(rA < 0) {
        compassHeading += 2 * Math.PI;
    }

    return compassHeading;
};