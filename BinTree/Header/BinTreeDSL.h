#ifndef BIN_TREE_DSL_H
#define BIN_TREE_DSL_H

#include "BinTreeFunc.h"
#include <math.h>



// --------------------------------------------------------------------------------------------------
// Эта DSL библиотека для взятия производных
// Основной принцип: Каждая команда возвращает указатель на новый узел
// Вычисление для математический операций будет происходить слева направо
// (- (6) (x)) = 6 - x
// Если у функции только один аргумент, то это R (L = NULL)
// У 'log' и 'pow' L = основание, R = аргумент (или степень)
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/// @brief Взятие производной правого поддерева
#define dR \
    ( diffNode (old_node->right, new_parent, name_var, table_var) )

/// @brief Взятие производной левого поддерева
#define dL \
    ( diffNode (old_node->left, new_parent, name_var, table_var) )
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/// @brief Копирование правого поддерева
#define cR \
    ( copyNode (old_node->right, new_parent) )

/// @brief Копирование правого поддерева
#define cL \
    ( copyNode (old_node->left, new_parent) )
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/// @brief Взятие сложной производной от f
#define COMPLEX_(f) ( createComplex (f, old_node, new_parent, table_var, name_var) )
// --------------------------------------------------------------------------------------------------

// Математические операции
// --------------------------------------------------------------------------------------------------
/// @brief Суммирование левого и правого поддеревьев
#define ADD_(l, r) ( createOperNodeBin (l, r, new_parent, ADD_OPER) )

/// @brief Вычитание левого и правого поддеревьев
#define SUB_(l, r) ( createOperNodeBin (l, r, new_parent, SUB_OPER) )

/// @brief Умножение левого и правого поддеревьев
#define MUL_(l, r) ( createOperNodeBin (l, r, new_parent, MUL_OPER) )

/// @brief Деление левого на правое поддеревьев
#define DIV_(l, r) ( createOperNodeBin (l, r, new_parent, DIV_OPER) )
// --------------------------------------------------------------------------------------------------

// Тригонометрия
// --------------------------------------------------------------------------------------------------
/// @brief Синус поддерева
#define SIN_(r) ( createOperNodeAtom (r, new_parent, SIN_OPER) )

/// @brief Косинус поддерева
#define COS_(r) ( createOperNodeAtom (r, new_parent, COS_OPER) )

/// @brief Тангенс поддерева
#define TAN_(r) ( createOperNodeAtom (r, new_parent, TAN_OPER) )

/// @brief Котангенс поддерева
#define COT_(r) ( createOperNodeAtom (r, new_parent, COT_OPER) )
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/// @brief Арксинус поддерева
#define ASIN_(r) ( createOperNodeAtom (r, new_parent, ASIN_OPER) )

/// @brief Арккосинус поддерева
#define ACOS_(r) ( createOperNodeAtom (r, new_parent, ACOS_OPER) )

/// @brief Арктангенс поддерева
#define ATAN_(r) ( createOperNodeAtom (r, new_parent, ATAN_OPER) )

/// @brief Арккотангенс поддерева
#define ACOT_(r) ( createOperNodeAtom (r, new_parent, ACOT_OPER) )
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/// @brief Возведение в степень
#define POW_(l, r) ( createOperNodeBin (l, r, new_parent, POW_OPER) )

/// @brief Возведение экспоненты
#define EXP_(r) ( createOperNodeAtom (r, new_parent, EXP_OPER) )

/// @brief Логорифм с переменным основанием
#define LOG_(l, r) ( createOperNodeBin (l, r, new_parent, LOG_OPER) )

/// @brief Натуральный логорифм
#define LN_(r) ( createOperNodeBin (NUM_NODE_(M_E), r, new_parent, LN_OPER) )
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/// @brief Гиперболический синус
#define SH_(r) ( createOperNodeAtom (r, new_parent, SH_OPER) )

/// @brief Гиперболический косинус
#define CH_(r) ( createOperNodeAtom (r, new_parent, CH_OPER) )

// --------------------------------------------------------------------------------------------------
/// @brief Создание числового узла
#define NUM_NODE_(value) ( createNumNode (new_parent, value) )

/// @brief Создание узла с переменной
#define VAR_NODE_(value) ( createVarNode (new_parent, nameTableFind (table_var, name_var)) )
// --------------------------------------------------------------------------------------------------





















#endif // BIN_TREE_DSL_H
