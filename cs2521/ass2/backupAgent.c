// Implementation of the Agent ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "Agent.h"
#include "Map.h"

// Stack ADT for implementing DFS strategy
struct stack {
    int top;
    unsigned capacity;
    int *array;
};

typedef struct stack *Stack;

Stack StackCreate(unsigned capacity)
{
    Stack stack = (Stack)malloc(sizeof(struct stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

void StackFree(Stack stack) {
    free(stack->array);
    free(stack);
}

int StackIsFull(Stack stack)
{
    return stack->top == stack->capacity - 1;
}

int StackIsEmpty(Stack stack)
{
    return stack->top == -1;
}

void StackPush(Stack stack, int item)
{
    if (StackIsFull(stack))
        return;
    stack->array[++stack->top] = item;
}

int StackPop(Stack stack)
{
    if (StackIsEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}

int StackPeek(Stack stack)
{
    if (StackIsEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}

// Agent structure
struct agent {
    char *name;
    int startLocation;
    int location;
    int maxStamina; // max stamina
    int stamina;    // current stamina
    int strategy;
    Map map;
    
    // Stage 1 - Cheapest Least Visited
    int *visited;

    // Stage 2 - DFS
    Stack stack;

    // Stage 3 - Least Turns Path
    int isFollowingTip;
    int *shortestPath;
    int shortestPathLen;
    int shortestPathIndex;
};

// Function declarations
void AgentShow(Agent agent);
static int filterRoads(Agent agent, struct road roads[], int numRoads,
                       struct road legalRoads[]);
static struct move chooseRandomMove(Agent agent, Map m);
static struct move chooseCheapestLeastVisitedMove(Agent agent, Map m);
static struct move chooseDFS(Agent agent, Map m);
static struct move chooseShortestPath(Agent agent, Map m);

/**
 * Creates a new agent
 */
Agent AgentNew(int start, int stamina, int strategy, Map m, char *name) {
    if (start >= MapNumCities(m)) {
        fprintf(stderr, "error: starting city (%d) is invalid\n", start);
        exit(EXIT_FAILURE);
    }
    
    Agent agent = malloc(sizeof(struct agent));
    if (agent == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    agent->startLocation = start;
    agent->location = start;
    agent->maxStamina = stamina;
    agent->stamina = stamina;
    agent->strategy = strategy;
    agent->map = m;
    agent->name = strdup(name);
    
    agent->visited = calloc(MapNumCities(m), sizeof(int));
    agent->visited[agent->startLocation]++;

    agent->stack = StackCreate(MapNumCities(m) + 1);

    agent->isFollowingTip = 0;
    agent->shortestPath = calloc(MapNumCities(m), sizeof(int));
    agent->shortestPathLen = 0;
    agent->shortestPathIndex = 0;

    // DEBUG PRINT
    printf("--- Agent created! ---");
    AgentShow(agent);

    return agent;
}

/**
 * Frees all memory associated with the agent
 * NOTE: You should not free the map because the map is owned by the
 *       main program, and the main program will free it
 */
void AgentFree(Agent agent) {
    free (agent->shortestPath);
    StackFree(agent->stack);
    free(agent->visited);
    free(agent->name);
    free(agent);
}

/**
 * Gets the name of the agent
 */
char *AgentName(Agent agent) {
    return agent->name;
}

/**
 * Gets the current location of the agent
 */
int AgentLocation(Agent agent) {
    return agent->location;
}

/**
 * Gets the amount of stamina the agent currently has
 */
int AgentStamina(Agent agent) {
    return agent->stamina;
}

/**
 * Calculates the agent's next move
 * NOTE: Does NOT actually carry out the move
 */
struct move AgentGetNextMove(Agent agent, Map m) {
    if (agent->isFollowingTip) {
        return chooseShortestPath(agent, m);
    }

    switch (agent->strategy) {
        case STATIONARY: return (struct move){agent->location, 0};
        case RANDOM:     return chooseRandomMove(agent, m);
        // TODO: Implement more strategies here
        case CHEAPEST_LEAST_VISITED: return chooseCheapestLeastVisitedMove(agent, m);
        case DFS: return chooseDFS(agent, m);
        default:
            printf("error: strategy not implemented yet\n");
            exit(EXIT_FAILURE);
    }
}

/*
* Filters out list of roads with roads agent has enough stamina to visit
*/
static int filterRoads(Agent agent, struct road roads[], int numRoads,
                       struct road legalRoads[]) {
    int numLegalRoads = 0;
    for (int i = 0; i < numRoads; i++) {
        if (roads[i].length <= agent->stamina) {
            legalRoads[numLegalRoads++] = roads[i];
        }
    }
    return numLegalRoads;
}

/*
* Executes move logic for random strategy
*/
static struct move chooseRandomMove(Agent agent, Map m) {
    struct road *roads = malloc(MapNumCities(m) * sizeof(struct road));
    struct road *legalRoads = malloc(MapNumCities(m) * sizeof(struct road));

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    struct move move;
    if (numLegalRoads > 0) {
        // Sort the roads using insertion sort
        for (int i = 1; i < numLegalRoads; i++) {
            struct road r = legalRoads[i];
            int j = i;
            while (j > 0 && r.to < legalRoads[j - 1].to) {
                legalRoads[j] = legalRoads[j - 1];
                j--;
            }
            legalRoads[j] = r;
        }

        // nextMove is randomly chosen from the legal roads
        int k = rand() % numLegalRoads;
        move = (struct move){legalRoads[k].to, legalRoads[k].length};
    } else {
        // The agent must stay in the same location
        move = (struct move){agent->location, 0};
    }

    free(legalRoads); // Free dynamically allocated memory
    free(roads);      // Free dynamically allocated memory
    return move;
}

/*
* Executes move logic for cheapest least-visited strategy
*/
static struct move chooseCheapestLeastVisitedMove(Agent agent, Map m) {
    struct road *roads = malloc(MapNumCities(m) * sizeof(struct road));
    struct road *legalRoads = malloc(MapNumCities(m) * sizeof(struct road));

    // Get all roads to adjacent cities
    size_t numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    size_t numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    struct move move;
    if (numLegalRoads > 0) {
        // Sort the roads using insertion sort
        for (int i = 1; i < numLegalRoads; i++) {
            struct road r = legalRoads[i];
            int j = i;
            while (j > 0 && r.to < legalRoads[j - 1].to) {
                legalRoads[j] = legalRoads[j - 1];
                j--;
            }
            legalRoads[j] = r;
        }

        // Choose nextMove based off of least visited city
        int k = 0;
        for (int i = 0; i < numLegalRoads; i++) {
            int ith_city = legalRoads[i].to;
            int ith_length = legalRoads[i].length;
            int kth_city = legalRoads[k].to;
            int kth_length = legalRoads[k].length;

            if (agent->visited[ith_city] < agent->visited[kth_city]) {
                k = i;
            } else if (agent->visited[ith_city] == agent->visited[kth_city]) {
                if (ith_length < kth_length) {
                    k = i;
                } else if (ith_length == kth_length) {
                    k = (ith_city < kth_city) ? i : k;
                }
            }
        }

        move = (struct move){legalRoads[k].to, legalRoads[k].length};
    } else {
        // The agent must stay in the same location
        move = (struct move){agent->location, 0};
    }

    free(legalRoads); // Free dynamically allocated memory
    free(roads);      // Free dynamically allocated memory
    return move;
}

/*
* Executes move logic for DFS strategy
*/
static struct move chooseDFS(Agent agent, Map m) {
    struct road *roads = malloc(MapNumCities(m) * sizeof(struct road));
    size_t numRoads = MapGetRoadsFrom(m, agent->location, roads);

    struct move move;

    int lowest_city = INT_MAX;
    struct road lowest_road = (struct road){0, 0};
    for (int i = 0; i < numRoads; i++) {
        int ith_city = roads[i].to;
        if (agent->visited[ith_city] == 0 && ith_city < lowest_city) {
            lowest_city = ith_city;
            lowest_road = roads[i];
        }
    }

    if (lowest_city == INT_MAX) {
        // All adjacent nodes visited
        if (!StackIsEmpty(agent->stack)) {
            int popped_to = StackPop(agent->stack);
            int popped_length = MapContainsRoad(m, agent->location, popped_to);

            move = (struct move){popped_to, popped_length};
        } else {
            printf("Stack is empty!\n");
            exit(1);
        }
    } else {
        // Adjacent nodes contain unvisited node
        if (lowest_road.length <= agent->stamina) {
            // Enough stamina
            move = (struct move){lowest_road.to, lowest_road.length};
            if (!StackIsFull(agent->stack)) {
                StackPush(agent->stack, lowest_road.to);
            } else {
                printf("Stack is full!\n");
                exit(1);
            }
        } else {
            // Not enough stamina
            move = (struct move){agent->location, 0};
        }
    }

    free(roads);
    return move;
}

/*
* Executes move logic for least turns path strategy
*/
static struct move chooseShortestPath(Agent agent, Map m) {
    struct move move;

    if (agent->shortestPathIndex < agent->shortestPathLen) {
        int to_city = agent->shortestPath[agent->shortestPathIndex];
        int to_length = MapContainsRoad(m, agent->location, to_city);
        if (agent->stamina > to_length) {
            move = (struct move){to_city, to_length};
            agent->shortestPathIndex++;
        } else {
            move = (struct move){agent->location, 0};
        }
    } else {
        agent->isFollowingTip = 0;
        for (int i = 0; i < MapNumCities(m); i++) agent->shortestPath[i] = 0;
        agent->shortestPathIndex = 0;
        agent->shortestPathLen = 0;

        move = AgentGetNextMove(agent, m);
    }

    return move;
}

/**
 * Executes a given move by updating the agent's internal state
 */
void AgentMakeNextMove(Agent agent, struct move move) {
    if (move.to == agent->location) {
        agent->stamina = agent->maxStamina;
    } else {
        agent->stamina -= move.staminaCost;
    }
    agent->location = move.to;
    agent->visited[move.to]++;

    // DEBUG PRINT
    printf("--- Agent made move! ---");
    AgentShow(agent);
}

/**
 * Tells the agent where the thief is
 */
void AgentTipOff(Agent agent, int thiefLocation) {
    int numCities = MapNumCities(agent->map);

    int dist[numCities];
    int prev[numCities];
    int visited[numCities];

    for (int i = 0; i < numCities; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1; // Use -1 to represent unvisited cities
        visited[i] = 0;
    }

    dist[agent->location] = 0;

    while (1) {
        // Find the unvisited city with the smallest distance
        int nextCity = -1;
        int minDistance = INT_MAX;
        for (int i = 0; i < numCities; i++) {
            if (!visited[i] && dist[i] < minDistance) {
                minDistance = dist[i];
                nextCity = i;
            }
        }

        if (nextCity == -1) {
            // No unvisited city found
            break;
        }

        visited[nextCity] = 1;

        struct road *roads = malloc(numCities * sizeof(struct road));
        int numRoads = MapGetRoadsFrom(agent->map, nextCity, roads);

        for (int i = 0; i < numRoads; i++) {
            int neighborCity = roads[i].to;
            int roadLength = roads[i].length;

            if (!visited[neighborCity] && dist[nextCity] + roadLength < dist[neighborCity]) {
                dist[neighborCity] = dist[nextCity] + roadLength;
                prev[neighborCity] = nextCity;
            }
        }

        free(roads);
    }

    // Reconstruct the shortest path
    int pathLength = 0;
    int *path = malloc(numCities * sizeof(int));
    int city = thiefLocation;
    while (city != agent->location) {
        path[pathLength++] = city;
        city = prev[city];
    }

    // Store the shortest path in the agent's data
    agent->shortestPathLen = pathLength;
    agent->shortestPath = realloc(agent->shortestPath, pathLength * sizeof(int));
    for (int i = 0; i < pathLength; i++) {
        agent->shortestPath[i] = path[pathLength - 1 - i];
    }

    agent->shortestPathIndex = 0;
    agent->isFollowingTip = 1;

    free(path); // Free dynamically allocated memory for path
}

/**
 * Prints information about the agent (for debugging purposes)
 */
void AgentShow(Agent agent) {
    printf("Name: %s\n", agent->name);
    printf("Start location: %d\n", agent->startLocation);
    printf("Current location: %d\n", agent->location);
    printf("Max stamina: %d\n", agent->maxStamina);
    printf("Current stamina: %d\n", agent->stamina);
    printf("Strategy: %d\n", agent->strategy);

    printf("Visited array: [");
    for (int i = 0; i < MapNumCities(agent->map); i++) {
        printf(" c%d = %d, ", i, agent->visited[i]);
    }
    printf("]\n");

    printf("Stack: [");
    for (int i = 0; i < agent->stack->top + 1; i++) {
        printf(" %d, ", agent->stack->array[i]);
    }
    printf("]\n");

    printf("Is following tip?: %d\n", agent->isFollowingTip);
    
    printf("Shortest path: [");
    for (int i = 0; i < agent->shortestPathLen; i++) {
        printf(" %d, ", agent->shortestPath[i]);
    }
    printf("]\n");

    printf("Shortest path index: %d\n", agent->shortestPathIndex);
    printf("Shortest path length: %d\n", agent->shortestPathLen);

    printf("\n\n");
}
