#ifndef BIN_TREE_DSL_H
#define BIN_TREE_DSL_H

#include "BinTreeFunc.h"

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
/// @brief Суммирование левого и правого поддеревьев
#define ADD_(l, r) ( createOperNode (l, r, new_parent, ADD_OPER) )

/// @brief Вычитание левого и правого поддеревьев
#define SUB_(l, r) ( createOperNode (l, r, new_parent, SUB_OPER) )

/// @brief Умножение левого и правого поддеревьев
#define MUL_(l, r) ( createOperNode (l, r, new_parent, MUL_OPER) )

/// @brief Деление левого на правое поддеревьев
#define DIV_(l, r) ( createOperNode (l, r, new_parent, DIV_OPER) )
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/// @brief Создание числового узла
#define NUM_NODE_(value) ( createNumNode (new_parent, value) )

/// @brief Создание узла с переменной
#define VAR_NODE_(value) ( createVarNode (new_parent, value) )
// --------------------------------------------------------------------------------------------------





















#endif // BIN_TREE_DSL_H
