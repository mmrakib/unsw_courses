package gardening.v2.flowers;

public class Sunflower extends Flower implements Sellable {
    public final static int MAX_SEEDS = 7;
    public final static int SEED_GROWTH = 3;

    private int numSeeds = 0;
    private int bloom = 0;

    @Override
    public void grow() {
        this.numSeeds = Math.min(MAX_SEEDS, this.numSeeds + SEED_GROWTH);
        this.bloom = numSeeds;
    }

    public int harvestSeeds() {
        int tmpSeeds = numSeeds;
        numSeeds = 0;
        return tmpSeeds;
    }

    @Override
    public boolean canHarvest() {
        return numSeeds == MAX_SEEDS;
    }

    @Override
    public int getPrice() {
        return 15 + bloom;
    }

}
