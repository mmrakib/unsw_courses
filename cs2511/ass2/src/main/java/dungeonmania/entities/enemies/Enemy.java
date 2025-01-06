package dungeonmania.entities.enemies;

import dungeonmania.Game;
import dungeonmania.battles.BattleStatistics;
import dungeonmania.battles.Battleable;
import dungeonmania.entities.Entity;
import dungeonmania.entities.Player;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;
import dungeonmania.entities.Moveable;

public abstract class Enemy extends Entity implements Battleable, Moveable {
    private BattleStatistics battleStatistics;
    private int movementFactor;

    public Enemy(Position position, double health, double attack) {
        super(position.asLayer(Entity.CHARACTER_LAYER));
        battleStatistics = new BattleStatistics(health, attack, 0, BattleStatistics.DEFAULT_DAMAGE_MAGNIFIER,
                BattleStatistics.DEFAULT_ENEMY_DAMAGE_REDUCER);
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return entity instanceof Player;
    }

    @Override
    public BattleStatistics getBattleStatistics() {
        return battleStatistics;
    }

    public void setBattleStatistics(BattleStatistics battleStatistics) {
        this.battleStatistics = battleStatistics;
    }

    public int getMovementFactor() {
        return movementFactor;
    }

    public void setMovementFactor(int movementFactor) {
        this.movementFactor = movementFactor;
    }

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        if (entity instanceof Player) {
            Player player = (Player) entity;
            map.getGame().battle(player, this);
        }
    }

    @Override
    public void onDestroy(GameMap map) {
        Game g = map.getGame();
        Player p = g.getPlayer();
        p.setEnemiesKilled(p.getEnemiesKilled() + 1);
        g.unsubscribe(getId());
    }

    @Override
    public void onMovedAway(GameMap map, Entity entity) {
        return;
    }

    public abstract void move(Game game);
}
