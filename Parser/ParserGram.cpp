#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// #include "ParserType.h"
#include "../BinTree/Header/BinTreeFunc.h"
#include "../NameTable/NameTableFunc.h"
// #include "../Common/AllTypes.h"
#include "ParserGram.h"


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания полного выражения (Правило Global)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] table_var Таблица имен переменных
 @param [in] table_cmd Таблица имен функций
 @return Указатель на поддерево выражения
*/
Node_t* parserGetGlobal (ParserPlaceInf_t* inf,
                         NameTable_t* table_var,
                         NameTable_t* table_cmd)
{
    assert (inf);
    assert (inf->pose);
    assert (table_var);
    assert (table_cmd);

    Node_t* null = parserGetAddSub (inf, table_var, table_cmd);
    null->parent = NULL;

    if (**(inf->pose) != '$')
        SYNTAX_ERROR(inf, NO_END_CHAR, null);

    return null;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания чисел и переменных (Правило NumVar)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] table_var Таблица имен переменных
 @param [in] table_cmd Таблица имен функций
 @return Указатель на поддерево выражения
*/
Node_t* parserGetNumVar (ParserPlaceInf_t* inf,
                         NameTable_t* table_var,
                         NameTable_t* table_cmd)
{
    assert (inf);
    assert (inf->pose);
    assert (table_var);
    assert (table_cmd);

    if (inf->error != NOT_ERROR)
        return NULL;
    Node_t* node = newNode ();

    char** pose = inf->pose;
    if (**pose >= '0' && **pose <= '9')
    {
        parserNum (inf, node);
    }
    else if (isalpha (**pose))
    {
        if (parserFunc (inf, node, table_cmd) == 1)
        {
            node->right = parserGetAddSub (inf, table_var, table_cmd);
            node->right->parent = node;
            if (**pose == ',')
            {
                ++ *pose;
                node->left = parserGetAddSub (inf, table_var, table_cmd);
                node->left->parent = node;
            }
            else
            {
                node->left = NULL;
            }
            ++ *pose; // Skip ')'
        }
        else
        {
            parserVar (inf, node, table_var);
        }
    }
    else
        SYNTAX_ERROR(inf, UNKOWN_NUM_VAR, node);

    return node;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция чтения чисел (Правило Num)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] node Указатель на узел, куда будет записано число
*/
int parserNum (ParserPlaceInf_t* inf,
               Node_t* node)
{
    assert (inf);
    assert (inf->pose);
    assert (node);

    char** pose = inf->pose;
    double val_whole = 0.0;
    double val_frac = 0.0;
    int amount_frac_num = 0;
    bool frac = false;

    while ((**pose >= '0' && **pose <= '9') || (**pose == '.'))
    {
        if (**pose == '.')
        {
            frac = true;
            continue;
        }
        if (frac)
        {
            val_frac += (double (**pose - '0')) / pow (10, amount_frac_num + 1);
            amount_frac_num++;
        }
        else
        {
            val_whole = val_whole * 10 + (double (**pose - '0'));
        }
        ++ *pose;
    }
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->type = _TYPE_NUM;
    node->value.dval = val_whole + val_frac;

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция чтения функций (правило Func)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] node Указатель на узел с функцией
 @param [in] table_cmd Таблица имен функций
 @return 1, если это функция, иначе 0
*/
int parserFunc (ParserPlaceInf_t* inf,
                Node_t* node,
                NameTable_t* table_cmd)
{
    assert (inf);
    assert (inf->pose);
    assert (node);
    assert (table_cmd);

    char* str = *(inf->pose);
    size_t i = 0;
    while (isalpha (str[i]) || str[i] == '_')   { i++; }

    if (str[i] != '(')
        return 0;

    char* name = (char*) calloc (i + 1, sizeof (char));
    if (name == NULL)
        EXIT_FUNC("NULL calloc", 0);

    strncpy (name, str, i);
    name[i] = '\0';

    int index = nameTableFind (table_cmd, name);
    free (name);
    if (index == -1)    { return 0; }

    node->type = _TYPE_OPER;
    node->value.ival = index;
    *(inf->pose) += i + 1; // Skip '('
    return 1;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция чтения переменных (Правило Var)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] node Указатель на узел с переменной
 @param [in] table_var Таблица имен переменных
*/
int parserVar (ParserPlaceInf_t* inf,
               Node_t* node,
               NameTable_t* table_var)
{
    assert (inf);
    assert (inf->pose);
    assert (node);
    assert (table_var);

    char* str = *(inf->pose);
    size_t i = 0;
    while (isalpha (str[i]) || isdigit (str[i]) || str[i] == '_')   { i++; }

    char* name = (char*) calloc (i + 1, sizeof (char));
    if (name == NULL)
        EXIT_FUNC("NULL calloc", 0);

    strncpy (name, str, i);
    name[i] = '\0';

    int index = nameTableFind (table_var, name);
    if (index == -1)
        index = nameTableAdd (table_var, name, 0.0, i);

    free (name);
    node->type = _TYPE_VAR;
    node->value.ival = index;
    *(inf->pose) += i; // Skip '('
    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания скобок (Правило Brack)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] table_var Таблица имен переменных
 @param [in] table_cmd Таблица имен функций
 @return Указатель на поддерево выражения
*/
Node_t* parserGetBrackets (ParserPlaceInf_t* inf,
                           NameTable_t* table_var,
                           NameTable_t* table_cmd)
{
    assert (inf);
    assert (inf->pose);
    assert (table_var);
    assert (table_cmd);

    Node_t* node = NULL;

    char** pose = inf->pose;
    if (**pose == '(')
    {
        ++ *pose;
        node = parserGetAddSub (inf, table_var, table_cmd);

        if (**pose != ')')
            SYNTAX_ERROR(inf, NO_BACK_BRACKET, node);

        ++ *pose;
    }
    else    { node = parserGetNumVar (inf, table_var, table_cmd); }
    return node;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания сложений и вычитаний выражений (Правило AddSub)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] table_var Таблица имен переменных
 @param [in] table_cmd Таблица имен функций
 @return Указатель на поддерево выражения
*/
Node_t* parserGetAddSub (ParserPlaceInf_t* inf,
                         NameTable_t* table_var,
                         NameTable_t* table_cmd)
{
    assert (inf);
    assert (inf->pose);
    assert (table_var);
    assert (table_cmd);

    Node_t* node = parserGetMulDiv (inf, table_var, table_cmd);
    char** pose = inf->pose;

    while (**pose == '+' || **pose == '-')
    {
        bool is_plus = false;
        if (**pose == '+')  { is_plus = true; }
        ++ *pose;

        Node_t* left = node;
        node = newNode ();

        node->type = _TYPE_OPER;
        node->left = left;
        node->right = parserGetMulDiv (inf, table_var, table_cmd);
        node->parent = NULL;
        node->left->parent = node;
        node->right->parent = node;

        if (is_plus)    { node->value.ival = ADD_OPER; }
        else            { node->value.ival = SUB_OPER; }
    }
    return node;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания умножения и деления выражений (Правило MulDiv)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @param [in] table_var Таблица имен переменных
 @param [in] table_cmd Таблица имен функций
 @return Указатель на поддерево выражения
*/
Node_t* parserGetMulDiv (ParserPlaceInf_t* inf,
                         NameTable_t* table_var,
                         NameTable_t* table_cmd)
{
    assert (inf);
    assert (inf->pose);
    assert (inf);
    assert (table_cmd);

    Node_t* node = parserGetBrackets (inf, table_var, table_cmd);
    char** pose = inf->pose;

    while (**pose == '*' || **pose == '/')
    {
        bool is_mul = false;
        if (**pose == '*')  { is_mul = true; }
        ++ *pose;

        Node_t* left = node;
        node = newNode ();

        node->type = _TYPE_OPER;
        node->left = left;
        node->right = parserGetBrackets (inf, table_var, table_cmd);
        node->parent = NULL;
        node->left->parent = node;
        node->right->parent = node;

        if (is_mul)     { node->value.ival = MUL_OPER; }
        else            { node->value.ival = DIV_OPER; }
    }
    return node;
}
// ---------------------------------------------------------------------------------------------------
