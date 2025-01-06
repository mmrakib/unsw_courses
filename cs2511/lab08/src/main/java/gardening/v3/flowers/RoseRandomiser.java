package gardening.v3.flowers;

import java.util.Random;

public class RoseRandomiser extends RoseMutation<Rose> {
    private int replacementIndex;
    private char replacementChar;
    private final static Random rng = new Random();

    public RoseRandomiser(Rose flower) {
        super(flower);
        this.replacementIndex = rng.nextInt(getFlower().revealMessage().length());
        this.replacementChar = (char) ('A' + rng.nextInt(26));
    }

    @Override
    public String revealMessage() {
        String message = super.revealMessage();

        StringBuilder builder = new StringBuilder(message);
        builder.setCharAt(replacementIndex, replacementChar);
        return builder.toString();
    }

    public String revealOriginalMessage() {
        return getFlower().revealMessage();
    }

}
