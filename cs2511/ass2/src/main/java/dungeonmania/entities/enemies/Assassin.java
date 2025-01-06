package dungeonmania.entities.enemies;

import dungeonmania.Game;
import dungeonmania.entities.Player;
import dungeonmania.util.Position;

public class Assassin extends Mercenary {
    public static final double DEFAULT_ATTACK = 15.0;
    public static final double DEFAULT_BRIBE_FAIL_RATE = 0.4;

    private double bribeFailRate;

    public Assassin(Position position, double health, double attack, int bribeAmount, int bribeRadius,
            double allyAttack, double allyDefence, double bribeFailRate) {
        super(position, health, attack, bribeAmount, bribeRadius, allyAttack, allyDefence);
        this.bribeFailRate = bribeFailRate;
    }

    @Override
    public void interact(Player player, Game game) {
        if (Math.random() > bribeFailRate) {
            this.setIsAllied(true);
        }
        bribe(player);

        if (!this.getIsAdjacentToPlayer() && Position.isAdjacent(player.getPosition(), getPosition()))
            this.setIsAdjacentToPlayer(true);
    }

    public double getBribeFailRate() {
        return bribeFailRate;
    }

    public void setBribeFailRate(double bribeFailRate) {
        this.bribeFailRate = bribeFailRate;
    }
}
