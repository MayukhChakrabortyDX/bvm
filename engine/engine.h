#pragma once

//defines a scheduler to work effectively.
//these are all green threads.
struct Scheduler {

    struct Fibre *ptr;
    struct Scheduler *next_fibre;
    struct Scheduler *previous_fibre;

};  
//the second argument is a function pointer to a syscall registry
//for now let's keep it single threaded.
void schedule_fibres(struct Scheduler *pool);