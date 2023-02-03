/*
 * Copyright (C) 2023 Daniel Martin Gomez
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

parser grammar CeresParser;
options { tokenVocab=CeresLexer; }

// https://github.com/antlr/antlr4/issues/606
start
    : program EOF
    ;

program
    : function_definition
    | function_definition program
    ;

function_definition
    : FN IDENTIFIER OPEN_PARENS CLOSE_PARENS block
    | FN IDENTIFIER OPEN_PARENS CLOSE_PARENS type block
    | FN IDENTIFIER OPEN_PARENS declaration_list CLOSE_PARENS block
    | FN IDENTIFIER OPEN_PARENS declaration_list CLOSE_PARENS type block
    ;

block
    : OPEN_BRACES statement_list CLOSE_BRACES
    | OPEN_BRACES CLOSE_BRACES
    ;

type
    : TYPE_U8
    | TYPE_I32
    | TYPE_I16
    | IDENTIFIER
    ;

declaration
    : IDENTIFIER COLON type
    | VAR IDENTIFIER COLON type
    ;

declaration_list
    : declaration
    | declaration COMMA declaration_list
    ;

declaration_statement
    : VAR IDENTIFIER COLON type SEMICOLON
    | VAR IDENTIFIER ASSIGN_OP expression SEMICOLON
    | VAR IDENTIFIER COLON type ASSIGN_OP expression SEMICOLON
    ;

statement
    : declaration_statement
    | assignment SEMICOLON
    | return_statement
    | if_statement
    | function_call SEMICOLON
    | while_statement
    | for_statement
    | expression
    ;

assignment
    : IDENTIFIER ASSIGN_OP expression
    ;

return_statement
    : RETURN expression SEMICOLON
    ;

if_statement
    : IF expression block
    | IF expression block ELSE block
    | IF expression block ELSE if_statement
    ;

while_statement
    : WHILE expression block
    ;

for_statement
    : FOR expression SEMICOLON expression SEMICOLON expression block
    ;

statement_list
    : (statement)+
    ;

// Implicit rule precedence (the first that matches)
// See The Definitive ANTLR 4 Reference, 5.4 Dealing with Precedence, Left Recursion, and Associativity
expression
    : function_call
    | IDENTIFIER
    | DECIMAL_LITERAL
    | expression MOD_OP expression
    | expression DIV_OP expression
    | expression MULT_OP expression
    | expression MINUS_OP expression
    | expression SUM_OP expression
    | expression GREATER_EQUAL_OP expression
    | expression LOWER_EQUAL_OP expression
    | expression GREATER_OP expression
    | expression LOWER_OP expression
    | expression LOGICAL_AND_OP expression
    | expression LOGICAL_OR_OP expression
    | expression NOT_EQUAL_OP expression
    | expression EQUAL_OP expression
    | OPEN_PARENS expression CLOSE_PARENS
    ;

function_call
    : IDENTIFIER OPEN_PARENS CLOSE_PARENS
    | IDENTIFIER OPEN_PARENS expression_list CLOSE_PARENS
    ;

expression_list
    : expression
    | expression COMMA expression_list
    ;
