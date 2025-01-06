package dungeonmania.entities.enemies;

import dungeonmania.Game;
import dungeonmania.battles.BattleStatistics;
import dungeonmania.entities.Entity;
import dungeonmania.entities.Interactable;
import dungeonmania.entities.Player;
import dungeonmania.entities.collectables.Treasure;
import dungeonmania.entities.collectables.potions.InvincibilityPotion;
import dungeonmania.entities.collectables.potions.InvisibilityPotion;
import dungeonmania.entities.enemies.movementstrategies.FollowHostileMovementStrategy;
import dungeonmania.entities.enemies.movementstrategies.InvincibilityPotionMovementStrategy;
import dungeonmania.entities.enemies.movementstrategies.InvisibilityPotionMovementStrategy;
import dungeonmania.entities.enemies.movementstrategies.MovementStrategy;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class Mercenary extends Enemy implements Interactable {
    public static final int DEFAULT_BRIBE_AMOUNT = 1;
    public static final int DEFAULT_BRIBE_RADIUS = 1;
    public static final double DEFAULT_ATTACK = 5.0;
    public static final double DEFAULT_HEALTH = 10.0;

    private int bribeAmount = Mercenary.DEFAULT_BRIBE_AMOUNT;
    private int bribeRadius = Mercenary.DEFAULT_BRIBE_RADIUS;

    private double allyAttack;
    private double allyDefence;
    private boolean allied = false;
    private boolean isAdjacentToPlayer = false;

    public Mercenary(Position position, double health, double attack, int bribeAmount, int bribeRadius,
            double allyAttack, double allyDefence) {
        super(position, health, attack);
        this.bribeAmount = bribeAmount;
        this.bribeRadius = bribeRadius;
        this.allyAttack = allyAttack;
        this.allyDefence = allyDefence;
    }

    public boolean isAllied() {
        return allied;
    }

    public void setIsAllied(boolean allied) {
        this.allied = allied;
    }

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        if (allied)
            return;
        super.onOverlap(map, entity);
    }

    /**
     * check whether the current merc can be bribed
     * @param player
     * @return
     */
    protected boolean canBeBribed(Player player) {
        return bribeRadius >= 0 && player.countEntityOfType(Treasure.class) >= bribeAmount;
    }

    /**
     * bribe the merc
     */
    protected void bribe(Player player) {
        for (int i = 0; i < bribeAmount; i++) {
            player.use(Treasure.class);
        }
    }

    public boolean getIsAdjacentToPlayer() {
        return isAdjacentToPlayer;
    }

    public void setIsAdjacentToPlayer(boolean isAdjacentToPlayer) {
        this.isAdjacentToPlayer = isAdjacentToPlayer;
    }

    @Override
    public void interact(Player player, Game game) {
        allied = true;
        bribe(player);
        if (!isAdjacentToPlayer && Position.isAdjacent(player.getPosition(), getPosition()))
            isAdjacentToPlayer = true;
    }

    @Override
    public void move(Game game) {
        Position nextPos;
        Player player = game.getPlayer();
        GameMap map = game.getMap();

        MovementStrategy movementStrategy;
        if (allied) {
            nextPos = isAdjacentToPlayer ? player.getPreviousDistinctPosition()
                    : map.dijkstraPathFind(getPosition(), player.getPosition(), this);
            if (!isAdjacentToPlayer && Position.isAdjacent(player.getPosition(), nextPos))
                isAdjacentToPlayer = true;
        } else if (map.getPlayer().getEffectivePotion() instanceof InvisibilityPotion) {
            // Move random
            movementStrategy = new InvisibilityPotionMovementStrategy();
            nextPos = movementStrategy.getNextMove(game, this);
        } else if (map.getPlayer().getEffectivePotion() instanceof InvincibilityPotion) {
            // Move while invincible
            movementStrategy = new InvincibilityPotionMovementStrategy();
            nextPos = movementStrategy.getNextMove(game, this);
        } else {
            // Follow hostile
            movementStrategy = new FollowHostileMovementStrategy();
            nextPos = movementStrategy.getNextMove(game, this);
        }

        if (this.getMovementFactor() > 1) {
            for (int i = 0; i < this.getMovementFactor(); i++) {
                game.tick();
            }
        }

        map.moveTo(this, nextPos);
    }

    @Override
    public boolean isInteractable(Player player) {
        return !allied && canBeBribed(player);
    }

    @Override
    public BattleStatistics getBattleStatistics() {
        if (!allied)
            return super.getBattleStatistics();
        return new BattleStatistics(0, allyAttack, allyDefence, 1, 1);
    }
}
