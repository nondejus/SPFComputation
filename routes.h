/*
 * =====================================================================================
 *
 *       Filename:  routes.h
 *
 *    Description:  This file declares the Data structure for routing table construction
 *
 *        Version:  1.0
 *        Created:  Monday 04 September 2017 12:06:27  IST
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

#ifndef __ROUTES__
#define __ROUTES__

#include "instance.h"

/*Routine to build the routing table*/
typedef enum RTE_INSTALL_STATUS{

    RTE_STALE,
    RTE_ADDED,
    RTE_UPDATED, /*Updated route means, route has been re-calculated in subsequent spf run, but may or may not be changed*/
    RTE_CHANGED,
    RTE_NO_CHANGE
} route_intall_status; 

typedef struct routes_{

    common_pfx_key_t rt_key;
    int version;
    FLAG flags;
    LEVEL level;
    node_t *hosting_node;
    unsigned int spf_metric;
    unsigned int lsp_metric; /*meaningful if this LSP route*/
    unsigned int ext_metric; /*External metric*/

    /* NH lists*//*ToDo : Convert it into arrays*/
    ll_t *primary_nh_list[NH_MAX];/*Taking it as a list to accomodate ECMP*/
    ll_t *backup_nh_list[NH_MAX]; /*List of node_t pointers*/

    /*same subnet prefix lists*/
    ll_t *like_prefix_list; 
    route_intall_status install_state; 
    unsigned int ecmp_dest_count;

    /*SR support*/
    mpls_label_t prev_mpls_label;/*MPLS label of this route in its previous incarnation*/

} routes_t;

routes_t *route_malloc();

routes_t *
search_route_in_spf_route_list(spf_info_t *spf_info,
                                prefix_t *prefix, rtttype_t);

void
route_set_key(routes_t *route, char *ipv4_addr, char mask);

void
free_route(routes_t *route);

#define ROUTE_ADD_NH(_route_nh_list, _internal_nh_t_ptr)     \
    singly_ll_add_node_by_val(_route_nh_list, _internal_nh_t_ptr)

static inline void
ROUTE_FLUSH_PRIMARY_NH_LIST(routes_t *route, nh_type_t nh){

    singly_ll_node_t *list_node = NULL;
    ITERATE_LIST_BEGIN(route->primary_nh_list[nh], list_node){

        free(list_node->data);
        list_node->data = NULL;
    } ITERATE_LIST_END;

    delete_singly_ll(route->primary_nh_list[nh]);
}

static inline void
ROUTE_FLUSH_BACKUP_NH_LIST(routes_t *route, nh_type_t nh){

    singly_ll_node_t *list_node = NULL;
    ITERATE_LIST_BEGIN(route->backup_nh_list[nh], list_node){

        free(list_node->data);
        list_node->data = NULL;
    } ITERATE_LIST_END;

    delete_singly_ll(route->backup_nh_list[nh]);
}

#define ROUTE_ADD_BACKUP_NH(routeptr, nodeptr)      \
    singly_ll_add_node_by_val(routeptr->backup_nh_list, nodeptr)

#define ROUTE_ADD_TO_ROUTE_LIST(spfinfo_ptr, routeptr, topo)               \
    singly_ll_add_node_by_val(spfinfo_ptr->routes_list[topo], routeptr);   \
    singly_ll_add_node_by_val(spfinfo_ptr->priority_routes_list[topo], routeptr)

#define ROUTE_DEL_FROM_ROUTE_LIST(spfinfo_ptr, routeptr, topo)    \
    singly_ll_delete_node_by_data_ptr(spfinfo_ptr->routes_list[topo], routeptr);  \
    singly_ll_delete_node_by_data_ptr(spfinfo_ptr->priority_routes_list[topo], routeptr)

#define ROUTE_GET_PR_NH_CNT(routeptr, _nh)   \
    GET_NODE_COUNT_SINGLY_LL(routeptr->primary_nh_list[_nh])

#define ROUTE_GET_BEST_PREFIX(routeptr)   \
    ((GET_HEAD_SINGLY_LL(routeptr->like_prefix_list))->data)

#define IS_DEFAULT_ROUTE(routeptr)  \
    (strncmp(routeptr->rt_key.u.prefix.prefix, "0.0.0.0", PREFIX_LEN) == 0 && \
        routeptr->rt_key.u.prefix.mask == 0)

void
spf_postprocessing(spf_info_t *spf_info,      /* routes are stored globally*/
                   node_t *spf_root,          /* computing node which stores the result of spf run*/
                   LEVEL level);              /*Level of spf run*/

void
build_routing_table(spf_info_t *spf_info,
                    node_t *spf_root, LEVEL level);

void
delete_all_routes(node_t *node, LEVEL level);

void
start_route_installation(spf_info_t *spf_info, 
                         LEVEL level);

void
start_spring_routes_installation(spf_info_t *spf_info, 
                         LEVEL level);

int
route_search_comparison_fn(void * route, void *key);

typedef struct rttable_entry_ rttable_entry_t;

void
prepare_new_rt_entry_template(spf_info_t *spf_info, rttable_entry_t *rt_entry_template,
                               routes_t *route, unsigned int version);

typedef struct nh_t_ nh_t;

void
prepare_new_nxt_hop_template(node_t *computing_node, /*Computing node running SPF*/
        internal_nh_t *nxt_hop_node,                        /*Nbr node*/
        nh_t *nh_template,
        LEVEL level, nh_type_t nh);                  /*SPF run level*/

char *
route_intall_status_str(route_intall_status install_status);

void
mark_all_routes_stale(spf_info_t *spf_info, LEVEL level, rtttype_t topology);

internal_nh_t *
backup_selection_policy(routes_t *route);

static inline boolean
is_route_local(routes_t *route){

    nh_type_t nh;
    unsigned int nhcount = 0;

    ITERATE_NH_TYPE_BEGIN(nh){
        nhcount += GET_NODE_COUNT_SINGLY_LL(route->primary_nh_list[nh]) ;   
    } ITERATE_NH_TYPE_END;
    return nhcount == 0;
}

boolean
is_independant_primary_next_hop_list(routes_t *route);

boolean
is_independant_primary_next_hop_list_for_nodes(node_t *S, node_t *dst_node, LEVEL level);

void
show_internal_routing_tree(node_t *node, char *prefix, char mask, rtttype_t rt_type);

#endif /* __ROUTES__ */
