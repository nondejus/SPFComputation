/*
 * =====================================================================================
 *
 *       Filename:  rlfa.h
 *
 *    Description:  This file declares the data structures used for computing Remote Loop free alternates
 *
 *        Version:  1.0
 *        Created:  Thursday 07 September 2017 03:35:42  IST
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

#ifndef __RLFA__
#define __RLFA__

#include "LinkedListApi.h"
#include "instanceconst.h"
#include "spfcomputation.h"

typedef ll_t * p_space_set_t;
typedef ll_t * q_space_set_t;
typedef ll_t * pq_space_set_t;

typedef struct _node_t node_t;
typedef struct _edge_t edge_t;

typedef enum{

    LINK_PROTECTION_LFA,
    LINK_PROTECTION_LFA_DOWNSTREAM,
    LINK_AND_NODE_PROTECTION_LFA,
    LINK_PROTECTION_RLFA,
    LINK_PROTECTION_RLFA_DOWNSTREAM,
    LINK_AND_NODE_PROTECTION_RLFA,
    UNKNOWN_LFA_TYPE
} lfa_type_t;

char *
get_str_lfa_type(lfa_type_t lfa_type);

typedef struct lfa_dest_pair_{
    node_t *lfa;
    edge_end_t *oif_to_lfa;
    node_t *dest;
    lfa_type_t lfa_type;
} lfa_dest_pair_t;

typedef struct lfa_{
    
    edge_end_t *protected_link;
    ll_t *lfa;
} lfa_t;

lfa_t *
get_new_lfa();

void
free_lfa();

void
print_lfa_info(lfa_t *lfa);

void
Compute_and_Store_Forward_SPF(node_t *spf_root,
                            spf_info_t *spf_info,
                            LEVEL level);
void
Compute_Neighbor_SPFs(node_t *spf_root, edge_t *edge, 
                      LEVEL level);

p_space_set_t 
compute_p_space(node_t *node, edge_t *failed_edge, LEVEL level);

p_space_set_t 
compute_extended_p_space(node_t *node, edge_t *failed_edge, LEVEL level);

q_space_set_t
compute_q_space(node_t *node, edge_t *failed_edge, LEVEL level);

pq_space_set_t
Intersect_Extended_P_and_Q_Space(p_space_set_t pset, q_space_set_t qset);

void
compute_rlfa(node_t *node, LEVEL level, edge_t *failed_edge, node_t *dest);

#define DIST_X_Y(X,Y,_level)    \
    ((spf_result_t *)(GET_SPF_RESULT((&(X->spf_info)), Y, _level)))->spf_metric

/*
LFA Link/Link-and-node Protection
====================================

    Inequality 1 : Loop Free Criteria
    DIST(N,D) < DIST(N,S) + DIST(S,D)  - Means, the LFA N will not send the traffic back to S in case of primary link failure (S-E link failure)

    Inequality 2 :  Downstream Path Criteria
    DIST(N,D) < DIST(S,D)              - Means, Select LFA among nbrs such that, N is a downstream router to Destination D

    Inequality 3 : Node protection Criteria
    DIST(N,D) < DIST(N,E) + DIST(E,D)  - Means, S can safely re-route traffic to N, since N's path to D do not contain failed Hop E.

              +-----+                      +-----+
              |  S  +----------------------+  N  |
              +--+--+                      +-+---+
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |                           |
                 |         +---------+       |
                 |         |         |       |
                 |         |    D    |       |
                 +---------+         +-------+
                           |         |
                           +----+----+
                                |
                                |
                                |
                                |
                                |
                                |
                                |
                                |
                                |
                                |
                                |
                          +-----+-----+
                          |           |
                          |    E      |
                          |           |
                          |           |
                          +-----------+



STEPS : To compute LFA, do the following steps
    1. Run SPF on S to know DIST(S,D)
    2. Run SPF on all nbrs of S except primary-NH(S) to know DIST(N,D) and DIST(N,S)
    3. Filter nbrs of S using inequality 1
    4. Narrow down the subset further using inequality 2
    5. Remain is the potential set of LFAs which each surely provides Link protection, but do not guarantee node protection
    6. We need to investigate this subset of potential LFAs to possibly find one LFA which provide node protection(hence link protection)
    7. Test the remaining set for inequality 3

*/

lfa_t *
link_protection_lfa_back_up_nh(node_t * S, edge_t *protected_link, LEVEL level, boolean strict_down_stream_lfa);

lfa_t *
node_protection_lfa_back_up_nh(node_t * S, edge_t *protected_link, LEVEL level);

#endif /* __RLFA__ */
