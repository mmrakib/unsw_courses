package dungeonmania.entities.inventory;

import java.util.List;

import dungeonmania.entities.collectables.Arrow;
import dungeonmania.entities.collectables.Key;
import dungeonmania.entities.collectables.Treasure;
import dungeonmania.entities.collectables.Wood;

public class CraftingService {
    private Inventory inventory;

    public CraftingService(Inventory inventory) {
        this.inventory = inventory;
    }

    public boolean canBuildBow() {
        int wood = inventory.count(Wood.class);
        int arrows = inventory.count(Arrow.class);

        return (wood >= 1 && arrows >= 3) ? true : false;
    }

    public boolean canBuildShield() {
        int wood = inventory.count(Wood.class);
        int treasure = inventory.count(Treasure.class);
        int keys = inventory.count(Key.class);

        return (wood >= 2 && (treasure >= 1 || keys >= 1)) ? true : false;
    }

    public void buildBow() {
        List<Wood> wood = inventory.getEntities(Wood.class);
        List<Arrow> arrows = inventory.getEntities(Arrow.class);

        inventory.remove(wood.get(0));
        inventory.remove(arrows.get(0));
        inventory.remove(arrows.get(1));
        inventory.remove(arrows.get(2));
    }

    public void buildShield() {
        List<Wood> wood = inventory.getEntities(Wood.class);
        List<Treasure> treasure = inventory.getEntities(Treasure.class);
        List<Key> keys = inventory.getEntities(Key.class);

        inventory.remove(wood.get(0));
        inventory.remove(wood.get(1));
        if (treasure.size() >= 1) {
            inventory.remove(treasure.get(0));
        } else {
            inventory.remove(keys.get(0));
        }
    }
}
