# Vela — Language Grammar

```
program        := declaration* EOF ;
```


## Declarations

```
declaration    := structDecl | classDecl | funcDecl | varDecl
                 | conceptDecl | comptimeVarDecl ;

structDecl     := "struct" IDENTIFIER templateHead? whereClause? "{" member* "}" ;
classDecl      := "class" IDENTIFIER templateHead? whereClause? "{" member* "}" ;
funcDecl       := "fn" returnType IDENTIFIER templateParams? "(" parameters? ")" whereClause? block ;
varDecl        := type IDENTIFIER ( "=" expression )? ";" ;

member         := field | funcDecl ;
field          := type IDENTIFIER ";" ;

templateHead   := templateParams
                 | templateArgs ;
```

## Templates

```
templateParams := "<" templateParam ( "," templateParam )* ">" ;
templateParam  := typeVariable "..."? ( "=" templateArg )?
                 | type "..."? IDENTIFIER ( "=" templateArg )? ;

templateArgs   := "<" templateArg ( "," templateArg )* ">" ;
templateArg    := ( type | bitwise_or ) "..."? ;

whereClause    := "where" "(" expression ")" ;
```

## Concepts

```
conceptDecl    := "concept" IDENTIFIER templateParams refinement? conceptBody ;

refinement     := ":" conceptRef ( "," conceptRef )* ;
conceptRef     := IDENTIFIER templateArgs ;

conceptBody    := "=>" expression ";"
                 | "{" requirement* "}" ;

requirement    := requirementBody ";" ;
requirementBody := "$" IDENTIFIER "." IDENTIFIER
                 | "fn" returnType IDENTIFIER "." IDENTIFIER "(" typeList? ")"
                 | expression ( "->" type )? ;

typeList       := type ( "," type )* ;
```


## Statements

```
statement       := exprStmt
                  | ifStmt
                  | forStmt
                  | whileStmt
                  | returnStmt
                  | comptimeIf
                  | comptimeFor
                  | comptimeBlock
                  | block ;

exprStmt       := expression ";" ;
ifStmt         := "if" "(" expression ")" statement ( "else" statement )? ;
forStmt        := "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement ;
whileStmt      := "while" "(" expression ")" statement ;
returnStmt     := "return" expression? ";" ; 

block          := "{" ( varDecl | comptimeVarDecl | statement )* "}" ;
```


## Comptime

```
comptimeVarDecl := "comptime" ( typeVariable "..."? | type "..."? IDENTIFIER ) "=" expression ";" ;

comptimeIf     := "comptime" "if" "(" expression ")" statement ( "else" statement )? ;
comptimeFor    := "comptime" "for" "(" ( typeVariable | type IDENTIFIER ) "in" ( range | expression ) ")" statement ;
comptimeBlock  := "comptime" block ;

comptimeMatch  := "comptime" "match" "(" ( type | expression ) ")" "{" matchArm ( "," matchArm )* ","? "}" ;
matchArm       := pattern "=>" expression ;
pattern        := type | expression ;
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
call           := primary ( "(" arguments? ")"
                           | "." IDENTIFIER templateArgs?
                           | "[" ( expression | range ) "]" )* ;

primary        := "true" | "false" | "null" | "this"
                 | INT | FLOAT | STRING
                 | IDENTIFIER templateArgs?
                 | "(" expression ")"
                 | IDENTIFIER templateArgs? "{" fieldInits? "}"
                 | "new" IDENTIFIER templateArgs? "{" fieldInits? "}"
                 | "new" ( primitiveType | IDENTIFIER templateArgs? ) "[" expression "]"
                 | foldExpr
                 | operatorRef
                 | packLiteral
                 | comptimeMatch ;
```


## Packs and Folds

```
foldExpr       := "(" "..." binaryOp unary ")"
                 | "(" unary binaryOp "..." ( binaryOp unary )? ")" ;

operatorRef    := "(" ( binaryOp | "!" | "~" ) ")" ;

binaryOp       := "+" | "-" | "*" | "/" | "%"
                 | "&" | "|" | "^" | "<<" | ">>"
                 | "&&" | "||" | "==" | "!=" | "<" | ">" | "<=" | ">=" ;

packLiteral    := "(" ")"
                 | "(" templateArg "," ( templateArg ( "," templateArg )* ","? )? ")" ;

range          := logic_or? ".." logic_or? ;
```


## Utility

```
returnType     := type | "void" ;
type           := typeAtom ( "[" ( expression | range ) "]" )? ( "[" "]" )? ;
typeAtom       := primitiveType | typeName | typeVariable ;
typeName       := IDENTIFIER templateArgs? ( "." IDENTIFIER )* ;
typeVariable   := typeName? "$" IDENTIFIER? templateArgs? ;
primitiveType  := "int" | "float" | "bool" | "string" ;

parameters     := parameter ( "," parameter )* ;
parameter      := type "..."? IDENTIFIER ;
arguments      := argument ( "," argument )* ;
argument       := expression "..."? ;

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
                 | "comptime" | "concept" | "where" | "in" | "match"
                 | primitiveType ;

PUNCTUATION    := "(" | ")" | "{" | "}" | "[" | "]" | "," | ";" | "."
                 | "+" | "-" | "*" | "/" | "%" | "!" | "~"
                 | "&" | "|" | "^" | "<" | ">" | "="
                 | "&&" | "||" | "==" | "!=" | "<=" | ">=" | "<<"
                 | "..." | ".." | "->" | "=>" | "$" | ":" ;
```


## Trivia

```
WHITESPACE     := " " | "\t" | "\r" | "\n" ;
COMMENT        := LINE_COMMENT | BLOCK_COMMENT ;

LINE_COMMENT   := "//" <any char except "\n">* ;
BLOCK_COMMENT  := "/*" <any char>* "*/" ;
```
