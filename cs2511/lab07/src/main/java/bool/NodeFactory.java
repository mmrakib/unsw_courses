package bool;

import org.json.JSONObject;

import bool.BooleanNode;
import bool.VariableNode;
import bool.AndNode;
import bool.OrNode;
import bool.NotNode;

public class NodeFactory {
    public static BooleanNode createNode(JSONObject json) {
        String nodeType = json.getString("node");

        if ("value".equals(nodeType)) {
            boolean value = json.getBoolean("value");
            return new VariableNode(value);
        } else if ("and".equals(nodeType)) {
            AndNode node = new AndNode(null, null);
            JSONObject subnode1Json = json.optJSONObject("subnode1");
            JSONObject subnode2Json = json.optJSONObject("subnode2");

            if (subnode1Json != null) {
                BooleanNode subnode1 = createNode(subnode1Json);
                node.setLeft(subnode1);
            }

            if (subnode2Json != null) {
                BooleanNode subnode2 = createNode(subnode2Json);
                node.setRight(subnode2);
            }

            return node;
        } else if ("or".equals(nodeType)) {
            OrNode node = new OrNode(null, null);
            JSONObject subnode1Json = json.optJSONObject("subnode1");
            JSONObject subnode2Json = json.optJSONObject("subnode2");

            if (subnode1Json != null) {
                BooleanNode subnode1 = createNode(subnode1Json);
                node.setLeft(subnode1);
            }

            if (subnode2Json != null) {
                BooleanNode subnode2 = createNode(subnode2Json);
                node.setRight(subnode2);
            }

            return node;
        } else if ("not".equals(nodeType)) {
            NotNode node = new NotNode(null);
            JSONObject subnodeJson = json.optJSONObject("subnode");

            if (subnodeJson != null) {
                BooleanNode subnode = createNode(subnodeJson);
                node.setNode(subnode);
            }

            return node;
        }
    }
}
