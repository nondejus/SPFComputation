/*
 * =====================================================================================
 *
 *       Filename:  prefix.c
 *
 *    Description:  Implementation of prefix.h
 *
 *        Version:  1.0
 *        Created:  Wednesday 30 August 2017 02:14:15  IST
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spfutil.h"
#include "prefix.h"
#include "LinkedListApi.h"
#include "instance.h"
#include "bitsop.h"
#include "routes.h"
#include "spftrace.h"
#include "sr_tlv_api.h"

extern instance_t *instance;

prefix_t *
create_new_prefix(const char *prefix, unsigned char mask, LEVEL level){

    prefix_t *prefix2 = calloc(1, sizeof(prefix_t));
    if(prefix)
        strncpy(prefix2->prefix, prefix, PREFIX_LEN);
    prefix2->prefix[PREFIX_LEN] = '\0';
    prefix2->mask = mask;
    prefix2->level = level;
    MARK_PREFIX_SR_INACTIVE(prefix2);
    set_prefix_property_metric(prefix2, DEFAULT_LOCAL_PREFIX_METRIC);
    return prefix2;
}

void
set_prefix_property_metric(prefix_t *prefix,
                           unsigned int metric){

    prefix->metric = metric;
}

static int
prefix_comparison_fn(void *_prefix, void *_key){

    prefix_t *prefix = (prefix_t *)_prefix;
    common_pfx_key_t *key = (common_pfx_key_t *)_key;
    if(strncmp(prefix->prefix, key->u.prefix.prefix, strlen(prefix->prefix)) == 0 &&
            strlen(prefix->prefix) == strlen(key->u.prefix.prefix) &&
            prefix->mask == key->u.prefix.mask)
        return TRUE;

    return FALSE;
}

/*Currently it decides based only on metric, in future we will
 * enhance this fn with other parameters such as external/internal routes
 * inter area/intra area routes etc*/
static int
prefix_order_comparison_fn(void *_prefix1, void *_prefix2){

    prefix_t *prefix1 = _prefix1;
    prefix_t *prefix2 = _prefix2;

    if(prefix1->metric < prefix2->metric)
        return -1;
    if(prefix1->metric == prefix2->metric)
        return 0;
    if(prefix1->metric > prefix2->metric)
        return 1;
    return 0;
}

prefix_pref_data_t
route_preference(FLAG route_flags, LEVEL level){

/* Same preference 0*/
    prefix_pref_data_t pref;

    if(level == LEVEL1 &&
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) && 
       !IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L1_INT_INT;
            pref.pref_str = "L1_INT_INT";
            return pref;
    }

    if(level == LEVEL1 &&
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) && 
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L1_EXT_INT;
            pref.pref_str = "L1_EXT_INT";
            return pref;
    }

/* Same preference 1*/
    if(level == LEVEL2 &&
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) && 
       !IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L2_INT_INT;
            pref.pref_str = "L2_INT_INT";
            return pref;
    }

    if(level == LEVEL2 &&
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&  
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
             pref.pref =  L2_EXT_INT;
             pref.pref_str = "L2_EXT_INT";
            return pref;
    }

    if(level == LEVEL2 && 
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref =  L1_L2_INT_INT;
            pref.pref_str = "L1_L2_INT_INT";
            return pref;
    }

    if(level == LEVEL2 && 
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref =  L1_L2_EXT_INT;
            pref.pref_str = "L1_L2_EXT_INT";
            return pref;
    }


    if(level == LEVEL2 &&
       (IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) || 1) && /* Up/Down bit is set but ignored as if it is unset*/
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref =  L2_L2_EXT_INT;
            pref.pref_str = "L2_L2_EXT_INT";
            return pref;
    }

/* Same preference 2*/
    if(level == LEVEL1 && 
        IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref =  L2_L1_INT_INT;
            pref.pref_str = "L2_L1_INT_INT";
            return pref;
    }

    if(level == LEVEL1 && 
        IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L2_L1_EXT_INT;
            pref.pref_str = "L2_L1_EXT_INT";
            return pref;
    }
    
    if(level == LEVEL1 &&
        IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
       !IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L1_L1_EXT_INT;
            pref.pref_str = "L1_L1_EXT_INT";
            return pref;
    }

/* Same preference 3*/
    if(level == LEVEL1 && 
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L1_EXT_EXT;
            pref.pref_str = "L1_EXT_EXT";
            return pref;
    }
    
/* Same preference 4*/
    if(level == LEVEL2 &&
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) && 
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L2_EXT_EXT;
            pref.pref_str = "L2_EXT_EXT";
            return pref;
    }

    if(level == LEVEL2 && 
       !IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
             pref.pref = L1_L2_EXT_EXT;
             pref.pref_str = "L1_L2_EXT_EXT";
            return pref;
    }

/* Same preference 5*/
    if(level == LEVEL1 &&
        IS_BIT_SET(route_flags, PREFIX_DOWNBIT_FLAG) && 
        IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = L2_L1_EXT_EXT;
            pref.pref_str = "L2_L1_EXT_EXT";
            return pref;
    }

/* Such a prefix is meant to be ignored*/
    if(!IS_BIT_SET(route_flags, PREFIX_EXTERNABIT_FLAG) &&
        IS_BIT_SET(route_flags, PREFIX_METRIC_TYPE_EXT)){
            pref.pref = ROUTE_UNKNOWN_PREFERENCE;
            pref.pref_str = "ROUTE_UNKNOWN_PREFERENCE";
            return pref;
    }

    assert(0);
    pref.pref = ROUTE_UNKNOWN_PREFERENCE;
    pref.pref_str = "ROUTE_UNKNOWN_PREFERENCE";
    return pref; /* Make compiler happy*/
}

void
init_prefix_key(common_pfx_key_t *pfx_key, char *_prefix, char mask){

    memset(pfx_key, 0, sizeof(common_pfx_key_t));
    apply_mask(_prefix, mask, pfx_key->u.prefix.prefix);
    pfx_key->u.prefix.prefix[PREFIX_LEN] = '\0';
    pfx_key->u.prefix.mask = mask;
}


comparison_fn
get_prefix_comparison_fn(){
    return prefix_comparison_fn;
}


order_comparison_fn
get_prefix_order_comparison_fn(){
    return prefix_order_comparison_fn;
}

/* Returns the prefix being leaked from L2 to L1 (Or otherwise). If the prefix is already
 * leaked, return NULL. This fn simply add the new prefix to new prefix list.*/

prefix_t *
leak_prefix(char *node_name, char *_prefix, char mask, 
                LEVEL from_level, LEVEL to_level){

    node_t *node = NULL;
    prefix_t *prefix = NULL, *leaked_prefix = NULL;
    common_pfx_key_t prefix_key;
    routes_t *route_to_be_leaked = NULL;

    if(!instance){
        printf("%s() : Network Graph is NULL\n", __FUNCTION__);
        return NULL;
    }

    node = (node_t *)singly_ll_search_by_key(instance->instance_node_list, node_name);

    prefix = node_local_prefix_search(node, from_level, _prefix, mask);
       
    /* Case 1 : leaking prefix on a local hosting node */ 
    if(prefix){
   
        /*Now add this prefix to to_level prefix list of node*/
        if(node_local_prefix_search(node, to_level, _prefix, mask)){
            printf("%s () : Error : Node : %s, prefix : %s already leaked/present in %s\n", 
            __FUNCTION__, node->node_name, STR_PREFIX(prefix), get_str_level(to_level));
            return NULL;
        }

        leaked_prefix = attach_prefix_on_node (node, prefix->prefix, prefix->mask, 
                        to_level, prefix->metric, prefix->prefix_flags);
        if(!leaked_prefix){
#ifdef __ENABLE_TRACE__            
            sprintf(instance->traceopts->b, "Node : %s, equal best prefix : %s already leaked/present in %s\n",
                node->node_name, STR_PREFIX(prefix), get_str_level(to_level));
            trace(instance->traceopts, SPF_PREFIX_BIT);
#endif
            return NULL;
        }
        leaked_prefix->ref_count = 0;

        if(from_level == LEVEL2 && to_level == LEVEL1)
            SET_BIT(leaked_prefix->prefix_flags, PREFIX_DOWNBIT_FLAG);

#ifdef __ENABLE_TRACE__        
        sprintf(instance->traceopts->b, "Node : %s : prefix %s/%u leaked from %s to %s", 
                node->node_name, STR_PREFIX(prefix), PREFIX_MASK(prefix), get_str_level(from_level), get_str_level(to_level));
        trace(instance->traceopts, SPF_PREFIX_BIT);
#endif

        return leaked_prefix;
    }

    /* case 2 : Leaking prefix on a remote node*/
    if(!prefix){
        /*if prefix do not exist, means, we are leaking remote prefix on a node*/

        init_prefix_key(&prefix_key, _prefix, mask);
        route_to_be_leaked = search_route_in_spf_route_list(&node->spf_info, &prefix_key, UNICAST_T);
        
        /*Route must exist on remote node*/
        if(!route_to_be_leaked){
            printf("%s() : Node : %s : Error : route %s/%u do not exist in %s\n", 
                    __FUNCTION__, node->node_name, 
                    _prefix, mask, 
                    get_str_level(from_level));
            return NULL;
        }

        /*We need to add this remote route which is leaked from L2 to L1 in native L1 prefix list
         * so that L1 router can compute route to this leaked prefix by running full spf run */

        leaked_prefix = attach_prefix_on_node (node, _prefix, mask, 
                        to_level, IS_BIT_SET(route_to_be_leaked->flags, PREFIX_EXTERNABIT_FLAG) ? 
                        route_to_be_leaked->ext_metric : route_to_be_leaked->spf_metric, 0);

        if(!leaked_prefix){
#ifdef __ENABLE_TRACE__            
            sprintf(instance->traceopts->b, "Node : %s, equal best prefix : %s already leaked/present in %s\n",
                node->node_name, STR_PREFIX(prefix), get_str_level(to_level));
            trace(instance->traceopts, SPF_PREFIX_BIT);
#endif
            return NULL;
        }

        leaked_prefix->prefix_flags = route_to_be_leaked->flags;
        leaked_prefix->ref_count = 0;

        if(from_level == LEVEL2 && to_level == LEVEL1)
            SET_BIT(leaked_prefix->prefix_flags, PREFIX_DOWNBIT_FLAG);

#ifdef __ENABLE_TRACE__        
        sprintf(instance->traceopts->b, "Node : %s : prefix %s/%u leaked from %s to %s", 
                node->node_name, route_to_be_leaked->rt_key.u.prefix.prefix, 
                route_to_be_leaked->rt_key.u.prefix.mask, get_str_level(from_level), 
                get_str_level(to_level)); 
        trace(instance->traceopts, SPF_PREFIX_BIT); 
#endif

        return leaked_prefix;
    }
    return NULL;
}

void
add_new_prefix_in_list(ll_t *prefix_list , prefix_t *prefix, 
                unsigned int prefix_hosting_node_metric){

    singly_ll_node_t *list_node_prev = NULL, 
                     *list_node_next = NULL;

    prefix_t *list_prefix = NULL;

    /* empty list*/
    if(is_singly_ll_empty(prefix_list)){
        singly_ll_add_node_by_val(prefix_list, prefix);
        return;
    }

    /* Only one node*/
    if(GET_NODE_COUNT_SINGLY_LL(prefix_list) == 1){
        if(prefix->metric + prefix_hosting_node_metric < ((prefix_t *)(prefix_list->head->data))->metric){
            singly_ll_add_node_by_val(prefix_list, prefix);
        }
        else{
            singly_ll_node_t *new_node = singly_ll_init_node(prefix);
            prefix_list->head->next = new_node;
            INC_NODE_COUNT_SINGLY_LL(prefix_list);
        }
        return;
    }

    /* If prefix need to be added as first node in a list*/
    if(prefix->metric + prefix_hosting_node_metric < 
                ((prefix_t *)(GET_HEAD_SINGLY_LL(prefix_list)->data))->metric){

        singly_ll_node_t *new_node = singly_ll_init_node(prefix);
        new_node->next = GET_HEAD_SINGLY_LL(prefix_list);
        prefix_list->head = new_node;
        INC_NODE_COUNT_SINGLY_LL(prefix_list);
        return;
    }


    ITERATE_LIST_BEGIN(prefix_list, list_node_next){

        list_prefix = list_node_next->data;
        if(!(prefix->metric + prefix_hosting_node_metric < list_prefix->metric)){ 

            list_node_prev = list_node_next;
            continue;
        }

        singly_ll_node_t *new_node = singly_ll_init_node(prefix);
        new_node->next = list_node_next;
        list_node_prev->next = new_node;
        INC_NODE_COUNT_SINGLY_LL(prefix_list);
        return;

    }ITERATE_LIST_END;

    /*Add in the end*/
    singly_ll_node_t *new_node = singly_ll_init_node(prefix);
    list_node_prev->next = new_node;
    INC_NODE_COUNT_SINGLY_LL(prefix_list);
}

FLAG
is_prefix_byte_equal(prefix_t *prefix1, prefix_t *prefix2, 
                    unsigned int prefix2_hosting_node_metric){

    if(strncmp(prefix1->prefix, prefix2->prefix, PREFIX_LEN) == 0   &&
        prefix1->mask == prefix2->mask                              &&
        prefix1->metric == prefix2->metric + prefix2_hosting_node_metric                         &&
        prefix1->hosting_node == prefix2->hosting_node)
            return 1;
    return 0;
}


/* Let us delegate all add logic to this fn*/
/* Returns 1 if prefix added, 0 if rejected*/
FLAG
add_prefix_to_prefix_list(ll_t *prefix_list, 
                          prefix_t *prefix, 
                          unsigned int hosting_node_metric){

    common_pfx_key_t key;
    memset(&key, 0 , sizeof(common_pfx_key_t));
    strncpy(key.u.prefix.prefix, prefix->prefix, PREFIX_LEN);
    key.u.prefix.prefix[PREFIX_LEN] = '\0'; 
    key.u.prefix.mask = prefix->mask;
    assert(!singly_ll_search_by_key(prefix_list, &key));
    add_new_prefix_in_list(prefix_list, prefix, hosting_node_metric);
    return 1;
}

void
delete_prefix_from_prefix_list(ll_t *prefix_list, char *prefix, char mask){

    prefix_t *old_prefix = NULL;
    common_pfx_key_t key;
    strncpy(key.u.prefix.prefix, prefix, PREFIX_LEN);
    key.u.prefix.prefix[PREFIX_LEN] = '\0';
    key.u.prefix.mask = mask;
    old_prefix = singly_ll_search_by_key(prefix_list, &key);
    if(!old_prefix)
        return;
    singly_ll_delete_node_by_data_ptr(prefix_list, old_prefix);
    free_prefix(old_prefix);
    old_prefix = NULL;
}

void 
free_prefix(prefix_t *prefix){
    
    if(!prefix->psid_thread_ptr){
        free(prefix);
        return;
    }
    free_prefix_sid(prefix);    
    free(prefix);
}

/*Return true if node is one of the best prefix originator for a route*/
boolean
is_node_best_prefix_originator(node_t *node, routes_t *route){

    singly_ll_node_t *list_node = NULL;
    prefix_t *prefix = NULL;

    prefix_pref_data_t route_pref = route_preference(route->flags, route->level);
    prefix_pref_data_t prefix_pref;

    ITERATE_LIST_BEGIN(route->like_prefix_list, list_node){
        prefix = list_node->data;
        prefix_pref = route_preference(prefix->prefix_flags, route->level);
        if(route_pref.pref != prefix_pref.pref)
            break;
        if(prefix->hosting_node == node)
            return TRUE;
    }ITERATE_LIST_END;
    return FALSE;
}

