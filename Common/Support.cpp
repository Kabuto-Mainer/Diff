#include <stdio.h>
#include "assert.h"
#include <sys/stat.h>
#include <unistd.h>

#include "Common.h"

// --------------------------------------------------------------------------------------------------
/**
 * @brief Хеш-функция
 * @param [in] string Строка, которая преобразуется в хеш
 * @return Хеш строки
*/
size_t getHash (const char* string)
{
    assert (string);

    size_t index = 0;
    size_t hash = 5381;
    while (string[index] != '\0')
    {
        hash = (hash * 33 + (size_t) string[index]);
        index++;
    }

    return hash;
}
// --------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция, находящая размер файла в байтах по его имени
 * @param [in] name_file Имя файла
 * @return Размер файла
*/
size_t getFileSize (const char* name_file)
{
    assert(name_file);

    struct stat file_stat = {};

    if (stat (name_file, &file_stat) == -1)
    {
        printf ("ERROR with stat\n");
        return 0;
    }
//     Вроде как размер не должен быть отрицательным
    return (size_t) file_stat.st_size;
}
// -------------------------------------------------------------------------------------------------------

