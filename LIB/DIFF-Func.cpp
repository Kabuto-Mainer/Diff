/*
    Это библиотека предназначена для работы с функциями.
    Основная задача - взятие производных от функции
*/
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#include "../BinTree/Header/BinTreeFunc.h"
#include "../BinTree/Header/BinTreeCalcFunc.h"
#include "../BinTree/Header/BinTreeConfig.h"
#include "../NameTable/NameTableFunc.h"
#include "../NameTable/NameTableType.h"

#include "DIFF_Func.h"
#include "../Common/Common.h"

// ---------------------------------------------------------------------------------------------------
/**
 @brief Очистка всех системных файлов этой библиотеки
*/
int DIFF_Init ()
{
    cleanDump ();
    cleanLogFile ();
    DIFF_Inf ("Initialization of DIFF_Lib");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция создания объекта для дальнейшей работы с ним
 @return Объект (binTree_t*)
*/
DIFF_Tree_t DIFF_TreeCtr ()
{
    DIFF_Inf ("Creating DIFF_Tree_t");
    BinTree_t* tree = (BinTree_t*) calloc (1, sizeof (BinTree_t));
    if (tree == NULL)
    {
        DIFF_Error ("NULL calloc");
        return NULL;
    }

    DIFF_Inf ("Successfully creating");
    binTreeCtr (tree);
    binTreeDumpHTML (tree, "After creating tree");
    return tree;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция загрузки математической функции в объект
 @param [in] tree Объект, куда будет загружена информация
 @param [in] file Имя файла, откуда будет взята информация
*/
int DIFF_GetTree (DIFF_Tree_t tree,
                  const char* file)
{
    assert (tree);
    assert (file);

    DIFF_Inf ("Start uploading tree");
    binTreeUpload (tree, file);
    DIFF_Inf ("Uploading tree successful");
    binTreeDumpHTML (tree, "After uploading tree");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция взятия производной математической функции, загруженной в объект
 @param [in] tree Объект
*/
int DIFF_BringDiff (DIFF_Tree_t* tree)
{
    assert (tree);
    assert (*tree);

    DIFF_Inf ("Starting bring diff");
    binTreeDumpHTML (*tree, "Before geting diff");
    TREE_LATEX (*tree, "Перед взятием производной");
    BinTree_t* new_tree = (BinTree_t*) calloc (1, sizeof (BinTree_t));
    if (new_tree == NULL)
    {
        DIFF_Error ("NULL calloc");
        return 1;
    }

    binTreeDiff (*tree, new_tree);
    free (*tree);
    *tree = new_tree;
    binTreeSave (new_tree);
    binTreeDumpHTML (new_tree, "After geting diff");
    TREE_LATEX (*tree, "После взятием производной");
    DIFF_Inf ("Ending bring diff");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция удаления объекта
 @param [in] tree Объект
*/
int DIFF_TreeDtr (DIFF_Tree_t tree)
{
    assert (tree);

    DIFF_Inf ("Starting delete tree");
    binTreeDtr (tree);
    free (tree);

    DIFF_Inf ("End delete tree");
    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция оптимизации объекта
 @param [in] tree Объект
*/
int DIFF_Optimization (DIFF_Tree_t tree)
{
    assert (tree);

    DIFF_Inf ("Starting optimization tree");
    binTreeDumpHTML (tree, "Before optimization tree");
    TREE_LATEX (tree, "Перед оптимизацией");

    size_t old_size = tree->size;
    size_t new_size = tree->size;

    do
    {
        old_size = new_size;
        calculateNum (tree->null, &new_size);
        abridgeNum (tree->null, &new_size);
    }
    while (new_size != old_size);
    tree->size = new_size;

    DIFF_Inf ("End optimization tree");
    binTreeDumpHTML (tree, "After optimization tree");
    TREE_LATEX (tree, "После оптимизацией");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция завершения работы библиотеки
*/
int DIFF_Quit ()
{
    FILE* stream = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    if (stream == NULL)
        EXIT_FUNC ("NULL file", 1);

    fprintf (stream, "\n\\end{document}");
    fclose (stream);

    char cmd[200] = "";
    sprintf (cmd, "pdflatex %s", STANDARD_DUMP_LATEX_ADR);
    int trash = system (cmd);
    (void) trash;

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция вычисления объекта
 @param [in] tree Объект
*/
int DIFF_Calc (DIFF_Tree_t tree)
{
    assert (tree);

    DIFF_Inf ("Starting replacing variables");
    binTreeDumpHTML (tree, "Before replace all variables");
    TREE_LATEX (tree, "Перед заменой переменных");

    replaceTreeVar (tree);

    DIFF_Inf ("End replacing variables and start calculate tree");
    binTreeDumpHTML (tree, "After replace all variables");
    TREE_LATEX (tree, "После заменой переменных");

    calculateNum (tree->null, &tree->size);

    DIFF_Inf ("End calculate tree");
    binTreeDumpHTML (tree, "After calculate tree");
    TREE_LATEX (tree, "После вычисления дерева");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
int DIFF_MakeTeylor (DIFF_Tree_t tree)
{
    assert (tree);

    int index_var = nameTableFind (tree->table_var, tree->diff_var);
    tree->table_var->data[index_var].value = 0.0;

    Node_t* old_node = tree->null;
    Node_t* all_parent = newNode ();
    Node_t* diff_node = NULL;
    Node_t* result = newNode ();
    result->type = _TYPE_NUM;
    result->value.dval = 0.0;

    FILE* stream = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    if (stream == NULL)
        EXIT_FUNC ("NULL file", 1);

    double power_count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (i != 0)
        {
            diff_node = diffNode (old_node, all_parent, tree->diff_var, tree->table_var);
            diff_node->parent = NULL;
        }
        else
        {
            diff_node = copyNode (tree->null, all_parent);
            diff_node->parent = NULL;
        }
        deleteNode (old_node);

        old_node = copyNode (diff_node, all_parent);
        replaceNodeVar (diff_node, tree->table_var);

        size_t old_size = (size_t) -1;
        size_t new_size = (size_t) -1; // Что бы не было проблем с размерами
        calculateNum (diff_node, &new_size);
        diff_node->value.dval /= fact (power_count);

        Node_t* power = newNode ();
        power->type = _TYPE_NUM;
        power->value.dval = power_count;
        power_count++;

        result = createOperNodeBin (result, createOperNodeBin (diff_node, createOperNodeBin
                 (createVarNode (result, index_var), power, all_parent, POW_OPER), all_parent, MUL_OPER), all_parent, ADD_OPER);
        result->parent = NULL;

        do
        {
            old_size = new_size;
            calculateNum (result, &new_size);
            Node_t* buffer = abridgeNum (result, &new_size);
            if (buffer != NULL) { result = buffer; }
        } while (old_size != new_size);
    }
    free (all_parent);
    deleteNode (old_node);

    BinTree_t new_tree = {};
    new_tree.null = result;
    new_tree.table_var = tree->table_var;
    new_tree.table_cmd = tree->table_cmd;
    new_tree.diff_var = tree->diff_var;
    new_tree.size = 4;

    size_t old_size = (size_t) -1;
    size_t new_size = (size_t) -1;

    binTreeDumpHTML (&new_tree, "Test");
    do
    {
        old_size = new_size;
        calculateNum (result, &new_size);
        result = abridgeNum (result, &new_size);
    }
    while (new_size != old_size);

    // FILE* stream = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    // if (stream == NULL)
    //     EXIT_FUNC ("NULL file", 1);

    fprintf (stream, "\n\\begin{equation}\n");
    fprintf (stream, "f(x)=");
    dumpNodeLaTex (result, stream, tree->table_var);
    fprintf (stream, "\n\\end{equation}\n");
    fclose (stream);

    tree->null = result;
    tree->size = getSizeTree (result);

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// Node_t* createMulTeylor (const int number)
// {


// }
