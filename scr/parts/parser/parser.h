#ifndef IFJ_PRJ_PARSER_H
#define IFJ_PRJ_PARSER_H

#include <stdbool.h>
#include "token.h"
#include "new_symtable.h"

extern token_t *lookahead; // TODO
extern bool nl_flag;    // new line // TODO

bool match(token_type_t type);

bool call_expr_parser(type_t *type, bool *is_literal);

int S();
bool CODE();
bool RETURN(funcData_t **funcData);
bool RET_EXPR(funcData_t **funcData);
bool VAR_DECL();
bool VAR_LET_TYPE();
bool VAR_LET_EXP(token_t *id);
bool LET_DECL();
bool FUNC_DECL();
bool FUNC_RET_TYPE(type_t *type);
bool PARAM_LIST(funcData_t **funcData);
bool PARAM(funcData_t **funcData);
bool PARAM_NAME(funcData_t **funcData);
bool NEXT_PARAM(funcData_t **funcData);
bool BRANCH();
bool BR_EXPR();
bool ELSE();
bool ELSE_IF();
bool WHILE_LOOP();
bool FOR_LOOP();
bool FOR_ID();
bool RANGE();
bool CALL_PARAM_LIST();
bool CALL_PARAM(string_t *call_params);
bool NEXT_CALL_PARAM(string_t *call_params);
bool ID_CALL_OR_ASSIGN();
bool NEXT_ID_CALL_OR_ASSIGN(token_t *id);
bool TYPE(type_t *type);
bool EXPR();
#endif //IFJ_PRJ_PARSER_H
