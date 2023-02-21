parser grammar CeresParser;
options { tokenVocab=CeresLexer; }

compilationUnit
    : (globalFunctionDefinition
    | globalVarDeclaration
    | externFunDeclaration )* EOF
    ;

globalVarDeclaration
    : PUB? varDeclaration SEMICOLON
    ;

globalFunctionDefinition
    : PUB? functionDefinition
    ;

externFunDeclaration
    : EXTERN FN IDENTIFIER OPEN_PARENS formalParameters? CLOSE_PARENS type? SEMICOLON
    ;

functionDefinition
    : FN IDENTIFIER OPEN_PARENS formalParameters? CLOSE_PARENS type? block
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

type
    : IDENTIFIER
    | INTEGER_LITERAL_SUFFIX
    | FLOAT_LITERAL_SUFFIX
    | BOOL
    ;

varDeclaration
    : (VAR|CONSTANT) IDENTIFIER (COLON type)? (ASSIGN_OP expression)?
    ;

statement
    : varDeclaration SEMICOLON      # var_decl_statement
    | returnStatement SEMICOLON     # return_statement
    | expression SEMICOLON          # expr_statement
    | functionDefinition            # fn_def_statement
    | ifStatement                   # if_statement
    | whileStatement                # while_statement
    | forStatement                  # for_statement
    | block                         # block_statement
    | SEMICOLON                     # empty_statement
    ;

returnStatement
    : RETURN (expression?)
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
    | <assoc=right> assignmentExpression binary_op=('=' | '+=' | '-=' | '*=' | '/=' | '&=' | '|=' | '^=' | '>>=' | '<<=' | '%=') assignmentExpression # assignment_expr
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
