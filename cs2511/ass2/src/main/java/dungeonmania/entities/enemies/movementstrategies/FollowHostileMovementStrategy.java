package dungeonmania.entities.enemies.movementstrategies;

import dungeonmania.Game;
import dungeonmania.entities.Player;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class FollowHostileMovementStrategy implements MovementStrategy {
    @Override
    public Position getNextMove(Game game, Enemy enemy) {
        Position nextPos;
        GameMap map = game.getMap();
        Player player = game.getPlayer();

        nextPos = map.dijkstraPathFind(enemy.getPosition(), player.getPosition(), enemy);

        return nextPos;
    }
}
