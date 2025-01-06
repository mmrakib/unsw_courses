package dungeonmania.mvp;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;

import dungeonmania.DungeonManiaController;
import dungeonmania.response.models.DungeonResponse;

public class SwampTileTest {
    @Test
    @Tag("17-1")
    @DisplayName("Test the player can not move across a swamp")
    public void testPlayerMovement() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_swampTileTest_testPlayerMovement", "c_swampTileTest_testPlayerMovement");

    }

    @Test
    @Tag("17-2")
    @DisplayName("Test the enemy can move across a swamp")
    public void testEnemyMovement() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_swampTileTest_testEnemyMovement", "c_swampTileTest_testEnemyMovement");

    }

    @Test
    @Tag("17-3")
    @DisplayName("Test mercenary correctly follows a player")
    public void testMercenaryMovement() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_swampTileTest_testMercenaryMovement",
                "c_swampTileTest_testMercenaryMovement");
    }
}
