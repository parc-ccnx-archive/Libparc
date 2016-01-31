/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file parc_CMacro.h
 * @ingroup Macro
 * @brief Utility C-Macros
 *
 * CMacro contains a set of utility macros for doing complicated macro processing
 *
 * @author Mike Slominski, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_CMacro_h
#define libparc_parc_CMacro_h

/**
 * parcCMacro_ThridParam expands to the "3rd" input parameter whatever that may be. It is a part
 * of the c-macro trick for implement a macro If-Else switch. If the first argument expands
 * to a comma then this macro expands to the second parameter, otherwise it expands to the 3rd parameter.
 */
#define parcCMacro_ThirdParam(A,B,C,...) C

/**
 * parcCMacro_IfElse is a c-macro trick for implementing a macro If-Else switch.
 * It uses parcCMacro_ThirdParam to select between A or B depending on whether __VA_ARGS__ expands to a comma.
 */
#define parcCMacro_IfElse(A,B,...) parcCMacro_ThirdParam(__VA_ARGS__, A, B, NOOP)

/** \cond */
#define _parcCMacro_Cat_(A,B) A##B
/** \endcond */

/**
 * parcCMacro_Cat combines two strings into a single token.
 */
#define parcCMacro_Cat(A,B) _parcCMacro_Cat_(A,B)

#endif //libparc_parc_CMacro_h
