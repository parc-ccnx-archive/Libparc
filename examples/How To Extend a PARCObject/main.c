//
//  main.c
//  How To Extend a PARCObject
//
//  Created by gscott on 3/29/16.
//  Copyright © 2016 CSL. All rights reserved.
//

#include <stdio.h>

#include <parc/algol/parc_SortedList.h>
#include <parc/algol/parc_String.h>

void
forward(void)
{
    PARCSortedList *sortedList = parcSortedList_Create();

    PARCString *aaa = parcString_Create("aaa");
    PARCString *aab = parcString_Create("aab");
    PARCString *aac = parcString_Create("aac");
    
    parcSortedList_Add(sortedList, aaa);
    parcSortedList_Add(sortedList, aab);
    parcSortedList_Add(sortedList, aac);
    
    parcSortedList_Display(sortedList, 0);
    parcString_Release(&aaa);
    parcString_Release(&aab);
    parcString_Release(&aac);
}


int
parcString_ReverseCompare(const PARCString *string, const PARCString *other)
{
    return parcString_Compare(string, other) * -1;
}

#define parcObject_Extends(_superType, _subtype, ...) \
    parcObject_Declaration(_subtype) = { \
        .super      = &parcObject_DescriptorName(_superType), \
        .name       = #_subtype, \
        __VA_ARGS__  \
    }

parcObject_Extends(PARCString, PARCReverseString,
                    .compare = (PARCObjectCompare *) parcString_ReverseCompare);

void
reverse(void)
{
    PARCSortedList *sortedList = parcSortedList_Create();
    
    PARCString *aaa = parcString_Create("aaa");
    PARCString *aab = parcString_Create("aab");
    PARCString *aac = parcString_Create("aac");
    parcObject_SetDescriptor(aaa, &parcObject_DescriptorName(PARCReverseString));
    parcObject_SetDescriptor(aab, &parcObject_DescriptorName(PARCReverseString));
    parcObject_SetDescriptor(aac, &parcObject_DescriptorName(PARCReverseString));
    
    parcSortedList_Add(sortedList, aaa);
    parcSortedList_Add(sortedList, aab);
    parcSortedList_Add(sortedList, aac);
    
    parcSortedList_Display(sortedList, 0);
    parcString_Release(&aaa);
    parcString_Release(&aab);
    parcString_Release(&aac);
}

int
main(int argc, const char * argv[])
{
    forward();
    reverse();
    return 0;
}
