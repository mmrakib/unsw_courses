#### CORE EXERCISE: GRAPH ITERATOR

**Do you think making the Graph Iterable makes semantic sense? Discuss briefly, and think of both sides.**

Making the Graph iterable provides a convenient way to traverse the graph. It simplifies the process of iteration, especially for users who want a default traversal strategy. However, it might not be suitable for all use cases. Some users might prefer different traversal strategies or more control over the iteration process. Additionally, the concept of a graph and an iterable are different, which might lead to confusion.

**We could change the definition of our Graph so that the traversal logic is done internally. Is a Graph an iterator or an iterable in this case? What would the .iterator method return in this case?**

In this case, *Graph* would both be an iterator and an iterable. Since it implements both an Iterator and an Iterable, we would be able to set it to both types.

The .iterator method, if defined, might return this (the Graph object itself), which could act as its own iterator. It essentially provides access to the iterator for the graph.

**There is a problem with this approach, though. Describe a test that would cause this implementation to fail.**

One problem with this approach is that it doesn't provide an option for users to specify a different traversal strategy. Since it internally performs BFS traversal, it might not be suitable for users who need custom traversal logic or want to use DFS or other strategies.

An example test would be as follows:

```java
Graph<Integer> graph = new Graph<>();
graph.addNode(1);
graph.addNode(2);
graph.addConnection(1, 2);
Iterator<Integer> iterator = graph.iterator();

// Remove a node during iteration
graph.removeNode(2);

// Attempt to continue the iteration
while (iterator.hasNext()) {
    System.out.println(iterator.next());
}
```
