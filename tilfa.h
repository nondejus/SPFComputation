/*
 * =====================================================================================
 *
 *       Filename:  tilfa.h
 *
 *    Description:  This file contains the definition and declaration of Data structures for TILFA 
 *
 *        Version:  1.0
 *        Created:  12/08/2019 07:04:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Juniper Networks (https://csepracticals.wixsite.com/csepracticals), sachinites@gmail.com
 *        Company:  Juniper Networks
 *
 *        This file is part of the SPFComputation distribution (https://github.com/sachinites) 
 *        Copyright (c) 2019 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify it under the terms of the GNU General 
 *        Public License as published by the Free Software Foundation, version 3.
 *        
 *        This program is distributed in the hope that it will be useful, but
 *        WITHOUT ANY WARRANTY; without even the implied warranty of
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *        General Public License for more details.
 *
 *        visit website : https://csepracticals.wixsite.com/csepracticals for more courses and projects
 *                                  
 * =====================================================================================
 */

#ifndef __TILFA__
#define __TILFA__

#include "instance.h"
#include "data_plane.h"
#include <stdint.h>
#include "complete_spf_path.h"

typedef struct edge_end_ interface_t;

typedef struct segment_list_{

    interface_t *oif;
    char gw_ip[PREFIX_LEN + 1];
    mpls_label_t mpls_label_out[MPLS_STACK_OP_LIMIT_MAX];
    MPLS_STACK_OP stack_op[MPLS_STACK_OP_LIMIT_MAX];
} segment_list_t;

typedef struct protected_resource_{
    
    node_t *plr_node;
    interface_t *protected_link;
    boolean link_protection;
    boolean node_protection;
} protected_resource_t;

static inline boolean
tlfa_protected_resource_equal(protected_resource_t *pr_res1,
                              protected_resource_t *pr_res2){

    if(pr_res1 == pr_res2) return TRUE;
    
    if(pr_res1->plr_node == pr_res2->plr_node &&
       pr_res1->protected_link == pr_res2->protected_link &&
       pr_res1->link_protection == pr_res2->link_protection &&
       pr_res1->node_protection == pr_res2->node_protection){

        return TRUE;
    }
    return FALSE;
}

typedef struct tilfa_segment_list_{

    /*protected Resource is the Key*/
    protected_resource_t pr_res;

    uint8_t n_segment_list;
    segment_list_t segment_list[MAX_NXT_HOPS];

    glthread_t segment_list_glue;
} tilfa_segment_list_t;
GLTHREAD_TO_STRUCT(tilfa_segment_list_to_segment_list, 
                    tilfa_segment_list_t, segment_list_glue, curr);

typedef struct tilfa_lcl_config_{

  char protected_link[IF_NAME_SIZE]; /*key*/
  boolean link_protection;
  boolean node_protection;
  glthread_t config_glue;
} tilfa_lcl_config_t;
GLTHREAD_TO_STRUCT(tilfa_lcl_config_to_config_glue, 
        tilfa_lcl_config_t, config_glue, curr);

typedef struct tilfa_cfg_globals_{
    
    boolean is_enabled;
    uint8_t max_segments_allowed;
} tilfa_cfg_globals_t;

typedef struct tilfa_info_ {

    tilfa_cfg_globals_t tilfa_gl_var;
    glthread_t tilfa_lcl_config_head;

    protected_resource_t current_resource_pruned;
    glthread_t post_convergence_spf_path[MAX_LEVEL];
    ll_t *tilfa_spf_results[MAX_LEVEL];
    
    /*To be stored in Remote Destinations, 
     * not local*/
    glthread_t tilfa_segment_list_head_L1;
    glthread_t tilfa_segment_list_head_L2;

    boolean is_tilfa_pruned;
} tilfa_info_t;

#define TLIFA_SEGMENT_LST_HEAD(tilfa_info_ptr, level)   \
    (level == LEVEL1 ? (&(tilfa_info_ptr->tilfa_segment_list_head_L1)) :\
             (&(tilfa_info_ptr->tilfa_segment_list_head_L2)));

segment_list_t *
tilfa_get_segment_list(node_t *node, 
                       node_t *plr_node, 
                       protected_resource_t *pr_res,
                       LEVEL level,
                       uint8_t *n_segment_list/*O/P*/);

void
init_tilfa(node_t *node);

boolean
tilfa_update_config(tilfa_info_t *tilfa_info,
                    char *protected_link,
                    boolean link_protection,
                    boolean node_protection,
                    boolean add_or_update);

boolean
tilfa_topology_prune_protected_resource(node_t *node,
    protected_resource_t *pr_res);

void
tilfa_topology_unprune_protected_resource(node_t *node,
    protected_resource_t *pr_res);

void
tilfa_run_post_convergence_spf(node_t *spf_root, LEVEL level,
                               protected_resource_t *pr_res);

ll_t *
tilfa_get_spf_result_list(node_t *node, LEVEL level);

spf_path_result_t *
TILFA_GET_SPF_PATH_RESULT(node_t *node, node_t *candidate_node,
                          LEVEL level);

glthread_t *
tilfa_get_spf_post_convergence_path_head(node_t *node, LEVEL level);

void
compute_tilfa(node_t *spf_root, LEVEL level);
#endif /* __TILFA__ */
