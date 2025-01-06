package dungeonmania.goals.goalstrategies;

import dungeonmania.Game;
import dungeonmania.entities.Switch;

public class BouldersGoalStrategy implements GoalStrategy {
    @Override
    public boolean achieved(Game game) {
        return game.getMap().getEntities(Switch.class).stream().allMatch(s -> s.isActivated());
    }
}
