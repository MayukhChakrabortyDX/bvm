typedef struct {
    int x;
    int y;
} Point;

//This tells a dropper. drop_x is the specification
void __drop_point__(Point *ptr); //destructor of sorts.
void __new_point(); //constructor of sorts.