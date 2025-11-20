#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../Common/Common.h"
#include "../Header/BinTreeType.h"
#include "../Header/BinTreeConfig.h"
#include "../Header/BinTreeFunc.h"


// -------------------------------------------------------------------------------------------------------
/// @brief Счетчик текущего количества изображений, полученных с помощью graphviz
static int AMOUNT_DUMP_IMAGE = 0;
// -------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция дампа дерева
 * @param [in] tree Указатель на структуру дерева
 * @param [in] reason Причина дампа
*/
int dumpTree (BinTree_t* tree,
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
                "<img src=%simg%d.png width=%zupx>\n",
                STANDARD_DUMP_IMAGE_ADR, AMOUNT_DUMP_IMAGE,
                tree->size * STANDARD_SIZE_GRAPH_BLOCK);
    }

    fclose (html_file);
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция создания графа по дереву
 * @param [in] tree Указатель на причину дампа
 * @return 0 (если не было ошибок), иначе 1
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
             "  splines=ortho;\n"
             "  nodesep=0.4;\n"
             "  ranksep=0.6;\n"
             "  node [shape=plaintext, style=filled, fontname=\"Helvetica\"];\n"
             "  edge [arrowhead=vee, arrowsize=0.6, penwidth=1.2];\n\n");

    createBlock (tree->null, file);
    createLine (tree->null, file);

    fprintf (file, "\n}\n");
    fclose (file);
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция создания блоков графа
 * @param [in] node Указатель на текущий узел
 * @param [in] file Указатель на dot файл
 * @return 0
*/
int createBlock (Node_t* node,
                 FILE* file)
{
    assert (node);

    if (node->left)
    {
        createBlock (node->left, file);
    }

    if (node->right)
    {
        createBlock (node->right, file);
    }


    fprintf (file,
             "block_%p [label=<\n<TABLE CELLSPACING=\"0\" CELLPADDING=\"4\">\n"
             "<TR><TD PORT=\"root\" BGCOLOR=\"#0308f9ff\" COLSPAN=\"2\"><B>%p</B></TD></TR>\n"
             "<TR><TD BGCOLOR=\"#b209ccff\" COLSPAN=\"2\">ROOT=%p</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\" COLSPAN=\"2\">type=%d</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\" COLSPAN=\"2\">value=" SPEC_NODE_VALUE "</TD></TR>\n"
             "<TR>\n<TD PORT=\"left\" BGCOLOR=\"#ff7301ff\">%p</TD>\n"
             "<TD PORT=\"right\" BGCOLOR=\"#08ff3aff\">%p</TD>\n</TR>\n</TABLE> >];\n\n",
             node, node, node->parent, node->type, node->value, node->left, node->right);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция создания связей между блоками
 * @param [in] node Указатель на текущий узел
 * @param [in] file Указатель на dot файл
 * @return 0
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
                     "block_%p:right -> block_%p:root [color=\"#0901faff\", penwidth = 1.5, arrowsize = 0.6, constraint = true, dir = both];\n",
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
                     "block_%p:left -> block_%p:root [color=\"#0901faff\", penwidth = 1.5, arrowsize = 0.6, constraint = true, dir = both];\n",
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
