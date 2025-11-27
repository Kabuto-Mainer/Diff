#include <stdio.h>
#include <assert.h>

#define SYNTAX_ERROR(__str)                      \
    printf ("Syntax error in str: %s\n", __str); \
    return 0;

int getN (char** cur_pose);
int getP (char** cur_pose);
int getE (char** cur_pose);
int getT (char** cur_pose);
int getG (char** cur_pose);

int main ()
{
    char equation[200] = "";
    sprintf (equation, "(5-3)*4$");
    char* pose = &(equation[0]);

    printf ("RES: %d\n", getG (&pose));
    return 0;
}


int getN (char** cur_pose)
{
    assert (cur_pose);
    assert (*cur_pose);

    int val = 0;
    int len = 0;
    while (**cur_pose >= '0' && **cur_pose <= '9')
    {
        val = val * 10 + (**cur_pose - '0');
        (*cur_pose)++;
        len++;
    }
    if (len == 0)
    {
        SYNTAX_ERROR (cur_pose);
    }
    return val;
}

int getP (char** cur_pose)
{
    assert (cur_pose);
    assert (*cur_pose);

    int val = 0;
    if (**cur_pose == '(')
    {
        ++ *cur_pose;
        val = getE (cur_pose);
        if (**cur_pose != ')')
        {
            SYNTAX_ERROR (cur_pose);
        }

        ++ *cur_pose;
    }
    else
    {
        val = getN (cur_pose);
    }
    return val;
}

int getE (char** cur_pose)
{
    assert (cur_pose);
    assert (*cur_pose);

    int val = getT (cur_pose);
    if (**cur_pose == '+' || **cur_pose == '-')
    {
        int oper = 0;
        if (**cur_pose == '+')  { oper = 1; }
        ++ *cur_pose;

        int val_2 = getT (cur_pose);
        if (oper == 1)  { val += val_2; }
        else            { val-= val_2; }
    }
    return val;
}

int getT (char** cur_pose)
{
    assert (cur_pose);
    assert (*cur_pose);

    int val = getP (cur_pose);
    if (**cur_pose == '*' || **cur_pose == '/')
    {
        int oper = 0;
        if (**cur_pose == '*')  { oper = 1; }
        ++ *cur_pose;

        int val_2 = getP (cur_pose);
        if (oper == 1)  { val *= val_2; }
        else            { val /= val_2; }
    }
    return val;
}

int getG (char** cur_pose)
{
    assert (cur_pose);
    assert (*cur_pose);

    int val = getE (cur_pose);
    if (**cur_pose != '$')
    {
        SYNTAX_ERROR (cur_pose);
    }
    return val;
}
