/******************** (C) COPYRIGHT 2012 Seletech srl *************************/
/* File Name          : sError.h                                              */
/* Author             : Edoardo Lotti, Paolo Torretta                         */
/* Version            : v1.0.0                                                */
/* Date               : 05/11/2015                                            */
/* Description        : This file includes all the error definitions.         */
/******************************************************************************/

/******************************************************************************/
/* Code definitions.                                                          */
/******************************************************************************/
#define ERROR_SYNTAX            0x80
#define ERROR_STRESS_TEST       0x81
#define ERROR_VPD_CHECKSUM      0x82

#define ERROR_EMAC_NOTREADY     0x100
#define ERROR_EMAC_PARAMETER    0x103
#define ERROR_EMAC_SPEED        0x104
#define ERROR_EMAC_PHY_BUSY     0x105
#define ERROR_MAC_MDIODIV       0x106

#define ERROR_PCAP_INIT         0x200
#define ERROR_PCAP_CLEAR        0x201
#define ERROR_PCAP_TRANSFER     0x202
#define ERROR_PCAP_TRANSFER_TO  0x203
#define ERROR_PCAP_CONFIG_TO    0x204
#define ERROR_PCAP_ERROR        0x205
#define ERROR_PCAP_DEVICE_BUSY  0x206
#define ERROR_PCAP_PARAM        0x207
#define ERROR_DEVICE_STARTED    0x208

#define ERROR_IMAGE_FAIL        0x300
#define ERROR_IMAGE_CHECKSUM    0x301
#define ERROR_IMAGE_MOVE        0x302

#define ERROR_TCP_SEND_CONN     0x600


/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
