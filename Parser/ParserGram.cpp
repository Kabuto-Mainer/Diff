#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "../BinTree/Header/BinTreeFunc.h"
// #include "../Common/AllTypes.h"
// #include "ParserType.h"
#include "ParserGram.h"


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания полного выражения (Правило Global)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @return Указатель на поддерево выражения
*/
Node_t* parserGetGlobal (ParserPlaceInf_t* inf)
{
    assert (inf);
    assert (inf->pose);

    Node_t* null = parserGetAddSub (inf);
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
 @return Указатель на поддерево выражения
*/
Node_t* parserGetNumVar (ParserPlaceInf_t* inf)
{
    assert (inf);
    assert (inf->pose);

    if (inf->error != NOT_ERROR)
        return NULL;
    Node_t* node = newNode ();

    char** pose = inf->pose;
    if ((**pose >= '0' && **pose <= '9'))
    {
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
        }
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->type = _TYPE_NUM;
        node->value.dval = val_whole + val_frac;
    }
    else    {}
    return node;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания скобок (Правило Brack)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @return Указатель на поддерево выражения
*/
Node_t* parserGetBrackets (ParserPlaceInf_t* inf)
{
    assert (inf);
    assert (inf->pose);

    Node_t* node = NULL;

    char** pose = inf->pose;
    if (**pose == '(')
    {
        ++ *pose;
        node = parserGetAddSub (inf);

        if (**pose != ')')
            SYNTAX_ERROR(inf, NO_BACK_BRACKET, node);

        ++ *pose;
    }
    else    { node = parserGetNumVar (pose); }
    return node;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция считывания сложений и вычитаний выражений (Правило AddSub)
 @param [in] inf Структура с информацией о текущем положении и ошибке
 @return Указатель на поддерево выражения
*/
Node_t* parserGetAddSub (ParserPlaceInf_t* inf)
{
    assert (inf);
    assert (inf->pose);

    Node_t* node = parserGetMulDiv (inf);
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
        node->right = parserGetMulDiv (inf);
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
 @return Указатель на поддерево выражения
*/
Node_t* parserGetMulDiv (ParserPlaceInf_t* inf)
{
    assert (inf);
    assert (inf->pose);

    Node_t* node = parserGetBrackets (inf);
    char** pose = inf->pose;

    while (**pose == '*' || **pose == '/')
    {
        bool is_mul = false;
        if (**pose == '+')  { is_mul = true; }
        ++ *pose;

        Node_t* left = node;
        node = newNode ();

        node->type = _TYPE_OPER;
        node->left = left;
        node->right = parserGetBrackets (inf);
        node->parent = NULL;
        node->left->parent = node;
        node->right->parent = node;

        if (is_mul)     { node->value.ival = MUL_OPER; }
        else            { node->value.ival = DIV_OPER; }
    }
    return node;
}
// ---------------------------------------------------------------------------------------------------
