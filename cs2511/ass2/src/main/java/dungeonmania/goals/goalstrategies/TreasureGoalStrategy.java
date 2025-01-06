package dungeonmania.goals.goalstrategies;

import dungeonmania.Game;

public class TreasureGoalStrategy implements GoalStrategy {
    private int target;

    public TreasureGoalStrategy(int target) {
        this.target = target;
    }

    @Override
    public boolean achieved(Game game) {
        return game.getCollectedTreasureCount() >= target;
    }
}
