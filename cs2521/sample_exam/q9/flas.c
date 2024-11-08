
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

List flas(List l) {
    List sublist = NULL;
    int sublist_length = 0;

    List currlist = NULL;
    int currlist_length = 0;

    bool has_target = false;

    while (l != NULL) {
        if (l->next != NULL) {
            if (l->next->val > l->val) {
                if (has_target == false) {
                    
                }
            }
        }
    }

    return sublist;
}

