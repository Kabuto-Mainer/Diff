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
    // printf ("OLD: %p\nNEW: %p\n", new_tree->null, new_null);
    free (new_tree->null);
    new_tree->null = new_null;
    new_tree->null->parent = NULL;
    // (void) new_null;
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
                  char* name_var,
                  NameTable_t* table_var)
{
    assert (old_node);
    assert (table_var);
    assert (name_var);
    assert (new_parent);

    LATEX (old_node, table_var, "До взятия производной\n");
    Node_t* node = NULL;
    if (old_node->type == _TYPE_NUM)
    {
        node = NUM_NODE_(0);
        return node;
    }

    if (old_node->type == _TYPE_VAR)
    {
        if (strcmp (table_var->data[old_node->value.ival].name, name_var) == 0)
        {
            node = NUM_NODE_(1);
            return node;
        }

        node = VAR_NODE_(old_node->value.ival);
        return node;
    }

    switch (old_node->value.ival)
    {
        case (ADD_OPER):   { node = ADD_(dL, dR); break; }
        case (SUB_OPER):   { node = SUB_(dL, dR); break; }
        case (MUL_OPER):   { node = ADD_( MUL_(dL, cR), MUL_(cL, dR) ); break; }
        case (DIV_OPER):   { node = DIV_( SUB_( MUL_(dL, cR), MUL_(cL, dR) ), MUL_(cR, cR)); break; }
        case (SIN_OPER):   { node = COMPLEX_ ( COS_ (cR) ); break; }
        case (COS_OPER):   { node = COMPLEX_ ( MUL_ ( SIN_ (cR), NUM_NODE_(-1) ) ); break; }
        case (TAN_OPER):   { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), POW_( COS_ (cR), NUM_NODE_(2)) ) ); break; }
        case (COT_OPER):   { node = COMPLEX_ ( DIV_ (NUM_NODE_(-1), POW_( SIN_ (cR), NUM_NODE_(2)) ) ); break; }
        case (ASIN_OPER):  { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), POW_( SUB_( NUM_NODE_(1), POW_(cR, NUM_NODE_(2)) ), NUM_NODE_(0.5) ))); break; }
        case (ACOS_OPER):  { node = COMPLEX_ ( DIV_ (NUM_NODE_(-1), POW_( SUB_( NUM_NODE_(1), POW_(cR, NUM_NODE_(2)) ), NUM_NODE_(0.5) ))); break; }
        case (ATAN_OPER):  { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), ADD_ (NUM_NODE_(1), POW_(cR, NUM_NODE_(2)) )) ); break; }
        case (ACOT_OPER):  { node = COMPLEX_ ( DIV_ (NUM_NODE_(-1), ADD_ (NUM_NODE_(1), POW_(cR, NUM_NODE_(2)) )) ); break; }
        case (LOG_OPER):   { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), MUL_ (cR, LN_ (cL)) ) ); break; }
        case (LN_OPER):    { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), cR) ); break; }
        case (POW_OPER):   { node = COMPLEX_ ( MUL_ (cR, POW_(cL, (SUB_ (cR, NUM_NODE_(1))) ))); break; }
        case (EXP_OPER):   { node = COMPLEX_ ( EXP_ (cR) ); break; }
        case (SH_OPER):    { node = COMPLEX_ ( CH_ (cR) ); break; }
        case (CH_OPER):    { node = COMPLEX_ ( SH_ (cR) ); break; }
        case (TH_OPER):    { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), POW_(CH_(cR), NUM_NODE_(2)) )); break; }
        case (CTH_OPER):   { node = COMPLEX_ ( DIV_ (NUM_NODE_(-1), POW_(SH_(cR), NUM_NODE_(2)) )); break; }
        case (ASH_OPER):   { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), POW_(ADD_(POW_(cR, NUM_NODE_(2)), NUM_NODE_(1)), NUM_NODE_(0.5)))); break; }
        case (ACH_OPER):   { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), POW_(SUB_(POW_(cR, NUM_NODE_(2)), NUM_NODE_(1)), NUM_NODE_(0.5)))); break; }
        case (ATH_OPER):   { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), SUB_(NUM_NODE_(1), POW_(cR, NUM_NODE_(2))) )); break; }
        case (ACTH_OPER):  { node = COMPLEX_ ( DIV_ (NUM_NODE_(1), SUB_(NUM_NODE_(1), POW_(cR, NUM_NODE_(2))) )); break; }

        default: { }
    }

    node->left->parent = node;
    node->right->parent = node;
    LATEX (node, table_var, "После взятия производной\n");
    return node;
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
Node_t* createOperNodeBin (Node_t* left,
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
    left->parent = new_node;
    right->parent = new_node;
    new_node->type = _TYPE_OPER;
    new_node->value.ival = oper;

    return new_node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция добавления узла с операцией
 @param [in] right Правое поддерево нового узла
 @param [in] parent Родитель нового узла
 @param [in] oper Тип добавляемой операции
 @return Указатель на новой узел
 @note left = NULL
*/
Node_t* createOperNodeAtom (Node_t* right,
                            Node_t* parent,
                            ALL_OPER oper)
{
    assert (right);
    assert (parent);

    Node_t* new_node = newNode ();
    new_node->parent = parent;
    new_node->left = NULL;
    new_node->right = right;
    right->parent = new_node;
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

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция взятия производной сложной функции
 @param [in] new_oper Производная операции
 @param [in] old_oper Первоначальная операция
 @param [in] parent Указатель на родительский узел
 @param [in] table_var Таблица имен
 @param [in] name_var Имя переменной, по которой берется производная
 @return Указатель на корень нового поддерева
*/
Node_t* createComplex (Node_t* new_oper,
                       Node_t* old_oper,
                       Node_t* parent,
                       NameTable_t* table_var,
                       char* name_var)
{
    assert (new_oper);
    assert (old_oper);
    assert (parent);
    assert (table_var);
    assert (name_var);

    if (old_oper->left != NULL && old_oper->type != _TYPE_NUM && findVar (old_oper->left, table_var, name_var) != 0)
    {
        if (old_oper->right != NULL && old_oper->type != _TYPE_NUM && findVar (old_oper->right, table_var, name_var) != 0)
        {
            Node_t* node = newNode ();
            node->right = new_oper;
            node->parent = parent;
            node->type = _TYPE_OPER;
            node->value.ival = MUL_OPER;

            node->right = new_oper;
            new_oper->parent = node;

            node->left = newNode ();
            node->left->parent = node;
            node->left->type = _TYPE_OPER;
            node->left->value.ival = MUL_OPER;
            node->left->left = diffNode (old_oper->left, node->left, name_var, table_var);
            node->left->right = diffNode (old_oper->right, node->left, name_var, table_var);
            return node;
        }
        else
        {
            Node_t* node = newNode ();
            node->type = _TYPE_OPER;
            node->value.ival = MUL_OPER;
            node->parent = parent;
            node->left = diffNode (old_oper->left, node, name_var, table_var);
            node->right = new_oper;
            new_oper->parent = node;
            return node;
        }
    }
    else
    {
        if (old_oper->right != NULL && old_oper->type != _TYPE_NUM && findVar (old_oper->right, table_var, name_var) != 0)
        {
            Node_t* node = newNode ();
            node->type = _TYPE_OPER;
            node->value.ival = MUL_OPER;
            node->parent = parent;
            node->left = diffNode (old_oper->right, node, name_var, table_var);
            node->right = new_oper;
            new_oper->parent = node;
            return node;
        }
        else
        {
            return new_oper;
        }
    }

    return NULL;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция сокращения констант
 @param [in] node Указатель на корень поддерева
 @param [in] size Указатель на размер дерева
 @return node, если это константа, иначе NULL
*/
Node_t* calculateNum (Node_t* node,
                      size_t* size)
{
    assert (node);
    assert (size);

    if (node->type == _TYPE_VAR)
    {
        return NULL;
    }

    if (node->type == _TYPE_NUM)
        return node;

    Node_t* left = NULL;
    Node_t* right = NULL;

    if (node->left)
        left = calculateNum (node->left, size);

    if (node->right)
        right = calculateNum (node->right, size);

    if (right && left)
    {
        *size -= 2; // Сокращение именно на 2 блока
        return CODE_WORDS[node->value.ival].func (node);
    }

    return NULL;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция сокращения ненужных операций
 @param [in] node Указатель на корень поддерева
 @param [in] size Указатель на размер дерева
 @return Указатель на оптимизированное поддерево
*/
Node_t* abridgeNum (Node_t* node,
                    size_t* size)
{
    assert (node);
    assert (size);

    if (node->type != _TYPE_OPER || node->left == NULL)
        return 0;

    Node_t* ret_node = node;
    if (node->left->type == _TYPE_OPER)
        abridgeNum (node->left, size);

    if (node->right->type == _TYPE_OPER)
        abridgeNum (node->right, size);

    if (node->left->type == _TYPE_NUM && fabs (node->left->value.dval) < EPS)
    {
        // printf ("L-0");
        // printf ("   NODE: %p\n", node);

        if (node->value.ival == ADD_OPER)
        {
            ret_node = node->right;
            replaceNearNode (node->right, node);
            free (node->left);
            free (node);
            *size -= 2;
        }
        else if (node->value.ival == MUL_OPER || node->value.ival == DIV_OPER)
        {
            ret_node = node->left;
            replaceNearNode (node->left, node);
            deleteNode (node->right);
            free (node);
            *size -= 2;
        }
    }
    else if (node->right->type == _TYPE_NUM && fabs (node->right->value.dval) < EPS)
    {
        // printf ("R-0");
        // printf ("   NODE: %p\n", node);

        if (node->value.ival == ADD_OPER || node->value.ival == SUB_OPER)
        {
            ret_node = node->left;
            replaceNearNode (node->left, node);
            free (node->right);
            free (node);
            *size -= 2;
        }
        else if (node->value.ival == MUL_OPER)
        {
            ret_node = node->right;
            replaceNearNode (node->right, node);
            deleteNode (node->left);
            free (node);
            *size -= 2;
        }
        else if (node->value.ival == POW_OPER)
        {
            ret_node = node->right;
            replaceNearNode (node->right, node);
            node->right->value.dval = 1.0;
            deleteNode (node->left);
            free (node);
            *size -= 2;
        }
    }
    else if (node->left->type == _TYPE_NUM && fabs (node->left->value.dval - 1.0) < EPS)
    {
        // printf ("L-1");
        // printf ("   NODE: %p\n", node);
        if (node->value.ival == MUL_OPER)
        {
            ret_node = node->right;
            replaceNearNode (node->right, node);
            free (node->left);
            free (node);
            *size -= 2;
        }
        else if (node->value.ival == POW_OPER)
        {
            ret_node = node->left;
            replaceNearNode (node->left, node);
            free (node->right);
            free (node);
            *size -= 2;
        }
    }
    else if (node->right->type == _TYPE_NUM && fabs (node->right->value.dval - 1.0) < EPS)
    {
        // printf ("R-1");
        // printf ("   NODE: %p\n", node);

        if (node->value.ival == MUL_OPER || node->value.ival == DIV_OPER)
        {
            ret_node = node->right;
            replaceNearNode (node->right, node);
            free (node->left);
            free (node);
            *size -= 2;
        }
        else if (node->value.ival == POW_OPER)
        {
            ret_node = node->left;
            replaceNearNode (node->left, node);
            free (node->right);
            free (node);
            *size -= 2;
        }
    }
    return ret_node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция налаживания родительских связей таким образом, что second заменяется на first
 @param [in] first Узел, на который заменяют
 @param [in] second Узел, который заменяют
*/
int replaceNearNode (Node_t* first,
                     Node_t* second)
{
    assert (first);
    assert (second);

    // printf ("FIRST BEFORE\n");
    // dumpNode (first);
    // printf ("SECOND BEFORE\n");
    // dumpNode (second);
    first->parent = second->parent;
    if (second->parent != NULL)
    {
        if (second->parent->left == second)     { second->parent->left = first; }
        else                                    { second->parent->right = first; }
    }
    // printf ("PARENT\n");
    // dumpNode (first->parent);
    // printf ("FIRST AFTER\n");
    // dumpNode (first);
    // printf ("SECOND AFTER\n");
    // dumpNode (second);
    return 0;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 @brief Функция замены переменных на их значения
 @param [in] tree Указатель на бинарное дерево
*/
int replaceTreeVar (BinTree_t* tree)
{
    assert (tree);

    getValueVar (tree->table_var);
    replaceNodeVar (tree->null, tree->table_var);

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция замены переменных в узлах поддерева
 @param [in] node Указатель на корень поддерева
 @param [in] table_var Таблица имен переменных
*/
int replaceNodeVar (Node_t* node,
                    NameTable_t* table_var)
{
    assert (node);
    assert (table_var);

    if (node->left)     { replaceNodeVar (node->left, table_var); }
    if (node->right)    { replaceNodeVar (node->right, table_var); }

    if (node->type == _TYPE_VAR)
    {
        node->value.dval = table_var->data[node->value.ival].value;
        node->type = _TYPE_NUM;
    }

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция получения значений переменных от пользователя
 @param [in] table_var Таблица имен переменных
*/
int getValueVar (NameTable_t* table_var)
{
    assert (table_var);

    for (size_t i = 0; i < table_var->size; i++)
    {
        printf ("\nЗначение %s:", table_var->data[i].name);

        double value = 0;
        while (true)
        {
            if (scanf ("%lf", &value) != 1)
            {
                printf ("\nПовторите ввод");
                continue;
            }
        }
        table_var->data[i].value = value;
    }
    return 0;
}
// --------------------------------------------------------------------------------------------------



