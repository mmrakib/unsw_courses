package gardening.v3.flowers;

public interface Flower {

    // each time called by the flower box, the flower grows a litte bit
    public abstract void grow();

    // once a flower is fully grown, it can be harvested
    public abstract boolean canHarvest();

    public abstract int getPrice();
}
