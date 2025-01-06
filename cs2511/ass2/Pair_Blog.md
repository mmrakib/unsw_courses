# Assignment II Pair Blog Template

## Task 1) Code Analysis and Refactoring ⛏️

### a) From DRY to Design Patterns

[Links to your merge requests](https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/H16A_FLYGON/assignment-ii/-/merge_requests/1)

> i. Look inside src/main/java/dungeonmania/entities/enemies. Where can you notice an instance of repeated code? Note down the particular offending lines/methods/fields.

In the `Mercenary` and `ZombieToast` classes, there is a lot of repeated code for movement logic in the `move()` method when the player has an `InvincibilityPotion` and `InvisibilityPotion` in effect. The movement calculation based on potion type is duplicated for both classes.

> ii. What Design Pattern could be used to improve the quality of the code and avoid repetition? Justify your choice by relating the scenario to the key characteristics of your chosen Design Pattern.

To address the issue regarding movement logic for players with `InvincibilityPotion` effects, we can use the _Strategy Pattern_. The Strategy Pattern allows us to create a family of algorithms and dynamically select the correct one at runtime. We can thus encapsulate the movement behaviour based on the player's potion type, as well as other types, such as default movement. We do this by creating a set of strategy classes that encapsulate each of these behaviours.

> iii. Using your chosen Design Pattern, refactor the code to remove the repetition.

I first made a new folder within the enemies folder named `movementstrategies`. In it, I define an interface known as `MovementStrategy`, which is my base type for all other movement strategies that I will implement on a case-by-case basis. The movement strategies I implemented, identified by their classes names, are as follows: `InvincibilityPotionMovementStrategy` for movement while invincible, `InvisibilityPotionMovementStrategy` for movement while invisible, `FollowHostileMovementStrategy` for mercenary movement while chasing hostiles, and `DefaultZombieMovementStrategy` for the default movement of the `ZombieToast` type objects.

I didn't define a `DefaultSpiderMovementStrategy` because it is not required as there is never a dynamic selection of movement strategies based off of circumstances; it is always the same. I also didn't define a `FollowAllyMovementStrategy` for the `Mercenary` class either since it is also unnecessary.

Then, for each `move()` function, based off the circumstances such as whether or not a potion is action, different movement strategies are created using the `new` keyword. The `nextPos` variable is calculated using the same `getNextMove` method in all movement strategies, and is then used to move the enemy.

### b) Observer Pattern

> Identify one place where the Observer Pattern is present in the codebase, and outline how the implementation relates to the key characteristics of the Observer Pattern.

The `GameMap` object that represents, as the name implies, the game map, somewhat fits into the framework of an _Observer Pattern_.

Firstly, there is a list of graph nodes `nodes`, each given a coordinate position in the map. This relates to the key characteristic of a _list of observers_, or objects that observe the subject, being the game map itself, and is notified by it of state changes.

Secondly, there is a set of methods that register different types of objects and their callbacks to the map. These include `initRegisterBombsAndSwitches()`, `initRegisterMovables()` and `initRegisterSpawners()`, among others. This relates to the key characteristic of _callback registration_, or when observers are registered along with their corresponding 'callbacks', or functions that execute a certain action when the subject tells it to.

Thirdly, there is a set of methods that trigger objects to execute their callbacks. These include `triggerMovingAwayEvent()` and `triggerOverlapEvent()`, among others. This relates to the key characteristic of _notifying observers_, where observers such as map nodes are notified to execute some form of action by the subject.

Although not the purest or most complete implementation of an observer pattern, the `GameMap` class is for the most part a good example.

### c) Inheritance Design

[Links to your merge requests](https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/H16A_FLYGON/assignment-ii/-/commit/6b477b5d6445b893200acdc24359cf2975541654 https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/H16A_FLYGON/assignment-ii/-/commit/3976be3c1fcfc9d466a402c99157d5b2bc5a2404)

> i. Name the code smell present in the above code. Identify all subclasses of Entity which have similar code smells that point towards the same root cause.

The code smell above is known as _dead code_. Dead code is code that is considered obsolete and capable of doing nothing for the overall program. Functions that simply `return` without doing anything, such as the code provided above in the `Exit` class, would be considered dead code, since it provides nothing meaningful to the overall functionality of the program.

There is another example of this code smell in the `entities` folder of the source, namely, the `Wall` class. We can therefore also sense the code smell of _code duplication_ in this problem as well.

> ii. Redesign the inheritance structure to solve the problem, in doing so remove the smells.

In order to fix this issue, we can utilise a design pattern known as the _Null Entity Pattern_. Essentially, we create a new class known as the _null object class_ that implements the behaviour of an object that does not display any behaviour or displays default behaviour. It acts simply as a placeholder object, improving maintainability and readability, all the while removing dead code that serves no purpose.

We thus create a `StaticEntity` class that acts as our null entity. This class just acts a simple static entity that 'does nothing' on the map. Exits and walls easily fit into this cateogry.

We override the `canMoveOnto()`, `onOverlap()`, `onMovedAway()` and `onDestroy()` methods as we did before, except this time it does not count as dead code since the do-nothing behaviour servers a purpose in the code; specifically, it servers to fulfil the purpose of the null entity object which is to 'do nothing'.

We then fix our inheritance relations for the `Exit` and `Wall` classes, making them inherit from `StaticEntity` rather than `Entity` directly. We then remove the specific `Exit` and `Wall` overrides for the above four methods, thus removing the dead code in each of these classes. Note that the `Wall` class has slightly more functionality for the `canMoveOnto()` method, but that is completely fine as our design pattern allows to easily extend the default behaviour of the null entity.

(NOTE: I accidentally forgot to create a separate merge request for these changes, so I have provided the links for the commits to the `master` branch instead. Apologies.)

### d) More Code Smells

[Links to your merge requests](https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/H16A_FLYGON/assignment-ii/-/merge_requests/2)

> i. What design smell is present in the above description?

The description seems to describe code that exhibit the code smells of _code duplication_ and _low cohesion_, as well as design smells such as _unfactored heirarchy_.

In every collectable class, we seem to override the methods `canMoveOnto()`, `onMoveAway()`, `onDestroy()`, and especially `onOverlap()`, with essentially the same logic. Every collectable is able to be picked up by a player, and every collectable after being picked up is removed from the map. This means this code is duplicated. Not only that, but it also means that code is not as cohesive as it could be, given that the behaviour of picking up collectable items is spread across multiple classes when it could have been centralized in one place.

The design smell here is that there is a layer of abstraction missing and that the heirarchy of abstracted is 'unfactored', or in other words, properties of the different types in the heirarchy do not share default behaviour when they act he same.

> ii. Refactor the code to resolve the smell and underlying problem causing it.

I added an extra layer of abstraction by defining an abstract class known as `Collectable`. This abstract class implements the default behaviours of `canMoveOnto()`, `onMoveAway()`, `onDestroy()` and `onOverlap()` methods. I then fixed the inheritance relations so that all collectable entities inherit from a centralised `Collectable` class rather than an `Entity` class, and I removed all duplicate implementation of the above four methods in each one. We have thus reduced code duplication, code is more cohesive and organised, and our heirarchy of abstraction indeed makes sense and share default behaviour where they are required to.

### e) Open-Closed Goals

[Links to your merge requests](https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/H16A_FLYGON/assignment-ii/-/merge_requests/3)

> i. Do you think the design is of good quality here? Do you think it complies with the open-closed principle? Do you think the design should be changed?

The _Open-Closed Principle_ states that software components should be _open for extension_ but _closed for modification_. The `Goal` object's implementation does not comply with this principle. This is because, if we wanted to add a new goal type, we would have to modify the `Goal` class itself, specifically the `achieved()` method. This means the `Goal` class is not closed for modification, and therefore violates the open-closed principle.

This violation of the open-closed principle is a sign of low quality design, indiciating that the design should be changed such that the _closed for modification_ rule is not broken.

> ii. If you think the design is sufficient as it is, justify your decision. If you think the answer is no, pick a suitable Design Pattern that would improve the quality of the code and refactor the code accordingly.

One way we can allow additions to the `Goal` class and thus make it open for extension _without_ violating the closed for modification rule, is that we can utilise the _Strategy Pattern_. Here, instead of the all the goal achievement logic being trapped in a single method, we outsource it to a set of `GoalStrategy` classes.

To do this, we define a common interface `GoalStrategy` with the method signature for `achieved()`. This is because all goals require a way of notifying the client if it has been achieved or not. We then create subclasses that implements the `GoalStrategy` interface, including `ExitGoalStrategy`, `BouldersGoalStrategy` and `TreasureGoalStrategy`. We then override the `achieved()` method in each of these classes for each goal strategy's specific achievement logic.

We can now easily extend the our set of `Goal` objects while keeping them closed for modification. We have greatly improved readability and maintability and have simplified our `achieved()` method.

### f) Open Refactoring

[Merge Request 1](https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/H16A_FLYGON/assignment-ii/-/merge_requests/4)

The `GameMap` object exhibits many code smells. The class is exceptionally large, is incredibly complex, and contains a lot of duplicated methods and code in general. The class violates the _Single Responsibility Principle_, which states that all classes should do one and one thing only. For this reason, we move a lot of the `GameMap` functionality into other classes to encapsulate those functionality within those classes, making the `GameMap` class more readable and maintable and hiding implementation details in these component classes.

One such component class added is the `NodeRegistry` class. This class acts as a node map for the `GameMap` class. Instead of handling node registering, unregistering and retrieval all in the `GameMap` class, which then utilises those methods as well, we do it within the `NodeRegistry` class so these implementation details are hidden. The `GameMap` class then keeps a `NodeRegistry` object as a component and acts as a client to this component by using its methods.

Other components include the `GameMapInitialiser` and the `Pathfinder`.

Firstly, the game map initialisation is incredibly long for a set of methods that only run once (at the start when map is created). It thus makes sense to move these initialisation methods into its own class `GameMapInitialiser` as a set of static methods, and have one primary `initialise()` static method to chain them together.

Secondly, the Djikstra's algorithm pathfinding is also incredibly long and not related enough to a `GameMap` object to be included within it. We thus move the Djikstra's pathfinding algorithm to its own class `Pathfinder`, and implement it as a static method `djikstra()`. The `GameMap` object then simply uses this `Pathfinder` component class whenever it needs to use the pathfinding algorithm.

Both these component classes once again hide implementation details and makes the `GameMap` code more readable and maintable, and thus improving the overall quality of the code.

[Merge Request 2](https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/H16A_FLYGON/assignment-ii/-/merge_requests/5)

The `Inventory` object, much like the `GameMap` object, also exhibits many code smells, a lot of them similar. The class is also exceptionally large, and also violates the _Single Responsibility Principle_. It does this by including item crafting logic within the inventory class itself, instead of encapsulating this logic elsewhere. For this reason, we move all of the crafting logic, including the hard-coded crafting recipes, into a `CraftingService` class that encapsulates this crafting logic for us and provides us with crafting specific functionality.

We create the `CraftingService` class and define for it two types of methods: `canBuildX()` methods and `buildX()` methods. The `canBuildX()` methods, such as `canBuildBow()` and `canBuildShield()`, checks if there are enough of the ingredients required by the item's corresponding recipe in order for that item to be built, and returns a boolean value based off of this check. The `buildX()` methods, such as `buildBow()` and `buildShield()`, _uses up_ the ingredients required by the item's corresponding recipe by removing them from the inventory.

This component object of the `Inventory` class allows us to hide the implementation details of crafting recipes and resolve the violation of the single-responsibility principle. It greatly simplies our `Inventory` object, making it more readable and maintable, seperating concerns into different classes and thus improving the overall quality of the code.

## Task 2) Evolution of Requirements 👽

### a) Microevolution - Enemy Goal

[Links to your merge requests](/put/links/here)

**Assumptions**
There exists enough enemies on the map to satisfy the goal.
The distance between the player and the enemy spawner is greater than the enemy goal.

Functional Requirements:

- The player must kill a certain number of enemies to complete the level.
- The number of enemies to kill is specified in the level file.
- The game must keep track of the number of enemies killed.
- The game must keep track of the amount of spawners destroyed.

**Design**

[Design]
Goal.Java
Addition of the case enemies to the class goal.java

EnemyGoalStrategy.Java
Class creation of enemy goalStategy to store the amount of enemies required to kill

Player.java
integer to keep track of the amount of enemies killed
integer to keep track of the amountof spawners killed
setter and getter methods for the above two integers

Enemy.java
Method to set player amount of enemies killed

**Changes after review**

[Design review/Changes made]

**Test list**

We will implement the following for the code behind enemy goals:

| **Tag** | **Display Name**                                           | **Description**                                                                                                       | **Dungeon Filename**             | **Config Filename**              |
|---------|------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|----------------------------------|----------------------------------|
| 13-5    | Test achieving an enemy goal with 3 enemies and 1 spawner  | A basic test where player must kill 3 enemies and a single spawner to accomplish the enemy goal.                      | d_basicGoalsTest_enemyBasic      | c_basicGoalsTest_enemyBasic      |
| 13-6    | Test achieving an enemy goal with no enemies and 1 spawner | An edge-case test where player is not required to kill any enemies but a single spawner to accomplish the enemy goal. | d_basicGoalsTest_enemyNoEnemies  | c_basicGoalsTest_enemyNoEnemies  |
| 13-7    | Test achieving an enemy goal with no spawners and 1 enemy  | An edge-case test where player is not required to kill any spawners but a single enemy to accomplish the enemy goal.  | d_basicGoalsTest_enemyNoSpawners | c_basicGoalsTest_enemyNoSpawners |

**Other notes**

[Any other notes]

### Choice 1 (Insert choice)

[Links to your merge requests](/put/links/here)

**Assumptions**

[Any assumptions made]

**Design**

[Design]

**Changes after review**

[Design review/Changes made]

**Test list**

[Test List]

**Other notes**

[Any other notes]

### Choice 2 (Insert choice)

[Links to your merge requests](/put/links/here)

**Assumptions**

[Any assumptions made]

**Design**

[Design]

**Changes after review**

[Design review/Changes made]

**Test list**

[Test List]

**Other notes**

[Any other notes]

### Choice 3 (Insert choice) (If you have a 3rd member)

[Links to your merge requests](/put/links/here)

**Assumptions**

[Any assumptions made]

**Design**

[Design]

**Changes after review**

[Design review/Changes made]

**Test list**

[Test List]

**Other notes**

[Any other notes]

## Task 3) Investigation Task ⁉️

[Merge Request 1](/put/links/here)

[Briefly explain what you did]

[Merge Request 2](/put/links/here)

[Briefly explain what you did]

Add all other changes you made in the same format here:
