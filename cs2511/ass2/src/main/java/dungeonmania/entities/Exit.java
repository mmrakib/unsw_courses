package dungeonmania.entities;

import dungeonmania.util.Position;

public class Exit extends StaticEntity {
    public Exit(Position position) {
        super(position.asLayer(Entity.ITEM_LAYER));
    }
}
