//
// Created by Spagetik on 08-Nov-23.
//

#include <string.h>
#include "new_symtable.h"
#include "parser.h"
#include "../macros.h"
#include <stdio.h>
#include <limits.h>
#include "memory.h"

node_t *currentScope;
node_t *functions;

stack_t *scopeStack;
stack_t *tmpScopeStack;

int scopeDepth;


void tree_init(node_t **root) {
    // if (root == NULL) return;
    *root = NULL;
}

void tree_destroy(node_t **root) {
    // if (root == NULL) return;
    if (*root == NULL) return;
    if ((*root)->left != NULL) {
        tree_destroy(&(*root)->left);
    }
    if ((*root)->right != NULL) {
        tree_destroy(&(*root)->right);
    }
    String.dtor((*root)->key);
    safe_free((*root)->data);
    safe_free(*root);
}

void std_func_init() {
    funcData_t *readString = safe_malloc(sizeof(funcData_t));
    readString->name = String.ctor();
    String.assign_cstr(readString->name, "readString");
    readString->params = String.ctor();
    String.assign_cstr(readString->params, "");
    readString->returnType = nil_string_type;
    add_func(readString);

    funcData_t *readInt = safe_malloc(sizeof(funcData_t));
    readInt->name = String.ctor();
    String.assign_cstr(readInt->name, "readInt");
    readInt->params = String.ctor();
    String.assign_cstr(readInt->params, "");
    readInt->returnType = nil_int_type;
    add_func(readInt);

    funcData_t *readDouble = safe_malloc(sizeof(funcData_t));
    readDouble->name = String.ctor();
    String.assign_cstr(readDouble->name, "readDouble");
    readDouble->params = String.ctor();
    String.assign_cstr(readDouble->params, "");
    readDouble->returnType = nil_double_type;
    add_func(readDouble);

    funcData_t *int2double = safe_malloc(sizeof(funcData_t));
    int2double->name = String.ctor();
    String.assign_cstr(int2double->name, "Int2Double");
    int2double->params = String.ctor();
    String.assign_cstr(int2double->params, "_:i:0");
    int2double->returnType = double_type;
    add_func(int2double);

    funcData_t *double2int = safe_malloc(sizeof(funcData_t));
    double2int->name = String.ctor();
    String.assign_cstr(double2int->name, "Double2Int");
    double2int->params = String.ctor();
    String.assign_cstr(double2int->params, "_:d:1");
    double2int->returnType = int_type;
    add_func(double2int);

    funcData_t *write = safe_malloc(sizeof(funcData_t));
    write->name = String.ctor();
    String.assign_cstr(write->name, "write");
    write->params = String.ctor();
    String.assign_cstr(write->params, "*");
    write->returnType = void_type;
    add_func(write);

    funcData_t *length = safe_malloc(sizeof(funcData_t));
    length->name = String.ctor();
    String.assign_cstr(length->name, "length");
    length->params = String.ctor();
    String.assign_cstr(length->params, "_:s:2");
    length->returnType = int_type;
    add_func(length);

    funcData_t *substr = safe_malloc(sizeof(funcData_t));
    substr->name = String.ctor();
    String.assign_cstr(substr->name, "substring");
    substr->params = String.ctor();
    String.assign_cstr(substr->params, "of:string:2#startingAt:i:0#endingBefore:j:0");
    substr->returnType = nil_string_type;
    add_func(substr);

    funcData_t *ord = safe_malloc(sizeof(funcData_t));
    ord->name = String.ctor();
    String.assign_cstr(ord->name, "ord");
    ord->params = String.ctor();
    String.assign_cstr(ord->params, "_:c:2");
    ord->returnType = int_type;
    add_func(ord);

    funcData_t *chr = safe_malloc(sizeof(funcData_t));
    chr->name = String.ctor();
    String.assign_cstr(chr->name, "chr");
    chr->params = String.ctor();
    String.assign_cstr(chr->params, "_:i:0");
    chr->returnType = string_type;
    add_func(chr);
}

void symtable_init() {
    if (!collect_funcs) {
        tree_init(&currentScope);
        scopeStack = Stack.init();
        tmpScopeStack = Stack.init();
        scopeDepth = 0;
        return;
    }
    tree_init(&currentScope);
    tree_init(&functions);
    scopeStack = Stack.init();
    tmpScopeStack = Stack.init();
    scopeDepth = 0;
    std_func_init();
}

void symtable_destroy() {
    tree_destroy(&currentScope);
    if (!collect_funcs) {
        tree_destroy(&functions);
    }
    int depth = scopeDepth;
    while (depth > 0) {
        depth--;
        node_t *tmp = Stack.top(scopeStack);
        tree_destroy(&tmp);
        Stack.push(tmpScopeStack, tmp);
        Stack.pop(scopeStack);
    }
    Stack.destroy(scopeStack);
    Stack.destroy(tmpScopeStack);
}

void push_frame() {
    Stack.push(scopeStack, currentScope);
    tree_init(&currentScope);
    scopeDepth++;
}

void pop_frame() {
    currentScope = Stack.top(scopeStack);
    Stack.pop(scopeStack);
    scopeDepth--;
    if (collect_funcs) return;
    stack_t *data = Stack.init();
    get_vars_and_lets_from_all_scopes(data);
    update_defines(data, scopeDepth);
}

int tree_get_height(node_t **root){
    if ((*root) == NULL)
        return -1;
    else
        return (*root)->height;
}

int tree_get_balance(node_t **root){
    if ((*root) == NULL)
        return 0;
    return (tree_get_height(&(*root)->right) -  tree_get_height(&((*root)->left)));
}

void tree_node_swap(node_t **first_node, node_t **second_node){
    string_t *temp_key = (*first_node)->key;
    symTableData_t *temp_data = (*first_node)->data;
    (*first_node)->key = (*second_node)->key;
    (*second_node)->key = temp_key;
    (*first_node)->data = (*second_node)->data;
    (*second_node)->data = temp_data;
}

void tree_update_height(node_t **root){
    int left_height = tree_get_height(&((*root)->left));
    int right_height = tree_get_height(&((*root)->right));
    (*root)->height = 1 + max(left_height, right_height);
}

void tree_left_rotate(node_t **root){
    tree_node_swap(&(*root), &(*root)->right);
    node_t *temp_node = (*root)->left;
    (*root)->left = (*root)->right;
    (*root)->right = (*root)->left->right;
    (*root)->right->left = (*root)->right->right;
    (*root)->left->right = (*root)->left->left;
    (*root)->left->left = temp_node;
    tree_update_height(&(*root)->left);
    tree_update_height(&(*root));
}

void tree_right_rotate(node_t **root){
    tree_node_swap(&(*root), &(*root)->left);
    node_t *temp_node = (*root)->right;
    (*root)->right = (*root)->left;
    (*root)->left = (*root)->right->left;
    (*root)->right->left = (*root)->right->right;
    (*root)->right->right = temp_node;
    tree_update_height(&(*root)->right);
    tree_update_height(&(*root));
}

bool tree_add(node_t **root, string_t *key, symTableData_t data) {
    // if (root == NULL) return;
    if (*root == NULL) {
        *root = safe_malloc(sizeof(node_t));
        (*root)->key = String.copy(key);
        (*root)->data = safe_malloc(sizeof(symTableData_t));
        (*root)->height = 0;
        (*root)->data->type = data.type;
        (*root)->data->funcData = data.funcData;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else {
        int cmp = String.cmp(key, (*root)->key);
        if (cmp < 0) {
            return tree_add(&(*root)->left, key, data);
        } else if (cmp > 0) {
            return tree_add(&(*root)->right, key, data);
        } else {
            fprintf(stderr, "Error: symbol already exists.\n");
            return false;
        }
    }
    tree_update_height(&(*root));
    int balance = tree_get_balance(&(*root));
    if (balance > 1) {
        if (tree_get_height(&(*root)->right->left) > tree_get_height(&(*root)->right->right)) {
            tree_right_rotate(&(*root)->right);
        }
        tree_left_rotate(&(*root));
    } else if (balance < -1) {
        if (tree_get_height(&(*root)->left->right) > tree_get_height(&(*root)->left->left)) {
            tree_left_rotate(&(*root)->left);
        }
        tree_right_rotate(&(*root));
    }
    return true;
}

int tree_find(node_t **root, string_t *key, symTableData_t **data, int depth) {
    // if (root == NULL) return -1;
    if (*root == NULL) {
        return -1;
    } else {
        int cmp = String.cmp(key, (*root)->key);
        if (cmp < 0) {
            return tree_find(&(*root)->left, key, data, depth);
        } else if (cmp > 0) {
            return tree_find(&(*root)->right, key, data, depth);
        } else {
            if (data != NULL) *data = (*root)->data;
            return depth;
        }
    }
}

int get_scope() {
    return scopeDepth;
}

int symtable_find(string_t *key, symTableData_t **data) {
    int depth = scopeDepth;
    int found = tree_find(&currentScope, key, data, depth);
    while (depth > 0 && found == -1) {
        depth--;
        node_t *tmp = Stack.top(scopeStack);
        found = tree_find(&tmp, key, data, depth);
        Stack.push(tmpScopeStack, tmp);
        Stack.pop(scopeStack);
    }
    while (depth < scopeDepth) {
        Stack.push(scopeStack, Stack.top(tmpScopeStack));
        Stack.pop(tmpScopeStack);
        depth++;
    }
    return found;
}

void symtable_add(string_t *key, symTableData_t data) {
    tree_add(&currentScope, key, data);
}

void tree_print(node_t **root) {
    // if (root == NULL) return;
    if (*root != NULL) {
        tree_print(&(*root)->left);
        printf("%s\n", (*root)->key->str);
        tree_print(&(*root)->right);
    }
}

void symtable_print() {
    int depth = scopeDepth;
    if (depth == 0) printf("GLOBAL SCOPE:\n");
    else printf("Scope depth: %d\n", depth);
    tree_print(&currentScope);
    while (depth > 0) {
        depth--;
        if (depth == 0) printf("GLOBAL SCOPE:\n");
        else printf("Scope depth: %d\n", depth);
        node_t *tmp = Stack.top(scopeStack);
        tree_print(&tmp);
        Stack.push(tmpScopeStack, tmp);
        Stack.pop(scopeStack);
    }
    while (depth < scopeDepth) {
        Stack.push(scopeStack, Stack.top(tmpScopeStack));
        Stack.pop(tmpScopeStack);
        depth++;
    }
}

bool check_func_signature(string_t *params, funcData_t *funcData) {
    char *paramsStr = safe_malloc(sizeof(char) * (params->length + 1));
    strcpy(paramsStr, params->str);
    char *funcParamsStr = safe_malloc(sizeof(char) * (funcData->params->length + 1));
    strcpy(funcParamsStr, funcData->params->str);
    if (strcmp(funcParamsStr, "*") == 0) {
        safe_free(funcParamsStr);
        safe_free(paramsStr);
        return true;
    }
    char *token = strtok(paramsStr, "#:");
    stack_t *aliasStack = Stack.init();
    stack_t *typeStack = Stack.init();
    char *elem;
    while (token != NULL) {
        elem = safe_malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(elem, token);
        Stack.push(aliasStack, elem);
        token = strtok(NULL, "#:");
        elem = safe_malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(elem, token);
        Stack.push(typeStack, elem);
        token = strtok(NULL, "#:");
    }
    token = strtok(funcParamsStr, "#:");
    stack_t *funcAliasStack = Stack.init();
    stack_t *funcTypeStack = Stack.init();
    while (token != NULL) {
        elem = safe_malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(elem, token);
        Stack.push(funcAliasStack, elem);
        strtok(NULL, "#:");
        token = strtok(NULL, "#:");
        elem = safe_malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(elem, token);
        Stack.push(funcTypeStack, elem);
        token = strtok(NULL, "#:");
    }
    safe_free(funcParamsStr);
    safe_free(paramsStr);
    while (Stack.top(aliasStack) && Stack.top(funcAliasStack)) {
        char *alias = Stack.top(aliasStack);
        char *funcAlias = Stack.top(funcAliasStack);
        if (strcmp(alias, funcAlias) != 0) {
            fprintf(stderr, "Error: function signature mismatch.\n");
            safe_exit(SEMANTIC_ERROR_4);
        }
        safe_free(alias);
        safe_free(funcAlias);
        Stack.pop(aliasStack);
        Stack.pop(funcAliasStack);
    }
    if (Stack.top(aliasStack) || Stack.top(funcAliasStack)) {
        fprintf(stderr, "Error: wrong number of params\n");
        safe_exit(SEMANTIC_ERROR_4);
    }
    while (Stack.top(typeStack) && Stack.top(funcTypeStack)) {
        char *type = Stack.top(typeStack);
        char *funcType = Stack.top(funcTypeStack);
        if (strcmp(type, funcType) != 0) {
            fprintf(stderr, "Error: function signature mismatch.\n");
            safe_exit(SEMANTIC_ERROR_4);
        }
        safe_free(type);
        safe_free(funcType);
        Stack.pop(typeStack);
        Stack.pop(funcTypeStack);
    }
    if (Stack.top(typeStack) || Stack.top(funcTypeStack)) {
        fprintf(stderr, "Error: wrong number of params\n");
        safe_exit(SEMANTIC_ERROR_4);
    }
    Stack.destroy(aliasStack);
    Stack.destroy(funcAliasStack);
    Stack.destroy(typeStack);
    Stack.destroy(funcTypeStack);
    return true;
}

bool add_func(funcData_t *funcData) {
    symTableData_t data;
    data.type = ndFunc;
    data.funcData = funcData;
    if (data.funcData->params->length == 0) {
        if (tree_find(&currentScope, funcData->name, NULL, 0) != -1) {
            fprintf(stderr, "Error: function name collision with var.\n");
            safe_exit(SEMANTIC_ERROR_9);
        }
    }
    if (collect_funcs) {
        if (data.funcData->params->length > 0) {
            char *paramsStr = safe_malloc(sizeof(char) * (funcData->params->length + 1));
            strcpy(paramsStr, funcData->params->str);
            if (strcmp(paramsStr, "*") != 0) {
                char *token = strtok(paramsStr, "#:");
                while (token != NULL) {
                    char *alias = safe_malloc(sizeof(char) * (strlen(token) + 1));
                    strcpy(alias, token);
                    token = strtok(NULL, "#:");
                    char *param = safe_malloc(sizeof(char) * (strlen(token) + 1));
                    strcpy(param, token);
                    if (strcmp(alias, param) == 0) {
                        fprintf(stderr, "Error: param name should be diff from its id.\n");
                        safe_exit(SEMANTIC_ERROR_9);
                    }
                    safe_free(alias);
                    safe_free(param);
                    strtok(NULL, "#:");
                    token = strtok(NULL, "#:");
                }
            }
            safe_free(paramsStr);
        }
        return tree_add(&functions, funcData->name, data);
    }
    return true;
}

bool add_var(varData_t *varData) {
    symTableData_t data;
    data.type = ndVar;
    data.varData = varData;
    data.varData->scope = scopeDepth;
    symTableData_t *funcData = safe_malloc(sizeof(symTableData_t));
    if (tree_find(&functions, varData->name, &funcData, 0) != -1) {
        if (funcData->funcData->params->length == 0) {
            fprintf(stderr, "Error: var name collision with function.\n");
            safe_exit(SEMANTIC_ERROR_9);
        }
    }
    safe_free(funcData);
    bool s = tree_add(&currentScope, varData->name, data);
    if (!s) {
        symTableData_t *data2 = safe_malloc(sizeof(symTableData_t));
        symtable_find(varData->name, &data2);
        bool tmp = data2->varData->canBeRedefined;
        data2->varData->canBeRedefined = false;
        return tmp;
    }
    return s;
}

bool add_let(letData_t *letData) {
    symTableData_t data;
    data.type = ndLet;
    data.letData = letData;
    data.letData->scope = scopeDepth;
    symTableData_t *funcData = safe_malloc(sizeof(symTableData_t));
    if (tree_find(&functions, letData->name, &funcData, 0) != -1) {
        if (funcData->funcData->params->length == 0) {
            fprintf(stderr, "Error: let name collision with function.\n");
            safe_exit(SEMANTIC_ERROR_9);
        }
    }
    safe_free(funcData);
    bool s = tree_add(&currentScope, letData->name, data);
    if (!s) {
        symTableData_t *data2 = safe_malloc(sizeof(symTableData_t));
        symtable_find(letData->name, &data2);
        bool tmp = data2->letData->canBeRedefined;
        data2->letData->canBeRedefined = false;
        return tmp;
    }
    return s;
}

void new_frame() {
    push_frame();
}

void del_frame() {
    pop_frame();
}

void get_vars_and_lets(node_t **root, stack_t *stack) {
    if (*root != NULL) {
        get_vars_and_lets(&(*root)->left, stack);
        if ((*root)->data->type == ndVar || (*root)->data->type == ndLet) {
            Stack.push(stack, (*root)->data->varData);
        }
        get_vars_and_lets(&(*root)->right, stack);
    }
}

void get_vars_and_lets_from_all_scopes(stack_t *stack) {
    int depth = scopeDepth;
    get_vars_and_lets(&currentScope, stack);
    while (depth > 0) {
        depth--;
        node_t *tmp = Stack.top(scopeStack);
        get_vars_and_lets(&tmp, stack);
        Stack.push(tmpScopeStack, tmp);
        Stack.pop(scopeStack);
    }
    while (depth < scopeDepth) {
        Stack.push(scopeStack, Stack.top(tmpScopeStack));
        Stack.pop(tmpScopeStack);
        depth++;
    }
}

void update_defines(stack_t *data, int scope) {
    // if var/let min defined scope is higher than current scope, reset it and swt to undefined
    while (Stack.top(data)) {
        varData_t *varData = (varData_t *) Stack.top(data);
        if (varData->minInitScope > scope) {
            if (varData->initInBranch) {
                if (varData->minInitBranchNumber == branch_number && varData->isDeclared) {
                    varData->initInAllBranches = varData->initInAllBranches && true;
                }
                else {
                    varData->initInAllBranches = false;
                }
                varData->isDeclared = false;
            } else {
                varData->isDeclared = false;
                varData->minInitScope = INT_MAX;
            }
        }
        Stack.pop(data);
    }
}

void update_defines_after_branch(int scope, int branch_blocks) {
    // if var/let min defined scope is higher than current scope, reset it and swt to undefined
    if (collect_funcs) return;
    stack_t *data = Stack.init();
    get_vars_and_lets_from_all_scopes(data);
    while (Stack.top(data)) {
        varData_t *varData = (varData_t *) Stack.top(data);
        if (varData->minInitScope > scope) {
            if (varData->initInBranch) {
                if (varData->initInAllBranches && varData->numberBranchBlocks == branch_blocks) {
                    varData->isDeclared = true;
                    varData->minInitScope = varData->minInitScope - 1;
                    varData->initInBranch = false;
                }
                else {
                    varData->isDeclared = false;
                    varData->initInBranch = false;
                    varData->minInitScope = INT_MAX;
                }
            } else {
                varData->isDeclared = false;
                varData->minInitScope = INT_MAX;
            }
        }
        Stack.pop(data);
    }
}

funcData_t *get_func(string_t *name) {
    symTableData_t *data = safe_malloc(sizeof(symTableData_t));
    int found = tree_find(&functions, name, &data, 0);
    if (found == -1) {
        if (collect_funcs) return NULL;
        fprintf(stderr, "Error: you use undefined func `%s`\n", name->str);
        safe_exit(SEMANTIC_ERROR_3);
    }
    return data->funcData;
}

varData_t *get_var(string_t *name) {
    symTableData_t *data = safe_malloc(sizeof(symTableData_t));
    int found = symtable_find(name, &data);
    if (found == -1) {
        fprintf(stderr, "Error: usage of undefined var\n");
        safe_exit(SEMANTIC_ERROR_5);
    }
    if (data->type != ndVar) {
        return NULL;
    }
    return data->varData;
}

letData_t *get_let(string_t *name) {
    symTableData_t *data = safe_malloc(sizeof(symTableData_t));
    int found = symtable_find(name, &data);
    if (found == -1) {
        fprintf(stderr, "Error: usage of undefined const\n");
        safe_exit(SEMANTIC_ERROR_5);
    }
    if (data->type != ndLet) {
        return NULL;
    }
    return data->letData;
}




