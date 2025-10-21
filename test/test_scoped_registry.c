#include <stdio.h>
enum Items {
    A, B, C, D
};

int main() {

    const void jump_table[D + 1] = {
        [A] = &&A,
        [B] = &&B,
        [C] = &&C,
        [D] = &&D
    };

    {
        A: printf("scope1");
        B: printf("scope1");
        C: printf("scope1");
        D: printf("scope1");
    }


    {
        A: printf("scope1");
        B: printf("scope1");
        C: printf("scope1");
        D: printf("scope1");
    }

}