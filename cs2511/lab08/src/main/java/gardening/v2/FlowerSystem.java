package gardening.v2;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

import gardening.v2.flowers.FlowerBox;
import gardening.v2.flowers.FlowerSeedling;
import gardening.v2.flowers.Rose;
import gardening.v2.flowers.RoseSeedling;
import gardening.v2.flowers.Sellable;
import gardening.v2.flowers.Sunflower;
import gardening.v2.flowers.SunflowerSeedling;

public class FlowerSystem {
    private final static int ROSE_SHIPMENT_DELAY = 2;

    private boolean suppression = false;

    private List<RoseSeedling> roseSeeds = new ArrayList<>();
    private List<SunflowerSeedling> sunflowerSeeds = new ArrayList<>();

    // TODO for students: Add in appropriate generic parameter
    private FlowerBox<> roseBox = new FlowerBox<>();
    // TODO for students: Add in appropriate generic parameter
    private FlowerBox<> sunflowerBox = new FlowerBox<>();

    private List<Sellable> flowersToSell = new ArrayList<>();

    private int balance = 0;
    private int numSunflowerSeeds = 0;
    private List<String> readMessages = new ArrayList<>();
    private int roseShipmentPeriod = 0;

    public void newShipment() {
        roseShipmentPeriod = (roseShipmentPeriod + 1) % ROSE_SHIPMENT_DELAY;

        if (roseShipmentPeriod == 0)
            roseSeeds.add(new RoseSeedling());

        sunflowerSeeds.add(new SunflowerSeedling());
    }

    public void sellFlowers() {
        balance += flowersToSell.stream().mapToInt(flower -> flower.getPrice()).sum();
        flowersToSell.clear();
    }

    public void tick() {
        this.newShipment();
        roseBox.growFlowers();
        sunflowerBox.growFlowers();
    }

    private void displayInfo() {
        System.out.println("Number of rose seedlings in storage: " + roseSeeds.size());
        System.out.println("Number of sunflower seedlings in storage: " + sunflowerSeeds.size());
        System.out.println("Number of roses to harvest: " + roseBox.numHarvestable());
        System.out.println("Number of sunflowers to harvest: " + sunflowerBox.numHarvestable());
        System.out.println("Number of messages read: " + readMessages.size());
        System.out.println("Number of seeds: " + numSunflowerSeeds);
        System.out.println("Number of flowers to sell: " + flowersToSell.size());
        System.out.println("Net worth: " + balance);
    }

    private void printMessageLog() {
        if (readMessages.size() == 0)
            System.out.println("No messages");
        else
            readMessages.forEach(message -> System.out.println(message));
    }

    private void routePackage(String[] args) {
        int numToSell = 1;
        if (args.length > 1)
            numToSell = Integer.parseInt(args[1]);

        List<? extends FlowerSeedling<?>> chosenFlowers;

        switch (args[0]) {
        case "rose":
            chosenFlowers = roseSeeds;
            break;
        case "sunflower":
            chosenFlowers = sunflowerSeeds;
            break;
        default:
            System.out.println("Parsing error");
            return;
        }

        if (chosenFlowers.size() == 0) {
            System.out.println("You have no " + (args[0] + "s") + " to be sold");
        } else if (numToSell > chosenFlowers.size()) {
            System.out.println("You have less " + (args[0] + "s") + " than requested. Adding all to be sold");
            flowersToSell.addAll(chosenFlowers);
            chosenFlowers.clear();
        } else {
            System.out.println("Adding " + numToSell + " " + (args[0] + "s") + " to be sold");
            flowersToSell.addAll(chosenFlowers.subList(0, numToSell));
            chosenFlowers.subList(0, numToSell).clear();
        }
    }

    private void routePlant(String[] args) {
        switch (args[0]) {
        case "rose":

            if (roseSeeds.size() > 0) {
                roseBox.addFlower(roseSeeds.get(0));
                roseSeeds.remove(0);
            } else {
                System.out.println("You have no roses to be planted");
            }

            break;

        case "sunflower":

            if (sunflowerSeeds.size() > 0) {
                sunflowerBox.addFlower(sunflowerSeeds.get(0));
                sunflowerSeeds.remove(0);
            } else {
                System.out.println("You have no sunflowers to be planted");
            }

            break;

        default:
            System.out.println("Parsing error");
            break;
        }
    }

    private void routeHarvest(String[] args) {
        switch (args[0]) {
        case "rose":
            List<Rose> harvestedRoses = roseBox.harvestFlowers();
            harvestedRoses.forEach((rose) -> {
                System.out.println(rose.revealMessage());
                readMessages.add(rose.revealMessage());
            });

            flowersToSell.addAll(harvestedRoses);

            break;

        case "sunflower":
            List<Sunflower> harvestedSunflowers = sunflowerBox.harvestFlowers();
            int harvested = harvestedSunflowers.stream().mapToInt((sunflower) -> sunflower.harvestSeeds()).sum();
            this.numSunflowerSeeds += harvested;
            System.out.println("Harvested: " + harvested + " Total: " + this.numSunflowerSeeds);

            flowersToSell.addAll(harvestedSunflowers);

            break;

        default:
            System.out.println("Parsing error");
            break;
        }
    }

    private boolean command(String[] args) {
        boolean running = true;

        switch (args[0]) {
        case "plant":
            routePlant(Arrays.copyOfRange(args, 1, args.length));
            break;
        case "harvest":
            routeHarvest(Arrays.copyOfRange(args, 1, args.length));
            break;
        case "tick":
            tick();
            break;
        case "log":
            printMessageLog();
            break;
        case "package":
            routePackage(Arrays.copyOfRange(args, 1, args.length));
            break;
        case "sell":
            sellFlowers();
            break;
        case "mutate":
            System.out.println(
                    "Mutation not supported in this package. You might have meant to run the code in the other package");
            break;
        case "s":
            suppression = !suppression;
            break;
        case "d":
            displayInfo();
            break;
        case "q":
            running = false;
            break;
        default:
            System.err.println("Parsing error");
            break;
        }

        return running;
    }

    public boolean command(String commandString) {
        String[] args = commandString.split(" ");
        return command(args);
    }

    public void run() {
        displayInfo();
        System.out.println();

        System.out.print("Enter command: ");
        Scanner scanner = new Scanner(System.in);

        while (command(scanner.nextLine())) {
            if (!suppression) {
                System.out.println();
                displayInfo();
                System.out.println();
            }

            System.out.print("Enter command: ");
        }

        scanner.close();

    }

    public static void main(String[] args) {
        FlowerSystem flowerSystem = new FlowerSystem();

        flowerSystem.run();
    }

}
