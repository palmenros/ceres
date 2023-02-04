/*
 * Copyright (C) 2023 Daniel Martin Gomez, Pedro Palacios Almendros
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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

// Identifier

IDENTIFIER : [a-zA-Z_][a-zA-Z_0-9]*;

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

DECIMAL_LITERAL:    ('0' | [1-9] (Digits? | '_'+ Digits)) INTEGER_LITERAL_SUFFIX?;
HEX_LITERAL:        '0' [xX] [0-9a-fA-F] ([0-9a-fA-F_]* [0-9a-fA-F])? INTEGER_LITERAL_SUFFIX?;
OCT_LITERAL:        '0' 'oO'* [0-7] ([0-7_]* [0-7])? INTEGER_LITERAL_SUFFIX?;
BINARY_LITERAL:     '0' [bB] [01] ([01_]* [01])? INTEGER_LITERAL_SUFFIX?;

FLOAT_LITERAL:      (Digits '.' Digits? | '.' Digits) ExponentPart? FLOAT_LITERAL_SUFFIX?
             |       Digits (ExponentPart FLOAT_LITERAL_SUFFIX? | FLOAT_LITERAL_SUFFIX)
             ;

HEX_FLOAT_LITERAL:  '0' [xX] (HexDigits '.'? | HexDigits? '.' HexDigits) [pP] [+-]? Digits FLOAT_LITERAL_SUFFIX?;

BOOL_LITERAL: 'true'
            | 'false'
            ;


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

fragment Digits
    : [0-9] ([0-9_]* [0-9])?
    ;
