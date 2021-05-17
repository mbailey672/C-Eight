#ifndef STACK_H
#define STACK_H

#include <stdint.h>

struct StackFrame{
    uint16_t data;
    StackFrame *link;
};

typedef StackFrame* StackFramePtr;

class Stack
{
    public:
        Stack();            //Initializes the object to an empty stack
        ~Stack();           //Destroys the stack and returns all the memory to the head

        Stack(const Stack& a_stack);    //Copy Constructor

        void push(uint16_t newByte);     //Add the_symbol to the stack
        uint16_t pop();                     //If the stack is not empty, return the top symbol from the stack
                                        //and removes that symbol from the top of the stack

        bool isEmpty() const;           //Returns true if the stack is empty, otherwise false

    private:
        StackFramePtr top;
};

#endif // STACK_H
