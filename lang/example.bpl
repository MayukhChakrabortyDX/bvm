tail fx fibonacci(a: int): int {

    if ( a < 1 ) {
        return -1; //invalid input
    } 

    if ( a == 1 ) {
        return 0;
    }

    if ( a == 2 ) {
        return 1;
    }

    return fibonacci(a - 1) + fibonacci(a);

}