pub mod lexer {

    enum TokenType {
        Keywords, //built-in
        Symbols, //characters like +/- etc
        String, //strings like "Hello"
        Literal //literal like hello = 10; hello is a literal.
    }
    pub struct Token {
        
        category: TokenType, //owned value
        value: String

    }

    

}