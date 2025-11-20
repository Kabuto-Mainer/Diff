#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../../Common/AllTypes.h"
#include "../../Common/Comands.h"
#include "../Header/BinTreeType.h"
#include "../Header/BinTreeDSL.h"
#include "../../NameTable/NameTableFunc.h"

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция дифференцирования дерева
 @param [in] old_tree Указатель на структуру старого дерева
 @param [in] new_tree Указатель на структуру нового дерева
 @note Удаляет старое
*/
int binTreeDiff (BinTree_t* old_tree,
                 BinTree_t* new_tree)
{
    assert (old_tree);
    assert (new_tree);

    if (old_tree->size == 0)
    {
        printf ("Дерево пустое\n");
        return 0;
    }

    // binTreeCtr (new_tree);
    new_tree->null = newNode ();
    if (old_tree->diff_var == NULL) { getVarDiff (old_tree); }

    Node_t* new_null = diffNode (old_tree->null, new_tree->null, old_tree->diff_var, old_tree->table_var);
    free (new_tree->null);
    new_tree->null = new_null;
    new_tree->table_cmd = old_tree->table_cmd;
    new_tree->table_var = old_tree->table_var;
    new_tree->diff_var = old_tree->diff_var;

    deleteNode (old_tree->null);
    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция дифференцирования поддерева
 @param [in] old_node Указатель на узел старого поддерева
 @param [in] new_parent Указатель на родитель узла нового поддерева
 @param [in] name_var Переменная дифференцирования
 @param [in] table_var Таблица переменных
 @return Указатель на корень поддерева
*/
Node_t* diffNode (Node_t* old_node,
                  Node_t* new_parent,
                  const char* name_var,
                  NameTable_t* table_var)
{
    assert (old_node);
    assert (table_var);
    assert (name_var);
    assert (new_parent);

    if (old_node->type == _TYPE_NUM)
        return NUM_NODE_(0);

    if (old_node->type == _TYPE_VAR)
    {
        if (strcmp (table_var->data[old_node->value.ival].name, name_var) == 0)
            return NUM_NODE_(1);

        return VAR_NODE_(old_node->value.ival);
    }

    switch (old_node->value.ival)
    {
        case (ADD_OPER): { return ADD_( dL, dR ); }
        case (SUB_OPER): { return SUB_( dL, dR ); }
        case (MUL_OPER): { return ADD_( MUL_( dL, cR ), MUL_( cL, dR ) ); }
        case (DIV_OPER): { return DIV_( SUB_ ( MUL_ ( dL, cR ), MUL_ ( cL, dR ) ), MUL_ ( cR, cR )); }

        default: { return NULL; }
    }

    return NULL;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция копирования поддерева в новое поддерево
 @param [in] old_node Указатель на корень старого поддерева
 @param [in] new_parent Указатель на корень нового поддерева
 @return Указатель на созданное поддерево
*/
Node_t* copyNode (Node_t* old_node,
                  Node_t* new_parent)
{
    assert (old_node);
    assert (new_parent);

    Node_t* new_node = newNode ();

    new_node->parent = new_parent;
    new_node->type = old_node->type;
    new_node->value = old_node->value;

    if (old_node->left)     { new_node->left = copyNode (old_node->left, new_node); }
    else                    { new_node->left = NULL; }

    if (old_node->right)    { new_node->right = copyNode (old_node->right, new_node); }
    else                    { new_node->right = NULL; }

    return new_node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция получения переменной дифференцирования
 @param [in] tree Указатель на структуру дерева
*/
int getVarDiff (BinTree_t* tree)
{
    assert (tree);

    char buffer[20] = "";

    while (true)
    {
        printf ("Введите переменную, по которой будет происходить дифференцирование\n");
        if (scanf ("%s", buffer) != 1)
        {
            printf ("Ошибка ввода\n");
            continue;
        }

        if (nameTableFind (tree->table_var, buffer) != 0)
        {
            printf ("Переменная не найдена\n");
            continue;
        }

        tree->diff_var = strdup (buffer);
        break;
    }
    return 0;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 @brief Функция добавления узла с операцией
 @param [in] left Левое поддерево нового узла
 @param [in] right Правое поддерево нового узла
 @param [in] parent Родитель нового узла
 @param [in] oper Тип добавляемой операции
 @return Указатель на новой узел
*/
Node_t* createOperNode (Node_t* left,
                        Node_t* right,
                        Node_t* parent,
                        ALL_OPER oper)
{
    assert (left);
    assert (right);
    assert (parent);

    Node_t* new_node = newNode ();
    new_node->parent = parent;
    new_node->left = left;
    new_node->right = right;
    new_node->type = _TYPE_OPER;
    new_node->value.ival = oper;

    return new_node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция создания узла с переменной
 @param [in] parent Указатель на родительский узел
 @param [in] index Индекс переменной в таблице имен
 @return Указатель на новый узел
*/
Node_t* createVarNode (Node_t* parent,
                       int index)
{
    assert (parent);

    Node_t* new_node = newNode ();
    new_node->parent = parent;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->type = _TYPE_VAR;
    new_node->value.ival = index;

    return new_node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция создания узла с числом
 @param [in] parent Указатель на родительский узел
 @param [in] value Значение числа
 @return Указатель на новый узел
*/
Node_t* createNumNode (Node_t* parent,
                       double value)
{
    assert (parent);

    Node_t* new_node = newNode ();
    new_node->parent = parent;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->type = _TYPE_NUM;
    new_node->value.dval = value;

    return new_node;
}
// --------------------------------------------------------------------------------------------------


