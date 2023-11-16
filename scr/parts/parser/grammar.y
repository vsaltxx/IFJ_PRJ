S ::= CODE TOKEN_EOF
CODE ::= VAR_DECL NL CODE
CODE ::= LET_DECL NL CODE
CODE ::= FUNC_DECL NL CODE
CODE ::= WHILE_LOOP NL CODE
CODE ::= FOR_LOOP NL CODE
CODE ::= BRANCH NL CODE
CODE ::= ID_CALL_OR_ASSIGN NL CODE
CODE ::= RETURN
CODE ::= TOKEN_BREAK
CODE ::= TOKEN_CONTINUE
CODE ::= ''
RETURN ::= TOKEN_RETURN RET_EXPR 
RET_EXPR ::= EXPR
RET_EXPR ::= ''
VAR_DECL ::= TOKEN_VAR TOKEN_IDENTIFIER VAR_LET_TYPE VAR_LET_EXP 
VAR_LET_TYPE ::= TOKEN_COLON TYPE
VAR_LET_TYPE ::= ''
VAR_LET_EXP ::= TOKEN_ASSIGNMENT EXPR
VAR_LET_EXP ::= ''
LET_DECL ::= TOKEN_LET TOKEN_IDENTIFIER VAR_LET_TYPE VAR_LET_EXP 
FUNC_DECL ::= TOKEN_FUNC TOKEN_IDENTIFIER TOKEN_LEFT_BRACKET PARAM_LIST TOKEN_RIGHT_BRACKET FUNC_RET_TYPE TOKEN_LEFT_BRACE CODE TOKEN_RIGHT_BRACE 
FUNC_RET_TYPE ::= TOKEN_ARROW TYPE
FUNC_RET_TYPE ::= ''
PARAM_LIST ::= PARAM NEXT_PARAM
PARAM_LIST ::= ''
PARAM ::= PARAM_NAME TOKEN_IDENTIFIER TOKEN_COLON TYPE
PARAM_NAME ::= TOKEN_IDENTIFIER
PARAM_NAME ::= TOKEN_UNDERSCORE
NEXT_PARAM ::= TOKEN_COMMA PARAM NEXT_PARAM
NEXT_PARAM ::= ''
BRANCH ::= TOKEN_IF BR_EXPR TOKEN_LEFT_BRACE CODE TOKEN_RIGHT_BRACE ELSE 
BR_EXPR ::= EXPR
BR_EXPR ::= TOKEN_LET TOKEN_IDENTIFIER
ELSE ::= TOKEN_ELSE ELSE_IF
ELSE ::= ''
ELSE_IF ::= TOKEN_IF BR_EXPR TOKEN_LEFT_BRACE CODE TOKEN_RIGHT_BRACE ELSE
ELSE_IF ::= TOKEN_LEFT_BRACE CODE TOKEN_RIGHT_BRACE
ELSE_IF ::= ''
WHILE_LOOP ::= TOKEN_WHILE EXPR TOKEN_LEFT_BRACE CODE TOKEN_RIGHT_BRACE 
FOR_LOOP ::= TOKEN_FOR FOR_ID TOKEN_IN EXPR RANGE TOKEN_LEFT_BRACE CODE TOKEN_RIGHT_BRACE 
FOR_ID ::= TOKEN_IDENTIFIER
FOR_ID ::= TOKEN_UNDERSCORE
RANGE ::= TOKEN_CLOSED_RANGE EXPR
RANGE ::= TOKEN_HALF_OPEN_RANGE EXPR
CALL_PARAM_LIST ::= CALL_PARAM NEXT_CALL_PARAM
CALL_PARAM_LIST ::= ''
CALL_PARAM ::= NAMED_CALL_PARAM
CALL_PARAM ::= POS_CALL_PARAM
NAMED_CALL_PARAM ::= CALL_PARAM_NAME TOKEN_COLON EXPR
CALL_PARAM_NAME ::= TOKEN_IDENTIFIER
CALL_PARAM_NAME ::= TOKEN_UNDERSCORE
POS_CALL_PARAM ::= EXPR
NEXT_CALL_PARAM ::= TOKEN_COMMA CALL_PARAM NEXT_CALL_PARAM
NEXT_CALL_PARAM ::= ''
ID_CALL_OR_ASSIGN ::= TOKEN_IDENTIFIER NEXT_ID_CALL_OR_ASSIGN 
NEXT_ID_CALL_OR_ASSIGN ::= TOKEN_LEFT_BRACKET CALL_PARAM_LIST TOKEN_RIGHT_BRACKET
NEXT_ID_CALL_OR_ASSIGN ::= TOKEN_ASSIGNMENT EXPR
TYPE ::= TOKEN_STRING_TYPE
TYPE ::= TOKEN_INT_TYPE
TYPE ::= TOKEN_BOOL_TYPE
TYPE ::= TOKEN_DOUBLE_TYPE
EXPR ::= TOKEN_IDENTIFIER
EXPR ::= TOKEN_REAL_LITERAL
EXPR ::= TOKEN_STRING_LITERAL
EXPR ::= TOKEN_NIL_LITERAL
EXPR ::= TOKEN_TRUE_LITERAL
EXPR ::= TOKEN_FALSE_LITERAL
EXPR ::= TOKEN_INTEGER_LITERAL
EXPR ::= TOKEN_ADDITION
EXPR ::= TOKEN_SUBTRACTION
EXPR ::= TOKEN_MULTIPLICATION
EXPR ::= TOKEN_DIVISION
EXPR ::= TOKEN_LESS_THAN
EXPR ::= TOKEN_LESS_THAN_OR_EQUAL_TO
EXPR ::= TOKEN_GREATER_THAN
EXPR ::= TOKEN_GREATER_THAN_OR_EQUAL_TO
EXPR ::= TOKEN_EQUAL_TO
EXPR ::= TOKEN_NOT_EQUAL_TO
EXPR ::= TOKEN_IS_NIL
EXPR ::= TOKEN_UNWRAP_NILABLE
EXPR ::= TOKEN_LOGICAL_AND
EXPR ::= TOKEN_LOGICAL_OR
EXPR ::= TOKEN_LOGICAL_NOT
EXPR ::= TOKEN_LEFT_BRACE
EXPR ::= TOKEN_RIGHT_BRACE