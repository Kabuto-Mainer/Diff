#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include "ctype.h"

#include "../../Common/AllTypes.h"
#include "../../Common/Common.h"
#include "../../Common/Comands.h"

#include "../Header/BinTreeType.h"
#include "../Header/BinTreeConst.h"
#include "../Header/BinTreeConfig.h"
#include "../Header/BinTreeFunc.h"
#include "../../NameTable/NameTableFunc.h"

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция создания бинарного дерева
 @param [in] tree Структура дерева
 @note Сразу создается первый элемент
*/
int binTreeCtr (BinTree_t* tree)
{
    assert (tree);
    tree->size = 1;
    tree->null = newNode ();

    tree->null->parent = NULL;
    tree->null->left = NULL;
    tree->null->right = NULL;
    tree->null->type = _TYPE_NUM;
    tree->null->value.dval = 0;
    tree->diff_var = NULL;

    tree->table_var = (NameTable_t*) calloc (1, sizeof (NameTable_t));
    if (tree->table_var == NULL)
        EXIT_FUNC("NULL calloc", {});

    nameTableCtr (tree->table_var);

    tree->table_cmd = (NameTable_t*) calloc (1, sizeof (NameTable_t));
    if (tree->table_cmd == NULL)
        EXIT_FUNC("NULL calloc", {});

    tree->table_cmd->data = (NameTableVar_t*) calloc (sizeof (CODE_WORDS) / sizeof (CODE_WORDS[0]), sizeof (NameTableVar_t));
    if (tree->table_cmd->data == NULL)
        EXIT_FUNC("NULL calloc", {});

    // Пусть мы и копируем данные, которые лежат в константах, но благодаря этому мв получаем:
    //     1. Общие функции для удаления таблицы имен с переменными и кодовыми словами
    //     2. Возникает меньше зависимостей и файл с константами нужно подключать в меньшее количество .cpp

    for (size_t i = 0; i < sizeof (CODE_WORDS) / sizeof (CODE_WORDS[0]); i++)
    {
        // printf ("CMD: %s\n", CODE_WORDS[i].name);
        tree->table_cmd->data[i].name = strdup (CODE_WORDS[i].name);
        tree->table_cmd->data[i].value = CODE_WORDS[i].type;
        tree->table_cmd->data[i].hash = CODE_WORDS[i].hash;
    }
    tree->table_cmd->size = sizeof (CODE_WORDS) / sizeof (CODE_WORDS[0]);
    tree->table_cmd->capacity = tree->table_cmd->size;

    createHtml ();
    createLaTex ();

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция выделения памяти под новый объект
 @note Связи необходимо прописать вручную
 @return Указатель на блок памяти, где хранится новый узел
*/
Node_t* newNode ()
{
    Node_t* node = (Node_t*) calloc (1, sizeof (Node_t));

    if (node == NULL)   { EXIT_FUNC("NULL calloc", NULL); }
    return node;
}
// ---------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция удаления дерева
 @param [in] tree Указатель на структуру дерева
*/
int binTreeDtr (BinTree_t* tree)
{
    assert (tree);

    deleteNode (tree->null);
    nameTableDtr (tree->table_var);
    nameTableDtr (tree->table_cmd);

    if (tree->diff_var != NULL)     { free (tree->diff_var); }

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция удаления поддерева
 @param [in] node Указатель на корень поддерева
*/
int deleteNode (Node_t* node)
{
    assert (node);

    if (node->left)     { deleteNode (node->left); }
    if (node->right)    { deleteNode (node->right); }

    free (node);
    return 0;
}
// ---------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция сохранения дерева в файл
 @param [in] tree Указатель на структуру дерева
*/
int binTreeSave (BinTree_t* tree)
{
    assert (tree);

    if (tree->size == 0)
    {
        return 0;
    }

    FILE* file = fopen (STANDARD_SAVE_ADR, "wb");
    if (file == NULL)
    {
        EXIT_FUNC("NULL file", 1);
    }

    saveNode (file, tree->null, tree->table_var, tree->table_cmd);
    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция сохранения поддерева в файл
 @param [in] stream Указатель на файл, куда будет производится запись
 @param [in] node Указатель на корень поддерева
 @param [in] table_var Таблица имен с переменными
 @param [in] table_cmd Таблица имен с кодовыми словами
*/
int saveNode (FILE* stream,
              Node_t* node,
              NameTable_t* table_var,
              NameTable_t* table_cmd)
{
    assert (stream);
    assert (node);
    assert (table_var);
    assert (table_cmd);

    fprintf (stream, "(");
    switch (node->type)
    {
        case (_TYPE_OPER):
        {
            fprintf (stream, SPEC_NAME_TABLE_NAME " ", nameTableGetName (table_cmd, node->value.ival));
            break;
        }
        case (_TYPE_VAR):
        {
            fprintf (stream, SPEC_NAME_TABLE_NAME " ", nameTableGetName (table_var, node->value.ival));
            break;
        }
        case (_TYPE_NUM):
        {
            fprintf (stream, "%lg", node->value.dval);
            break;
        }
        default:
        {
            EXIT_FUNC("Unknown type", 1);
        }
    }

    if (node->left)     { saveNode (stream, node->left, table_var, table_cmd); }
    else                { fprintf (stream, " nil "); }

    if (node->right)    { saveNode (stream, node->right, table_var, table_cmd); }
    else                { fprintf (stream, " nil "); }

    fprintf (stream, ")");
    return 0;
}
// ---------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция загрузки дерева из файла
 @param [in] tree Указатель на структуру дерева
*/
int binTreeUpload (BinTree_t* tree)
{
    assert (tree);

    size_t size_buffer = getFileSize (STANDARD_UPLOAD_ADR);
    char* buffer = (char*) calloc (size_buffer + 1, sizeof (char));

    FILE* stream = fopen (STANDARD_UPLOAD_ADR, "rb");
    size_t trash = fread (buffer, sizeof (char), size_buffer, stream);
    (void) trash;
    fclose (stream);

    binTreeDtr (tree);
    binTreeCtr (tree);

    char* buffer_to_free = buffer;
    tree->null = uploadNode (tree->null, &buffer, tree->table_var, tree->table_cmd);
    free (tree->null->parent);
    tree->null->parent = NULL;
    free (buffer_to_free);
    // Нулевой элемент необходим в дереве для его существования, но в загружено дереве он мешает

    dumpToLaTex (tree->null, tree->table_var);
    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция парсинга буфера
 @param [in] parent Указатель на родительский элемент
 @param [in] cur_pose Указатель на адрес хранения адреса текущего рассматриваемого символа
 @param [in] table_var Таблица имен переменных
 @param [in] table_cmd Таблица имен команд
 @return Указатель на новый узел в случае необходимости его создания, иначе NULL
*/
Node_t* uploadNode (Node_t* parent,
                    char** cur_pose,
                    NameTable_t* table_var,
                    NameTable_t* table_cmd)
{
    assert (parent);
    assert (cur_pose);
    assert (*cur_pose);
    assert (table_var);
    assert (table_cmd);

    skipVoid (cur_pose);
    if (**cur_pose == '(')
    {
        Node_t* node = newNode ();
        ++ *cur_pose; // Skip '('
        skipVoid (cur_pose);

        switch (**cur_pose)
        {
            case '+':
            {
                node->type = _TYPE_OPER;
                node->value.ival = ADD_OPER;
                ++ *cur_pose;
                break;
            }
            case '-':
            {
                if (*(*cur_pose + 1) == ' ')
                {
                    node->type = _TYPE_OPER;
                    node->value.ival = SUB_OPER;
                    ++ *cur_pose;
                }
                else
                {
                    char* new_pose = NULL;
                    node->type = _TYPE_NUM;
                    node->value.dval = strtod (*cur_pose, &new_pose);
                    *cur_pose = new_pose;
                }
                break;
            }
            case '*':
            {
                node->type = _TYPE_OPER;
                node->value.ival = MUL_OPER;
                ++ *cur_pose;
                break;
            }
            case '/':
            {
                node->type = _TYPE_OPER;
                node->value.ival = DIV_OPER;
                ++ *cur_pose;
                break;
            }
            default:
            {
                int index = 0;
                if (isdigit (**cur_pose))
                {
                    char* new_pose = NULL;
                    node->type = _TYPE_NUM;
                    node->value.dval = strtod (*cur_pose, &new_pose);
                    *cur_pose = new_pose;
                }

                else if ((index = nameTableFind (table_cmd, *cur_pose)) != -1)
                {
                    node->type = _TYPE_OPER;
                    node->value.ival = index;
                    *cur_pose += lenName (*cur_pose);
                }

                else if ((index = nameTableFind (table_var, *cur_pose)) == -1)
                {
                    node->type = _TYPE_VAR;
                    node->value.ival = nameTableAdd (table_var, *cur_pose, 0, (size_t) lenName (*cur_pose));
                    *cur_pose += lenName (*cur_pose);
                }

                else
                {
                    node->type = _TYPE_VAR;
                    node->value.ival = index;
                    *cur_pose += lenName (*cur_pose);
                }
            }
        }

        node->left = uploadNode (node, cur_pose, table_var, table_cmd);
        node->right = uploadNode (node, cur_pose, table_var, table_cmd);
        node->parent = parent;

        skipVoid (cur_pose);
        ++ *cur_pose;
        return node;
    }

    else
    {
        if (strncmp ("nil", *cur_pose, 3) == 0)
        {
            *cur_pose += 3; // strlen ("nil")
            return NULL;
        }

        printf ("ERROR in syntax: %s\n", *cur_pose);
        return NULL;
    }

    return NULL;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция нахождения длины имени (до первого ' ')
 @param [in] name Полученное имя
 @return Длина имени
*/
int lenName (const char* name)
{
    assert (name);

    int len = 0;
    while (name[len] != ' ')
        len++;

    return len;
}
// ---------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция скипа всего ненужного синтаксиса, указанного в SYNTAX_VOID
 * @param [in] Указатель на адрес хранения текущего символа
*/
int skipVoid (char** cur_pose)
{
    assert (cur_pose);
    assert (*cur_pose);

    while (1)
    {
        int flag = 0;
        for (size_t i = 0; i < sizeof (SYNTAX_VOID) * sizeof (SYNTAX_VOID[0]); i++)
        {
            if (**cur_pose == SYNTAX_VOID[i])
            {
                (*cur_pose)++;
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            return 0;
        }
    }
    return -1;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
/**
 @brief Функция поиска переменной в таблице имен
 @param [in] node Указатель на корень поддерева
 @param [in] table_var Таблица имен
 @param [in] name Имя переменной
 @return 0, если нет переменной, иначе 1 и больше
*/
int findVar (Node_t* node,
             NameTable_t* table_var,
             const char* name)
{
    assert (node);
    assert (table_var);
    assert (name);

    size_t hash = getHash (name);
    if (node->type == _TYPE_VAR && table_var->data[node->value.ival].hash == hash)
    {
        if (strcmp (table_var->data[node->value.ival].name, name) == 0)
            return 1;
    }

    int find_bool = 0;
    if (node->left)
        find_bool += findVar (node->left, table_var, name);

    if (node->right)
        find_bool += findVar (node->right, table_var, name);

    return find_bool;
}
// ----------------------------------------------------------------------------------------------------

