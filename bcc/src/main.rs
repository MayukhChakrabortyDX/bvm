mod lexer;
use std::{collections::HashSet, hash::Hash, vec};

fn main() {

    let _keywords = vec![
        "fx", "impl", "grant", "loan", "if", "elif", "else", "while", "handle", "object", "import",
        "export", "for", "do", "var", "new", "clone", "int", "long", "float", "double", "char", "byte",
        "Null", "Error", "Ok", "Result"
    ];

    let mut keywords: HashSet<String> = HashSet::new();
    let mut identifiers: HashSet<char> = HashSet::new();

    let check_identifier: Vec<char> = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_".chars().collect();

    for n in &check_identifier {
        identifiers.insert(n.clone());
    }

    for n in &_keywords {
        keywords.insert(n.to_string());
    }

    keywords.insert(String::from("fx"));

}
