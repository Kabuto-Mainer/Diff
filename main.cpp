#include <stdio.h>

#include "diff.h"


int main ()
{
    BinTree_t tree = {};
    binTreeCtr (&tree);
    binTreeUpload (&tree);
    binTreeDumpHTML (&tree, "Test");

    BinTree_t new_tree = {};
    binTreeDiff (&tree, &new_tree);
    binTreeDumpHTML (&new_tree, "Test");
    calculateNum (new_tree.null);
    binTreeDumpHTML (&new_tree, "Test");


    binTreeSave (&new_tree);
    binTreeDtr (&new_tree);

    return 0;
}
