package bool;

public class VariableNode implements BooleanNode {
    private String nodeType;
    private boolean value;

    public VariableNode(boolean value) {
        this.value = value;
        this.nodeType = "value";
    }

    public void setValue(boolean value) {
        this.value = value;
    }

    @Override
    public boolean evaluate() {
        return value;
    }

    @Override
    public String prettyPrint() {
        return (this.value) ? "true" : "false";
    }
}
