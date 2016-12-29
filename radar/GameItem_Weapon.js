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

    // Radar info...
    this.radarInfo = {
        showAsCircle: false,
        label: "Wm"
    };
}
Utils.extend(GameItem, GameItem_Weapon); // Derived from GameItem

