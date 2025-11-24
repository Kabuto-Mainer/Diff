#include <stdio.h>

#include "diff.h"


int main ()
{
    DIFF_Init ();
    DIFF_Tree_t tree = DIFF_TreeCtr ();
    DIFF_GetTree (tree, "Data/Input.txt");
    DIFF_BringDiff (&tree);
    DIFF_Optimization (tree);
    DIFF_TreeDtr (tree);
    DIFF_Quit ();
    return 0;
}
