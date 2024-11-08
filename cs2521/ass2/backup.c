static struct move chooseCheapestLeastVisitedMove(Agent agent, Map m) {
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

        // Find the city that has been visited the least number of times
        int leastVisitedAmount = INT_MAX;
        unsigned int leastVisitedCity = UINT_MAX; // Use unsigned int here

        for (int i = 0; i < numLegalRoads; i++) {
            int city = legalRoads[i].to;

            if (agent->visited[city] < leastVisitedAmount) {
                leastVisitedAmount = agent->visited[city];
                leastVisitedCity = city;
            } else if (agent->visited[city] == leastVisitedAmount) {
                // Handle tie-breaking based on road length and city ID
                if (legalRoads[i].length < legalRoads[leastVisitedCity].length) {
                    leastVisitedCity = city;
                } else if (legalRoads[i].length > legalRoads[leastVisitedCity].length) {
                    // Do nothing since we have a longer road
                } else {
                    // Handle tie-breaking based on city ID (lowest ID is chosen)
                    leastVisitedCity = (city < leastVisitedCity) ? city : leastVisitedCity;
                }
            }
        }

        move = (struct move){leastVisitedCity, legalRoads[leastVisitedCity].length};
    } else {
        // The agent must stay in the same location
        move = (struct move){agent->location, 0};
    }

    free(legalRoads);
    free(roads);
    return move;
}




static struct move chooseDFS(Agent agent, Map m) {
    struct road *roads = malloc(MapNumCities(m) * sizeof(struct road));

    // Get all roads to adjacent cities
    size_t numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Sort the roads using insertion sort
    for (int i = 1; i < numRoads; i++) {
        struct road r = roads[i];
        int j = i;
        while (j > 0 && r.to < roads[j - 1].to) {
            roads[j] = roads[j - 1];
            j--;
        }
        roads[j] = r;
    }

    for (int i = 0; i < numRoads; i++) {
        int ith_city = roads[i].to;

        if (agent->visited[ith_city] == 0) {
            agent->stack[agent->sp] = ith_city;
            agent->sp++;
        }
    }

    struct move move;
    int top_to = agent->stack[agent->sp - 1];
    int top_length = MapContainsRoad(m, agent->location, top_to);
    if (top_length <= agent->stamina) {
        move = (struct move){top_to, top_length};
        agent->sp--;
    } else {
        move = (struct move){agent->location, 0};
    }

    free(roads);      // Free dynamically allocated memory
    return move;
}

// WHOT THING BACK UP 1 

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
        int k = 0; // Saved index of NUM LEGAL ROADS, and NOT the city ID
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
        //printf("All nodes visited");
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
        //printf("Lowest road: to = %d, length = %d\n", lowest_road.to, lowest_road.length);
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

// STAGE 3 BACKUP

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
        for (int i = 0; i < MapNumCities(m); i++) agent->shortestPath[i] = 0;
        agent->shortestPathIndex = 0;
        agent->shortestPathLen = 0;

        // Reset to original strategy
        agent->isFollowingTip = 0;

        move = AgentGetNextMove(agent, m);
    }
    
    return move;
}

void AgentTipOff(Agent agent, int thiefLocation) {
    // Switch move logic from strategy to following tip
    agent->isFollowingTip = 1;

    // Size of graph
    int numCities = MapNumCities(agent->map);

    // Djikstra variables
    int dists[numCities];
    int visitedNodes[numCities];
    int prevNodes[numCities];

    // Initialize Djikstra variables
    for (int i = 0; i < numCities; i++) {
        visitedNodes[i] = 0;
        prevNodes[i] = 0;
        if (i == agent->location)
            dists[i] = 0;
        else
            dists[i] = INT_MAX;
    }

    int currentCity = agent->location;
    while (currentCity != thiefLocation) {
        // Mark node as visited if not already
        visitedNodes[currentCity] = 1;

        // Find all adjacent nodes
        struct road *roads = malloc(numCities * sizeof(struct road));
        int numRoads = MapGetRoadsFrom(agent->map, currentCity, roads);

        // Iterate through all *unvisited* adjacent nodes
        int lowest_city = -1;
        int lowest_length = INT_MAX;
        for (int i = 0; i < numRoads; i++) {
            visitedNodes[i] = 1;

            int ith_city = roads[i].to;
            int ith_length = roads[i].length;

            if (agent->visited[ith_city] == 0) {
                int newDist = dists[currentCity] + ith_length;
                if (newDist < dists[ith_city]) {
                    dists[ith_city] = newDist;
                    prevNodes[ith_city] = currentCity;
                }

                if (ith_length < lowest_length) {
                    lowest_city = ith_city;
                    lowest_length = ith_length;
                }
            }
        }
        printf("currentCity: %d\n", currentCity);

        if (lowest_city == -1) {
            // All adjacent nodes visited
            break;
        } else {
            // Some adjacent nodes unvisited
            currentCity = lowest_city;
        }

        printf("numRoads: %d\n", numRoads);
        printf("lowest_city: %d\n", lowest_city);
        printf("lowest_length: %d\n", lowest_length);

        printf("roads: [");
        for (int i = 0; i < numRoads; i++) printf(" (%d, %d), ", roads[i].to, roads[i].length);
        printf("]\n\n");
        
        free(roads);
    }
    
    printf("dists: [");
    for (int i = 0; i < numCities; i++) printf(" %d, ", dists[i]);
    printf("]\n");

    printf("visitedNodes: [");
    for (int i = 0; i < numCities; i++) printf(" %d, ", visitedNodes[i]);
    printf("]\n");

    printf("prevNodes: [");
    for (int i = 0; i < numCities; i++) printf(" %d, ", prevNodes[i]);
    printf("]\n");


    // int len = 0;
    // while (prevNodes[currentCity] != agent->location) {
    //     agent->shortestPath[len] = currentCity;
    //     len++;
    // }
    // agent->shortestPath[len] = agent->location;
    // len++;

    // agent->shortestPathLen = len;
}

// PRINT DJIKSTRA INITIALISATION

    printf("visited: [");
    for (int i = 0; i < numCities; i++) {
        printf(" %d, ", visited[i]);
    }
    printf("]\n");

    printf("dist: [");
    for (int i = 0; i < numCities; i++) {
        printf(" %d, ", dist[i]);
    }
    printf("]\n");

    printf("prev: [");
    for (int i = 0; i < numCities; i++) {
        printf(" %d, ", prev[i]);
    }
    printf("]\n");

    printf("shortestPath: [");
    for (int i = 0; i < agent->shortestPathLen; i++) {
        printf(" %d, ", agent->shortestPath[i]);
    }
    printf("]\n");
