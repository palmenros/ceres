lexer grammar CeresLexer;

// Keywords

FN : 'fn';
PUB: 'pub';
VAR : 'var';
CONSTANT : 'const';
RETURN : 'return';
IF : 'if';
ELSE : 'else';
FOR : 'for';
WHILE : 'while';
EXTERN: 'extern';

// Literals

INTEGER_LITERAL_SUFFIX: 'u8'
                      | 'u16'
                      | 'u32'
                      | 'u64'
                      | 'i8'
                      | 'i16'
                      | 'i32'
                      | 'i64'
                      ;

FLOAT_LITERAL_SUFFIX: 'f32'
                    | 'f64'
                    ;

DEC_LITERAL: DecDigit (DecDigit | '_')*;
HEX_LITERAL: '0x' '_'* HexDigit (HexDigit | '_')*;
OCT_LITERAL: '0o' '_'* OctDigit (OctDigit | '_')*;
BIN_LITERAL: '0b' '_'* [01] [01_]*;

FLOAT_LITERAL:      (Digits '.' Digits? | '.' Digits) ExponentPart?
             |       Digits ExponentPart
             ;

HEX_FLOAT_LITERAL:  '0' [xX] (HexDigits '.'? | HexDigits? '.' HexDigits) [pP] [+-]? Digits;

BOOL_LITERAL: 'true'
            | 'false'
            ;

// Identifier

IDENTIFIER : [a-zA-Z_][a-zA-Z_0-9]*;

// Note: We don't have special parsing for primitive types, they will be handled in code via
//       the type symbol table.

// Comments

INLINE_COMMENT : '//' .*? '\n' -> skip;
COMMENT : '/*' (COMMENT|.)*? '*/' -> skip;

// Arithmetic operators

UNARY_PLUS_PLUS_OP : '++';
UNARY_MINUS_MINUS_OP : '--';

PLUS_OP : '+';
MINUS_OP : '-';
MULT_OP : '*';
DIV_OP : '/';
MOD_OP : '%';

BITWISE_OR : '|';
BITWISE_AND : '&';
BITWISE_XOR : '^';
BITWISE_NOT : '~';

// Logical operators

LOGICAL_NOT : '!';
LOGICAL_AND_OP : '&&';
LOGICAL_OR_OP : '||';

// Relational operators

EQUAL_OP : '==';
NOT_EQUAL_OP : '!=';
GREATER_EQUAL_OP : '>=';
LOWER_EQUAL_OP : '<=';
GREATER_OP : '>';
LOWER_OP : '<';

// Assignment operators

ASSIGN_OP : '=';
PLUS_ASSIGN_OP: '+=';
MINUS_ASSIGN_OP: '-=';
MULT_ASSIGN_OP : '*=';
DIV_ASSIGN_OP : '/=';
MOD_ASSIGN_OP : '%=';
BITWISE_AND_ASSIGN_OP : '&=';
BITWISE_OR_ASSIGN_OP : '|=';
BITWISE_XOR_ASSIGN_OP : '^=';
BITWISE_RIGHT_SHIFT_ASSIGN_OP : '>>=';
BITWISE_LEFT_SHIFT_ASSIGN_OP : '<<=';

// Punctuation
DOT : '.';
COMMA : ',';
COLON : ':';
SEMICOLON : ';';
OPEN_BRACES : '{';
CLOSE_BRACES : '}';
OPEN_BRACKETS : '[';
CLOSE_BRACKETS : ']';
OPEN_PARENS : '(';
CLOSE_PARENS : ')';

// Whitespaces

WHITESPACES : [ \t\n\r\f]+ -> skip;

// Fragment rules

fragment ExponentPart
    : [eE] [+-]? Digits
    ;

fragment HexDigits
    : HexDigit ((HexDigit | '_')* HexDigit)?
    ;

fragment HexDigit
    : [0-9a-fA-F]
    ;

fragment OctDigit: [0-7];

fragment DecDigit: [0-9];


fragment Digits
    : [0-9] ([0-9_]* [0-9])?
    ;
