package bool;

public class NotNode implements BooleanNode {
    private String nodeType;
    private BooleanNode expr;

    public OrNode(BooleanNode expr) {
        this.expr = expr;
        this.nodeType = "not";
    }

    public void setNode(BooleanNode node) {
        this.expr = node;
    }

    @Override
    public boolean evaluate() {
        return (!expr.evaluate());
    }

    @Override
    public String prettyPrint() {
        return String.format("(NOT %s)", expr.prettyPrint());
    }
}
