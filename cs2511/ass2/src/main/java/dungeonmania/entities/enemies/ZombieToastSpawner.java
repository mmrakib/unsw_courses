package dungeonmania.entities.enemies;

import dungeonmania.Game;
import dungeonmania.entities.Entity;
import dungeonmania.entities.Interactable;
import dungeonmania.entities.Player;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class ZombieToastSpawner extends Entity implements Interactable {
    public static final int DEFAULT_SPAWN_INTERVAL = 0;

    public ZombieToastSpawner(Position position, int spawnInterval) {
        super(position);
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return true;
    }

    public void spawn(Game game) {
        game.getEntityFactory().spawnZombie(game, this);
    }

    @Override
    public void onDestroy(GameMap map) {
        Game g = map.getGame();
        Player p = g.getPlayer();
        p.setSpawnersKilled(p.getSpawnersKilled() + 1);
        g.unsubscribe(getId());
    }

    @Override
    public void interact(Player player, Game game) {
        player.getInventory().getWeapon().use(game);
    }

    @Override
    public boolean isInteractable(Player player) {
        return Position.isAdjacent(player.getPosition(), getPosition()) && player.hasWeapon();
    }

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        if (entity instanceof Player) {
            onDestroy(map);
        }
    }

    @Override
    public void onMovedAway(GameMap map, Entity entity) {
        return;
    }
}
