package gardening.v3;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.Arrays;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

import gardening.MyStringUtils;
import gardening.v3.flowers.SunflowerPlant;
import gardening.v3.flowers.RosePlant;

public class GardeningV3Test {
    private ByteArrayOutputStream outBytes = new ByteArrayOutputStream();
    private PrintStream testOutPrint = new PrintStream(outBytes, true);
    private PrintStream originalOut = System.out;

    private int numRoses = 0;
    private int numSuns = 0;
    private int numRosesHarvestable = 0;
    private int numSunsHarvestable = 0;
    private int numMutatedRosesHarvestable = 0;
    private int numMutatedSunsHarvestable = 0;
    private int numMessagesRead = 0;
    private int numSeeds = 0;
    private int numFlowersToSell = 0;
    private int balance = 0;

    private FlowerSystem flowerSystem = new FlowerSystem();

    // maybe should just also have the system return a display string
    // benefit of checking stdout is making sure the system actually does what we
    // expect rather than checking subcomponents...end to end

    public String displayString() {
        StringBuilder builder = new StringBuilder();
        builder.append("Number of roses in storage: " + numRoses).append(System.lineSeparator());
        builder.append("Number of sunflowers in storage: " + numSuns).append(System.lineSeparator());
        builder.append("Number of roses to harvest: " + numRosesHarvestable).append(System.lineSeparator());
        builder.append("Number of sunflowers to harvest: " + numSunsHarvestable).append(System.lineSeparator());
        builder.append("Number of mutated roses to harvest: " + numMutatedRosesHarvestable)
                .append(System.lineSeparator());
        builder.append("Number of mutated sunflowers to harvest: " + numMutatedSunsHarvestable)
                .append(System.lineSeparator());
        builder.append("Number of messages read: " + numMessagesRead).append(System.lineSeparator());
        builder.append("Number of seeds: " + numSeeds).append(System.lineSeparator());
        builder.append("Number of flowers to sell: " + numFlowersToSell).append(System.lineSeparator());
        builder.append("Net worth: " + balance);
        return builder.toString();
    }

    @BeforeEach
    public void setupRedirect() {
        System.setOut(testOutPrint);
    }

    @BeforeEach
    public void resetState() {
        numRoses = 0;
        numSuns = 0;
        numRosesHarvestable = 0;
        numSunsHarvestable = 0;
        numMessagesRead = 0;
        numSeeds = 0;
        numFlowersToSell = 0;
        balance = 0;

        flowerSystem = new FlowerSystem();
    }

    @AfterEach
    public void cleanRedirect() {
        System.setOut(originalOut);
        outBytes.reset(); // reset the output bytes
    }

    @Test
    public void testInitialState() {
        flowerSystem.command("d");
        assertEquals(displayString(), outBytes.toString().trim());
    }

    @Test
    public void testShipment() {
        flowerSystem.command("tick"); // progresses time
        flowerSystem.command("d");
        numSuns++;
        // trim gets rid of trailing new line
        assertEquals(displayString(), outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("tick");
        flowerSystem.command("d");
        numRoses++;
        numSuns++;
        assertEquals(displayString(), outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("tick");
        flowerSystem.command("d");
        numSuns++;
        assertEquals(displayString(), outBytes.toString().trim());
    }

    @Test
    public void testPlantNoRoses() {
        flowerSystem.command("plant rose");
        assertEquals("You have no roses to be planted", outBytes.toString().trim());
    }

    @Test
    public void testPlantNoSunflowers() {
        flowerSystem.command("plant sunflower");
        assertEquals("You have no sunflowers to be planted", outBytes.toString().trim());
    }

    @Test
    public void testPlantRose() {
        flowerSystem.command("tick");
        numSuns++;
        flowerSystem.command("tick"); // gain rose
        numRoses++;
        numSuns++;

        outBytes.reset();
        flowerSystem.command("d");
        assertEquals(displayString(), outBytes.toString().trim());

        outBytes.reset();
        flowerSystem.command("plant rose");
        numRoses--;
        flowerSystem.command("d");
        assertEquals(displayString(), outBytes.toString().trim());
    }

    @Test
    public void testPlantSunflower() {
        flowerSystem.command("tick"); // gain sunflower
        numSuns++;

        flowerSystem.command("d");
        assertEquals(displayString(), outBytes.toString().trim());

        outBytes.reset();
        flowerSystem.command("plant sunflower");
        numSuns--;
        flowerSystem.command("d");
        assertEquals(displayString(), outBytes.toString().trim());
    }

    @Test
    public void testPlantRoseExceed() {
        // should get 2 roses
        for (int i = 0; i < 4; i++)
            flowerSystem.command("tick");

        numRoses += 2;
        numSuns += 4;

        // try and plant 3 roses
        // haven't implemented numbering for this one yet
        flowerSystem.command("plant rose");
        flowerSystem.command("plant rose");
        flowerSystem.command("plant rose");

        numRoses -= 2;

        assertEquals("You have no roses to be planted", outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @Test
    public void testPlantSunflowerExceed() {
        // should get 2 sunflowers
        for (int i = 0; i < 2; i++)
            flowerSystem.command("tick");

        numRoses += 1;
        numSuns += 2;

        // try and plant 3 sunflowers
        // haven't implemented numbering for this one yet
        flowerSystem.command("plant sunflower");
        flowerSystem.command("plant sunflower");
        flowerSystem.command("plant sunflower");

        numSuns -= 2;

        assertEquals("You have no sunflowers to be planted", outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @Test
    public void testSunflowerHarvest() {
        // int ticksToHarvest = (Sunflower.MAX_SEEDS / Sunflower.SEED_GROWTH);
        // ticksToHarvest += Sunflower.MAX_SEEDS % Sunflower.SEED_GROWTH == 0 ? 0 : 1;

        flowerSystem.command("tick");
        numSuns++;
        flowerSystem.command("tick");
        numSuns++;
        numRoses++;
        flowerSystem.command("tick");
        numSuns++;

        flowerSystem.command("plant sunflower");
        numSuns--;
        flowerSystem.command("tick");
        numSuns++;
        numRoses++;
        flowerSystem.command("plant sunflower");
        numSuns--;
        flowerSystem.command("tick");
        numSuns++;
        flowerSystem.command("tick"); // first sun ready
        numSuns++;
        numRoses++;
        numSunsHarvestable++;

        flowerSystem.command("d");
        assertEquals(displayString(), outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("tick"); // second sun ready
        numSuns++;
        numSunsHarvestable++;
        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("harvest sunflower");
        numSunsHarvestable -= 2;
        numFlowersToSell += 2;
        numSeeds += SunflowerPlant.MAX_SEEDS * 2;

        assertEquals("Harvested: " + SunflowerPlant.MAX_SEEDS * 2 + " Total: " + numSeeds, outBytes.toString().trim());

        outBytes.reset();
        flowerSystem.command("d");
        assertEquals(displayString(), outBytes.toString().trim());
    }

    @ParameterizedTest
    @ValueSource(strings = {
            "sunflower", "rose"
    })
    public void testPackageFlowers(String flowertType) {
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        numSuns = 2;
        numRoses = 1;

        flowerSystem.command("package " + flowertType);

        numFlowersToSell++;
        // ternary wasn't working???
        if (flowertType.equals("sunflower"))
            numSuns--;
        else
            numRoses--;

        assertEquals("Adding " + 1 + " " + (flowertType + "s") + " to be sold", outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @ParameterizedTest
    @ValueSource(strings = {
            "sunflower", "rose"
    })
    public void testPackageFlowersEmpty(String flowertType) {
        flowerSystem.command("package " + flowertType);

        assertEquals("You have no " + (flowertType + "s") + " to be sold", outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @ParameterizedTest
    @ValueSource(strings = {
            "sunflower", "rose"
    })
    public void testPackageFlowersLessThanRequested(String flowertType) {
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        numSuns = 2;
        numRoses = 1;

        int requested = (flowertType.equals("sunflower") ? numSuns : numRoses) + 1;

        flowerSystem.command("package " + flowertType + " " + requested);

        numFlowersToSell = requested - 1;
        if (flowertType.equals("sunflower"))
            numSuns -= numFlowersToSell;
        else
            numRoses -= numFlowersToSell;

        assertEquals("You have less " + (flowertType + "s") + " than requested. Adding all to be sold",
                outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @ParameterizedTest
    @ValueSource(strings = {
            "sunflower", "rose"
    })
    public void testPackageFlowersRequestFulfilled(String flowertType) {
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        numSuns = 6;
        numRoses = 3;

        int requested = (flowertType.equals("sunflower") ? numSuns : numRoses) - 1;

        flowerSystem.command("package " + flowertType + " " + requested);

        numFlowersToSell = requested;
        if (flowertType.equals("sunflower"))
            numSuns -= numFlowersToSell;
        else
            numRoses -= numFlowersToSell;

        assertEquals("Adding " + numFlowersToSell + " " + (flowertType + "s") + " to be sold",
                outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @Test
    public void testLog() {
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");

        flowerSystem.command("plant rose");
        flowerSystem.command("plant rose");

        // roses ready to harvest after reveal duration ticks. in this case 3...make
        // constant public???

        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");

        numSuns = 7;
        numRoses = 1;

        flowerSystem.command("harvest rose");
        numMessagesRead = 2;

        numFlowersToSell = 2;

        // printed messages to terminal
        // assumes the harvest printed the messages
        String messageLog = outBytes.toString().trim();

        outBytes.reset();

        outBytes.reset();

        String[] messages = messageLog.split(System.lineSeparator());
        for (String message : messages)
            assertTrue(Arrays.asList(RosePlant.MESSAGES).contains(message));

        flowerSystem.command("log");

        assertEquals(messageLog, outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());

    }

    @ParameterizedTest
    @ValueSource(strings = {
            "rose", "sunflower"
    })
    public void mutateNoFlower(String flowerType) {
        flowerSystem.command("mutate " + flowerType);

        assertEquals("You have no " + (flowerType + "s") + " to be mutated", outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @ParameterizedTest
    @ValueSource(strings = {
            "", "1", "2", "3"
    })
    public void mutateSunflower(String amount) {
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        numSuns = 2;
        numRoses = 1;

        flowerSystem.command("mutate sunflower " + amount);
        // rather than doing computation, could probably just supply multiple parameters
        int actualAmount = Math.min((amount.equals("") ? 1 : Integer.parseInt(amount)), numSuns);
        numSuns -= actualAmount;

        if (!amount.equals("3"))
            assertEquals("Adding " + actualAmount + " sunflowers to be mutated", outBytes.toString().trim());
        else
            assertEquals("You have less " + ("sunflower" + "s") + " than requested. Adding all to be mutated",
                    outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
        outBytes.reset();

        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        numSuns += 3;
        numRoses += 1;
        numMutatedSunsHarvestable += actualAmount;

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
        outBytes.reset();

        flowerSystem.command("harvest sunflower mutation");
        numSeeds = 7 * 5 * actualAmount;
        numMutatedSunsHarvestable -= actualAmount;
        numFlowersToSell += actualAmount;

        assertEquals("Harvested: " + 7 * 5 * actualAmount + " Total: " + numSeeds, outBytes.toString().trim());
        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

    @ParameterizedTest
    @ValueSource(strings = {
            "", "1", "2", "3"
    })
    public void mutateRose(String amount) {
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        numSuns = 4;
        numRoses = 2;

        flowerSystem.command("mutate rose " + amount);
        // rather than doing computation, could probably just supply multiple parameters
        int actualAmount = Math.min((amount.equals("") ? 1 : Integer.parseInt(amount)), numRoses);
        numRoses -= actualAmount;

        if (!amount.equals("3"))
            assertEquals("Adding " + actualAmount + " roses to be mutated", outBytes.toString().trim());
        else
            assertEquals("You have less " + ("rose" + "s") + " than requested. Adding all to be mutated",
                    outBytes.toString().trim());

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
        outBytes.reset();

        flowerSystem.command("tick");
        flowerSystem.command("tick");
        flowerSystem.command("tick");
        numSuns += 3;
        numRoses += 1;
        numMutatedRosesHarvestable += actualAmount;

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
        outBytes.reset();

        flowerSystem.command("harvest rose mutation");
        numMutatedRosesHarvestable -= actualAmount;
        numFlowersToSell += actualAmount;
        numMessagesRead += actualAmount;

        String[] lines = outBytes.toString().trim().split(System.lineSeparator());

        assertTrue(lines.length > 0);

        for (String line : lines) {
            // should really just use a regex capture probably
            String[] parts = line.split(" Original Message: ");
            parts[0] = parts[0].replaceFirst("Mutated Message: ", "");
            assertTrue(MyStringUtils.numDifferences(parts[0], parts[1]) == 1);

            assertTrue(Arrays.asList(RosePlant.MESSAGES).contains(parts[1]));
        }

        outBytes.reset();

        flowerSystem.command("d");

        assertEquals(displayString(), outBytes.toString().trim());
    }

}
