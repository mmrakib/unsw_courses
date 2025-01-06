package bool;

public class OrNode implements BooleanNode {
    private String nodeType;
    private BooleanNode left;
    private BooleanNode right;

    public OrNode(BooleanNode left, BooleanNode right) {
        this.left = left;
        this.right = right;
        this.nodeType = "or";
    }

    public void setLeft(BooleanNode node) {
        this.left = node;
    }

    public void setRight(BooleanNode node) {
        this.right = node;
    }

    @Override
    public boolean evaluate() {
        return (left.evaluate() || right.evaluate());
    }

    @Override
    public String prettyPrint() {
        return String.format("(OR %s %s)", left.prettyPrint(), right.prettyPrint());
    }
}
