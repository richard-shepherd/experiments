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

    // The item type...
    this.itemType = GameItem.ItemType.UNKNOWN;
}

/**
 * ItemType
 * --------
 * An enum for different types of item.
 */
GameItem.ItemType = {
    UNKNOWN: 0,
    PLAYER: 1,
    ITEM: 2
};
