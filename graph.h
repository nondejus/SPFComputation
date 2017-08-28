/*
 * =====================================================================================
 *
 *       Filename:  graph.h
 *
 *    Description:  This is a header file to declare structures to define the Network topology
 *
 *        Version:  1.0
 *        Created:  Wednesday 2[MAX_LEVEL] August 2017 01:51:55  IST
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
 *        the Free Software Foundation, version [MAX_LEVEL].
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

#ifndef __GRAPH__
#define __GRAPH__

#include "graphconst.h"
#include "LinkedListApi.h"
#include <stdlib.h>
#include <assert.h>

typedef struct edge_end_ edge_end_t;

typedef struct _node_t{
    char node_name[NODE_NAME_SIZE];
    AREA area;
    edge_end_t *edges[MAX_NODE_INTF_SLOTS];
    NODE_TYPE node_type[MAX_LEVEL];
    unsigned int spf_metric[MAX_LEVEL];
    struct _node_t *next_hop[MAX_LEVEL][MAX_NXT_HOPS];
    struct _node_t *direct_next_hop[MAX_LEVEL][MAX_NXT_HOPS];
    edge_end_t *pn_intf[MAX_LEVEL];
    char attributes[MAX_LEVEL];                             /*1 Bytes of router attributes*/
    ll_t *attached_nodes;                                   /*Every node should know the L2 router(s) within a local area which are attached to another Area*/
    char traversing_bit;                                    /*This bit is only used to traverse the graph, otherwise it is not specification requirement. 1 if the node has been visited, zero otherwise*/
} node_t;

struct edge_end_{
    node_t *node;
    char intf_name[IF_NAME_SIZE];
    char prefix[MAX_LEVEL][PREFIX_LEN_WITH_MASK + 1];
    EDGE_END_DIRN dirn;/*dirn of edge is not level dependant*/
};

typedef struct _edge_t{
    edge_end_t from;
    unsigned int metric[MAX_LEVEL];
    LEVEL level;
    edge_end_t to;
} edge_t;

typedef struct graph_{
    node_t *graph_root;
    ll_t *graph_node_list;
    ll_t *spf_run_result[MAX_LEVEL];
} graph_t;

node_t *
create_new_node(graph_t *graph, char *node_name, AREA area);


edge_t *
create_new_edge(char *from_ifname, 
                char *to_ifname, 
                unsigned int metric, 
                char *from_prefix, 
                char *to_prefix,
                LEVEL level);

void
insert_edge_between_2_nodes(edge_t *edge, 
                            node_t *from_node, 
                            node_t *to_node, DIRECTION dirn);

void
set_graph_root(graph_t *graph, node_t *root);

graph_t *
get_new_graph();

void
dump_nbrs(node_t *node, LEVEL level);

void
dump_node_info(node_t *node);

void 
dump_edge_info(edge_t *edge);

void
mark_node_pseudonode(node_t *node, LEVEL level);
/* Macros */

/*Iterate over nbrs of a given node*/

#define GET_EGDE_PTR_FROM_FROM_EDGE_END(edge_end_ptr)   \
    (edge_t *)((char *)edge_end_ptr - (unsigned int)&(((edge_t *)0)->from))

#define GET_EGDE_PTR_FROM_TO_EDGE_END(edge_end_ptr)     \
    (edge_t *)((char *)edge_end_ptr - (unsigned int)&(((edge_t *)0)->to))

static inline edge_t *
GET_EGDE_PTR_FROM_EDGE_END(edge_end_t *edge_end){
    
    edge_t *edge = NULL;
    if(edge_end->dirn == OUTGOING)
        edge = GET_EGDE_PTR_FROM_FROM_EDGE_END(edge_end);
    else if(edge_end->dirn == INCOMING)
        edge = GET_EGDE_PTR_FROM_TO_EDGE_END(edge_end);
    else
        assert(0);

    return edge;
}

static inline LEVEL
GET_EDGE_END_LEVEL(edge_end_t *edge_end){
    
    edge_t *edge = GET_EGDE_PTR_FROM_EDGE_END(edge_end);
    return edge->level;
}

/* The function test whether the given edge
 * is outgoing edge or incoming edge wrt to a goven node*/
static inline EDGE_END_DIRN
get_edge_direction(node_t *node, edge_t *edge){

    if(edge->from.node == node && edge->from.dirn == OUTGOING)
        return OUTGOING;
    else if(edge->to.node == node && edge->to.dirn == INCOMING)
        return INCOMING;

    return EDGE_END_DIRN_UNKNOWN;
}


#define ITERATE_NODE_NBRS_BEGIN(_node, _nbr_node, _edge, _level)  \
    _nbr_node = NULL;                                             \
    _edge = NULL;                                                 \
    do{                                                           \
        unsigned int _i = 0;                                      \
        edge_end_t *_edge_end = 0;                                \
        for(;_i < MAX_NODE_INTF_SLOTS; _i++){                     \
            _edge_end = _node->edges[_i];                         \
            if(!_edge_end) break;                                 \
            if(_edge_end->dirn != OUTGOING)                       \
                continue;                                         \
            _edge = GET_EGDE_PTR_FROM_FROM_EDGE_END(_edge_end);   \
            if(!IS_LEVEL_SET(_edge->level, _level))               \
                continue;                                         \
            _nbr_node = _edge->to.node;
             
#define ITERATE_NODE_NBRS_END   }}while(0)      
   
int
is_two_way_nbrship(node_t *node, node_t *node_nbr, LEVEL level);


#endif /* __GRAPH__ */


