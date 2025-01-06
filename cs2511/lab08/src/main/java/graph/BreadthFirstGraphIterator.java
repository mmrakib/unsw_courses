package graph;

import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;
import java.util.List;

public class BreadthFirstGraphIterator<N extends Comparable<N>> implements Iterator<N> {
    private Graph<N> graph;
    private Queue<N> queue;
    private Set<N> visited;

    public BreadthFirstGraphIterator(Graph<N> graph) {
        this.graph = graph;
        this.queue = new LinkedList<>();
        this.visited = new HashSet<>();

        // Find the first unvisited node to start traversal
        for (N node : graph.getNodes()) {
            if (!visited.contains(node)) {
                queue.add(node);
                visited.add(node);
                break;
            }
        }
    }

    @Override
    public boolean hasNext() {
        return !queue.isEmpty();
    }

    @Override
    public N next() {
        if (!hasNext()) {
            throw new IllegalStateException("No more nodes to visit");
        }

        N currentNode = queue.poll();
        List<N> adjacentNodes = graph.getAdjacentNodes(currentNode);

        for (N neighbor : adjacentNodes) {
            if (!visited.contains(neighbor)) {
                queue.add(neighbor);
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
