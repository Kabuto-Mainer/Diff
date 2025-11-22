#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"

#include "../Common/Common.h"
#include "NameTableType.h"
#include "NameTableConfig.h"
#include "NameTableFunc.h"

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция создания таблицы имен
 @param [in] table Указатель на структуру таблицы имен
*/
int nameTableCtr (NameTable_t* table)
{
    assert (table);

    table->data = (NameTableVar_t*) calloc (START_SIZE_TABLE, sizeof (NameTableVar_t));
    if (table->data == NULL)
    {
        EXIT_FUNC("NULL calloc", 1);
    }

    table->capacity = START_SIZE_TABLE;
    table->size = 0;

    return 0;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция удаления структуры таблицы имен
 @param [in] table Указатель на структуру таблицы имен
*/
int nameTableDtr (NameTable_t* table)
{
    assert (table);

    for (size_t i = 0; i < table->size; i++)
        free (table->data[i].name);

    free (table->data);
    free (table);

    return 0;
}
// ---------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция поиска имени в таблице имен
 @param [in] table Указатель на структуру таблицы имен
 @param [in] name Полученное имя
 @return Индекс имени в массива в случае поиска, иначе -1
*/
int nameTableFind (NameTable_t* table,
                   NameType_t name)
{
    assert (table);
    ASSERT_NAME_TABLE_NAME(name);

    // printf ("NAME_TABLE: %s\n", name);
    for (size_t i = 0; i < table->size; i++)
    {
        // printf ("F: %s\n", table->data[i].name);
        if (compareName (name, table->data[i].name) == 0)
            return (int) i;
    }

    return -1;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция сравнения имен таблицы имен
 @param [in] name_1 Первое имя
 @param [in] name_2 Второе имя
 @return 0, если совпадают, иначе 1
*/
int compareName (NameType_t name_1,
                 NameType_t name_2)
{
    ASSERT_NAME_TABLE_NAME(name_1);
    ASSERT_NAME_TABLE_NAME(name_2);

    // if (strcmp (name_1, name_2) == 0)
    //     return 0;

    int index = 0;
    while (name_1[index] != ' ' && name_1[index] != '\0' && name_1[index] != '\n' &&
           name_2[index] != ' ' && name_2[index] != '\0' && name_2[index] != '\n')
    {
        if (name_1[index] != name_2[index])
            return 1;
        index++;
    }
    return 0;
}
// ---------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция, возвращающая имя, лежащее под полученным индексом
 @param [in] table Указатель на таблицу имен
 @param [in] index Полученный индекс
 @return Имя, иначе NULL
*/
NameType_t nameTableGetName (NameTable_t* table,
                             int index)
{
    assert (table);
    if (index < 0 || index > (int) table->size)
        return NULL;

    return table->data[index].name;
}
// ---------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция получения текущего размера таблицы имен
 @param [in] table Указатель на структуру таблицы
 @return Размер
*/
size_t nameTableGetSize (NameTable_t* table)
{
    assert (table);
    return table->size;
}
// ---------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------
/**
 @brief Функция добавления элемента в таблицу имен
 @param [in] table Указатель на таблицу имен
 @param [in] name Имя добавляемого объекта
 @param [in] value Значение, соответствующее имени
 @param [in] len_name Длина имени
 @return Индекс имени в таблице имен (при ошибке -1)
*/
int nameTableAdd (NameTable_t* table,
                  NameType_t name,
                  NameValue_t value,
                  size_t len_name)
{
    assert (table);
    ASSERT_NAME_TABLE_NAME (name);

    if (table->size >= table->capacity)
    {
        NameTableVar_t* buffer = (NameTableVar_t*) realloc (table->data, table->capacity * 2);
        if (buffer == NULL)
            EXIT_FUNC("NULL realloc", -1);

        table->data = buffer;
        table->capacity *= 2;
    }

    table->data[table->size].name = strndup (name, len_name);
    table->data[table->size].value = value;
    table->data[table->size].hash = getHash (table->data[table->size].name);
    table->size++;

    return (int) table->size - 1;
}
// ---------------------------------------------------------------------------------------------------



