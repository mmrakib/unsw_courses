package dungeonmania.entities.enemies.movementstrategies;

import dungeonmania.Game;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.util.Position;

public interface MovementStrategy {
    Position getNextMove(Game game, Enemy enemy);
}
