package dungeonmania.goals;

import dungeonmania.Game;
import dungeonmania.goals.goalstrategies.GoalStrategy;
import dungeonmania.goals.goalstrategies.ExitGoalStrategy;
import dungeonmania.goals.goalstrategies.BouldersGoalStrategy;
import dungeonmania.goals.goalstrategies.EnemyGoalStrategy;
import dungeonmania.goals.goalstrategies.TreasureGoalStrategy;

public class Goal {
    private String type;
    private int target;
    private Goal goal1;
    private Goal goal2;

    public Goal(String type) {
        this.type = type;
    }

    public Goal(String type, int target) {
        this.type = type;
        this.target = target;
    }

    public Goal(String type, Goal goal1, Goal goal2) {
        this.type = type;
        this.goal1 = goal1;
        this.goal2 = goal2;
    }

    /**
     * @return true if the goal has been achieved, false otherwise
     */
    public boolean achieved(Game game) {
        if (game.getPlayer() == null)
            return false;

        GoalStrategy goalStrategy;
        switch (type) {
        case "exit":
            goalStrategy = new ExitGoalStrategy();
            return goalStrategy.achieved(game);
        case "boulders":
            goalStrategy = new BouldersGoalStrategy();
            return goalStrategy.achieved(game);
        case "treasure":
            goalStrategy = new TreasureGoalStrategy(target);
            return goalStrategy.achieved(game);
        case "enemies":
            goalStrategy = new EnemyGoalStrategy(target);
            return goalStrategy.achieved(game);
        case "AND":
            return goal1.achieved(game) && goal2.achieved(game);
        case "OR":
            return goal1.achieved(game) || goal2.achieved(game);
        default:
            break;
        }

        return false;
    }

    public String toString(Game game) {
        if (this.achieved(game))
            return "";
        switch (type) {
        case "exit":
            return ":exit";
        case "boulders":
            return ":boulders";
        case "treasure":
            return ":treasure";
        case "enemies":
            return ":enemies";
        case "AND":
            return "(" + goal1.toString(game) + " AND " + goal2.toString(game) + ")";
        case "OR":
            if (achieved(game))
                return "";
            else
                return "(" + goal1.toString(game) + " OR " + goal2.toString(game) + ")";
        default:
            return "";
        }
    }

}
