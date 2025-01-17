package dungeonmania.entities;

import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class StaticEntity extends Entity {
    public StaticEntity(Position position) {
        super(position.asLayer(Entity.ITEM_LAYER));
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return true;
    }

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        // No action
        return;
    }

    @Override
    public void onMovedAway(GameMap map, Entity entity) {
        // No action
        return;
    }

    @Override
    public void onDestroy(GameMap gameMap) {
        // No action
        return;
    }
}
