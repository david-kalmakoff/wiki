# Writing An Interpreter In Go

https://interpreterbook.com/

## Parts

An interpreter is made up of a couple parts, the lexer and parser.

The lexer is responsible for converting code into tokens. Tokens a representation the language's identifiers, keywords, operators, literals, etc.

The parser is responsible for converting a sequence of tokens (token stream) into an abstract syntax tree. An AST is a data structure representation of the program.

In this book we are using a recursive descent parser to parse the program.

## Lexer

For the lexer, we are moving through the text character by character checking for specific token types. Peeking at the next char to check for multi character tokens and separately handling identifiers and numbers.

## Tokens

Tokens are what make up the languages vocabulary including identifiers, literals, operators, delimiters, and keywords.

## Parser

### Recursive Decent

## AST

The AST is that data structure to store the representation of the program.

_TODO: What is the importance of each of these in the program?_

### Node

### Statement

### Expression

### Program

The programs is a list of statements.
