// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "place.h"
#include "Pq.h"

////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
double calcDistance(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    double distance = sqrt(dx * dx + dy * dy);

    return distance;
}

int planGrid1(struct place cities[], int numCities,
              struct place powerPlant,
              struct powerLine powerLines[]) {
    Graph mst = GraphNew(numCities);
    Pq pq = PqNew();

    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            if (i != j) {
                struct edge e;
                e.v = i;
                e.w = j;
                e.weight = calcDistance(cities[i].x, cities[i].y, cities[j].x, cities[j].y);

                PqInsert(pq, e);
            }
        }
    }

    int num_powerlines = 0;

    while (!PqIsEmpty(pq)) {
        struct edge e = PqExtract(pq);
        GraphInsertEdge(mst, e);

        if (GraphHasCycle(mst)) {
            GraphRemoveEdge(mst, e.v, e.w);
        } else {
            struct powerLine pl;
            struct place p1, p2;

            p1 = (e.v == -1) ? powerPlant : cities[e.v];
            p2 = cities[e.w];

            pl.p1 = p1;
            pl.p2 = p2;

            powerLines[num_powerlines] = pl;
            num_powerlines++;
        }
    }

    PqFree(pq);
    GraphFree(mst);

    return num_powerlines;
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(struct place cities[], int numCities,
              struct place powerPlants[], int numPowerPlants,
              struct powerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
