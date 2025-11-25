#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "../../Common/AllTypes.h"
#include "../../Common/Comands.h"
#include "../Header/BinTreeType.h"
#include "../Header/BinTreeCalcFunc.h"


// --------------------------------------------------------------------------------------------------
/**
 @brief Функция расчета математических операций
 @param [in] node Указатель на узел с операцией
 @return Указатель на числовой узел
*/
Node_t* calc_Math (Node_t* node)
{
    assert (node);

    if (node->value.ival == DIV_OPER && fabs (node->right->value.dval) < EPS)
        CALC_ERROR("Деление на 0", node);

    node->type = _TYPE_NUM;
    switch (node->value.ival)
    {
        case ADD_OPER: { node->value.dval = node->left->value.dval + node->right->value.dval; break; }
        case SUB_OPER: { node->value.dval = node->left->value.dval - node->right->value.dval; break; }
        case MUL_OPER: { node->value.dval = node->left->value.dval * node->right->value.dval; break; }
        case DIV_OPER: { node->value.dval = node->left->value.dval / node->right->value.dval; break; }

        default: { CALC_ERROR("Не корректный тип", node); }
    }

    free (node->left);
    free (node->right);

    node->left = NULL;
    node->right = NULL;

    return node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция расчета тригонометрических операций
 @param [in] node Указатель на узел с операцией
 @return Указатель на числовой узел
*/
Node_t* calc_Trig (Node_t* node)
{
    assert (node);

    if (node->value.ival == TAN_OPER && fabs (cos (node->right->value.dval)) < EPS)
        CALC_ERROR("Косинус равен нулю в вычислении тангенса", node);

    if (node->value.ival == COT_OPER && fabs (sin (node->right->value.dval)) < EPS)
        CALC_ERROR("Синус равен нулю в вычислении котангенса", node);

    node->type = _TYPE_NUM;
    switch (node->value.ival)
    {
        case SIN_OPER: { node->value.dval = sin (node->right->value.dval); break; }
        case COS_OPER: { node->value.dval = cos (node->right->value.dval); break; }
        case TAN_OPER: { node->value.dval = tan (node->right->value.dval); break; }
        case COT_OPER: { node->value.dval = cot (node->right->value.dval); break; }

        default: { CALC_ERROR("Не корректный тип", node); }
    }

    if (node->left != NULL)     { free (node->left); }
    free (node->right);

    node->left = NULL;
    node->right = NULL;

    return node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция расчета обратных тригонометрических операций
 @param [in] node Указатель на узел с операцией
 @return Указатель на числовой узел
*/
Node_t* calc_ATrig (Node_t* node)
{
    assert (node);

    if ((node->value.ival== ASIN_OPER || node->value.ival == ACOS_OPER) &&
        (node->right->value.dval < -1 || node->right->value.dval > 1))
        CALC_ERROR("Модуль аргумента asin\\acos |x| > 1", node);

    if (node->value.ival == ACOT_OPER && fabs (node->right->value.dval) < EPS)
        CALC_ERROR("Нулевой аргумент у arccot", node);

    node->type = _TYPE_NUM;
    switch (node->value.ival)
    {
        case (ASIN_OPER): { node->value.dval = asin (node->right->value.dval); break; }
        case (ACOS_OPER): { node->value.dval = acos (node->right->value.dval); break; }
        case (ATAN_OPER): { node->value.dval = atan (node->right->value.dval); break; }
        case (ACOT_OPER): { node->value.dval = acot (node->right->value.dval); break; }

        default: { CALC_ERROR("Не корректный тип", node); }
    }

    if (node->left != NULL) { free (node->left); }
    free (node->right);

    node->left = NULL;
    node->right = NULL;

    return node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция расчета степенных операций
 @param [in] node Указатель на узел с операцией
 @return Указатель на числовой узел
*/
Node_t* calc_Degree (Node_t* node)
{
    assert (node);

    ALL_OPER t_oper = (ALL_OPER) node->value.ival;
    if ((t_oper == LN_OPER) &&
        node->right->value.dval < 0)
        CALC_ERROR("Аргумент логарифма меньше 0", node);

    if (t_oper == POW_OPER && node->left->value.dval < 0 &&
        fabs (node->right->value.dval - round (node->right->value.dval)) > EPS)
        CALC_ERROR("Основание степени меньше 0, а показатель не N", node);

    if (t_oper == LOG_OPER && (node->left->value.dval <= 0 ||
        node->right->value.dval <=0 ))
        CALC_ERROR("Аргумент или основание логарифма меньше 0", node);

    node->type = _TYPE_NUM;
    switch (node->value.ival)
    {
        case (LOG_OPER): { node->value.dval = log (node->right->value.dval) / log (node->left->value.dval); break; }
        case (LN_OPER):  { node->value.dval = log (node->right->value.dval); break; }
        case (POW_OPER): { node->value.dval = pow (node->left->value.dval, node->right->value.dval); break; }
        case (EXP_OPER): { node->value.dval = exp (node->right->value.dval); break; }

        default: { CALC_ERROR("Не корректный тип", node); }
    }

    if (t_oper == POW_OPER || node->left)   { free (node->left); }
    free (node->right);

    node->left = NULL;
    node->right = NULL;

    return node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция расчета гиперболических операций
 @param [in] node Указатель на узел с операцией
 @return Указатель на числовой узел
*/
Node_t* calc_Hype (Node_t* node)
{
    assert (node);

    if (node->value.ival == CH_OPER && node->right->value.dval < 0)
        CALC_ERROR ("Аргумент гиперболического косинуса меньше 0", node);

    node->type = _TYPE_NUM;
    switch (node->value.ival)
    {
        case (SH_OPER):  { node->value.dval = sinh (node->right->value.dval); break; }
        case (CH_OPER):  { node->value.dval = cosh (node->right->value.dval); break; }
        case (TH_OPER):  { node->value.dval = tanh (node->right->value.dval); break; }
        case (CTH_OPER): { node->value.dval = coth (node->right->value.dval); break; }

        default: { CALC_ERROR("Не корректный тип", node); }
    }

    if (node->left != NULL)     { free (node->left); }
    free (node->right);

    node->left = NULL;
    node->right = NULL;

    return node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция вычисления факториала
 @param [in] node Указатель на узел с операцией
 @return Указатель на числовой узел
*/
Node_t* calc_Fact (Node_t* node)
{
    assert (node);

    if (fabs (node->right->value.dval - round (node->right->value.dval)) >= EPS)
        CALC_ERROR ("Аргумент факториала не целый", node);

    node->type = _TYPE_NUM;
    node->value.dval = fact (node->right->value.dval);

    if (node->left != NULL)     { free (node->left); }
    free (node->right);

    node->left = NULL;
    node->right = NULL;

    return node;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 @brief Функция взятия cot (x)
 @param [in] x Полученный аргумент
 @return cot (x)
*/
double cot (double x)
{
    return 1.0 / tan (x);
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция взятия acot (x)
 @param [in] x Полученный аргумент
 @return acot (x)
*/
double acot (double x)
{
    return M_PI/2 - atan (x);
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция взятия coth (x)
 @param [in] x Полученный аргумент
 @return coth (x)
*/
double coth (double x)
{
    return 1.0 / tanh (x);
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция нахождения факториала
 @param [in] x Полученный аргумент
 @return Факториал аргумента
*/
double fact (double x)
{
    if (x > EPS)
        x *= fact (x - 1);

    else if (x < (EPS * -1.0))
        x *= fact (x + 1);

    else
        return 1;

    return x;
}
// --------------------------------------------------------------------------------------------------

