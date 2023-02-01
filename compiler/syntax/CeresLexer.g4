lexer grammar CeresLexer;

// Keywords

FN : 'fn';
VAR : 'var';
RETURN : 'return';
IF : 'if';
ELSE : 'else';
FOR : 'for';
WHILE : 'while';

// Identifier

IDENTIFIER : [a-zA-Z_][a-zA-Z_0-9]*;

// Number literals

DECIMAL_LITERAL : [0-9]+;

// Types

TYPE_U8 : 'u8';
TYPE_I16 : 'i16';
TYPE_I32 : 'i32';

// Comments

INLINE_COMMENT : '//' .*? '\n' -> skip;
COMMENT : '/*' (COMMENT|.)*? '*/' -> skip;

// Arithmetic operators

SUM_OP : '+';
MINUS_OP : '-';
MULT_OP : '*';
DIV_OP : '/';
MOD_OP : '%';

// Logical operators

LOGICAL_AND_OP : '&&';
LOGICAL_OR_OP : '||';

// Relation operators

ASSIGN_OP : '=';
EQUAL_OP : '==';
NOT_EQUAL_OP : '!=';
GREATER_EQUAL_OP : '>=';
LOWER_EQUAL_OP : '<=';
GREATER_OP : '>';
LOWER_OP : '<';

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
