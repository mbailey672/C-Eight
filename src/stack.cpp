#include <iostream>
#include "stack.h"

#include <stdint.h>

Stack::Stack(){
    top = NULL;
}

Stack::Stack(const Stack& aStack)
{
    if(aStack.top == NULL)
        top = NULL;
    else{
        StackFramePtr temp = aStack.top;    //temp moves through the nodes from top to bottom of aStack

        StackFramePtr endPtr;               //Points to the end of the new stack
        endPtr = new StackFrame;

        if(endPtr == NULL){
            std::cout << "Insufficient Memory" << std::endl;
            exit(1);
        }

        endPtr->data = temp->data;
        top = endPtr;

        //First node created and filled with data.
        //New nodes are now added AFTER this first node

        temp = temp->link;
        while(temp != NULL){
            endPtr->link = new StackFrame;
            if(endPtr==NULL){
                std::cout << "Insufficient Memory" << std::endl;
                exit(1);
            }

            endPtr = endPtr->link;
            endPtr->data = temp->data;
            temp = temp->link;
        }
        endPtr->link = NULL;
    }
}

Stack::~Stack()
{
    uint16_t next;
    while(!isEmpty())
        next = pop();
}

bool Stack::isEmpty() const{
    return(top==NULL);
}

void Stack::push(uint16_t newByte){
    StackFramePtr tempPtr;
    tempPtr = new StackFrame;
    if(tempPtr == NULL){
        std::cout << "Stack Overflow: Insufficient Memory" << std::endl;
        exit(1);
    }

    tempPtr->data = newByte;

    tempPtr->link = top;
    top = tempPtr;
}

uint16_t Stack::pop(){
    if(isEmpty()){
        std::cout << "Error: popping an empty stack" << std::endl;
        exit(1);
    }

    uint16_t result = top->data;

    StackFramePtr tempPtr;
    tempPtr = top;
    top = top->link;

    delete tempPtr;

    return result;
}

