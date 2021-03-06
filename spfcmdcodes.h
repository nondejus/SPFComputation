/*
 * =====================================================================================
 *
 *       Filename:  spfdcm.h
 *
 *    Description:  This file declares the structures and conmmand code constants for spf clis
 *
 *        Version:  1.0
 *        Created:  Sunday 03 September 2017 10:32:31  IST
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

#ifndef __SPFCMDCODES__H
#define __SPFCMDCODES__H

/*CLI command codes*/

#define CMDCODE_CONFIG_NODE_SLOT_ENABLE                     1   /*config node <node-name> [no] interface <slot-name> enable*/
#define CMDCODE_CONFIG_INSTANCE_IGNOREBIT_ENABLE            2   /*config node <node-name> [no] ignorebit enable*/
#define CMDCODE_CONFIG_INSTANCE_ATTACHBIT_ENABLE            3   /*config node <node-name [no] attachbit enable>*/
#define CMDCODE_SHOW_SPF_STATS                              4   /*show spf run level <level-no> [root <node-name> [statistics]]*/
#define CMDCODE_SHOW_SPF_RUN                                5   /*show spf run level <level-no> [root <node-name>]*/
#define CMDCODE_SHOW_SPF_RUN_INVERSE                        6   /*show spf run level <level-no> [root <node-name>] inverse*/
#define CMDCODE_SHOW_INSTANCE_LEVEL                         11  /*show instance level <level-no>*/
#define CMDCODE_SHOW_INSTANCE_NODE_LEVEL                    12  /*show instance node <node-name> level <level-no>*/
#define CMDCODE_CONFIG_NODE_EXPORT_PREFIX                   13  /*show instance node <node-name> export prefix <prefix> <mask> level <level-no>*/
#define CMDCODE_CONFIG_NODE_LEAK_PREFIX                     14  /*config node <node-name> leak prefix <prefix> mask level <from level-no> <To level-no>*/
#define CMDCODE_CONFIG_NODE_OVERLOAD                        15  /*config node <node-name> overload level <level-no>*/
#define CMDCODE_DEBUG_INSTANCE_NODE_ALL_ROUTES              16  /*debug instance node <node-name> route*/
#define CMDCODE_DEBUG_INSTANCE_NODE_ROUTE                   17  /*debug instance node <node-name> route <prefix> <mask>*/
#define CMDCODE_DEBUG_INSTANCE_NODE_ALL_SPRING_ROUTES       74  /*debug instance node <node-name> route*/
#define CMDCODE_DEBUG_INSTANCE_NODE_SPRING_ROUTE            75  /*debug instance node <node-name> route <prefix> <mask>*/
#define CMDCODE_CONFIG_NODE_OVERLOAD_STUBNW                 18  /*config node <node-name> overload level <level-no> interface <slot-no> */
#define CMFCODE_CONFIG_NODE_SLOT_METRIC_CHANGE              19  /*config node <node-name> interface <intf-name> level <level-no> metric <metric value>*/
#define CMDCODE_SHOW_SPF_RUN_PRC                            20  /*show spf run level <level-no>* [root <node-name>] prc*/
#define CMDCODE_CONFIG_NODE_RSVPLSP                         21  /*config node <node-name> lsp <lsp-name> metric <metric-value> to <tail-end-ip>*/
#define CMDCODE_RUN_INSTANCE_SYNC                           22  /*run instance sync*/
/*Administrative tag support*/
#define CMDCODE_CONFIG_INSTANCE_NODE_TAG_PRIORITY           23  /*config node <node-name> tag <tag-value> priority <high | medium | low>*/
#define CMDCODE_CONFIG_NODE_SLOT_TAG_VALUE                  24  /*config node <node-name> [no] interface <slot-name> tag <tag-value>*/

/*LFAs*/
#define CMDCODE_CONFIG_INTF_LINK_PROTECTION                 25 /*config node <node-name> interface <ifname> link-protection*/ 
#define CMDCODE_CONFIG_INTF_NODE_LINK_PROTECTION            26 /*config node <node-name> interface <ifname> node-link-protection*/
#define CMDCODE_CONFIG_NODE_REMOTE_BACKUP_CALCULATION       27 /*config node <node-name> backup-spf-options remote-backup-calculation*/
#define CMDCODE_CONFIG_NODE_BACKUP_PER_PREFIX_CALCULATION   76 /*config node <node-name> backup-spf-options per-prefix-calculation*/ 
#define CMDCODE_CONFIG_INTF_NO_ELIGIBLE_BACKUP              28 /*config node <node-name> interface <ifname> no-eligible-backup */ 
#define CMDCODE_CONFIG_RSVPLSP_AS_BACKUP                    29 /*config node <node-name> lsp <lsp-name> backup to <tail-end ip address>*/
#define CMDCODE_CONFIG_ALL_INTERFACE_LINK_PROTECTION        30 /*config node <node-name> interface all link-protection*/
#define CMDCODE_CONFIG_ALL_INTERFACE_NODE_LINK_PROTECTION   31 /*config node <node-name> interface all node-link-protection*/
#define CMDCODE_CONFIG_NODE_SPF_BACKUP_OPTIONS              32 /*config node <node-name> spf-backup-options */
#define CMDCODE_CONFIG_INTF_LINK_PROTECTION_RLFA            33 /*config node <node-name> interface <ifname> link-protection rlfa*/ 
#define CMDCODE_SHOW_NODE_TRACEROUTE_PRIMARY                34 /*show instance node <node-name> traceroute <destination prefix>*/
//#define CMDCODE_SHOW_NODE_TRACEROUTE_BACKUP                 35 /*show instance node <node-name> traceroute <destination prefix> backup*/
#define CMDCODE_DEBUG_SHOW_NODE_INTF_EXPSPACE               36 /*debug show instance node <node-name> interface <slot-no> expspace*/
#define CMDCODE_DEBUG_SHOW_NODE_INTF_PQSPACE                37 /*debug show instance node <node-name> interface <slot-no> pqspace*/

#define CMDCODE_SHOW_SPF_RUN_INIT                           38 /*show spf run level <level-no> [root <node-name>] init*/
#define CMDCODE_DEBUG_SHOW_IMPACTED_DESTINATIONS            39 /*debug show instance node <node-name> interface <slot-no> impacted destination*/

#define CMDCODE_CONFIG_NODE_LINK_DEGRADATION                40 /*config node <node-name> backup-spf-options node-link-degradation*/
#define CMDCODE_SHOW_BACKUP_SPF_RESULTS                     41 /*show instance node <node-name> backup-spf-results*/

#define CMDCODE_DEBUG_TRACEOPTIONS_DIJKASTRA                42 /*conf debug set trace dijkastra*/
#define CMDCODE_DEBUG_TRACEOPTIONS_ROUTE_INSTALLATION       43 /*conf debug set trace route-installation*/
#define CMDCODE_DEBUG_TRACEOPTIONS_ROUTE_CALCULATION        44 /*conf debug set trace route-calculation*/
//#define CMDCODE_DEBUG_TRACEOPTIONS_LFA                    45 /*conf debug set trace lfa*/
#define CMDCODE_DEBUG_TRACEOPTIONS_BACKUPS                  46 /*conf debug set trace backup*/
#define CMDCODE_DEBUG_TRACEOPTIONS_ALL                      47 /*conf debug set trace all*/
#define CMDCODE_DEBUG_TRACEOPTIONS_PREFIXES                 48 /*conf debug set trace prefixes*/
#define CMDCODE_DEBUG_TRACEOPTIONS_ROUTING_TABLE            49 /*conf debug set trace routing-table*/
#define CMDCODE_DEBUG_TRACEOPTIONS_CONFLICT_RESOLUTION      50 /*conf debug set trace conflict-resolution*/
#define CMDCODE_DEBUG_LOG_ENABLE_DISABLE                    51 /*debug log enable|disable*/
#define CMDCODE_DEBUG_LOG_FILE_ENABLE_DISABLE               52 /*debug log enable|disable file*/

/*Segment routing*/

#define CMDCODE_CONFIG_NODE_SEGMENT_ROUTING_ENABLE          53 /*config node <node-name> source-packet-routing*/
#define CMDCODE_CONFIG_NODE_SR_SRGB_RANGE                   54 /*config node <node-name> source-packet-routing index-range <index-range>*/
#define CMDCODE_CONFIG_NODE_SR_SRGB_START_LABEL             72 /*config node <node-name> source-packet-routing srgb start-label <label value>*/
#define CMDCODE_DEBUG_SHOW_PREFIX_CONFLICT_RESULT           55 /*debug show instance node <node-name> level <level-no> prefix-conflict-result*/
#define CMDCODE_DEBUG_SHOW_PREFIX_SID_CONFLICT_RESULT       56 /*debug show instance node <node-name> level <level-no> sid-prefix-conflict-result*/
#define CMDCODE_CONFIG_NODE_SR_PREFIX_SID                   57 /*config node <node-name> source-packet-routing node-segment <node-sid-value>*/
#define CMDCODE_CONFIG_NODE_SPRING_BACKUPS                  70 /*config node <node-name> backup-spf-options use-source-packet-routing*/
#define CMDCODE_CONFIG_NODE_SR_PREFIX_SID_INTF              58 /*config node <node-name> interface <if-name> prefix-sid <prefix-sid>*/
//#define CMDCODE_CONFIG_NODE_SR_ADJ_SID                      59 /*config node <node-name> interface <if-name> adjacency-sid <adj-sid>*/
#define CMDCODE_SHOW_NODE_SPRING                            60 /*show instance node <node-name> level <level-no> spring*/
//#define CMDCODE_DEBUG_TRACEOPTIONS_MPLS_ROUTE_INSTALLATION  61 /*conf debug set trace mpls-route-installation*/
//#define CMDCODE_CONFIG_NODE_STATIC_INSTALL_MPLS_ROUTE       62 /*conf node <node-name> mpls install route <dst-prefix> <mask>*/
#define CMDCODE_SHOW_NODE_MPLS_FORWARDINNG_TABLE            63 /*show instance node <node-name> mpls forwarding-table*/
//#define CMDCODE_SHOW_NODE_TRACEROUTE_SR_MPLS                64 /*show instance node <node-name> traceroute <prefix> sr-mpls*/
//#define CMDCODE_DEBUG_TRACE_SR_MPLS_SID_STACK               65 /*debug trace mpls stack labels <label1> <label2> <label3> <label4>*/
#define CMDCODE_CONFIG_NODE_SR_EXPLICIT_NULL                71 /*config node <node-name> source-packet-routing explicit-null*/
#define CMDCODE_DEBUG_TRACEOPTIONS_SPRING_ROUTE_CAL         73 /*config debug set trace spring-route-calculation*/

/* Global CLIs*/
#define CMDCODE_NODE_PING                                   66 /*ping <prefix>*/

/* CLEAR Commands*/
#define CMDCODE_CLEAR_NODE_ROUTE_DB                         67 /*clear instance node <node-name> routes*/

/*Routing table commands*/
#define CMDCODE_SHOW_NODE_INTERNAL_ROUTES                   68 /*show instance node <node-name> routes*/
#define CMDCODE_SHOW_NODE_FORWARDING_TABLE                  69 /*show instance node <node-name> inet forwarding-table*/


/*LDP*/
#define CMDCODE_SHOW_NODE_MPLS_LDP_BINDINGS                 77 /*show instance node <node-name> mpls ldp bindings <prefix> <mask>*/
#define CMDCODE_CONFIG_NODE_LDP_SR_PREFERENCE               78 /*config node <node-name> ldp sr preference set*/
#define CMDCODE_CONFIG_NODE_ENABLE_LDP                      79 /*config node <node-name> ldp*/
#define CMDCODE_CONFIG_NODE_LDP_TUNNEL                      89 /*config node <node-name> ldp tunnel <router-id>*/

/*Mapping Server*/
#define CMDCODE_CONFIG_NODE_SRMS_LCL_POLICY                 80 /*config node <node-name> source-packet-routing mapping-server <prefix> <mask> <first-sid-index> range <range>*/
#define CMDCODE_SHOW_NODE_SRMS_LCL_POLICY                   81 /*show instance node <node-name> source-packet-routing mapping-server prefix-sid-map [<prefix> <mask>]*/
#define CMDCODE_SHOW_NODE_SRMS_CLIENT_ACTIVE_POLICY         82 /*show instance node <node-name> source-packet-routing prefix-sid-map active-policy*/
#define CMDCODE_SHOW_NODE_SRMS_CLIENT_BACKUP_POLICY         83 /*show instance node <node-name> source-packet-routing prefix-sid-map backup-policy*/
#define CMDCODE_CONFIG_NODE_SRMS                            84 /*config node <node-name> source-packet-routing prefix-sid-map advertise-local*/ /*Configure the node as SRMS*/ 
#define CMDCODE_CONFIG_NODE_SRMS_CLIENT_DISABLE             85 /*config node <node-name> source-packet-routing prefix-sid-map recieve disable*//*Disable the node as Mapping client*/
#define CMDCODE_SHOW_NODE_INET3_FORWARDING_TABLE            86 /*show instance node <node-name> inet.3 forwarding-table*/

/*RSVP*/
#define CMDCODE_CONFIG_NODE_ENABLE_RSVP                     87 /*config node <node-name> rsvp*/
#define CMDCODE_SHOW_NODE_MPLS_RSVP_BINDINGS                88 /*show instance node <node-name> mpls rsvp bindings <prefix> <mask>*/
#define CMDCODE_CONFIG_NODE_RSVP_TUNNEL                     90 /*config node <node-name> rsvp tunnel <router-id> <tunnel-name>*/
#define CMDCODE_SHOW_NODE_MPLS_RSVP_LSP                     91 /*show instance node <node-name> mpls rsvp*/

/*Complete SPF path trace*/
#define CMDCODE_DEBUG_SHOW_SPF_PATH_TRACE                   92 /*debug show instance node <node-name> level <level-no> pred-db */
#define CMDCODE_SHOW_SPF_PATH_LIST                          93 /*show instance node <node-name> level <level-no> spf-path <dest-node-name>*/
#define CMDCODE_SHOW_SR_TUNNEL                              94 /*show instance node <node-name> sr-tunnel <prefix>*/

/*Create Topology Dynamically*/
/*Config topology */

#define TOPO_CREATE_NODE                                    95 /*config topo node node create <node-name1>*/
#define TOPO_NODE_ASSIGN_LOOPBACK_IP                        96 /*config topo node <node-name1> loopback <ip-address>*/
#define TOPO_NODE_INSERT_LINK                               97 /*config topo node <node-name1> from-if <intf-name1> peer <node-name2> to-if <intf-name2>*/
#define TOPO_NODE_INTF_ASSIGN_IP_ADDRESS                    98 /*config topo node <node-name1> interface <intf-name1> ip <ip-address> <mask>*/
#define TOPO_NODE_INTF_ASSIGN_MAC_ADDRESS                   99 /*config topo node <node-name1> interface <intf-name1> mac <mac-address>*/
#define CONFIG_TOPO                                         100 /*config topo*/

#define CMDCODE_SHOW_INSTANCE_NODE_INTERFACES               101 /*show instance node <node_name> interfaces*/
#define CMDCODE_SHOW_INSTANCE_NODE                          102 /*show instance node <node_name> */

/*traffic engineering*/
#define CMDCODE_CONFIG_NODE_TRAFFIC_ENGINEERING             103 /*config node <node-name> traffic-engineering*/
#define CMDCODE_CONFIG_NODE_TE_IGNORE_LSP_METRICS           104 /*config node <node-name> traffic-engineering ignore-lsp-metric*/
#define CMDCODE_CONFIG_NODE_INET_TE_SHORTCUTS               105 /*config node <node-name> traffic-engineering inet shortcuts*/
#define CMDCODE_CONFIG_NODE_INET_MPLS_TE_SHORTCUTS          106 /*config node <node-name> traffic-engineering inet-mpls shortcuts*/

/*Configuration Adjacency SIDS*/
#define CMDCODE_CONFIG_NODE_INTF_P2P_ADJ_SID_PROTECTED      107 /*config node <node-name> interface <intf-name> level <level-no> ipv4-adjacency-segment <protected|unprotected> label <lebel-no>*/
#define CMDCODE_CONFIG_NODE_INTF_P2P_ADJ_SID_UNPROTECTED    108
#define CMDCODE_CONFIG_NODE_INTF_LAN_ADJ_SID_PROTECTED      109 /*config node <node-name> interface <intf-name> level <level-no> ipv4-adjacency-segment lan-neighbor <router-id> <protected|unprotected> label <lebel-no>*/
#define CMDCODE_CONFIG_NODE_INTF_LAN_ADJ_SID_UNPROTECTED    110
#define CMDCODE_SHOW_NODE_INTF_ADJ_SIDS                     111 /*show instance node <node-name> interface <intf-name> adjacency-sids*/

#define CMDCODE_DEBUG_SHOW_TILFA                            112 /*debug show instance node <node-name> tilfa*/
#define CMDCODE_CONFIG_NODE_TILFA_ENABLE                    113 /*config node <node-name> source-packet-routing use-post-convergence-paths*/
#define CMDCODE_DEBUG_TRACEOPTIONS_TILFA                    114 /*config debug set trace tilfa*/
#endif /* __SPFCMDCODES__H */
