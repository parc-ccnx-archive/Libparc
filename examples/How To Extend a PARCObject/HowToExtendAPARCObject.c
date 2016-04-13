/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this file is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this file contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this file is complete, up to date or accurate. It
 * # is up to the contributors to maintain their section in this file up to date
 * # and up to the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this file must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 *
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */
/**
 *
 * This example shows a simple extention of an existing PARC Object implementation
 * (PARCString) to replace the default implementation of the Compare() function with another.
 *
 * The demonstration shows how to reverse the sort order of a PARCSortedList
 * containing a list of PARCString instances without changing PARCSortedList nor PARCString.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <stdio.h>

#include <parc/algol/parc_SortedList.h>
#include <parc/algol/parc_String.h>

/*
 * This is the default behaviour of the PARCSortedList implementation.
 */
void
forwardOrder(void)
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

/*
 * This function will be substituted for the default Compare implementation in the PARCString object.
 */
int
parcString_ReverseCompare(const PARCString *string, const PARCString *other)
{
    return parcString_Compare(string, other) * -1;
}

parcObject_Extends(PARCReverseString, PARCString,
                    .compare = (PARCObjectCompare *) parcString_ReverseCompare);


PARCString *
parcMyString_Create(const char *string)
{
    PARCString *result = parcString_Create(string);

    // By setting the descriptor to our special descriptor here, we effectively
    // substitute the default compare function with our parcString_ReverseCompare
    parcObject_SetDescriptor(result, &parcObject_DescriptorName(PARCReverseString));
    
    return result;
}

void
reverseOrder(void)
{
    PARCSortedList *sortedList = parcSortedList_Create();
    
    PARCString *aaa = parcMyString_Create("aaa");
    PARCString *aab = parcMyString_Create("aab");
    PARCString *aac = parcMyString_Create("aac");
    
    parcSortedList_Add(sortedList, aaa);
    parcSortedList_Add(sortedList, aab);
    parcSortedList_Add(sortedList, aac);
    
    parcSortedList_Display(sortedList, 0);
    parcString_Release(&aaa);
    parcString_Release(&aab);
    parcString_Release(&aac);
}

int
main(int argc, const char * argv[argc])
{
    forwardOrder();
    reverseOrder();
    return 0;
}
