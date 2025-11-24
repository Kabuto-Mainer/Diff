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

#include "config.h"
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

// int DIFF_Calc (DIFF_Tree_t tree)
// {

// }

// int DIFF_MakeTeylor ()
// {

// }
