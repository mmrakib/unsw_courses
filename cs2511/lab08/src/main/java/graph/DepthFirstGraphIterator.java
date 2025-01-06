package graph;

import java.util.Deque;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;
import java.util.HashSet;

public class DepthFirstGraphIterator<N extends Comparable<N>> implements Iterator<N> {
    private Graph<N> graph;
    private Deque<N> stack;
    private Set<N> visited;

    public DepthFirstGraphIterator(Graph<N> graph) {
        this.graph = graph;
        this.stack = new LinkedList<>();
        this.visited = new HashSet<>();
        for (N startNode : graph.getNodes()) {
            if (!visited.contains(startNode)) {
                stack.push(startNode);
                visited.add(startNode);
                break; // Start with the first unvisited node found.
            }
        }
    }

    @Override
    public boolean hasNext() {
        return !stack.isEmpty();
    }

    @Override
    public N next() {
        if (!hasNext()) {
            throw new IllegalStateException("No more nodes to visit");
        }

        N currentNode = stack.pop();
        for (N neighbor : graph.getAdjacentNodes(currentNode)) {
            if (!visited.contains(neighbor)) {
                stack.push(neighbor);
                visited.add(neighbor);
            }
        }

        return currentNode;
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException("Remove operation is not supported.");
    }
}
