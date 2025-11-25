#ifndef COMANDS_H
#define COMANDS_H

#include "../BinTree/Header/BinTreeType.h"
#include "../NameTable/NameTableType.h"
#include "../BinTree/Header/BinTreeCalcFunc.h"

// ---------------------------------------------------------------------------------------------------
/**
 @brief Enum со всеми возможными операциями
 @note Значение enum является индексом в массиве кодовых слов
 @see CODE_WORDS
*/
enum ALL_OPER
{
    ADD_OPER   = 0,
    SUB_OPER   = 1,
    MUL_OPER   = 2,
    DIV_OPER   = 3,
    SIN_OPER   = 4,
    COS_OPER   = 5,
    TAN_OPER   = 6,
    COT_OPER   = 7,
    ASIN_OPER  = 8,
    ACOS_OPER  = 9,
    ATAN_OPER  = 10,
    ACOT_OPER  = 11,
    LOG_OPER   = 12,
    LN_OPER    = 13,
    POW_OPER   = 14,
    EXP_OPER   = 15,
    SH_OPER    = 16,
    CH_OPER    = 17,
    TH_OPER    = 18,
    CTH_OPER   = 19,
    ASH_OPER   = 20,
    ACH_OPER   = 21,
    ATH_OPER   = 22,
    ACTH_OPER  = 23,
    FACT_OPER  = 24
};
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/// @brief Тип функции вычисления констант
typedef Node_t* (*calc_func)(Node_t*);
// ---------------------------------------------------------------------------------------------------
/// @brief Структура операций
struct OperData_t
{
    const char* name;
    size_t hash;
    ALL_OPER type;
    calc_func func;
};
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Массив структур таблицы имен со всеми кодовыми словами
 @see ALL_OPER
*/
const OperData_t CODE_WORDS[] =
{
    { "+",     0,   ADD_OPER,   *calc_Math},
    { "-",     0,   SUB_OPER,   *calc_Math},
    { "*",     0,   MUL_OPER,   *calc_Math},
    { "/",     0,   DIV_OPER,   *calc_Math},
    { "sin",   0,   SIN_OPER,   *calc_Trig},
    { "cos",   0,   COS_OPER,   *calc_Trig},
    { "tan",   0,   TAN_OPER,   *calc_Trig},
    { "cot",   0,   COT_OPER,   *calc_Trig},
    { "asin",  0,   ASIN_OPER,  *calc_ATrig},
    { "acos",  0,   ACOS_OPER,  *calc_ATrig},
    { "atan",  0,   ATAN_OPER,  *calc_ATrig},
    { "acot",  0,   ACOT_OPER,  *calc_ATrig},
    { "log",   0,   LOG_OPER,   *calc_Degree},
    { "ln",    0,   LN_OPER,    *calc_Degree},
    { "pow",   0,   POW_OPER,   *calc_Degree},
    { "exp",   0,   EXP_OPER,   *calc_Degree},
    { "sh",    0,   SH_OPER,    *calc_Hype},
    { "ch",    0,   CH_OPER,    *calc_Hype},
    { "th",    0,   TH_OPER,    *calc_Hype},
    { "cth",   0,   CTH_OPER,   *calc_Hype},
    { "ash",   0,   ASH_OPER,   NULL},
    { "ach",   0,   ACH_OPER,   NULL},
    { "ath",   0,   ATH_OPER,   NULL},
    { "acth",  0,   ACTH_OPER,  NULL},
    { "fact",  0,   FACT_OPER,  *calc_Fact}
};
// ---------------------------------------------------------------------------------------------------
/// @brief Команда, начиная с которой начинается цикл поиска команды
const int START_COMAND = SIN_OPER; // Первая после математических операций
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/// @brief
const char LATEX_COMAND[][40] =
{
    "+",
    "-",
    "\\cdot",
    "\\frac",
    "\\sin",
    "\\cos",
    "\\tan",
    "\\cot",
    "\\arcsin",
    "\\arccos",
    "\\arctan",
    "\\arccot",
    "\\log",
    "\\ln",
    "^",
    "\\exp",
    "\\sinh",
    "\\cosh",
    "\\tanh",
    "\\coth",
    "\\operatorname{arsinh}",
    "\\operatorname{arcosh}",
    "\\operatorname{artanh}",
    "\\operatorname{arcoth}",
    "!"
};
// ---------------------------------------------------------------------------------------------------



#endif // COMANDS_H
