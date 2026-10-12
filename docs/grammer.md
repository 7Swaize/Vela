# Vela — Language Grammar

```
program        := declaration* EOF ;
```


## Declarations

```
declaration    := structDecl | classDecl | funcDecl | varDecl ;

structDecl     := "struct" IDENTIFIER "{" member* "}" ;
classDecl      := "class" IDENTIFIER "{" member* "}" ;
funcDecl       := "fn" returnType IDENTIFIER "(" parameters? ")" block ;
varDecl        := type IDENTIFIER ( "=" expression )? ";" ;

member         := field | funcDecl ;
field          := type IDENTIFIER ";" ;
```


## Statements

```
statement       := exprStmt
                  | ifStmt
                  | forStmt
                  | whileStmt
                  | returnStmt
                  | block ;

exprStmt       := expression ";" ;
ifStmt         := "if" "(" expression ")" statement ( "else" statement )? ;
forStmt        := "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement ;
whileStmt      := "while" "(" expression ")" statement ;
returnStmt     := "return" expression? ";" ; 

block          := "{" (varDecl | statement)* "}" ;
```


## Expressions

```
expression     := assignment ;

assignment     := logic_or ( "=" assignment )? ;

logic_or       := logic_and ( "||" logic_and )* ;
logic_and      := equality ( "&&" equality )* ;
equality       := comparison ( ( "!=" | "==" ) comparison )* ;
comparison     := bitwise_or ( ( ">" | ">=" | "<" | "<=" ) bitwise_or )* ;
bitwise_or     := bitwise_xor ( "|" bitwise_xor )* ;
bitwise_xor    := bitwise_and ( "^" bitwise_and )* ;
bitwise_and    := shift ( "&" shift )* ;
shift          := term ( ( "<<" | ">>" ) term )* ;
term           := factor ( ( "-" | "+" ) factor )* ;
factor         := unary ( ( "/" | "*" | "%" ) unary )* ;

unary          := ( "!" | "~" | "-" ) unary | call ;
call           := primary ( "(" arguments? ")" | "." IDENTIFIER | "[" expression "]" )* ;

primary        := "true" | "false" | "null" | "this"
                 | INT | FLOAT | STRING | IDENTIFIER | "(" expression ")"
                 | IDENTIFIER "{" fieldInits? "}"
                 | "new" IDENTIFIER "{" fieldInits? "}"
                 | "new" (primitiveType | IDENTIFIER) "[" expression "]" ;

```


## Utility 

```
returnType     := type | "void" ;
type           := (primitiveType | IDENTIFIER) ("[" "]")? ;
primitiveType  := "int" | "float" | "bool" | "string" ;

parameters     := type IDENTIFIER ( "," type IDENTIFIER )* ;
arguments      := expression ( "," expression)* ;

fieldInits     := fieldInit ( "," fieldInit )* ","? ;
fieldInit      := IDENTIFIER "=" expression ; 
```


## Lexical Grammar

```
INT            := DIGIT+ ;
FLOAT          := DIGIT+ "." DIGIT+ ;
STRING         := "\"" <any char except "\"">* "\"" ;
IDENTIFIER     := ( ALPHA ( ALPHA | DIGIT )* ) - KEYWORD;
ALPHA          := "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT          := "0" ... "9";

KEYWORD        := "struct" | "class" | "fn" | "if" | "else"
                 | "for" | "while" | "return" | "true" | "false"
                 | "null" | "this" | "new" 
                 | "void"
                 | primitiveType ;
```


## Trivia 

```
WHITESPACE     := " " | "\t" | "\r" | "\n" ;
COMMENT        := LINE_COMMENT | BLOCK_COMMENT ;

LINE_COMMENT   := "//" <any char except "\n">* ;
BLOCK_COMMENT  := "/*" (BLOCK_COMMENT | <any char> )* "*/" ;
```