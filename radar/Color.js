/**
 * Color
 * -----
 * Represents an RGB color.
 * @constructor
 */
function Color(r, g, b) {
    this.r = r;
    this.g = g;
    this.b = b;

    // We calculate LAB values for the color on demand...
    this._lab = null;

}

// Some color constants...
Color.black = new Color(0, 0, 0);

/**
 * isMatch
 * -------
 * Returns true if the two colors match to the tolerance specified,
 * false otherwise.
 *
 * The tolerance is specified as a number between 0-100, where
 * zero requires a perfect match, and 100 allows any differences.
 *
 * The color ratios must all be within this tolerance of their
 * counterparts for the color to be a match.
 */
Color.prototype.isMatch = function(other, tolerance) {
    // We get the LAB values of the two colors...
    var thisLAB = this.getLAB();
    var otherLAB = other.getLAB();

    // We set the Ls to a standard value...
    var thisColor = {L: 40.0, A: thisLAB.A, B: thisLAB.B};
    var otherColor = {L: 40.0, A: otherLAB.A, B: otherLAB.B};

    // We find the difference between the colors...
    var deltaE = DeltaE.getDeltaE94(thisColor, otherColor);
    return (deltaE < tolerance);
};

/**
 * getLAB
 * ------
 * Returns LAB values for this color in an object like:
 * {L: number, A: number, B: number}
 */
Color.prototype.getLAB = function (){
    // We may have the LAB values cached...
    if(this._lab !== null) {
        return this._lab;
    }

    // We need to calculate the LAB values...
    var r = this.r / 255.0;
    var g = this.g / 255.0;
    var b = this.b / 255.0;

    r = (r > 0.04045) ? Math.pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
    g = (g > 0.04045) ? Math.pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
    b = (b > 0.04045) ? Math.pow((b + 0.055) / 1.055, 2.4) : b / 12.92;

    var x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;
    var y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;
    var z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;

    x = (x > 0.008856) ? Math.pow(x, 1/3) : (7.787 * x) + 16/116;
    y = (y > 0.008856) ? Math.pow(y, 1/3) : (7.787 * y) + 16/116;
    z = (z > 0.008856) ? Math.pow(z, 1/3) : (7.787 * z) + 16/116;

    this._lab = {
        L: (116 * y) - 16,
        A: 500 * (x - y),
        B: 200 * (y - z)
    };
    return this._lab;
};





