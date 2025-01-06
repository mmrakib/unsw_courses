package dungeonmania.mvp;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;

import dungeonmania.DungeonManiaController;
import dungeonmania.exceptions.InvalidActionException;
import dungeonmania.response.models.DungeonResponse;
import dungeonmania.util.Direction;
import dungeonmania.util.Position;

public class AssassinTest {
    private Position getAssassinPos(DungeonResponse res) {
        return TestUtils.getEntities(res, "assassin").get(0).getPosition();
    }

    @Test
    @Tag("16-1")
    @DisplayName("Test assassin bribe success")
    public void testAssassinBribeSuccess() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_assassinTest_bribeSuccess", "c_assassinTest_bribeSuccess");

        String assassinId = TestUtils.getEntitiesStream(res, "assassin").findFirst().get().getId();

        // pick up first treasure
        res = dmc.tick(Direction.RIGHT);
        assertEquals(1, TestUtils.getInventory(res, "treasure").size());
        assertEquals(new Position(7, 1), getAssassinPos(res));

        // attempt bribe
        assertThrows(InvalidActionException.class, () -> dmc.interact(assassinId));
        assertEquals(1, TestUtils.getInventory(res, "treasure").size());

        // pick up second treasure
        res = dmc.tick(Direction.RIGHT);
        assertEquals(2, TestUtils.getInventory(res, "treasure").size());
        assertEquals(new Position(6, 1), getAssassinPos(res));

        // attempt bribe
        assertThrows(InvalidActionException.class, () -> dmc.interact(assassinId));
        assertEquals(2, TestUtils.getInventory(res, "treasure").size());

        // pick up third treasure
        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.getInventory(res, "treasure").size());
        assertEquals(new Position(5, 1), getAssassinPos(res));

        // achieve bribe
        res = assertDoesNotThrow(() -> dmc.interact(assassinId));
        assertEquals(0, TestUtils.getInventory(res, "treasure").size());
    }

    @Test
    @Tag("16-2")
    @DisplayName("Testing assassin can be bribed within a radius")
    public void testAssassinBribeRadius() {
        //                                         Wall     Wall    Wall    Wall  Wall
        // P1       P2/Treasure      P3    P4      M4       M3       M2     M1    Wall
        //                                         Wall     Wall    Wall    Wall  Wall
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_assassinTest_bribeRadius", "c_assassinTest_bribeRadius");

        String assassinId = TestUtils.getEntitiesStream(res, "y").findFirst().get().getId();

        // pick up treasure
        res = dmc.tick(Direction.RIGHT);
        assertEquals(1, TestUtils.getInventory(res, "treasure").size());
        assertEquals(new Position(7, 1), getAssassinPos(res));

        // attempt bribe
        assertDoesNotThrow(() -> dmc.interact(assassinId));
        assertEquals(1, TestUtils.getInventory(res, "treasure").size());
    }

    @Test
    @Tag("16-3")
    @DisplayName("Testing an allied assassin does not battle the player")
    public void allyBattle() {
        //                                  Wall    Wall    Wall
        // P1       P2/Treasure      .      M2      M1      Wall
        //                                  Wall    Wall    Wall
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_assassinTest_allyBattle", "c_assassinTest_allyBattle");

        String assassinId = TestUtils.getEntitiesStream(res, "assassin").findFirst().get().getId();

        // pick up treasure
        res = dmc.tick(Direction.RIGHT);
        assertEquals(1, TestUtils.getInventory(res, "treasure").size());

        // achieve bribe
        res = assertDoesNotThrow(() -> dmc.interact(assassinId));
        assertEquals(0, TestUtils.getInventory(res, "treasure").size());

        // walk into assassin, a battle does not occur
        res = dmc.tick(Direction.RIGHT);
        assertEquals(0, res.getBattles().size());
    }
}
