/**
 * GameItem_Weapon (derived from GameItem)
 * ---------------
 * Represents a weapon.
 *
 * @constructor
 */
function GameItem_Weapon() {
    // We call the base class's constructor...
    GameItem.call(this);
}
Utils.extend(GameItem, GameItem_Weapon); // Derived from GameItem

/**
 * radarInfo
 * ---------
 * Returns info about how to show this item on the radar.
 */
GameItem_Weapon.prototype.radarInfo = function() {
    return {
        showAsCircle: false,
        label: "Wm"
    };
};
