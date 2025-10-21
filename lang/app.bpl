import std::io::{ input, println };
import std::string::*;
import std::api::*;

object Student {
    name: string,
    grade: byte
}

apply Printable on Student {

    fx print(loan self: Student): string {

        var str: string = "";
        let item = str <- "Name: " <- self.name <- "Grade: " <- self.grade.print();

    }

}

fx main() {

    //take input from stdio
    var input: string = input();
    println( loan input );

} 