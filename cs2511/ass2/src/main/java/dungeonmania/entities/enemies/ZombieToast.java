package dungeonmania.entities.enemies;

import dungeonmania.Game;
import dungeonmania.entities.collectables.potions.InvincibilityPotion;
import dungeonmania.entities.enemies.movementstrategies.DefaultZombieMovementStrategy;
import dungeonmania.entities.enemies.movementstrategies.InvincibilityPotionMovementStrategy;
import dungeonmania.entities.enemies.movementstrategies.MovementStrategy;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class ZombieToast extends Enemy {
    public static final double DEFAULT_HEALTH = 5.0;
    public static final double DEFAULT_ATTACK = 6.0;

    public ZombieToast(Position position, double health, double attack) {
        super(position, health, attack);
    }

    @Override
    public void move(Game game) {
        Position nextPos;
        GameMap map = game.getMap();

        MovementStrategy movementStrategy;
        if (map.getPlayer().getEffectivePotion() instanceof InvincibilityPotion) {
            movementStrategy = new InvincibilityPotionMovementStrategy();
        } else {
            movementStrategy = new DefaultZombieMovementStrategy();
        }
        nextPos = movementStrategy.getNextMove(game, this);

        if (this.getMovementFactor() > 1) {
            for (int i = 0; i < this.getMovementFactor(); i++) {
                game.tick();
            }
        }

        game.getMap().moveTo(this, nextPos);
    }

}
