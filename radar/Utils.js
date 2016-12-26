/**
 * Utils
 * -----
 * Static utility functions.
 * @constructor
 */
function Utils() {
}

/**
 * extend
 * ------
 * Derives one 'class' from another.
 * From: http://stackoverflow.com/questions/4152931/javascript-inheritance-call-super-constructor-or-use-prototype-chain
 */
Utils.extend = function(base, sub) {
    var origProto = sub.prototype;
    sub.prototype = Object.create(base.prototype);
    for (var key in origProto)  {
        sub.prototype[key] = origProto[key];
    }
    sub.prototype.constructor = sub;
    Object.defineProperty(sub.prototype, 'constructor', {
        enumerable: false,
        value: sub
    });
};

/**
 * colorToString
 * -------------
 * Converts a color to a hex string.
 */
Utils.colorToString = function(color) {
    return Utils.rgbToString(color.r, color.g, color.b);
};

/**
 * rgbToString
 * -----------
 * Converts RGB values to a color string.
 */
Utils.rgbToString = function(r, g, b) {
    function componentToHex(c) {
        var hex = c.toString(16);
        return hex.length == 1 ? "0" + hex : hex;
    }
    r = Math.round(r);
    g = Math.round(g);
    b = Math.round(b);
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
};

/**
 * Returns the percentage difference between a and b (as a percentage
 * of a) as a number like 0.1 for 10% difference.
 */
Utils.percentageDifference = function(a, b) {
    var percentageDifference = 1.0;
    if(a !== 0.0) {
        percentageDifference =  Math.abs(a - b) / Math.abs(a);
    }
    return percentageDifference;
};

