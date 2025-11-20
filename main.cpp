#include <stdio.h>

#include "diff.h"


int main ()
{
    BinTree_t tree = binTreeCtr ();
    binTreeUpload (&tree);
    dumpTree (&tree, "Test");
    binTreeSave (&tree);

    binTreeDtr (&tree);

    return 0;
}
