#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct node {
    struct node *next;
    int          data;
};

// Simple create node function
struct node *create_node(int data) {
    struct node *new = malloc(sizeof (struct node));
    new->data = data;
    new->next = NULL;
    return new;
}

// fix_counting is given a pointer to a linked list containing consecutive integers
// except there may be one missing integer
// fix_counting should return a pointer to a list
// with the missing integer added (if there was a missing integer)
struct node *fix_counting(struct node *head) {
    struct node *curr = head;
    struct node *prev = head;
    int i = head->data;

    // Iterates through list to see if integers are consecutive
    while (curr != NULL && curr->data == i) {
        curr = curr->next;
        i++;
    }

    // If iterator reaches the end, just returns list as it is and skips insertion
    if (curr == NULL) {
        return head;
    }

    // Otherwise, inserts missing integer (specification says at most one integer can be missing)

    // Iterates to node before current node
    while (prev != NULL && prev->next != curr) {
        prev = prev->next;
    }

    // Creates new node
    struct node *new = create_node(i--);

    // Completes insertion
    if (head == curr) {
        head = new;
        new->next = curr;
    } else {
        prev->next = new;
        new->next = curr;
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

// DO NOT CHANGE THIS MAIN FUNCTION

struct node *strings_to_list(int len, char *strings[]);
void print_list(struct node *head);

int main(int argc, char *argv[]) {

    // create linked list from command line arguments
    struct node *head = NULL;
    if (argc > 1) {
        // list has elements
        head = strings_to_list(argc - 1, &argv[1]);
    }

    struct node *new_head = fix_counting(head);
    print_list(new_head);

    return 0;
}


// DO NOT CHANGE THIS FUNCTION
// create linked list from array of strings
struct node *strings_to_list(int len, char *strings[]) {
    struct node *head = NULL;
    int i = len - 1;
    while (i >= 0) {
        struct node *n = malloc(sizeof (struct node));
        assert(n != NULL);
        n->next = head;
        n->data = atoi(strings[i]);
        head = n;
        i -= 1;
    }   
    return head;
}

// DO NOT CHANGE THIS FUNCTION
// print linked list
void print_list(struct node *head) {
    printf("[");    
    struct node *n = head;
    while (n != NULL) {
        // If you're getting an error here,
        // you have returned an invalid list
        printf("%d", n->data);
        if (n->next != NULL) {
            printf(", ");
        }
        n = n->next;
    }
    printf("]\n");
}
