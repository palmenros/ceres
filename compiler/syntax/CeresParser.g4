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

parser grammar CeresParser;
options { tokenVocab=CeresLexer; }

compilationUnit
    : (functionDefinition
    | globalVarDeclaration )* EOF
    ;

globalVarDeclaration
    : PUB? varDeclaration SEMICOLON
    ;

functionDefinition
    : PUB? FN IDENTIFIER OPEN_PARENS formalParameters? CLOSE_PARENS type? block
    ;

formalParameters
    : parameter (COMMA parameter)*
    ;

parameter
    : VAR? IDENTIFIER COLON type
    ;

block
    : OPEN_BRACES (statement)* CLOSE_BRACES
    ;

// Note: We don't have special parsing for primitive types, they will be handled in code via
//       the type symbol table.
type
    : IDENTIFIER
    | INTEGER_LITERAL_SUFFIX
    | FLOAT_LITERAL_SUFFIX
    ;

varDeclaration
    : (VAR|CONSTANT) IDENTIFIER (COLON type)? (ASSIGN_OP expression)?
    ;

statement
    : varDeclaration SEMICOLON
    | returnStatement SEMICOLON
    | expression SEMICOLON
    | ifStatement
    | whileStatement
    | forStatement
    | block
    | SEMICOLON
    ;

returnStatement
    : RETURN expression
    ;

ifStatement
    : IF expression block
    | IF expression block ELSE (else_block=block | else_if=ifStatement)
    ;

whileStatement
    : WHILE expression block
    ;

forStatement
    : FOR (varDeclaration | decl_expr=expression)? SEMICOLON (cond_expr=expression)? SEMICOLON (update_expr=expression)? block
    | FOR OPEN_PARENS (varDeclaration | decl_expr=expression)? SEMICOLON (cond_expr=expression)? SEMICOLON (update_expr=expression)? CLOSE_PARENS block // Allow parenthesis
    ;

// Implicit rule precedence (the first that matches)
// See The Definitive ANTLR 4 Reference, 5.4 Dealing with Precedence, Left Recursion, and Associativity

// AssignmentExpression is any expression that doesn't contains commas at the top level.
// This is required to avoid ambiguities in function calls
assignmentExpression
    : primaryExpression                                                                 # primary_expr
    | functionCall                                                                      # function_call_expr
    | assignmentExpression postfix=('++' | '--')                                        # postfix_expr
    | prefix=('+' | '-' |'++'|'--') assignmentExpression                                # prefix_expr
    | prefix=('~'|'!') assignmentExpression                                             # prefix_expr
    | assignmentExpression binary_op=('*'|'/'|'%') assignmentExpression                 # binary_op_expr
    | assignmentExpression binary_op=('+'|'-') assignmentExpression                     # binary_op_expr
    | assignmentExpression ('<' '<' | '>' '>') assignmentExpression                     # binary_op_expr
    | assignmentExpression binary_op=('<=' | '>=' | '>' | '<') assignmentExpression     # binary_op_expr
    | assignmentExpression binary_op='&' assignmentExpression                           # binary_op_expr
    | assignmentExpression binary_op='^' assignmentExpression                           # binary_op_expr
    | assignmentExpression binary_op='|' assignmentExpression                           # binary_op_expr
    | assignmentExpression binary_op=('==' | '!=') assignmentExpression                 # binary_op_expr
    | assignmentExpression binary_op='&&' assignmentExpression                          # binary_op_expr
    | assignmentExpression binary_op='||' assignmentExpression                          # binary_op_expr
    | <assoc=right> IDENTIFIER binary_op=('=' | '+=' | '-=' | '*=' | '/=' | '&=' | '|=' | '^=' | '>>=' | '<<=' | '%=') assignmentExpression # assignment_expr
 ;

expression
    :   assignmentExpression (',' assignmentExpression)*
    ;

primaryExpression
    : OPEN_PARENS expression CLOSE_PARENS   # parens_expr
    | IDENTIFIER                            # id_expr
    | intLiteral                            # int_literal_expr
    | floatLiteral                          # float_literal_expr
    | BOOL_LITERAL                          # bool_literal_expr
    ;

intLiteral
    : DEC_LITERAL INTEGER_LITERAL_SUFFIX?
    | HEX_LITERAL INTEGER_LITERAL_SUFFIX?
    | OCT_LITERAL INTEGER_LITERAL_SUFFIX?
    | BIN_LITERAL INTEGER_LITERAL_SUFFIX?
    ;

floatLiteral
    : FLOAT_LITERAL FLOAT_LITERAL_SUFFIX?
    | DEC_LITERAL FLOAT_LITERAL_SUFFIX
    | HEX_FLOAT_LITERAL FLOAT_LITERAL_SUFFIX?
    ;

functionCall
    : IDENTIFIER OPEN_PARENS ( assignmentExpression (COMMA assignmentExpression)* )? CLOSE_PARENS
    ;
