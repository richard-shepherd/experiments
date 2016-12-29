/**
 * GameItem_Ammo (derived from GameItem)
 * -------------
 * Represents ammo.
 *
 * @constructor
 */
function GameItem_Ammo() {
    // We call the base class's constructor...
    GameItem.call(this);
}
Utils.extend(GameItem, GameItem_Ammo); // Derived from GameItem

/**
 * radarInfo
 * ---------
 * Returns info about how to show this item on the radar.
 */
GameItem_Ammo.prototype.radarInfo = function() {
    return {
        showAsCircle: false,
        label: "As"
    };
};
