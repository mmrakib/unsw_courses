package bool;

import bool.VariableNode;
import bool.AndNode;
import bool.OrNode;
import bool.NotNode;

public class BooleanEvaluator {
    public static boolean evaluate(BooleanNode expression) {
        return expression.evaluate();
    }

    public static String prettyPrint(BooleanNode expression) {
        System.out.println(expression.prettyPrint());
    }

    public static void main(String[] args) {
        // !(true && false)
        VariableNode x = new VariableNode(true);
        VariableNode y = new VariableNode(false);
        AndNode a = new AndNode(x, y);
        NotNode n = new NotNode(a);
        System.out.println(n.evaluate());
        n.prettyPrint();
    }
}
