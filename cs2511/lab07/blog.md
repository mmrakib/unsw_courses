#### CORE EXERCISE: BOOLEAN LOGIC

**Question 1: What are the compound nodes and the leaf nodes in this problem? Write the answer in your blog post.**  

The leaf nodes, or the nodes without any children, are the boolean expressions and values that are either 'true' or 'false'. Every boolean expression ultimately evalutes to one of these two values, and thus it makes sense that these are the leaf nodes, as you can't evaluate them any further.

The composite nodes, or the nodes with children, thus being composed of simpler objects/values, are the expressions themselves. Specifically, these include *AND*, *OR* and *NOT*, the three boolean operators being considered in this problem.

**Question 2: Will you use keep the BooleanNode as an abstract class or use an interface to represent the composite type?**

Although an abstract class would work, and interface is better because it ensures that there is no default behaviour in methods, and that only the method signatures exist. This makes *BooleanNode* strictly abstract, and allows all other nodes that inherit from it to contain implementations. Interfaces also stay true their existence as 'contracts', or purely abstract rules and signatures defining how classes work. This is more useful for a composite design pattern because it means that there is no default implementation. It is also much more flexible than abstract classes.

**Question 3: In our factory, what are the different types of objects we need to create? What are the different fields they will have?**

The following types of objects need to be created: we need a *NodeFactory* object that contains the factory method, we need the products of the factory object including the abstract class *BooleanNode* and all its concrete product objects (leaf nodes), including *VariableNode* for variables in general, *AndNode* for 'AND' expressions, *OrNode* for 'OR' expressions and *NotNode* for 'NOT' expressions.

#### CORE/CHOICE EXERCISE: THE ART OF RANDOMNESS

**Question 1: How would you write tests for Game with the default constructor that prove the battle function works as expected? Think about this and be prepared to answer during marking. Write your answer down in your blog post.**

The primary approach is to perform statistical testing. This involves running a large number of battles and examining the distribution of results. Given that the battle function has a 0.5 chance of winning, you expect roughly 50% of the battles to result in a win (return true) and 50% to result in a loss (return false). 
