#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Stack ADT
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
  
// Stack is full when top is equal to the last index
int StackIsFull(Stack stack)
{
    return stack->top == stack->capacity - 1;
}
  
// Stack is empty when top is equal to -1
int StackIsEmpty(Stack stack)
{
    return stack->top == -1;
}
  
// Function to add an item to stack.  It increases top by 1
void StackPush(Stack stack, int item)
{
    if (StackIsFull(stack))
        return;
    stack->array[++stack->top] = item;
}
  
// Function to remove an item from stack.  It decreases top by 1
int StackPop(Stack stack)
{
    if (StackIsEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}
  
// Function to return the top from stack without removing it
int StackPeek(Stack stack)
{
    if (StackIsEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}
  
// Driver program to test above functions
int main()
{
    Stack stack = StackCreate(100);
  
    StackPush(stack, 10);
    StackPush(stack, 20);
    StackPush(stack, 30);
    StackPush(stack, 1);
    StackPush(stack, 1235125);

    while (!StackIsEmpty(stack)) {
        printf("%d popped from stack\n", StackPop(stack));
    }
  
    return 0;
}
