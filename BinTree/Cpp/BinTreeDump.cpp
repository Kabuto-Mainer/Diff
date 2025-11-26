#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../../Common/Common.h"
#include "../Header/BinTreeType.h"
#include "../Header/BinTreeConfig.h"
#include "../Header/BinTreeConst.h"
#include "../Header/BinTreeFunc.h"

#include "../../NameTable/NameTableFunc.h"


// -------------------------------------------------------------------------------------------------------
/// @brief Счетчик текущего количества изображений, полученных с помощью graphviz
static int AMOUNT_DUMP_IMAGE = 0;
// -------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция дампа дерева
 @param [in] tree Указатель на структуру дерева
 @param [in] reason Причина дампа
*/
int binTreeDumpHTML (BinTree_t* tree,
                     const char* reason)
{
    assert (tree);

    createGraph (tree);
    FILE* html_file = fopen (STANDARD_DUMP_HTML_ADR, "a");
    if (html_file == NULL)
    {
        EXIT_FUNC("NULL file", 1);
    }

    char comand[200] = "";
    sprintf (comand, "dot %s -T png -o %simg%d.png",
             STANDARD_DUMP_DOT_ADR,
             STANDARD_DUMP_IMAGE_ADR, AMOUNT_DUMP_IMAGE);

    int trash = system (comand);
    (void) trash;

    fprintf (html_file, "\n* Dump *\n");
    if (reason == NULL)     { fprintf (html_file, "No reason to dump (standard dump)\n"); }
    else                    { fprintf (html_file, "Reason: %s\n", reason); }

    fprintf (html_file, "Tree stats:\n"
             " - Size: %zu\n", tree->size);

    if (tree->size == 0)
    {
        fprintf (html_file, "Not enough nodes to create graph\n");

    }
    else
    {
        fprintf (html_file,
                "<img src=Image/img%d.png width=%zupx>\n",
                AMOUNT_DUMP_IMAGE,
                tree->size * STANDARD_SIZE_GRAPH_BLOCK);
    }
    AMOUNT_DUMP_IMAGE++;

    fclose (html_file);
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция создания графа по дереву
 @param [in] tree Указатель на причину дампа
 @return 0 (если не было ошибок), иначе 1
*/
int createGraph (BinTree_t* tree)
{
    assert (tree);

    FILE* file = fopen (STANDARD_DUMP_DOT_ADR, "w");
    if (file == NULL)
    {
        EXIT_FUNC("NULL file", 1);
    }

    fprintf (file,
             "digraph {\n"
             "  rankdir=UD;\n"
             "  bgcolor=\"#1e1e1e\"\n"
            //  "  splines=ortho;\n"
             "  nodesep=0.4;\n"
             "  ranksep=0.6;\n"
             "  node [shape=plaintext, style=filled, fontname=\"Helvetica\"];\n"
             "  edge [arrowhead=vee, arrowsize=0.6, penwidth=1.2];\n\n");

    createBlock (tree->null, file, tree->table_var, tree->table_cmd);
    createLine (tree->null, file);

    fprintf (file, "\n}\n");
    fclose (file);
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция создания блоков графа
 @param [in] node Указатель на текущий узел
 @param [in] stream Указатель на dot файл
 @param [in] table_var Таблица имен для переменных
 @param [in] table_cmd Таблица имен для кодовых слов
*/
int createBlock (Node_t* node,
                 FILE* stream,
                 NameTable_t* table_var,
                 NameTable_t* table_cmd)
{
    assert (node);
    assert (stream);

    // dumpNode (node);
    if (node->left)     { createBlock (node->left, stream, table_var, table_cmd); }
    if (node->right)    { createBlock (node->right, stream, table_var, table_cmd); }
    printFullBlock (node, stream, table_var, table_cmd);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

#ifdef DEBUG_MODE
// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция описания блока дерева для полного дампа
 @param [in] node Узел, блок которого описывается
 @param [in] stream Файл для вывода
 @param [in] table_var Таблица имен для переменных
 @param [in] table_cmd Таблица имен для кодовых слов
*/
int printFullBlock (Node_t* node,
                    FILE* stream,
                    NameTable_t* table_var,
                    NameTable_t* table_cmd)
{
    assert (node);
    assert (stream);
    assert (table_var);
    assert (table_cmd);

    char shape[20] = "";
    char type[20] = "";
    char color[10] = "";
    char label[40] = "";

    if (node->type == _TYPE_OPER)
    {
        strcpy (shape, "box");
        strcpy (type, BIN_TREE_NAME_TYPES_NODE[_TYPE_OPER]);
        strcpy (color, "#bd0fbaff");
        sprintf (label, "\'%s\'(%d)", nameTableGetName (table_cmd, node->value.ival), node->value.ival);
    }
    else if (node->type == _TYPE_VAR)
    {
        strcpy (shape, "box");
        strcpy (type, BIN_TREE_NAME_TYPES_NODE[_TYPE_VAR]);
        strcpy (color, "#1bc32cff");
        sprintf (label, "\'%s\'(%d)", nameTableGetName (table_var, node->value.ival), node->value.ival);
    }
    else
    {
        strcpy (shape, "box");
        strcpy (type, BIN_TREE_NAME_TYPES_NODE[_TYPE_NUM]);
        strcpy (color, "#0f32e0ff");
        sprintf (label, "%lg", node->value.dval);
    }

    fprintf (stream, "block_%p [shape=%s, label=<\n<TABLE CELLSPACING=\"0\" CELLPADDING=\"4\">\n"
            "<TR><TD PORT=\"root\" BGCOLOR=\"#0308f9ff\" COLSPAN=\"2\"><B>", node, shape);
    printAddress (stream, node);

    fprintf (stream, "</B></TD></TR>\n"
             "<TR><TD BGCOLOR=\"#b209ccff\" COLSPAN=\"2\">ROOT=");
    printAddress (stream, node->parent);

    fprintf (stream, "</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\" COLSPAN=\"2\">type=%s</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\" COLSPAN=\"2\">value=%s", type, label);


    fprintf (stream, "</TD></TR>\n"
             "<TR>\n<TD PORT=\"left\" BGCOLOR=\"#ff7301ff\">");
    printAddress (stream, node->left);

    fprintf (stream, "</TD>\n"
             "<TD PORT=\"right\" BGCOLOR=\"#08ff3aff\">");
    printAddress (stream, node->right);

    fprintf (stream, "</TD>\n</TR>\n</TABLE> >];\n\n");

    return 0;
}
// -------------------------------------------------------------------------------------------------------
#else // DEBUG_NODE
// ------------------------------------------------------------------------------------------------------
/**
 @brief Функция описания блока дерева для кратного дампа
 @param [in] node Узел, блок которого описывается
 @param [in] stream Файл для вывода
 @param [in] table_var Таблица имен для переменных
 @param [in] table_cmd Таблица имен для кодовых слов
*/
int printFullBlock (Node_t* node,
                    FILE* stream,
                    NameTable_t* table_var,
                    NameTable_t* table_cmd)
{
    assert (node);
    assert (stream);
    assert (table_var);
    assert (table_cmd);

    char shape[20] = "";
    char type[20] = "";
    char color[10] = "";
    char label[40] = "";

    if (node->type == _TYPE_OPER)
    {
        strcpy (shape, "circle");
        strcpy (type, BIN_TREE_NAME_TYPES_NODE[_TYPE_OPER]);
        strcpy (color, "#bd0fbaff");
        sprintf (label, "\'%s\'(%d)", nameTableGetName (table_cmd, node->value.ival), node->value.ival);
    }
    else if (node->type == _TYPE_VAR)
    {
        strcpy (shape, "diamond");
        strcpy (type, BIN_TREE_NAME_TYPES_NODE[_TYPE_VAR]);
        strcpy (color, "#1bc32cff");
        sprintf (label, "\'%s\'(%d)", nameTableGetName (table_var, node->value.ival), node->value.ival);
    }
    else
    {
        strcpy (shape, "box");
        strcpy (type, BIN_TREE_NAME_TYPES_NODE[_TYPE_NUM]);
        strcpy (color, "#0f32e0ff");
        sprintf (label, "%lg", node->value.dval);
    }

    fprintf (stream, "block_%p [shape=%s, style=filled, "
             "fillcolor=\"%s\", label=\"%s\"];\n",
             node, shape, color, label);
    return 0;
}
// -------------------------------------------------------------------------------------------------------
#endif // DEBUG_MODE

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция распечатки адреса в HEX-формате
 @param [in] stream Файл для вывода
 @param [in] address Полученный адрес
*/
int printAddress (FILE* stream,
                  const void* address)
{
    assert (stream);

    if (address == NULL)
    {
        fprintf (stream, "NIL");
        return 0;
    }
    fprintf (stream, "%p", address);

    // const unsigned char* bytes = (const unsigned char*) &address;
    // size_t size = sizeof (address);

    // for (size_t i = 0; i < size; i++)     { fprintf (stream, "%X", bytes[i]); }

    return 0;
}
// -------------------------------------------------------------------------------------------------------

#ifdef DEBUG_MODE
// -------------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция создания связей между блоками
 @param [in] node Указатель на текущий узел
 @param [in] file Указатель на dot файл
 @return 0
*/
int createLine (Node_t* node,
                FILE* file)
{
    assert (node);
    assert (file);

    if (node->left)
    {
        createLine (node->left, file);
    }

    if (node->right)
    {
        createLine (node->right, file);
    }

    if (node->right)
    {
        if (node->right->parent == node)
        {
            fprintf (file,
                     "block_%p:right -> block_%p:root [color=\"#fefefeff\", penwidth = 1.5, arrowsize = 0.6, constraint = true, dir = both];\n",
                     node, node->right);
        }

        else
        {
            fprintf (file,
                     "block_%p:right -> block_%p:root [color=\"#fa0101ff\", penwidth = 1.5, arrowsize = 0.6, constraint = true];\n",
                     node, node->right);
        }
    }

    if (node->left)
    {
        if (node->left->parent == node)
        {
            fprintf (file,
                     "block_%p:left -> block_%p:root [color=\"#ffffffff\", penwidth = 1.5, arrowsize = 0.6, constraint = true, dir = both];\n",
                     node, node->left);
        }

        else
        {
            fprintf (file,
                     "block_%p:left -> block_%p:root [color=\"#fa0101ff\", penwidth = 1.5, arrowsize = 0.6, constraint = true];\n",
                     node, node->left);
        }
    }
    return 0;
}
// -------------------------------------------------------------------------------------------------------
#else // DEBUG_MODE
// -------------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция создания связей между блоками
 @param [in] node Указатель на текущий узел
 @param [in] file Указатель на dot файл
 @return 0
*/
int createLine (Node_t* node,
                FILE* file)
{
    assert (node);
    assert (file);

    if (node->left)     { createLine (node->left, file); }
    if (node->right)    { createLine (node->right, file); }

    if (node->right)
    {
        if (node->right->parent == node)
            fprintf (file,
                     "block_%p -> block_%p [color=\"#fefefeff\", penwidth = 1.5, arrowsize = 0.6, constraint = true, dir = both];\n",
                     node, node->right);

        else
            fprintf (file,
                     "block_%p -> block_%p [color=\"#fa0101ff\", penwidth = 1.5, arrowsize = 0.6, constraint = true];\n",
                     node, node->right);
    }

    if (node->left)
    {
        if (node->left->parent == node)
            fprintf (file,
                     "block_%p -> block_%p [color=\"#ffffffff\", penwidth = 1.5, arrowsize = 0.6, constraint = true, dir = both];\n",
                     node, node->left);

        else
            fprintf (file,
                     "block_%p -> block_%p [color=\"#fa0101ff\", penwidth = 1.5, arrowsize = 0.6, constraint = true];\n",
                     node, node->left);
    }
    return 0;
}
// -------------------------------------------------------------------------------------------------------
#endif // DEBUG_MODE



// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция дампа дерева в LaTex файл
 @param [in] tree Указатель на структуру дерева
 @param [in] reason Причина вызова дампа
*/
int binTreeDumpLaTex (BinTree_t* tree,
                      const char* reason)
{
    assert (tree);
    // reason == NULL - не ошибка
    dumpToLaTex (tree->null, tree->table_var, reason);
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция дампа
 @param [in] node Указатель на корень поддерева
 @param [in] table_var Таблица переменных
 @param [in] reason Причина дампа
 @see LATEX
*/
int dumpToLaTex (Node_t* node,
                 NameTable_t* table_var,
                 const char* reason)
{
    assert (node);
    assert (table_var);

    FILE* stream = fopen (STANDARD_DUMP_LATEX_ADR, "a");
    if (stream == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (stream, "\\begin{dmath}\n");
    fprintf (stream, "\\text{");
    if (reason != NULL)     { fprintf (stream, "%s", reason); }
    else                    { printRandomPhraze (STANDARD_PHRAZE_ADR, stream); }
    fprintf (stream, ": } \\frac {dx}{d}(");

    dumpNodeLaTex (node, stream, table_var);
    fprintf (stream, ")= 0\n\\end{dmath}\n");

    fclose (stream);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Рекурсивная функция записи формулы по поддереву
 @param [in] node Указатель на корень поддерева
 @param [in] stream Файл для вывода
 @param [in] table_var Таблица переменных
*/
int dumpNodeLaTex (Node_t* node,
                   FILE* stream,
                   NameTable_t* table_var)
{
    assert (node);
    assert (stream);
    assert (table_var);

    if (node->type == _TYPE_OPER && node->value.ival == DIV_OPER)
    {
        fprintf (stream, " %s {", LATEX_COMAND[DIV_OPER]);
        dumpNodeLaTex (node->left, stream, table_var);
        fprintf (stream, "}{");
        dumpNodeLaTex (node->right, stream, table_var);
        fprintf (stream, "}");
        return 0;
    }

    if (node->left)     { dumpNodeLaTex (node->left, stream, table_var); }

    if (node->type == _TYPE_OPER)       { fprintf (stream, " %s ", LATEX_COMAND[node->value.ival]); }
    else if (node->type == _TYPE_VAR)   { fprintf (stream, " %s ", nameTableGetName (table_var, node->value.ival)); }
    else                                { fprintf (stream, " %lg ", node->value.dval); }

    if (node->type == _TYPE_OPER && node->value.ival == POW_OPER)
    {
        fprintf (stream, "{");
        dumpNodeLaTex (node->right, stream, table_var);
        fprintf (stream, "}");
        return 0;
    }

    if (node->right)    { dumpNodeLaTex (node->right, stream, table_var); }
    return 0;
}
// -------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция очистки html файла и заполнения его заголовков
*/
int cleanHtml ()
{
    FILE* stream = fopen (STANDARD_DUMP_HTML_ADR, "w");
    if (stream == NULL)
        EXIT_FUNC("NULL file", 1);

    fprintf (stream, "<pre style=\"font-family: 'Courier New', monospace;font-size: "
             "14px; color: #e0e0e0; background-color: #1e1e1e; padding: 10px; border-radius: 6px;\">\n");
    fclose(stream);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция очистки LaTex файла и заполнение его заголовков
*/
int cleanLaTex ()
{
    FILE* stream = fopen (STANDARD_DUMP_LATEX_ADR, "w");
    if (!stream)
        EXIT_FUNC("NULL file", 1);

    fprintf (stream,
             "\\documentclass[a4paper,12pt]{article}\n\n"
             "\\usepackage[utf8]{inputenc}\n"
             "\\usepackage[russian]{babel}\n"
             "\\usepackage{amsmath}\n"
             "\\usepackage{breqn}\n"
             "\\usepackage{graphicx}\n"
             "\\graphicspath{ {%s/} }\n"
            //  "\\usepackage{mathtools}\n"
             "\\usepackage{amssymb}\n"
             "\\usepackage{caption}\n"
             "\\begin{document}\n",
             STANDARD_GRAPHIC_PNG_ADR);
    fclose (stream);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция очистки директории дампа бинарного дерева
*/
int cleanDump ()
{
    cleanDir (STANDARD_DUMP_DIR_ADR);
    cleanLaTex ();
    cleanHtml ();

    char cmd[200] = "";
    sprintf (cmd, "mkdir %s/Image", STANDARD_DUMP_DIR_ADR);
    int trash = system (cmd);

    sprintf (cmd, "mkdir %s/Graphic", STANDARD_DUMP_DIR_ADR);
    trash = system (cmd);
    (void) trash;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция очистки выбранной директории
 * @param [in] address_dir Адрес выбранной директории
*/
int cleanDir (const char* address_dir)
{
    assert (address_dir);

    char comand[200] = "";
    sprintf (comand, "rm -rf ./%s/*", address_dir);
    int trash = system (comand);
    (void) trash;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция вывода всей информации о узле
 @param [in] node Указатель на узел
*/
int dumpNode (Node_t* node)
{
    assert (node);

    printf ("---------\nNODE [%p]\n", node);
    printf ("ROOT [%p]\nLEFT [%p]\nRIGHT [%p]\n", node->parent, node->left, node->right);
    printf ("TYPE [%d]\n", node->type);

    if (node->type == _TYPE_NUM)    { printf ("VALUE [%lg]\n", node->value.dval); }
    else                            { printf ("VALUE [%d]\n", node->value.ival); }
    printf ("--------\n\n");

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 @brief Функция выведения случайной фразы из файла
 @param [in] input_file Файл с фразами
 @param [in] output_stream Файл, куда эта фраза запишется
*/
int printRandomPhraze (const char* input_file,
                      FILE* output_stream)
{
    assert (input_ifile);
    assert (output_stream);

    FILE* stream = fopen (input_file, "r");
    if (stream == NULL)
        EXIT_FUNC("NULL file", 1);

    size_t size = getFileSize (input_file);
    char* buffer = (char*) calloc (size + 1, sizeof (char));
    if (buffer == NULL)
        EXIT_FUNC("NULL calloc", 1);

    buffer[size - 1] = '\0';

    fread (buffer, size, sizeof (char), stream);
    fclose (stream);

    char** mass_adr = (char**) calloc (size / 10, sizeof (char*));
    if (mass_adr == NULL)
        EXIT_FUNC("NULL calloc", 1);

    size_t amount_str  = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            mass_adr[amount_str] = buffer + i + 1;
            amount_str++;
            continue;
        }
        if (buffer[i] == '\0')  { break; }
    }
    int number = int ((size_t) rand () / (RAND_MAX / amount_str));
    fprintf (output_stream, "%s", mass_adr[number]);
    free (mass_adr);
    free (buffer);

    return 0;
}
// -------------------------------------------------------------------------------------------------------
