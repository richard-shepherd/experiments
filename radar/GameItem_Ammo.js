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

    // Radar info...
    this.radarInfo = {
        showAsCircle: false,
        label: "As"
    };
}
Utils.extend(GameItem, GameItem_Ammo); // Derived from GameItem

