/*
 * =====================================================================================
 *
 *       Filename:  instanceconst.h
 *
 *    Description:  This file contains Graph Constants only
 *
 *        Version:  1.0
 *        Created:  Thursday 24 August 2017 01:51:01  IST
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

#ifndef __GRAPHCONST__
#define __GRAPHCONST__

#include <assert.h>

#define IF_NAME_SIZE            16
#define NODE_NAME_SIZE          16
#define MAX_NODE_INTF_SLOTS     20
#define PREFIX_LEN              15
#define PREFIX_LEN_WITH_MASK    (PREFIX_LEN + 3)
#define MAX_NXT_HOPS            16
#define INFINITE_METRIC         (0xFE000000)
#define LINK_DEFAULT_METRIC     10
#define DEFAULT_LINK_BW         1 /*1GIG*/
#define STRING_REASON_LEN       256
#define MPLS_STACK_OP_LIMIT_MAX 8
#define TOPOLOGY_NAME_SIZE      32
#define MAC_LEN                 6
#define ZERO_IP                 "0.0.0.0"
#define NH_NAME_SIZE            32
#define TILFA_MAX_SEGMENTS      8
/* instance global flags */

#define IGNOREATTACHED  1   /*If this bit is set, then L1-only router will not install default gateway to L1L2 router of the local Area*/
#define OVERLOAD_BIT    2   /*the 2nd least significant bit represents the status of router wrt to overload in node_t->attributes (count from zero)*/

typedef char FLAG;
typedef char BYTE;
typedef unsigned int mpls_label_t;

#define NO_TAG  0xFFFFFFFF

/*Edge properties*/
typedef enum{
    UNIDIRECTIONAL,
    BIDIRECTIONAL,
    DIRECTION_UNKNOWN
} DIRECTION;

typedef enum{
    OUTGOING,
    INCOMING,
    EDGE_END_DIRN_UNKNOWN
} EDGE_END_DIRN;

typedef enum{
    LEVEL_UNKNOWN,
    LEVEL1,
    LEVEL2,
    MAX_LEVEL,
    LEVEL12 = LEVEL1 | LEVEL2
} LEVEL;

static inline LEVEL 
get_other_level(LEVEL level){

    LEVEL l;
    assert(level == LEVEL1 || level == LEVEL2);
    l = (level == LEVEL1) ? LEVEL2 : LEVEL1;
    return l;
}

typedef enum{
    NON_PSEUDONODE,
    PSEUDONODE
} NODE_TYPE;

/*Let The routers can have any area 
 * among these*/

typedef enum{
    AREA1,
    AREA2,
    AREA3,
    AREA4,
    AREA5,
    AREA6,
    AREA_UNKNOWN
} AREA;

typedef enum nh_type{

    IPNH,
    LSPNH,
    NH_MAX
} nh_type_t;

#define ITERATE_NH_TYPE_BEGIN(_nh)     \
    for(_nh = IPNH; _nh < NH_MAX; _nh++)
   
#define ITERATE_NH_TYPE_END

typedef enum{
    FALSE,
    TRUE,
    DONT_KNOW
} boolean;

typedef enum{

    /* when protected link is P2P*/
    NO_LFA,
    LINK_PROTECTION_LFA,            /*Ineq 1 only*/
    LINK_PROTECTION_LFA_DOWNSTREAM, /*Ineq 1 and 2 only*/
    LINK_AND_NODE_PROTECTION_LFA,   /*Ineq 1 2 and 3 only*/

    /*When protected Link is Broadcast Link*/
    BROADCAST_LINK_PROTECTION_LFA,           /*Ineq 1 and 4*/
    BROADCAST_LINK_PROTECTION_LFA_DOWNSTREAM,/*Ineq 1 2 and 4*/
    BROADCAST_ONLY_NODE_PROTECTION_LFA,  /*Ineq 1 2 and 3 and N is reachable from S through protected link, this LFA is not loop free wrt to LAN segment it can pump the traffic back into LAN segment*/
    BROADCAST_LINK_AND_NODE_PROTECTION_LFA,/*(Ineq 1 2 and 3 and N is reachable from S from link other than protected link) OR (Ineq 1 2 3 4)*/

    /*RLFA cases*/
    LINK_PROTECTION_RLFA,
    LINK_PROTECTION_RLFA_DOWNSTREAM,
    LINK_AND_NODE_PROTECTION_RLFA,

    /*When protected Link is Broadcast Link*/
    BROADCAST_LINK_PROTECTION_RLFA,
    BROADCAST_LINK_PROTECTION_RLFA_DOWNSTREAM,
    BROADCAST_LINK_AND_NODE_PROTECTION_RLFA,
    BROADCAST_NODE_PROTECTION_RLFA,

    /*Per prefix LFAs*/
    PER_PREFIX_LINK_PROTECTION_LFA,
    PER_PREFIX_LINK_AND_NODE_PROTECTION_LFA,
    BROADCAST_PER_PREFIX_LINK_PROTECTION_LFA,
    BROADCAST_PER_PREFIX_LINK_AND_NODE_PROTECTION_LFA,

    TILFA,
    UNKNOWN_LFA_TYPE
} lfa_type_t;

typedef enum{

    BACKUP_REQ_UNKNOWN,
    /*SET when Destination node do not need any backups by virtue of independant ECMP*/
    NO_BACKUP_REQUIRED,
    /*SET by default*/
    BACKUPS_REQUIRED,
    /*SET when Destination has ECMP*/
    NO_LINK_PROTECTING_BACKUPS_REQUIRED
} node_backup_req_t;

/*To reach a certain destination, it is possible
 * that more than one type of routes are available.
 * For example, to reach a destination A.B.C.D, IP nexthop,
 * spring nexthop, ldp nexthop or rsvp nexthop may be
 * available on the router. This enums defines the preference
 * order to select one out of many alternatives. Change this
 * prefrence order in enum to change nexthop selection order.
 * */

typedef enum{

    ROUTE_TYPE_MIN,
    SPRING_ROUTE,
    LDP_ROUTE,
    RSVP_ROUTE,
    IP_ROUTE,
    ROUTE_TYPE_MAX
} trace_route_pref_order_t;

typedef enum{

    RT_PROTO_IGP,
    RT_PROTO_RSVP,
    RT_PROTO_LDP,
    RT_PROTO_LIGP,
    RT_PROTO_SRTE
} RT_PROTO;

typedef enum{
    IGP_PROTO,   /*nexthop installed by IGP*/
    LDP_PROTO,   /*nexthop installed by LDP*/
    L_IGP_PROTO, /*nexthop installed by SPRING*/
    RSVP_PROTO,  /*nexthop installed by RSVP*/
    UNKNOWN_PROTO
} PROTOCOL;

static inline char *
protocol_name(PROTOCOL proto){
    switch(proto){
        case IGP_PROTO:
            return "IGP_PROTO";
        case LDP_PROTO:
            return "LDP_PROTO";
        case L_IGP_PROTO:
            return "L_IGP_PROTO";
        case RSVP_PROTO:
            return "RSVP_PROTO";
        default:
            assert(0);
    }
}


#endif /* __GRAPHCONST__ */
