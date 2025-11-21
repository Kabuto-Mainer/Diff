#ifndef COMANDS_H
#define COMANDS_H

#include "../BinTree/Header/BinTreeType.h"
#include "../NameTable/NameTableType.h"

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
    ACTH_OPER  = 23
};
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/// @brief Структура операций
struct OperData_t
{
    const char* name;
    size_t hash;
    ALL_OPER type;
};
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Массив структур таблицы имен со всеми кодовыми словами
 @see ALL_OPER
*/
const OperData_t CODE_WORDS[] =
{
    { "+",     0,   ADD_OPER},
    { "-",     0,   SUB_OPER},
    { "*",     0,   MUL_OPER},
    { "/",     0,   DIV_OPER},
    { "sin",   0,   SIN_OPER},
    { "cos",   0,   COS_OPER},
    { "tan",   0,   TAN_OPER},
    { "cot",   0,   COT_OPER},
    { "asin",  0,   ASIN_OPER},
    { "acos",  0,   ACOS_OPER},
    { "atan",  0,   ATAN_OPER},
    { "acot",  0,   ACOT_OPER},
    { "log",   0,   LOG_OPER},
    { "ln",    0,   LN_OPER},
    { "pow",   0,   POW_OPER},
    { "exp",   0,   EXP_OPER},
    { "sh",    0,   SH_OPER},
    { "ch",    0,   CH_OPER},
    { "th",    0,   TH_OPER},
    { "cth",   0,   CTH_OPER},
    { "ash",   0,   ASH_OPER},
    { "ach",   0,   ACH_OPER},
    { "ath",   0,   ATH_OPER},
    { "acth",  0,   ACTH_OPER}
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
    "\\operatorname{arcoth}"
};
// ---------------------------------------------------------------------------------------------------



#endif // COMANDS_H
