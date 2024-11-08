/*
* COMP2521 - Assignment 2 - Task 1 
* Implementation of Agent ADT
*
* By Mohammad Mayaz Rakib (z5361151)
*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"

#define MAX_LEN_CITY_NAME 256 // Arbritrary max length for city name

struct map { 
    int numCities;   // Number of vertices
    int numRoads;    // Number of edges
    int **roads;     // Adjacency matrix
    char **names;    // Names of cities
};

/**
 * Creates a new map with the given number of cities
 * Assumes that `numCities` is positive
 */
Map MapNew(int numCities) {
    Map m = malloc(sizeof(struct map));

    m->numCities = numCities;
    m->numRoads = 0;

    // Initialise adjacency matrix
    m->roads = malloc(m->numCities * sizeof(int *));
    for (int i = 0; i < m->numCities; i++) {
        m->roads[i] = calloc(m->numCities, sizeof(int));
    }

    // Initialise names array (array of strings, hence double pointer)
    m->names = malloc(m->numCities * sizeof(char *));
    for (int i = 0; i < m->numCities; i++) {
        m->names[i] = calloc(MAX_LEN_CITY_NAME, sizeof(char));
        strcpy(m->names[i], "unnamed");
    }

    return m;
}

/**
 * Frees all memory allocated to the given map
 */
void MapFree(Map m) {
    for (int i = 0; i < m->numCities; i++) {
        free(m->roads[i]);
    }
    free(m->roads);

    for (int i = 0; i < m->numCities; i++) {
        free(m->names[i]);
    }
    free(m->names);

    free(m);
}

/**
 * Returns the number of cities on the given map
 */
int MapNumCities(Map m) {
    return m->numCities;
}

/**
 * Returns the number of roads on the given map
 */
int MapNumRoads(Map m) {
    return m->numRoads;
}

/**
 * Sets the name of the given city
 * If the city's name has already been set, renames it
 */
void MapSetName(Map m, int city, char *name) {
    strcpy(m->names[city], name);
}

/**
 * Returns the name of the given city, or "unnamed" if the city's name
 * has not been set
 */
char *MapGetName(Map m, int city) {
    return m->names[city];
}

/**
 * Inserts a road between two cities with the given length
 * Does nothing if there is already a road between the two cities
 * Assumes that the cities are valid and are not the same
 * Assumes that the length of the road is positive
 */
void MapInsertRoad(Map m, int city1, int city2, int length) {
    if (m->roads[city1][city2] == 0) {
        m->roads[city1][city2] = length;
        m->roads[city2][city1] = length;
        m->numRoads++;
    }
}

/**
 * Returns the length of the road between two cities, or 0 if no such
 * road exists
 */
int MapContainsRoad(Map m, int city1, int city2) {
    return m->roads[city1][city2];
}

/**
 * Stores the roads connected to the given city in the given `roads`
 * array in any order and returns the number of roads stored. The `from`
 * field should be equal to `city` for all roads in the array.
 * Assumes that the roads array is at least as large as the number of
 * cities on the map.
 */
int MapGetRoadsFrom(Map m, int city, struct road roads[]) {
    int numRoads = 0;

    for (int i = 0; i < m->numCities; i++) {
        if (m->roads[city][i] != 0) {
            struct road r;
            r.from = city;
            r.to = i;
            r.length = m->roads[city][i];

            roads[numRoads] = r;
            numRoads++;
        }
    }

    return numRoads;
}

/**
 * Displays the map
 * !!! DO NOT EDIT THIS FUNCTION !!!
 * This function will work once the other functions are working
 */
void MapShow(Map m) {
    printf("Number of cities: %d\n", MapNumCities(m));
    printf("Number of roads: %d\n", MapNumRoads(m));
    
    struct road *roads = malloc(MapNumRoads(m) * sizeof(struct road));
    if (roads == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);    
    }
    
    for (int i = 0; i < MapNumCities(m); i++) {
        printf("[%d] %s has roads to:", i, MapGetName(m, i));
        int numRoads = MapGetRoadsFrom(m, i, roads);
        for (int j = 0; j < numRoads; j++) {
            if (j > 0) {
                printf(",");
            }
            printf(" [%d] %s (%d)", roads[j].to, MapGetName(m, roads[j].to),
                   roads[j].length);
        }
        printf("\n");
    }
    
    free(roads);
}
