// exam_q6.c
//
// This program was written by z5555555
// on INSERT-DATE-HERE

#include <stdio.h>
#include <stdlib.h>

// Do not edit this struct. You may use it exactly as
// it is but you cannot make changes to it

// Truth definitions
#define TRUE 1
#define FALSE 0

// A node in a linked list
struct node {
    int height;
    struct node *next;
};

// ADD ANY FUNCTION DECLARATIONS YOU WISH TO USE HERE

// Allocates a new node and returns it
struct node *create_node(int height) {
    struct node *new = malloc(sizeof (struct node));
    new->height = height;
    new->next = NULL;
    return new;
}

// Adds already allocated node to end of pre-existing list
void append_node(struct node *head, struct node *new) {
    struct node *curr = head;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = new;
} 

// Returns number of nodes in list
int list_length(struct node *head) {
    struct node *curr = head;
    int length = 0;

    while (curr != NULL) {
        curr = curr->next;
        length++;
    }

    return length;
}

// Frees all nodes of a list
void remove_list(struct node *head) {
    struct node *prev = head;

    while (head != NULL) {
        head = head->next;

        free(prev);
        prev = head;
    }
}

struct node *mountaineer(struct node *head) {
    struct node *curr = head;

    // Find the maximum height
    int max_height = 0;
    while (curr != NULL) {
        if (curr->height > max_height) {
            max_height = curr->height;
        }
        curr = curr->next;
    }

    // Find node containing maximum height
    struct node *max = head;
    while (max != NULL && max->height != max_height) {
        max = max->next;
    }

    // Creates linked lists representing both left and right paths
    struct node *left_head = create_node(max->height);
    struct node *right_head = create_node(max->height);

    // Left and right path iterators
    struct node *left_curr = max;
    struct node *right_curr = max;

    // A 'previous node' pointer used for iterating left
    struct node *prev = head;

    // 'Iteration finished' conditions
    int finished_left = FALSE;
    int finished_right = FALSE;

    // Previously highest height for left and right iterators
    int prev_highest_left = max_height;
    int prev_highest_right = max_height;

    // Executes iteration and left/right list building
    while (TRUE) {
        // Iterate right
        if (!finished_right) {
            if (right_curr != NULL) {
                right_curr = right_curr->next;
            } else {
                finished_right = TRUE;
            }

            // Add to right list if descending
            if (right_curr != NULL && right_curr->height < prev_highest_right) {
                struct node *new = create_node(right_curr->height);
                append_node(right_head, new);
                prev_highest_right = right_curr->height;
            }
        }

        // Iterate left
        if (!finished_left) {
            prev = head;
            if (left_curr != head) {
                while (prev != NULL && prev->next == left_curr) {
                    prev = prev->next;
                }
                left_curr = prev;
            } else {
                finished_left = TRUE;
            }

            // Add to left list if descending
            if (left_curr->height < prev_highest_left) {
                struct node *new = create_node(left_curr->height);
                append_node(left_head, new);
                prev_highest_left = left_curr->height;
            }
        }

        // If done with iterations on both sides, exit loop
        if (finished_left && finished_right) {
            break;
        }
    }

    int left_size = list_length(left_head);
    int right_size = list_length(right_head);

    // Original list to be discarded
    struct node *old = head;

    if (left_size >= right_size) {
        head = left_head;
        remove_list(old);
        remove_list(right_head);
    } else {
        head = right_head;
        remove_list(old);
        remove_list(left_head);
    }

    return head;
}


///////////////////////////////////////////////////////////////
//
// YOU DO NOT NEED TO UNDERSTAND CODE BELOW THIS COMMENT
// 
// DO NOT CHANGE CODE BELOW THIS COMMENT
//
// IF YOU THINK YOU NEED TO CHANGE ANYTHING BELOW THIS COMMENT,
// YOU HAVE MISUNDERSTOOD THE QUESTION
//
///////////////////////////////////////////////////////////////

void print_and_free_list(struct node *head) {
    if (head == NULL) {
        printf("\n");
        return;
    }
    printf("%d, ", head->height);
    print_and_free_list(head->next);
    free(head);
}

struct node *make_list(int length, char *argv[]) {
    struct node *head = malloc(sizeof (struct node));
    struct node *n = head;
    int i = 0;
    while (i < length) {
        n->height = strtol(argv[i + 1], NULL, 10);
        if (i < length - 1) {
            // there are more nodes to make
            n->next = malloc(sizeof (struct node));
            n = n->next;
        } else {
            n->next = NULL;
        }
        i++;
    }
    return head;
}

int main(int argc, char *argv[]) {
    struct node *head = make_list(argc - 1, argv);
    struct node *return_path = mountaineer(head);
    printf("Given stopping points: ");
    print_and_free_list(head);

    printf("Return stopping points: ");
    print_and_free_list(return_path);
    return 0;
}
