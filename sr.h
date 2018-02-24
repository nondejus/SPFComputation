/*
 * =====================================================================================
 *
 *       Filename:  sr.h
 *
 *    Description: defines the common Segment routing APIs and constants 
 *
 *        Version:  1.0
 *        Created:  Saturday 24 February 2018 11:52:09  IST
 *       Revision:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Networking Developer (AS), sachinites@gmail.com
 *        Company:  Brocade Communications(Jul 2012- Mar 2016), Current : Juniper Networks(Apr 2017 - Present)
 *        
 *        This file is part of the SPFComputation distribution (https://github.com/sachinites).
 *        Copyright (c) 2017 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify
 *        it under the terms of the GNU General Public License as published by  
 *        the Free Software Foundation, version 3.
 *
 *        This program is distributed in the hope that it will be useful, but 
 *        WITHOUT ANY WARRANTY; without even the implied warranty of 
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *        General Public License for more details.
 *
 *        You should have received a copy of the GNU General Public License 
 *        along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * =====================================================================================
 */

#ifndef __SR__
#define __SR__

#include "Stack/stack.h"

#define SRGB_DEF_LOWER_BOUND    16000
#define SRGB_DEF_UPPER_BOUND    23999
#define SRGB_MAX_SIZE           65536

typedef struct _srgb_range{
    unsigned int low_val;
    unsigned int upper_val;
} srgb_range_t;

typedef struct _srgb{
    srgb_range_t srgb_range;
    char *index_bitmap;
} srgb_t;

void
init_srgb_default_range(srgb_range_t *srgb);

typedef enum {
    CONTINUE,
    NEXT,
    POP
} SR_SEGMENT_OP;

typedef enum{
    PENULTIMATE_HOP_POPPING,
    ULTIMATE_HOP_POPPING,
    HOP_POPPING_NONE
} HOP_POPPING;

typedef enum{

    NODAL_SEGMENT,
    ADJ_SEGMENT_LOCAL,
    ADJ_SEGMENT_GLOBAL,
    PREFIX_SEGMENT = NODAL_SEGMENT,
    SEGMENT_TYPE_MAX,
    UNKNOWN_SEGMENT = SEGMENT_TYPE_MAX
} SEGMENT_TYPE;

typedef struct _prefix_sid_t prefix_sid_t;
typedef struct _adj_sid_t ajd_sid_t;

typedef struct _segment_t{
    
    SEGMENT_TYPE seg_type;
    union{
        prefix_sid_t *psid;
        ajd_sid_t *adj_sid;
    } seg;
} segment_t;

unsigned int
get_available_srgb_index(srgb_t *srgb);

void
mark_srgb_index_available(unsigned int index, srgb_t *srgb);

typedef struct _sr_policy_stack{
    stack_t *stack;
} sr_policy_stack_t;

/*SR policy stack operations*/
void
process_active_segment(sr_policy_stack_t *sr_stack);

#endif /* __SR__ */
