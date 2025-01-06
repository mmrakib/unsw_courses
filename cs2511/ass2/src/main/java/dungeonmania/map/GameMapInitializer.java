package dungeonmania.map;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import dungeonmania.Game;
import dungeonmania.entities.Portal;
import dungeonmania.entities.Switch;
import dungeonmania.entities.collectables.Bomb;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.entities.enemies.ZombieToastSpawner;
import dungeonmania.util.Position;

public class GameMapInitializer {
    public static void initialise(Game game, GameMap gameMap) {
        initRegisterBombsAndSwitches(gameMap);
        initPairPortals(gameMap);
        initRegisterMovables(game, gameMap);
        initRegisterSpawners(game, gameMap);
    }

    private static void initRegisterBombsAndSwitches(GameMap gameMap) {
        List<Bomb> bombs = gameMap.getEntities(Bomb.class);
        List<Switch> switchs = gameMap.getEntities(Switch.class);
        for (Bomb b : bombs) {
            for (Switch s : switchs) {
                if (Position.isAdjacent(b.getPosition(), s.getPosition())) {
                    b.subscribe(s);
                    s.subscribe(b);
                }
            }
        }
    }

    // Pair up portals if there's any
    private static void initPairPortals(GameMap gameMap) {
        NodeRegistry nodeRegistry = gameMap.getNodeRegistry();
        Map<Position, GraphNode> nodes = nodeRegistry.getNodes();
        Map<String, Portal> portalsMap = new HashMap<>();
        nodes.forEach((k, v) -> {
            v.getEntities().stream().filter(Portal.class::isInstance).map(Portal.class::cast).forEach(portal -> {
                String color = portal.getColor();
                if (portalsMap.containsKey(color)) {
                    portal.bind(portalsMap.get(color));
                } else {
                    portalsMap.put(color, portal);
                }
            });
        });
    }

    private static void initRegisterMovables(Game game, GameMap gameMap) {
        List<Enemy> enemies = gameMap.getEntities(Enemy.class);
        enemies.forEach(e -> {
            game.register(() -> e.move(game), Game.AI_MOVEMENT, e.getId());
        });
    }

    private static void initRegisterSpawners(Game game, GameMap gameMap) {
        List<ZombieToastSpawner> zts = gameMap.getEntities(ZombieToastSpawner.class);
        zts.forEach(e -> {
            game.register(() -> e.spawn(game), Game.AI_MOVEMENT, e.getId());
        });
        game.register(() -> game.getEntityFactory().spawnSpider(game), Game.AI_MOVEMENT, "spawnSpiders");
    }
}
