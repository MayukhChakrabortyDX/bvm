mod std {

    obj Stack<T> {

        stack: Array<T>,
        top: int

    }

    def stdstack<T> {

        static fx init<T>(initial: T);

    }

    def stdstack on Stack<T> {

        static fx init<T>(initial: T) {

            val: Stack<T> = new Stack{ stack = new Array<T>, top = 0 };
            return val;

        }

        fx push<T>(self: grant Stack<T>, v: T) {

            self.stack.add( v );
            self.top++;

        }

        fx pop<T>(self: grant Stack<T>): Null<T> {

            val: Null<T> = null;

            if ( self.top != 0 ) {
                val = clone self.stack[top];
                self.top--;
            }

            return val;

        }

    }

}