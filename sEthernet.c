/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
/* File Name          : sEthernet.c                                           */
/* Author             : Edoardo Lotti, Paolo Torretta                         */
/* Version            : v1.0.0                                                */
/* Date               : 05/11/2015                                            */
/* Description        : This code includes all the functions related to the   */
/*                      ethernet controller.                                  */
/******************************************************************************/

#include <stdio.h>

#include "sEthernet.h"
#include "sErrors.h"


/******************************************************************************/
/* Code definitions.                                                          */
/******************************************************************************/


/******************************************************************************/
/* Code variables.                                                            */
/******************************************************************************/
Emac           XEmac;
unsigned char  LinkUp,AutoNeg;
unsigned short EthSpeed;
unsigned       MIIModel,MIIRevision,MIIIdent;

unsigned       EthRxPtr,EthRxOff;
unsigned       EthTxPtr,EthTxOff;


/******************************************************************************/
/* External variables.                                                        */
/******************************************************************************/
extern unsigned char  MacAddress[6]; 


/*****************************************************************************/
/* EthernetControllerInit                                                    */
/* This function initializes the Ethernet controller for the proper use.     */
/*****************************************************************************/
int EthernetControllerInit(unsigned char *TmRegArea,unsigned char *BdArea)
{
unsigned addr;
unsigned rc;

EthRxPtr = (unsigned )BdArea;              //Ethernet Rx Frame Buffer.
EthTxPtr = (unsigned )BdArea+0x10000;      //Ethernet Tx Frame Buffer.
EthRxOff = 0;
EthTxOff = 0;

*(unsigned *)(TmRegArea+0x8118) = 0x00800; //Eth Rx management.
*(unsigned *)(TmRegArea+0x8118) = 0x00801; //Eth Rx management.

*(unsigned *)(TmRegArea+0x811C) = 0x00800; //Eth Tx management.
*(unsigned *)(TmRegArea+0x811C) = 0x00801; //Eth Tx management.

return 0;
}


/*****************************************************************************/
/* Ethernet_SendFrame                                                        */
/* This function sends one Ethernet frame.                                   */
/*****************************************************************************/
void Ethernet_SendFrame(unsigned char *data,int len)
{

MemCopyH(EthTxPtr+EthTxOff+4,data,len);
*(unsigned *)(EthTxPtr+EthTxOff) = 0x80000000 | (unsigned )len;

EthTxOff += 0x0800;
if (EthTxOff >= 0x10000)
  EthTxOff = 0;
}


/*****************************************************************************/
/* Ethernet_GetRxFrameManagement                                             */
/* This function gets one Ethernet frame and send this to the TCP/IP stack   */
/* code.                                                                     */
/*****************************************************************************/
void Ethernet_GetRxFrameManagement(void)
{
int      i;
unsigned Descriptor;

do{
  Descriptor = *(unsigned *)(EthRxPtr+EthRxOff);
  printf("\n%08X %08X Descriptor %08X\n",EthRxPtr,EthRxOff,Descriptor);     //edo
  if (Descriptor & 0x80000000){         //This is a valid buffer with a valid
                                        //ethernet packet.
//edo    UartSendString("\n\r",2);                          //edo
//edo    UartSendWord(Descriptor);                          //edo
//edo
//edo    UartSendString(" ",1);                             //edo
//edo    for (i = 14;i < 20;i++)                            //edo
//edo      UartSendByte(*(unsigned char *)(EthRxPtr+i));    //edo
//edo    UartSendString(" ",1);                             //edo
//edo    for (;i < 26;i++)                                  //edo
//edo      UartSendByte(*(unsigned char *)(EthRxPtr+i));    //edo
//edo    for (;i < 38;i++){                                 //edo
//edo      UartSendString(" ",1);                           //edo
//edo      UartSendByte(*(unsigned char *)(EthRxPtr+i));    //edo
//edo      }                                                //edo


    for (i = 0;i < 6;i++)
      if (*(unsigned char *)(EthRxPtr+EthRxOff+6+i) != 0xFF)
        break;
    if (i == 6)                         //Broadcast ethernet packet;
      ProcessBroadcastEthernetPacket(EthRxPtr+EthRxOff+4,Descriptor & 0x1FFF);
    else{                               //Unicast ethernet packet
      ProcessUnicastEthernetPacket(EthRxPtr+EthRxOff+4,Descriptor & 0x1FFF);
      }
                                        //Descriptor invalidation.
    *(unsigned *)(EthRxPtr+EthRxOff) &= 0x7FFFFFFF;

    EthRxOff += 0x0800;
    if (EthRxOff >= 0x10000)
      EthRxOff = 0;
    }
  else
    return;
  } while (1);
}


/*****************************************************************************/
/* Ethernet_SentTxFrameManagement                                            */
/* This function manages the occurrence of the interrupt on Tx Frame sent.   */
/*****************************************************************************/
void Ethernet_SentTxFrameManagement(void)
{
Emac_Bd  *txBd;
unsigned bdCntrl;

txBd = XEmac.TxBdRing.Tail;
bdCntrl = txBd->Control;

while (bdCntrl & 0x80000000){           /* This is a valid Buffer descriptor  */
  if (bdCntrl & 0x08000000)
    XEmac.TxFrameCorrupted++;

  txBd->Control &= 0x40000000;

  if (bdCntrl & 0x40000000)
    XEmac.TxBdRing.Tail = XEmac.TxBdRing.First;
  else
    XEmac.TxBdRing.Tail++;

  txBd = XEmac.TxBdRing.Tail;
  bdCntrl = txBd->Control;
  }
}


/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
