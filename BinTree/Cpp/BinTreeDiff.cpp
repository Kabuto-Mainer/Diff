#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../../Common/AllTypes.h"
#include "../../Common/Comands.h"

#include "../Header/BinTreeType.h"
#include "../Header/BinTreeDSL.h"
#include "../Header/BinTreeConfig.h"
#include "../Header/BinTreeConst.h"

#include "../../NameTable/NameTableFunc.h"

// --------------------------------------------------------------------------------------------------
static int AMOUNT_GRAPHICS = 0;
// --------------------------------------------------------------------------------------------------

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
    new_tree->null->parent = NULL;

    new_tree->table_cmd = old_tree->table_cmd;
    new_tree->table_var = old_tree->table_var;
    new_tree->diff_var = old_tree->diff_var;
    new_tree->size = getSizeTree (new_tree->null);

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

    LATEX (old_node, table_var);
    Node_t* node = NULL;
    if (old_node->type == _TYPE_NUM)
    {
        node = NUM_(0);
        return node;
    }

    if (old_node->type == _TYPE_VAR)
    {
        if (strcmp (table_var->data[old_node->value.ival].name, name_var) == 0)
        {
            node = _1;
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
        case (SIN_OPER):   { node = COMPND_ ( COS_ (cR) ); break; }
        case (COS_OPER):   { node = COMPND_ ( MUL_ ( SIN_ (cR), NUM_(-1) ) ); break; }
        case (TAN_OPER):   { node = COMPND_ ( DIV_ (_1, POW_( COS_ (cR), _2) ) ); break; }
        case (COT_OPER):   { node = COMPND_ ( DIV_ (NUM_(-1), POW_( SIN_ (cR), _2) ) ); break; }
        case (ASIN_OPER):  { node = COMPND_ ( DIV_ (_1, POW_( SUB_( _1, POW_(cR, _2) ), NUM_(0.5) ))); break; }
        case (ACOS_OPER):  { node = COMPND_ ( DIV_ (NUM_(-1), POW_( SUB_( _1, POW_(cR, _2) ), NUM_(0.5) ))); break; }
        case (ATAN_OPER):  { node = COMPND_ ( DIV_ (_1, ADD_ (_1, POW_(cR, _2) )) ); break; }
        case (ACOT_OPER):  { node = COMPND_ ( DIV_ (NUM_(-1), ADD_ (_1, POW_(cR, _2) )) ); break; }
        case (LOG_OPER):   { node = COMPND_ ( DIV_ (_1, MUL_ (cR, LN_ (cL)) ) ); break; }
        case (LN_OPER):    { node = COMPND_ ( DIV_ (_1, cR) ); break; }
        case (POW_OPER):   { node = COMPND_ ( MUL_ (cR, POW_(cL, (SUB_ (cR, _1)) ))); break; }
        case (EXP_OPER):   { node = COMPND_ ( EXP_ (cR) ); break; }
        case (SH_OPER):    { node = COMPND_ ( CH_ (cR) ); break; }
        case (CH_OPER):    { node = COMPND_ ( SH_ (cR) ); break; }
        case (TH_OPER):    { node = COMPND_ ( DIV_ (_1, POW_(CH_(cR), _2) )); break; }
        case (CTH_OPER):   { node = COMPND_ ( DIV_ (NUM_(-1), POW_(SH_(cR), _2) )); break; }
        case (ASH_OPER):   { node = COMPND_ ( DIV_ (_1, POW_(ADD_(POW_(cR, _2), _1), NUM_(0.5)))); break; }
        case (ACH_OPER):   { node = COMPND_ ( DIV_ (_1, POW_(SUB_(POW_(cR, _2), _1), NUM_(0.5)))); break; }
        case (ATH_OPER):   { node = COMPND_ ( DIV_ (_1, SUB_(_1, POW_(cR, _2)) )); break; }
        case (ACTH_OPER):  { node = COMPND_ ( DIV_ (_1, SUB_(_1, POW_(cR, _2)) )); break; }

        default: { }
    }

    node->left->parent = node;
    node->right->parent = node;
    LATEX (node, table_var);
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

    Node_t* left = (Node_t*) 1; // У некоторых функций есть только правый аргумент
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
    assert(size);
    // printf ("ADR: %p\n", node);
    if (node == NULL) return NULL;

    if (node->type != _TYPE_OPER)
        return node;

    if (node->left)
        node->left = abridgeNum(node->left, size);
    if (node->right)
        node->right = abridgeNum(node->right, size);

    Node_t* L = node->left;
    Node_t* R = node->right;

    if (!L && !R)
    {
        deleteNode (node);
        (*size)--;
        return NULL;
    }

    // dumpNode (node);
    int oper = node->value.ival;
    if (L &&L->type == _TYPE_NUM && fabs (L->value.dval) < EPS)
    {
        if (oper == ADD_OPER)
        {
            node->type = R->type;
            node->value.dval = R->value.dval;
            node->left = R->left;
            node->right = R->right;
            free (L);
            free (R);
            (*size) -= 2;
        }
        else if (oper == MUL_OPER || oper == DIV_OPER)
        {
            node->type = _TYPE_NUM;
            node->value.dval = 0.0;
            node->left = NULL;
            node->right = NULL;
            deleteNode (L);
            deleteNode (R);
            (*size) -= 2;
        }
    }

    else if (R && R->type == _TYPE_NUM && fabs (R->value.dval) < EPS)
    {
        if (oper == ADD_OPER || oper == SUB_OPER)
        {
            node->type = L->type;
            node->value.dval = L->value.dval;
            node->left = L->left;
            node->right = L->right;
            free (L);
            free (R);
            (*size) -= 2;
        }
        else if (node->value.ival == MUL_OPER)
        {
            node->type = _TYPE_NUM;
            node->value.dval = 0.0;
            node->left = NULL;
            node->right = NULL;
            deleteNode (L);
            deleteNode (R);
            (*size) -= 2;
        }
        else if (node->value.ival == POW_OPER)
        {
            node->type = _TYPE_NUM;
            node->value.dval = 1.0;
            node->left = NULL;
            node->right = NULL;
            deleteNode (L);
            deleteNode (R);
            (*size) -= 2;
        }
    }

    else if (L && L->type == _TYPE_NUM && fabs (L->value.dval - 1.0) < EPS)
    {
        if (oper == MUL_OPER)
        {
            node->type = R->type;
            node->value.dval = R->value.dval;
            node->left = R->left;
            node->right = R->right;
            free (L);
            free (R);
            (*size) -= 2;
        }
        if (node->value.ival == POW_OPER)
        {
            node->type = _TYPE_NUM;
            node->value.dval = 1.0;
            node->left = NULL;
            node->right = NULL;
            deleteNode (L);
            deleteNode (R);
            (*size) -= 2;
        }
    }

    else if (R && R->type == _TYPE_NUM && fabs (R->value.dval - 1.0) < EPS)
    {
        if (oper == MUL_OPER || oper == DIV_OPER || oper == POW_OPER)
        {
            node->type = L->type;
            node->value.dval = L->value.dval;
            node->left = L->left;
            node->right = L->right;
            free (L);
            free (R);
            (*size) -= 2;
        }
    }

    return node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция налаживания родительских связей таким образом, что second заменяется на first
 @param [in] first Узел, на который заменяют
 @param [in] second Узел, который заменяют
*/
int replaceNearNode (Node_t* first, Node_t* second)
{
    assert(first);
    assert(second);

    first->parent = second->parent;
    if (second->parent != NULL)
    {
        if (second->parent->left == second)     { second->parent->left = first; }
        else                                    { second->parent->right = first; }
    }
    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция оптимизации поддерева
 @param [in] node Указатель на поддерево
 @param [in] size Указатель на размер поддерева
*/
int optimizeNode (Node_t* node,
                  size_t* size)
{
    assert (node);
    assert (size);

    size_t old_size = *size;
    size_t new_size = *size;

    printf ("ADR: %p\n", node);
    // dumpNode (node);
    do
    {
        old_size = new_size;
        calculateNum (node, size);
        Node_t* buffer = abridgeNum (node, size);

        if (buffer != NULL)
            node = buffer;
    }
    while (old_size != new_size);

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

// --------------------------------------------------------------------------------------------------
/**
 @brief Функция создания ряда Тейлора до 0(x^4)
 @param [in] tree Указатель на дерево
*/
int makeTaylor (BinTree_t* tree)
{
    assert (tree);

    if (tree->diff_var == NULL)
    {
        getVarDiff (tree);
    }
    int index_var = nameTableFind (tree->table_var, tree->diff_var);
    tree->table_var->data[index_var].value = 0.0;

    Node_t* old_node = tree->null;
    Node_t* all_parent = newNode ();
    Node_t* diff_node = NULL;
    Node_t* result = newNode ();
    result->type = _TYPE_NUM;
    result->value.dval = 0.0;

    size_t size = (size_t) -1;

    cleanGraphic ();
    FILE* stream = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    if (stream == NULL)
        EXIT_FUNC ("NULL file", 1);

    FILE* gnuplot = fopen (STANDARD_GRAPHIC_ADR, "a");
    if (gnuplot == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (gnuplot, "f(x) =");
    pushGnuPlotFunc (gnuplot, tree->null, tree->table_var);
    fprintf (gnuplot, "\n");

    double power_count = 0;
    for (int i = 0; i < POWER_TAYLOR; i++)
    {
        char text[200] = "";
        if (i != 0)
        {
            diff_node = diffNode (old_node, all_parent, tree->diff_var, tree->table_var);
            diff_node->parent = NULL;
            sprintf (text, "%d производная для построения Тейлора", i);
            LATEX (diff_node, tree->table_var, text);
        }
        else
        {
            diff_node = copyNode (tree->null, all_parent);
            diff_node->parent = NULL;
            sprintf (text, "Нулевая производная для построения Тейлора");
            LATEX (diff_node, tree->table_var, text);
        }
        deleteNode (old_node);

        optimizeNode (diff_node, &size);
        old_node = copyNode (diff_node, all_parent);
        replaceNodeVar (diff_node, tree->table_var);
        optimizeNode (diff_node, &size);
        diff_node->value.dval /= fact (power_count);

        Node_t* power = newNode ();
        power->type = _TYPE_NUM;
        power->value.dval = power_count;
        power_count++;

        result = createOperNodeBin (result, createOperNodeBin (diff_node, createOperNodeBin
                 (createVarNode (result, index_var), power, all_parent, POW_OPER), all_parent, MUL_OPER), all_parent, ADD_OPER);
        result->parent = NULL;
        optimizeNode (result, &size);

        fprintf (stream, "\n\\begin{equation}\n");
        fprintf (stream, "f{%d}(x)=", i);
        dumpNodeLaTex (result, stream, tree->table_var);
        fprintf (stream, "+o(x^%d)\n\\end{equation}\n", i);
        fprintf (gnuplot, "f%d(x) =", i);
        pushGnuPlotFunc (gnuplot, result, tree->table_var);
        fprintf (gnuplot, "\n");
    }
    free (all_parent);
    deleteNode (old_node);

    result->left->parent = result;
    result->right->parent = result;

    fprintf (gnuplot, "plot \\\n");
    fprintf (gnuplot, "    f(x) lt rgb \"#000000\" title \"Сама функция\",\\\n");
    for (int i = 0; i < POWER_TAYLOR; i++)
    {
        fprintf (gnuplot, "     f%d(x) lt rgb \"%s\" title \"Тейлор до %d\"",
                 i, COLOR_TAYLOR[i], i);
        if (i != POWER_TAYLOR - 1)
        {
            fprintf (gnuplot, ",\\\n");
        }
        else
        {
            fprintf (gnuplot, "\n");
        }
    }
    fclose (stream);
    fclose (gnuplot);

    char cmd[200] = "";
    sprintf (cmd, "gnuplot %s", STANDARD_GRAPHIC_ADR);
    int trash = system (cmd);
    (void) trash;

    FILE* latex = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    if (latex == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (latex, "\\includegraphics{%d}\n", AMOUNT_GRAPHICS);
    fclose (latex);


    tree->null = result;
    tree->size = getSizeTree (result);

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Фукнция очистки файла с графиком и заполнение его заголовков
*/
int cleanGraphic ()
{
    FILE* stream = fopen (STANDARD_GRAPHIC_ADR, "w");
    if (stream == NULL)
        EXIT_FUNC("NULL file", 1);

    AMOUNT_GRAPHICS++;
    fprintf (stream, "set terminal png\n"
             "set output \'%s/%d.png\'\n"
             "set grid\n"
             "set xlabel \"x\"\n"
             "set ylabel \"y\"\n"
             "set xrange [%d:%d]\n",
             STANDARD_GRAPHIC_PNG_ADR,
             AMOUNT_GRAPHICS,
             -1 * WIDTH_GRAPH, WIDTH_GRAPH);
    fclose (stream);
    return 0;
}

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция записи поддерева в файла в формате GnuPlot
 @param [in] stream Указатель на файл
 @param [in] node Корень поддерева
 @param [in] table_var Таблица переменных
*/
int pushGnuPlotFunc (FILE* stream,
                     Node_t* node,
                     NameTable_t* table_var)
{
    assert (node);
    assert (stream);
    assert (table_var);

    if (node->type == _TYPE_NUM)
    {
        if (node->value.dval > 0)   { fprintf (stream, " %lg ", node->value.dval); }
        else                        { fprintf (stream, " (%lg) ", node->value.dval); }
        return 0;
    }
    if (node->type == _TYPE_VAR)
    {
        fprintf (stream, "%s", nameTableGetName (table_var, node->value.ival));
        return 0;
    }

    int oper = node->value.ival;
    if (GNUPLOT_COMAND[oper].type_push == ATOM_PRE_OPER)
    {
        fprintf (stream, "%s(", GNUPLOT_COMAND[oper].name);
        pushGnuPlotFunc (stream, node->right, table_var);
        fprintf (stream, ")");
    }
    else if (GNUPLOT_COMAND[oper].type_push == BIN_IN_OPER)
    {
        pushGnuPlotFunc (stream, node->left, table_var);
        fprintf (stream, "%s", GNUPLOT_COMAND[oper].name);
        pushGnuPlotFunc (stream, node->right, table_var);
    }
    else
    {
        fprintf (stream, "%s(", GNUPLOT_COMAND[oper].name);
        pushGnuPlotFunc (stream, node->left, table_var);
        fprintf (stream, ", ");
        pushGnuPlotFunc (stream, node->right, table_var);
        fprintf (stream, ")");
    }

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция построения графика по математической формуле
 @param [in] tree Указатель на дерево
*/
int makeGraphic (BinTree_t* tree)
{
    assert (tree);

    FILE* stream = fopen (STANDARD_GRAPHIC_ADR, "a");
    if (stream == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (stream, "f(x) =");
    pushGnuPlotFunc (stream, tree->null, tree->table_var);
    fprintf (stream, "\nplot f(x)\n");
    fclose (stream);

    char cmd[200] = "";
    sprintf (cmd, "gnuplot %s", STANDARD_GRAPHIC_ADR);
    int trash = system (cmd);
    (void) trash;

    FILE* latex = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    if (latex == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (latex, "\\includegraphics{%d}\n", AMOUNT_GRAPHICS);
    fclose (latex);

    return 0;
}
// -------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция записи формулы по дереву для постройки графика с проведением касательной к выбранной точке
 @param [in] tree Указатель на дерево
*/
int makeGraphicSwing (BinTree_t* tree)
{
    assert (tree);

    FILE* stream = fopen (STANDARD_GRAPHIC_ADR, "a");
    if (stream == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (stream, "f(x) =");
    pushGnuPlotFunc (stream, tree->null, tree->table_var);

    Node_t* buff = copyNode (tree->null, newNode ());
    free (buff->parent);
    buff->parent = NULL;

    double x = getCoordSwing ();
    tree->table_var->data[nameTableFind (tree->table_var, tree->diff_var)].value = x;
    replaceNodeVar (tree->null, tree->table_var);
    calculateNum (tree->null, &(tree->size));
    double y = tree->null->value.dval;

    Node_t* diff = diffNode (buff, newNode (), tree->diff_var, tree->table_var);
    free (diff->parent);
    deleteNode (buff);
    diff->parent = NULL;
    LATEX (diff, tree->table_var, "Производная для вычисления касательной");

    size_t size = (size_t) -1;
    replaceNodeVar (diff, tree->table_var);
    calculateNum (diff, &size);
    double k = diff->value.dval;
    free (diff);

    // set xrange [LEFT:RIGHT]

    // fprintf (stream, "set xrange [%d:%d]\n", round (x) - WIDTH_GRAPH, round (x) + WIDTH_GRAPH);

    double b = y - k * x;
    fprintf (stream, "\ng(x) = %lg * x + %lg\n", k, b);
    fprintf (stream, "plot f(x), g(x)\n");
    fclose (stream);

    char cmd[200] = "";
    sprintf (cmd, "gnuplot %s", STANDARD_GRAPHIC_ADR);
    int trash = system (cmd);
    (void) trash;

    FILE* latex = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    if (latex == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (latex, "\\includegraphics{%d}\n", AMOUNT_GRAPHICS);
    fclose (latex);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция получения координаты точки касания
 @return X точки касания
*/
double getCoordSwing (void)
{
    double x = 0;
    while (true)
    {
        printf ("Введите X, для которого найдется касательная\n> ");
        if (scanf ("%lf", &x) != 1)
        {
            printf ("Пожалуйста, повторите ввод\n");
            continue;
        }
        break;
    }

    return x;
}
// -------------------------------------------------------------------------------------------------------
