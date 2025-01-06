package dungeonmania.entities;

import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class SwampTile extends Entity {
    private int movementFactor;

    public SwampTile(Position position, int movementFactor) {
        super(position.asLayer(Entity.ITEM_LAYER));
        this.movementFactor = movementFactor;
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return true;
    }

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        if (entity instanceof Moveable) {
            Moveable moveable = (Moveable) entity;
            moveable.setMovementFactor(movementFactor);
        }
    }

    @Override
    public void onMovedAway(GameMap map, Entity entity) {
        if (entity instanceof Moveable) {
            Moveable moveable = (Moveable) entity;
            moveable.setMovementFactor(1);
        }
    }

    @Override
    public void onDestroy(GameMap gameMap) {
        return;
    }
}
