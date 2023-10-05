//
// Created by Spagetik on 02.10.2023.
//

#ifndef IFJ_PRJ_LEXER_H
#define IFJ_PRJ_LEXER_H

#include "utils.h"

typedef enum {
    TOKEN_IDENTIFIER,           //      S to [a-zA-Z_] to [a-zA-Z0-9_]
    TOKEN_OPERATOR,             //
    TOKEN_KEYWORD,              //
    TOKEN_LITERAL,              //
    TOKEN_PUNCTUATOR,           //
} token_type_t;

typedef enum {
    ASSIGNMENT,                 // =    S to =

    ADDITION,                   // +    S to +
    SUBTRACTION,                // -    S to -
    MULTIPLICATION,             // *    S to *
    DIVISION,                   // /    S to /

    LESS_THAN,                  // <    S to <
    LESS_THAN_OR_EQUAL_TO,      // <=   S to < to =
    GREATER_THAN,               // >    S to >
    GREATER_THAN_OR_EQUAL_TO,   // >=   S to > to =
    EQUAL_TO,                   // ==   S to = to =
    NOT_EQUAL_TO,               // !=   S to ! to =

    NILABLE,                    // ?    S to ?
    IS_NIL,                     // ??   S to ? to ?
    UNWRAP_NILABLE,             // .!   S to !

    // PREMIUM
    LOGICAL_AND,                // &&   S to & to &
    LOGICAL_OR,                 // ||   S to | to |
    LOGICAL_NOT,                // !.   S to !
} operator_type_t;

typedef enum {
    INTEGER,                    // int      S to [0-9]
    REAL,                       // real     S to [0-9] to . to [0-9]
                                //          S to [0-9] to . to [0-9] to e|E to [0-9]
                                //          S to [0-9] to . to [0-9] to e|E to +|- to [0-9]
    STRING,                     // string   S to " to [^"] to "
                                //          S to " to " to " to \n to [^"] to " to " to " to \n
    NIL,                        // nil

    // PREMIUM
    BOOL,                       // bool
} literal_type_t;

typedef enum {
    DOUBLE_TYPE,                // Double
    ELSE,                       // else
    FUNC,                       // func
    IF,                         // if
    INT_TYPE,                   // Int
    LET,                        // let
    RETURN,                     // return
    STRING_TYPE,                // String
    VAR,                        // var
    WHILE,                      // while

    // PREMIUM
    BOOL_TYPE,                  // Bool
} keyword_type_t;

typedef enum {
    LEFT_BRACKET,           // (        S to (
    RIGHT_BRACKET,              // )        S to )
    LEFT_BRACE,                 // {        S to {
    RIGHT_BRACE,                // }        S to }
    COMMA,                      // ,        S to ,
    SEMICOLON,                  // ;        S to ;
    ARROW,                      // ->       S to - to >
} punctuator_type_t;

typedef union {
    operator_type_t operator_type;
    literal_type_t literal_type;
    keyword_type_t keyword_type;
    punctuator_type_t punctuator_type;
} token_subtype_t;

typedef struct {
    token_type_t type;
    token_subtype_t subtype;
    string_t *lexeme;
} token_t;

typedef struct {
    token_t **array;
    int length;
    int allocated;
} token_array_t;

typedef enum {
    START_S,
} fsm_state_t;

token_t *token_init(token_type_t type, token_subtype_t subtype, string_t *lexeme);

void token_dtor(token_t *token);

token_array_t *token_array_init();

void token_array_dtor(token_array_t *token_array);

int token_array_add(token_array_t *token_array, token_t *token);

token_array_t *source_code_to_tokens(file_t *file);



#endif //IFJ_PRJ_LEXER_H
