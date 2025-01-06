package dungeonmania.goals.goalstrategies;

import dungeonmania.Game;
import dungeonmania.map.GameMap;
import dungeonmania.entities.Player;
import dungeonmania.entities.enemies.ZombieToastSpawner;

public class EnemyGoalStrategy implements GoalStrategy {
    private int numEnemiesToKill;

    public EnemyGoalStrategy(int numEnemiesToKill) {
        this.numEnemiesToKill = numEnemiesToKill;
    }

    @Override
    public boolean achieved(Game game) {
        GameMap map = game.getMap();
        Player player = game.getPlayer();

        int totalSpawners = map.getEntities(ZombieToastSpawner.class).size();

        return (player.getEnemiesKilled() >= numEnemiesToKill && player.getSpawnersKilled() >= totalSpawners) ? true
                : false;
    }
}
