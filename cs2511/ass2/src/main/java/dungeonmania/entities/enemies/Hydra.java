package dungeonmania.entities.enemies;

import dungeonmania.Game;
import dungeonmania.battles.BattleStatistics;
import dungeonmania.entities.Entity;
import dungeonmania.entities.Player;
import dungeonmania.entities.collectables.potions.InvincibilityPotion;
import dungeonmania.entities.enemies.movementstrategies.DefaultZombieMovementStrategy;
import dungeonmania.entities.enemies.movementstrategies.InvincibilityPotionMovementStrategy;
import dungeonmania.entities.enemies.movementstrategies.MovementStrategy;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class Hydra extends Enemy {
    public static final double DEFAULT_HEALTH = 15.0;
    public static final double DEFAULT_ATTACK = 10.0;
    public static final double DEFAULT_HEALTH_INCREASE_RATE = 0.4;
    public static final double DEFAULT_HEALTH_INCREASE_AMOUNT = 5;

    private double healthIncreaseRate;
    private double healthIncreaseAmount;

    public Hydra(Position position, double health, double attack, double healthIncreaseRate,
            double healthIncreaseAmount) {
        super(position, health, attack);
        this.healthIncreaseRate = healthIncreaseRate;
        this.healthIncreaseAmount = healthIncreaseAmount;
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

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        if (entity instanceof Player) {
            if (Math.random() > healthIncreaseRate) {
                Player player = (Player) entity;
                map.getGame().battle(player, this);
            } else {
                BattleStatistics battleStatistics = this.getBattleStatistics();
                battleStatistics.setHealth(battleStatistics.getHealth() + healthIncreaseAmount);
                this.setBattleStatistics(battleStatistics);
            }
        }
    }
}
