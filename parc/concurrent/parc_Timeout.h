/*
 * Copyright (c) 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Glenn Scott, Computing Science Laboratory, PARC
 * @copyright 2016 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#ifndef parc_Timeout_h
#define parc_Timeout_h
#include <stdint.h>

typedef uint64_t PARCTimeout;

/**
 * @def PARCTimeout_Never
 * This represents a timeout that will never happen.
 */
#define PARCTimeout_Never NULL

/*
 * @def PARCTimeout_Immediate
 * This represents a timeout that immediately happens.
 * Equivalent to PARCTimeout_MicroSeconds(0)
 */
#define PARCTimeout_Immediate ((PARCTimeout) 0)

/*
 * @def PARCTimeout_NanoSeconds
 * This represents a timeout that will occur in the specified number of nanoseconds.
 */
#define PARCTimeout_NanoSeconds(_nsec_) ((PARCTimeout) _nsec_ * 1000)

/*
 * @def PARCTimeout_MicroSeconds
 * This represents a timeout that will occur in the specified number of microseconds.
 */
#define PARCTimeout_MicroSeconds(_usec_) ((PARCTimeout) _usec_ * 1000)

/*
 * @def PARCTimeout_MilliSeconds
 * This represents a timeout that will occur in the specified number of microseconds.
 */
#define PARCTimeout_MilliSeconds(_msec_) ((PARCTimeout) _msec_ * 1000000)
#endif /* parc_Timeout_h */
