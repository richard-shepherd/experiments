/**
 * GameItem_Player (derived from GameItem)
 * ---------------
 * Represents a player.
 *
 * @constructor
 */
function GameItem_Player(playerNumber, color) {
    // We call the base class's constructor...
    GameItem.call(this);

    this.playerNumber = playerNumber;
    this.color = color;
}
Utils.extend(GameItem, GameItem_Player); // Derived from GameItem
