// Assignment 2 21T2 COMP1511: Pokedex
// pokedex.c
//
// This program was written by MOHAMMAD M. RAKIB (z5361151)
// on 02/08/2021
//
// Version 1.0.0: Release
// Version 1.0.1: Fixed references to create_new_dexnode in comments and error messages

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ext_save.h"

// Note you are not allowed to use the strstr function in this assignment
// There are some techniques we would like you to learn to code
// that this function makes easier.

// ----------------------------------------------
// Add any extra #includes your code needs here.
// ----------------------------------------------

#include "pokedex.h"
//#include "pokemon.h"
//#include "pokemon_types.h"

// ----------------------------------------------
// Add your own #defines here.
// ----------------------------------------------
#define TRUE 1
#define FALSE 0

#define NUM_POKEMON_TYPES 19

// Note you are not permitted to use arrays in struct pokedex,
// you must use a linked list.
//
// The only exception is that char arrays are permitted for
// search_pokemon and functions it may call, as well as the string
// returned by pokemon_name (from pokemon.h).
//
// You will need to add fields to struct pokedex.
// You may change or delete the head field.
struct pokedex {
    struct dexnode *head;
    struct dexnode *selected;
};


// You don't have to use the provided struct dexnode, you are free to
// make your own struct instead.
//
// If you use the provided struct dexnode, you will need to add fields
// to it to store other information.
struct dexnode {
    Pokemon pokemon;
    struct dexnode *next;
    int found;
};


// ----------------------------------------------
// Add your own structs here.
// ----------------------------------------------




// ----------------------------------------------
// Add prototypes for your own functions here.
// ----------------------------------------------

// Creates a new dexnode struct and returns a pointer to it.
static struct dexnode *create_new_dexnode(Pokemon pokemon);


// You need to implement the following functions in their stages.
// In other words, replace the lines calling fprintf & exit with your code.
// You can find descriptions of what each function should do in pokedex.h


// Creates a new Pokedex, and returns a pointer to it.
// Note: you will need to modify this function.
Pokedex new_pokedex(void) {
    // Malloc memory for a new Pokedex, and check that the memory
    // allocation succeeded.
    Pokedex pokedex = malloc(sizeof(struct pokedex));
    assert(pokedex != NULL);

    // Set the head of the linked list to be NULL.
    // (i.e. set the Pokedex to be empty)
    pokedex->head = NULL;
    pokedex->selected = NULL;

    // TODO: Add your own code here.

    return pokedex;
}

// Create a new dexnode struct and returns a pointer to it.
//
// This function should allocate memory for a dexnode, set its pokemon
// pointer to point to the specified pokemon, and return a pointer to
// the allocated memory.
static struct dexnode *create_new_dexnode(Pokemon pokemon) {
    struct dexnode *new = malloc(sizeof(struct dexnode));

    new->pokemon = pokemon;
    new->next = NULL;
    new->found = TRUE;

    return new;
}

//////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                        //
//////////////////////////////////////////////////////////////////////

// Add a new Pokemon to the Pokedex.
void add_pokemon(Pokedex pokedex, Pokemon pokemon) {
    struct dexnode *new = create_new_dexnode(pokemon);

    if (pokedex->head == NULL) {
        pokedex->head = new;
        pokedex->selected = new;
    } else {
        struct dexnode *i = pokedex->head;

        while (i->next != NULL) {
            i = i->next;
        }

        i->next = new;
    }
}

void print_pokemon(Pokemon pokemon) {
    printf("ID: %03d\nName: %s\nHeight: %.6f m\nWeight: %.6f kg\n",
        pokemon_id(pokemon), pokemon_name(pokemon), pokemon_height(pokemon), pokemon_weight(pokemon));

    printf("Type: ");
    if (pokemon_first_type(pokemon) != NONE_TYPE) {
        printf("%s ", type_code_to_str(pokemon_first_type(pokemon)));
    }
    if (pokemon_second_type(pokemon) != NONE_TYPE) {
        printf("%s ", type_code_to_str(pokemon_second_type(pokemon)));
    }
    printf("\n");
}

// Print out all of the Pokemon in the Pokedex.
void print_pokedex(Pokedex pokedex) {
    struct dexnode *i = pokedex->head;

    while (i != NULL) {
        if (i->found == TRUE) {
            if (i == pokedex->selected) {
                printf("--> #%03d: %s\n", pokemon_id(i->pokemon), pokemon_name(i->pokemon));
            } else {
                printf("    #%03d: %s\n", pokemon_id(i->pokemon), pokemon_name(i->pokemon));
            }
        } else {
            if (i == pokedex->selected) {
                printf("--> #%03d: ", pokemon_id(i->pokemon));

                for (int j = 0; j < strlen(pokemon_name(i->pokemon)); j++) {
                    printf("*");
                }
                printf("\n");
            } else {
                printf("    #%03d: ", pokemon_id(i->pokemon));

                for (int j = 0; j < strlen(pokemon_name(i->pokemon)); j++) {
                    printf("*");
                }
                printf("\n");
            }
        }

        // //ERROR
        // printf("Before Iteration\n");
        // printf("i = %p\n", i);
        // if (i != NULL) printf("i->next = %p\n", i->next);

        i = i->next;

        // //ERROR
        // printf("After Iteration\n");
        // printf("i = %p\n", i);
        // if (i != NULL) printf("i->next = %p\n", i->next);
    }
}

// Print out the details of the currently selected Pokemon.
void detail_current_pokemon(Pokedex pokedex) {
    if (pokedex->selected->found) {
        print_pokemon(get_current_pokemon(pokedex));
    } else {
        printf("ID: %03d\nName: ", pokemon_id(pokedex->selected->pokemon));

        for (int i = 0; i < strlen(pokemon_name(pokedex->selected->pokemon)); i++) {
                printf("*");
        }
        printf("\n");

        printf("Height: --\nWeight: --\nType: --\n");
    }
}

// Return the currently selected Pokemon.
Pokemon get_current_pokemon(Pokedex pokedex) {
    return pokedex->selected->pokemon;
}

// Change the currently selected Pokemon to be the next Pokemon in the Pokedex.
void next_pokemon(Pokedex pokedex) {
    if (pokedex->selected->next != NULL) {
        pokedex->selected = pokedex->selected->next;
    }
}

// Change the currently selected Pokemon to be the previous Pokemon in the Pokedex.
void prev_pokemon(Pokedex pokedex) {
    if (pokedex->selected != pokedex->head) {
        struct dexnode *i = pokedex->head;

        while (i->next != pokedex->selected) {
            i = i->next;
        }

        pokedex->selected = i;
    }
}

// Change the currently selected Pokemon to be the Pokemon with the ID `id`.
void jump_pokemon(Pokedex pokedex, int id) {
    struct dexnode *i = pokedex->head;

    while (pokemon_id(i->pokemon) != id) {
        i = i->next;
    }

    pokedex->selected = i;
}

//////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                        //
//////////////////////////////////////////////////////////////////////

// Sets the currently selected pokemon's "found" status
void set_current_pokemon_found(Pokedex pokedex, int found) {
    if (pokedex->selected != NULL) {
        pokedex->selected->found = found;
    }
}

// Add a pokemon before the current pokemon.
void insert_pokemon_before_current(Pokedex pokedex, Pokemon pokemon) {
    // struct dexnode *i = pokedex->head;

    // if (i == NULL) {
    //     add_pokemon(pokedex, pokemon);
    // } else {
    //     while (i->next != NULL) {
    //         if (i->next == pokedex->selected) {
    //             break;
    //         }

    //         i = i->next;
    //     } 

    //     if (pokedex->selected == pokedex->head) {
    //         // struct dexnode *temp = i;
    //         // pokedex->head = create_new_dexnode(pokemon);
    //         // pokedex->head->next = temp;

    //         // pokedex->selected = pokedex->head;

    //         struct dexnode *temp = create_new_dexnode(pokemon);
    //         temp->next = pokedex->head->next;
    //         pokedex->head = pokedex->head->next;

    //         pokedex->selected = pokedex->head;
    //     } else {
    //         i->next = create_new_dexnode(pokemon);
    //         i->next->next = pokedex->selected;

    //         pokedex->selected = i->next;
    //     }
    // }

    // pokedex->selected->found = FALSE;

    struct dexnode *new = create_new_dexnode(pokemon);

    if (pokedex->head == pokedex->selected) {
        pokedex->head = new;
        new->next = pokedex->selected;
        pokedex->selected = new;
    } else {
        struct dexnode *temp = pokedex->head;

        while (temp->next != pokedex->selected) {
            temp = temp->next;
        }

        temp->next = new;
        new->next = pokedex->selected;
        pokedex->selected = new;
    }

    pokedex->selected->found = FALSE;
}

// Return the total number of Pokemon in the Pokedex.
int count_pokemon_of_type(Pokedex pokedex, char *type) {
    int typecode = type_str_to_code(type);

    struct dexnode *i = pokedex->head;
    int count = 0;

    while (i != NULL) {
        if (pokemon_first_type(i->pokemon) == typecode ||
            pokemon_second_type(i->pokemon) == typecode) {
            count++;
        }

        i = i->next;
    }

    return count;
}

//////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                        //
//////////////////////////////////////////////////////////////////////


// Remove the currently selected Pokemon from the Pokedex.
void remove_pokemon(Pokedex pokedex) {
    struct dexnode *i = pokedex->head;

    if (i != NULL) {
        while (i->next != NULL) {
            if (i->next == pokedex->selected) {
                break;
            }

            i = i->next;
        }

        if (pokedex->selected == pokedex->head) {
            struct dexnode *temp = pokedex->head->next;
            destroy_pokemon(pokedex->head->pokemon);
            free(pokedex->head);
            pokedex->head = temp;
            pokedex->selected = pokedex->head;

            //ERROR
            // printf("pokedex->head: %p", pokedex->head);
            // printf("temp: %p", temp);
        } else {
            i->next = pokedex->selected->next;
            destroy_pokemon(pokedex->selected->pokemon);
            free(pokedex->selected);
            pokedex->selected = i;

            //ERROR
            // printf("HERE!");
        }
    }   
}

// Destroy the given Pokedex and free all associated memory.
void destroy_pokedex(Pokedex pokedex) {
    struct dexnode *i = pokedex->head;
    struct dexnode *temp = NULL;

    while (i != NULL) {
        temp = i; 
        i = i->next;
        destroy_pokemon(temp->pokemon);
        free(temp);
    }

    free(pokedex);
}

// Prints out all types of found Pokemon stored in the Pokedex
void show_types(Pokedex pokedex) {
    // struct dexnode *i = pokedex->head;

    // int types_found[NUM_POKEMON_TYPES] = {FALSE};

    // while (i != NULL) {
    //     if (i->found == TRUE) {
    //         types_found[pokemon_first_type(i->pokemon)] = TRUE;
    //         types_found[pokemon_second_type(i->pokemon)] = TRUE;
    //     }

    //     i = i->next;
    // }

    // for (int j = 0; j < NUM_POKEMON_TYPES; j++) {
    //     if (types_found[j] == TRUE && j != NONE_TYPE) {
    //         printf("%s\n", type_code_to_str(j));
    //     }
    // }

    struct dexnode *i = pokedex->head;

    while (i != NULL) {
        int first = pokemon_first_type(i->pokemon);
        int second = pokemon_second_type(i->pokemon);

        int can_print_first = FALSE;
        int can_print_second = FALSE;

        if (i->found == TRUE) {
            can_print_first = TRUE;
            can_print_second = TRUE;
        }

        struct dexnode *prev = pokedex->head;

        while (prev != i) {
            int first_2 = pokemon_first_type(prev->pokemon);
            int second_2 = pokemon_second_type(prev->pokemon);

            if (first == first_2 || first == second_2) {
                can_print_first = FALSE;
            }

            if (second == first_2 || second == second_2) {
                can_print_second = FALSE;
            }

            prev = prev->next;
        }

        if (can_print_first == TRUE && first != NONE_TYPE) {
            printf("%s\n", type_code_to_str(first));
        }

        if (can_print_second == TRUE && second != NONE_TYPE) {
            printf("%s\n", type_code_to_str(second));
        }

        i = i->next;
    }
}

// Return the number of Pokemon in the Pokedex that have been found.
int count_found_pokemon(Pokedex pokedex) {
    struct dexnode *i = pokedex->head;
    int count = 0;

    while (i != NULL) {
        if (i->found == TRUE) {
            count++;
        }

        i = i->next;
    }
    
    return count;
}

// Return the total number of Pokemon in the Pokedex.
int count_total_pokemon(Pokedex pokedex) {
    struct dexnode *i = pokedex->head;
    int count = 0;

    while (i != NULL) {
        count++;    
        i = i->next;
    }
    
    return count;
}

// NOTE: Make sure you submit ass2_pokedex before completing these.
//////////////////////////////////////////////////////////////////////
//                     Stage 4 Extension Functions                  //
//////////////////////////////////////////////////////////////////////

/*
 * Saves a pokedex to a text file
 * Use the functions in ext_save.h to save the contents to a file
 */
void save_pokedex(Pokedex pokedex, char* filename) {
    fprintf(stderr, "exiting because you have not implemented the save_pokedex function\n");
    exit(1);

}

/*
 * Loads a pokedex from a text file
 * Use the functions in ext_save.h to load the text from a file
 */
Pokedex load_pokedex(char* filename) {
    fprintf(stderr, "exiting because you have not implemented the load_pokedex function\n");
    exit(1);

    return NULL;
}

//////////////////////////////////////////////////////////////////////
//                     Stage 5 Extension Functions                  //
//////////////////////////////////////////////////////////////////////

// Create a new Pokedex which contains only the Pokemon of a specified
// type from the original Pokedex.
Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    fprintf(stderr, "exiting because you have not implemented the get_pokemon_of_type function\n");
    exit(1);
}

// Create a new Pokedex which contains only the Pokemon that have
// previously been 'found' from the original Pokedex.
Pokedex get_found_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the get_found_pokemon function\n");
    exit(1);
}

// Create a new Pokedex containing only the Pokemon from the original
// Pokedex which have the given string appearing in its name.
Pokedex search_pokemon(Pokedex pokedex, char *text) {
    fprintf(stderr, "exiting because you have not implemented the search_pokemon function\n");
    exit(1);
}

// Free's the current sub-list and returns the original pokedex state, 
// prior to the search
Pokedex end_search(Pokedex sub_pokedex) {
    fprintf(stderr, "exiting because you have not implemented the end_search function\n");
    exit(1);
}

// Add definitions for your own functions here.
