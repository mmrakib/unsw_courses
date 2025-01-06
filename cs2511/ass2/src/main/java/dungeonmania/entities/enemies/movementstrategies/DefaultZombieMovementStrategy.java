package dungeonmania.entities.enemies.movementstrategies;

import java.util.List;
import java.util.Random;
import java.util.stream.Collectors;

import dungeonmania.Game;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class DefaultZombieMovementStrategy implements MovementStrategy {
    @Override
    public Position getNextMove(Game game, Enemy enemy) {
        Position nextPos;
        GameMap map = game.getMap();
        Random randGen = new Random();

        List<Position> pos = enemy.getPosition().getCardinallyAdjacentPositions();
        pos = pos.stream().filter(p -> map.canMoveTo(enemy, p)).collect(Collectors.toList());
        if (pos.size() == 0) {
            nextPos = enemy.getPosition();
        } else {
            nextPos = pos.get(randGen.nextInt(pos.size()));
        }

        return nextPos;
    }
}
