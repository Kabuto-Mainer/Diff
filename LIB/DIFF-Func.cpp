/*
    Это библиотека предназначена для работы с функциями.
    Основная задача - взятие производных от функции
*/
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#include "../BinTree/Header/BinTreeFunc.h"
// #include "../BinTree/Header/BinTreeCalcFunc.h"
#include "../BinTree/Header/BinTreeConfig.h"
// #include "../NameTable/NameTableFunc.h"
// #include "../NameTable/NameTableType.h"

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
    srand( unsigned (time (NULL)));
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

    DIFF_Inf ("Start bring diff");
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
    DIFF_Inf ("End bring diff");

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

    DIFF_Inf ("Start delete tree");
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

    DIFF_Inf ("Start optimization tree");
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

    DIFF_Inf ("Start replacing variables");
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
/**
 @brief Функция создания ряда Тейлора объекта
 @param [in] tree Объект
*/
int DIFF_MakeTaylor (DIFF_Tree_t tree)
{
    assert (tree);

    DIFF_Inf ("Start making Taylor row");
    binTreeDumpHTML (tree, "Before making Taylor row");
    TREE_LATEX (tree, "Перед созданием ряда Тейлора");

    makeTaylor (tree);

    DIFF_Inf ("End making Taylor row");
    binTreeDumpHTML (tree, "After making Taylor row");
    TREE_LATEX (tree, "Сам ряд Тейлора");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция проведения касательной к графику функции
 @param [in] tree Объект
*/
int DIFF_Swing (DIFF_Tree_t tree)
{
    assert (tree);

    cleanGraphic ();
    DIFF_Inf ("Start making swing");
    makeGraphicSwing (tree);
    DIFF_Inf ("End making swing");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция создания графика
 @param [in] tree Объект
*/
int DIFF_Graph (DIFF_Tree_t tree)
{
    assert (tree);

    cleanGraphic ();
    DIFF_Inf ("Start making common graphic");
    makeGraphic (tree);
    DIFF_Inf ("End making praphic");

    return 0;
}
// ---------------------------------------------------------------------------------------------------

