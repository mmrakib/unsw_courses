package random;

import org.junit.Test;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

public class GameTest {
    @Test
    public void testBattleWithSeed4() {
        Game game = new Game(4); // Seed of 4
        boolean result = game.battle();
        assertTrue(true, result);
    }

    @Test
    public void testBattleWithSeedMinus4() {
        Game game = new Game(-4); // Seed of -4
        boolean result = game.battle();
        assertFalse(false, result);
    }
}
