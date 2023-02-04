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
CONST : 'const';
RETURN : 'return';
IF : 'if';
ELSE : 'else';
FOR : 'for';
WHILE : 'while';

TRUE : 'true';
FALSE : 'false';

// Identifier

IDENTIFIER : [a-zA-Z_][a-zA-Z_0-9]*;

// Number literals

DECIMAL_LITERAL : [0-9]+;

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
