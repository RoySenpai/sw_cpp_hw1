/*
 *  Software Systems CPP Course Assignment 1
 *  Copyright (C) 2023  Roy Simanovich
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

/*
 * Struct: AdptArray_
 * Size: 16 bytes
 * --------------------
 * This struct is the implementation of the AdptArray ADT.
 * It contains the size of the array, the array itself, and the functions
 * that are used to copy, delete, and print the elements in the array.
 * 
*/
struct AdptArray_
{
    /*
     * Field: Array Size
     * Size: 4 bytes
     * --------------------
     * This field is the size of the array.
     * 
    */
    int ArrSize;

    /*
     * Field: Array
     * Size: 8 bytes
     * --------------------
     * This field is the array itself.
     * 
    */
    PElement *pElemArr;

    /*
     * Field: Copy Function
     * Size: 8 bytes
     * --------------------
     * This field is the function that is used to copy the elements in the array.
     * 
    */
    COPY_FUNC copyfunc;

    /*
     * Field: Delete Function
     * Size: 8 bytes
     * --------------------
     * This field is the function that is used to delete the elements in the array.
     * 
    */
    DEL_FUNC delfunc;

    /*
     * Field: Print Function
     * Size: 8 bytes
     * --------------------
     * This field is the function that is used to print the elements in the array.
     * 
    */
    PRINT_FUNC printfunc;
};

/*
 * Function: CreateAdptArray
 * --------------------
 * This function creates a new AdptArray ADT.
 * 
 * copyfunc: The function that is used to copy the elements in the array.
 * 
 * delfunc: The function that is used to delete the elements in the array.
 * 
 * printfunc: The function that is used to print the elements in the array.
 * 
*/
PAdptArray CreateAdptArray(COPY_FUNC copyfunc, DEL_FUNC delfunc, PRINT_FUNC printfunc) {
    PAdptArray new_arr = (PAdptArray)malloc(sizeof(struct AdptArray_));

    if (new_arr == NULL)
    {
        fprintf(stderr, "Error: malloc failed");
        return NULL;
    }

    new_arr->ArrSize = 0;
    new_arr->pElemArr = NULL;
    new_arr->copyfunc = copyfunc;
    new_arr->delfunc = delfunc;
    new_arr->printfunc = printfunc;

    return new_arr;
}

/*
 * Function: DeleteAdptArray
 * --------------------
 * This function deletes an AdptArray ADT.
 * 
 * pArr: The AdptArray ADT to be deleted.
 * 
*/
void DeleteAdptArray(PAdptArray pArr) {
    if (pArr == NULL)
    {
        fprintf(stderr, "Error: parr is NULL");
        return;
    }

    else if (pArr->pElemArr != NULL)
    {
        for (int i = 0; i < pArr->ArrSize; ++i)
        {
            if (pArr->pElemArr[i] != NULL)
                pArr->delfunc(pArr->pElemArr[i]);
        }

        free(pArr->pElemArr);
    }

    free(pArr);
}

/*
 * Function: SetAdptArrayAt
 * --------------------
 * This function sets the element at a given index of an AdptArray ADT.
 * If the index is out of range, the array is extended to fit the new element.
 * If the index is in range, the previous element is deleted and the new element is set.
 * 
 * pArr: The AdptArray ADT.
 * 
 * idx: The index of the element to be set (must be non-negative).
 * 
 * pNewElement: The new element to be set.
 * 
*/
Result SetAdptArrayAt(PAdptArray pArr, int idx, PElement pNewElement) {
    PElement *newpElemArr = NULL;

    if (pArr == NULL)
        return FAIL;
    
    // If idx is out of range, realloc
    if (idx >= pArr->ArrSize)
    {
        // Extend Array
        if ((newpElemArr = (PElement *)calloc((idx + 1), sizeof(PElement))) == NULL)
            return FAIL;

        // Init new array and copy old array to new array
        if (pArr->pElemArr != NULL)
        {
            for (int i = 0; i < pArr->ArrSize; ++i)
            {
                if (pArr->pElemArr[i] != NULL)
                {
                    newpElemArr[i] = pArr->copyfunc(pArr->pElemArr[i]);
                    pArr->delfunc(pArr->pElemArr[i]);
                }
            }
        }

        // Free old array and save new array
        free(pArr->pElemArr);
        pArr->pElemArr = newpElemArr;
    }

    // Delete Previous Elem and Set New Elem
    if (pArr->pElemArr[idx] != NULL)
        pArr->delfunc(pArr->pElemArr[idx]);

    pArr->pElemArr[idx] = pArr->copyfunc(pNewElement);

    // Update Array Size
    pArr->ArrSize = (idx >= pArr->ArrSize) ? (idx + 1) : pArr->ArrSize;

    return SUCCESS;
}

/*
 * Function: GetAdptArrayAt
 * --------------------
 * This function returns a copy of the element at a given index of an AdptArray ADT.
 * 
 * pArr: The AdptArray ADT to be checked.
 * 
 * idx: The index of the element to be returned (must be non-negative).
 *
*/
PElement GetAdptArrayAt(PAdptArray pArr, int idx) {
    if (pArr == NULL)
        return NULL;

    else if (idx >= pArr->ArrSize || idx < 0)
        return NULL;

    return (pArr->pElemArr[idx] != NULL ? pArr->copyfunc(pArr->pElemArr[idx]):NULL);
}

/*
 * Function: GetAdptArraySize
 * --------------------
 * This function returns the size of an AdptArray ADT.
 * 
 * pArr: The AdptArray ADT to be checked.
 *  
*/
int GetAdptArraySize(PAdptArray pArr) {
    if (pArr == NULL)
        return -1;

    return pArr->ArrSize;
}

/*
 * Function: PrintDB
 * --------------------
 * This function prints the elements of an AdptArray ADT.
 * 
 * pArr: The AdptArray ADT to be printed.
 * 
*/
void PrintDB(PAdptArray pArr) {
    if (pArr == NULL)
    {
        fprintf(stderr, "Error: pArr is NULL");
        return;
    }

    for (int i = 0; i < pArr->ArrSize; ++i)
    {
        if (pArr->pElemArr[i] != NULL)
            pArr->printfunc(pArr->pElemArr[i]);
    }
}