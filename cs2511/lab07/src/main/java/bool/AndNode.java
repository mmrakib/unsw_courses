package bool;

public class AndNode implements BooleanNode {
    private String nodeType;
    private BooleanNode left;
    private BooleanNode right;

    public AndNode(BooleanNode left, BooleanNode right) {
        this.left = left;
        this.right = right;
        this.nodeType = "and";
    }

    public void setLeft(BooleanNode node) {
        this.left = node;
    }

    public void setRight(BooleanNode node) {
        this.right = node;
    }

    @Override
    public boolean evaluate() {
        return (left.evaluate() && right.evaluate());
    }

    @Override
    public String prettyPrint() {
        return String.format("(AND %s %s)", left.prettyPrint(), right.prettyPrint());
    }
}
