/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
/* File Name          : sTcpip.c                                              */
/* Author             : Edoardo Lotti, Paolo Torretta                         */
/* Version            : v1.0.0                                                */
/* Date               : 05/11/2015                                            */
/* Description        : This code includes all the functions related to the   */
/*                      TCP/IP protocol stack.                                */
/******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "sTcpip.h"
#include "sEthernet.h"
#include "sErrors.h"

#define ARP_REQUEST             0x0100
#define ARP_REPLY               0x0200

#define ICMP_PROTOCOL           1
#define TCP_PROTOCOL            6
#define UDP_PROTOCOL            11


/******************************************************************************/
/* Global variables.                                                          */
/******************************************************************************/
int            EthBuffLen;
unsigned short CurPackId;        // ID for multi-fragment packets
unsigned       BytesReceived;    // multi-fragment byte counter
unsigned       BytesToRec;       // total multi-fragment byte length
unsigned       EthReqIpAddr;
unsigned char  EthReqMacAddr[6];
unsigned char  *EthBuff;         // point to static buffer

unsigned short PacketId=0;              // keep track of packet S/N
unsigned char  UDPheader[8];            // holds UDP header

/*----------------------------------------------------------------------------*/
/*  The IP and MAC addresses are set-up in their respective global variables  */
/*  during Ethernet initialization (values are read from VPD area in flash)   */
/*----------------------------------------------------------------------------*/
unsigned       IpAddress=0x3200A8C0;    // IP address is stored in VPD
                                        // MAC address is stored in VPD
unsigned char  MacAddress[6]={0x00,0x01,0x02,0x03,0x04,0x06}; 
unsigned char  BroadcastAddress[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
unsigned short UDPport;

/*----------------------------------------------------------------------------*/
/* Connection messages.                                                       */
/*----------------------------------------------------------------------------*/
const unsigned char  WelcomeMsg[]="\n\rCoaXpress Camera\n\r$>\0";
const unsigned short WelcomeMsgLen=22;

/*----------------------------------------------------------------------------*/
/* TCP connection data structures.                                            */
/*----------------------------------------------------------------------------*/
TCPIP_conn     TCPIP_Conn[2];
unsigned short TcpSeqNumber;
unsigned       NextSeqNumber;
unsigned short ConnectionTimeOut;

/******************************************************************************/
/* External variables.                                                        */
/******************************************************************************/
extern Emac    XEmac;


/******************************************************************************/
void MemCopyW(void *dest,void *src,int len)
{
memcpy((unsigned char *)dest,(unsigned char *)src,len);
}


/******************************************************************************/
void MemCopyH(void *dest,void *src,int len)
{
memcpy((unsigned char *)dest,(unsigned char *)src,len);
}


/******************************************************************************/
unsigned SwapWord(unsigned w)
{
return (w >> 24)|((w >> 8) & 0xFF00)|((w << 8) & 0xFF0000)|(w << 24);
}


/******************************************************************************/
unsigned short SwapHalf(unsigned short h)
{
return (h >> 8)|(h << 8);
}


/******************************************************************************/
unsigned short IpCheckSum(unsigned short *ptr,int len)
{
unsigned IpChecksum=0;

while (len > 0){
  IpChecksum += *ptr++;
  len -= 2;
  }

IpChecksum = (IpChecksum & 0xFFFF) + (IpChecksum >> 16);
if (IpChecksum & 0xFFFF0000)
  IpChecksum = (IpChecksum & 0xFFFF) + (IpChecksum >> 16);

return IpChecksum ^ 0xFFFF;
}


/******************************************************************************/
unsigned TcpCheckSum(unsigned TcpChecksum,unsigned short *ptr,int len)
{
while (len > 0){
  TcpChecksum += *ptr++;
  len -= 2;
  }

return TcpChecksum;
}


/******************************************************************************/
void InterruptEnable(void)
{

return;
}


/******************************************************************************/
void InterruptDisable(void)
{

return;
}


/******************************************************************************/
/* ICMP_Replay function.                                                      */
/* This function is able to replay at the ICMP request frames from a remote   */
/* host system.                                                               */
/* The MAC source address is inserted automatically by the Ethernet control-  */
/* ler.                                                                       */
/******************************************************************************/
void ICMP_Replay(struct icmp_packet *pck)
{
unsigned short len;
ICMP_packet    IcmpPck;

printf(".");            //edo

len = SwapHalf(pck->TotalLen);
MemCopyH(IcmpPck.Mac0,pck->Mac1,6);
MemCopyH(IcmpPck.Mac1,pck->Mac0,6);
MemCopyH((unsigned char *)&IcmpPck.Type,(unsigned char *)&pck->Type,14);
IcmpPck.ProtSrc = pck->ProtDest;
IcmpPck.ProtDest = pck->ProtSrc;
if (len > 60){
  len = 60;
  IcmpPck.TotalLen = SwapHalf(len);
  }
IcmpPck.IcmpType = 0;
IcmpPck.IcmpCode = 0;
IcmpPck.Checksum = 0;
memcpy((unsigned char *)&IcmpPck.Identifier,(unsigned char *)&pck->Identifier,
                                                                        len-24);
//edo IcmpPck.Checksum =
//edo                SwapHalf(IpCheckSum((unsigned short *)&IcmpPck.IcmpType,len-20));
IcmpPck.Checksum = IpCheckSum((unsigned short *)&IcmpPck.IcmpType,len-20);
printf(".");            //edo

Ethernet_SendFrame(IcmpPck.Mac0,len+14);
}


/******************************************************************************/
/* ARP_Replay function.                                                       */
/* This function is able to replay at the ARP request frames from a remote    */
/* host system.                                                               */
/* The MAC source address is inserted automatically by the Ethernet control-  */
/* ler.                                                                       */
/******************************************************************************/
void ARP_Replay(struct arp_packet *pck)
{
ARP_packet ArpPck;

printf(":");     //edo
                                        // check other ARP header values
if ((pck->HwType != 0x0100)||(pck->ProtType != 0x0008)||
                           (pck->HpLen != 0x0406)||(pck->OpCode != ARP_REQUEST))
  return;

printf(":",1);                  //edo
printf("%02X",*(unsigned char *)&pck->DstIp[0]);  //edo
printf("%02X",*(unsigned char *)&pck->DstIp[1]);  //edo
printf("%02X",*(unsigned char *)&pck->DstIp[2]);  //edo
printf("%02X",*(unsigned char *)&pck->DstIp[3]);  //edo

if (memcmp(pck->DstIp,(unsigned char *)&IpAddress,4) != 0)
  return;

MemCopyH(ArpPck.Mac0,pck->SrcMac,6);
MemCopyH(ArpPck.Mac1,MacAddress,6);
ArpPck.Type = pck->Type;
ArpPck.HwType = pck->HwType;
ArpPck.ProtType = pck->ProtType;
ArpPck.HpLen = pck->HpLen;
ArpPck.OpCode = ARP_REPLY;
MemCopyH(ArpPck.SrcMac,MacAddress,6);
MemCopyH(ArpPck.SrcIp,pck->DstIp,4);
MemCopyH(ArpPck.DstMac,pck->SrcMac,10); /* Dest Mac and IP copied together.   */
                                        /* reply to ARP packet.               */
printf(":");     //edo
Ethernet_SendFrame(ArpPck.Mac0,42);
}


/******************************************************************************/
/* TcpIpInit                                                                  */
/* This function initializes the main variables related the TcpIp protocol    */
/* stack with the VPD data.                                                   */
/******************************************************************************/
void TcpIpInit(unsigned TcpTimeout,unsigned IpAddr,unsigned char *MacAddr)
{                                       //Connection timeout is 30 seconds.
ConnectionTimeOut = TcpTimeout;
                                        //IP Address initialization.
memcpy((char *)&IpAddress,(char *)&IpAddr,4);
                                        //MAC Address initialization.
memcpy(MacAddress,MacAddr,6);
}


/******************************************************************************/
/* TCP_SendData function.                                                     */
/* This function sends data to an host system.                                */
/* TCP Checksum is calculated by the Hardware.                                */
/******************************************************************************/
int TCP_SendData(unsigned char cId,unsigned char *pdata,unsigned short len)
{
unsigned   TCPchksum;
TCP_packet TCP_Pck;
TCPIP_conn *tcpConn;

if (TCPIP_Conn[cId].ConnStatus == 0)
  return ERROR_TCP_SEND_CONN;

tcpConn = &TCPIP_Conn[cId];

MemCopyH(TCP_Pck.Mac0,tcpConn->Mac,6);
MemCopyH(TCP_Pck.Mac1,MacAddress,6);
TCP_Pck.Type = 0x0008;
TCP_Pck.IpVer = 0x45;
TCP_Pck.ServiceFields = 0;
TCP_Pck.Identification = SwapHalf(++TcpSeqNumber);
TCP_Pck.Flags = 0x40;
TCP_Pck.FragOff = 0;
TCP_Pck.TimeToLive = 0x80;
TCP_Pck.Protocol = 0x06;
TCP_Pck.HeaderChecksum = 0;

TCP_Pck.ProtSrc = IpAddress;
TCP_Pck.ProtDest = tcpConn->ProtSrc;
TCP_Pck.PortSrc = tcpConn->PortDest;
TCP_Pck.PortDest = tcpConn->PortSrc;
                                        // TCP Header to 20 bytes.
TCP_Pck.HeaderLen = 0x50;
TCP_Pck.Checksum = 0;
TCP_Pck.Options[0] = 0;
TCP_Pck.Options[1] = 0;
TCP_Pck.WindowSize = SwapHalf(48000);   //was 32000;

TCP_Pck.TcpOption = 0x18;

memcpy(&TCP_Pck.Options[2],pdata,len);
if ((len & 1) != 0)                     //In case the number of bytes is odd we
  TCP_Pck.Options[2+len] = 0;           //need to add 1 byte to 0.
  
TCP_Pck.TcpSeqNumber = SwapWord(tcpConn->SeqNumber);
TCP_Pck.AckSeqNumber = SwapWord(tcpConn->AckSeqNumber);

tcpConn->SeqNumber += len;
len += 0x28;

TCP_Pck.TotalLen = SwapHalf(len);

/*----------------------------------------------------------------------------*/
/* Checksum calculation.                                                      */
/*----------------------------------------------------------------------------*/
                                        // IP checksum.
TCP_Pck.HeaderChecksum =
                      SwapHalf(IpCheckSum((unsigned short *)&TCP_Pck.IpVer,20));
                                        // TCP checksum.
TCPchksum = TcpCheckSum(0,&TCP_Pck.PortSrc,len-20);
TCPchksum = TcpCheckSum(TCPchksum,(unsigned short *)&TCP_Pck.ProtSrc,8);
TCPchksum += (6 + len - 20);
TCP_Pck.Checksum = SwapHalf((unsigned short )~(TCPchksum + (TCPchksum >> 16)));

InterruptDisable();                     /* Disable interrupts.                */
Ethernet_SendFrame(TCP_Pck.Mac0,len+14);
InterruptEnable();                      /* Enable interrupts.                 */
return 0;
}


/******************************************************************************/
/* TCP_GetData function.                                                      */
/* This function gets data from the TCP connection buffer.                    */
/******************************************************************************/
void TCP_GetData(unsigned char *cId,unsigned char *pdata,unsigned short *num)
{
unsigned short tmp;
TCPIP_conn     *tcpConn;

if (*num == 0)                          /* No space in the data buffer.       */
  return;
  
if ((TCPIP_Conn[0].ConnStatus != 0)&&(TCPIP_Conn[0].NumBytes != 0))
  *cId = 0;
else
  if ((TCPIP_Conn[1].ConnStatus != 0)&&(TCPIP_Conn[1].NumBytes != 0))
    *cId = 1;
else{
  *num = 0;
  return;
  }

tcpConn = &TCPIP_Conn[*cId];

if (tcpConn->NumBytes <= *num)
  *num = tcpConn->NumBytes;

tmp = *num;
if (tcpConn->TailPtr + tmp > TCP_BUFF_SIZE){
  tmp = (unsigned short )(TCP_BUFF_SIZE - tcpConn->TailPtr);
  memcpy(pdata,tcpConn->buffer+tcpConn->TailPtr,tmp);
  tcpConn->TailPtr = 0;
  pdata += tmp;
  tmp = (unsigned short )(*num - tmp);
  }
memcpy(pdata,tcpConn->buffer+tcpConn->TailPtr,tmp);
tcpConn->TailPtr = (tcpConn->TailPtr + tmp) & TCP_BUFF_SIZE_MASK;

InterruptDisable();                     /* Disable interrupts.                */
tcpConn->NumBytes -= *num;
InterruptEnable();                      /* Enable interrupts.                 */
}


/******************************************************************************/
/* TCP_AddToBuffer function.                                                  */
/* This function manages data received by the Host system.                    */
/******************************************************************************/
void TCP_AddToBuffer(unsigned char cId,unsigned char *pdata,unsigned short num)
{
unsigned short tmp;
TCPIP_conn     *tcpConn;

tcpConn = &TCPIP_Conn[cId];

if (tcpConn->AutoDataSize != 0){
  MemCopyW(tcpConn->AutoDestAddr,pdata,num);
  tcpConn->AutoDestAddr += num;
  if (tcpConn->AutoDataSize < num){
    printf("@");
    pdata += tcpConn->AutoDataSize;
    num -= tcpConn->AutoDataSize;
    tcpConn->AutoDataSize = 0;
    }
  else{
    tcpConn->AutoDataSize -= num;
    num = 0;
    }
  }
if (num > 0){
  if ((tcpConn->NumBytes + num) < TCP_BUFF_SIZE){
    tcpConn->NumBytes += num;
    if (tcpConn->HeadPtr + num > TCP_BUFF_SIZE){
      tmp = (unsigned short )(TCP_BUFF_SIZE - tcpConn->HeadPtr);
      memcpy(tcpConn->buffer+tcpConn->HeadPtr,pdata,tmp);
      tcpConn->HeadPtr = 0;
      pdata += tmp;
      num -= tmp;
      }
    memcpy(tcpConn->buffer+tcpConn->HeadPtr,pdata,num);
    tcpConn->HeadPtr = (tcpConn->HeadPtr + num) & TCP_BUFF_SIZE_MASK;
    }
  }
}


/******************************************************************************/
/* TCP_SheduleLoadData function.                                              */
/* This function schedules a data receive from Host with the printer data.    */
/* The address and the size are provided for data check coherency.            */
/******************************************************************************/
void TCP_SheduleLoadData(unsigned char cId,unsigned address,unsigned size)
{
TCPIP_conn *tcpConn;

tcpConn = &TCPIP_Conn[cId];

tcpConn->AutoDestAddr = address;
tcpConn->AutoDataSize = size;
}


/******************************************************************************/
/* TCP_Management function.                                                   */
/* This function is able to manage the TCP packets.  Of course we cannot ma-  */
/* nage all the TCP packets coming from the network, but only what is rela-   */
/* ted to our specific data transfer functions.                               */
/* TCP and IP Checksum are calculated by the Hardware.                        */
/******************************************************************************/
void TCP_Management(TCP_packet *pck)
{
unsigned char  cId;
unsigned short len,i;
unsigned       rcvAck,rcvSeq;
unsigned       TCPchksum;
TCP_packet     TCP_Rpck;
TCPIP_conn     *tcpConn;

rcvAck = SwapWord(pck->AckSeqNumber);
rcvSeq = SwapWord(pck->TcpSeqNumber);
for (cId = 0;cId < 2;cId++)
  if (TCPIP_Conn[cId].ConnStatus != 0){
    if ((TCPIP_Conn[cId].PortSrc == pck->PortSrc)&&
                                   (TCPIP_Conn[cId].PortDest == pck->PortDest)&&
                                (memcmp(TCPIP_Conn[cId].Mac,pck->Mac1,6) == 0)&&
                                      (TCPIP_Conn[cId].ProtSrc == pck->ProtSrc))
      break;
    }

if (cId < 2)
  tcpConn = &TCPIP_Conn[cId];
else{
  if (TCPIP_Conn[0].ConnStatus == 0)
    cId = 0;
  else
    if (TCPIP_Conn[1].ConnStatus == 0)
      cId = 1;
  else
    return;

  tcpConn = &TCPIP_Conn[cId];
  tcpConn->ProtSrc = pck->ProtSrc;
  if (pck->TcpOption == 0x02){
    tcpConn->ConnStatus = 1;
    MemCopyH(tcpConn->Mac,pck->Mac1,6);
    tcpConn->PortSrc = pck->PortSrc;
    tcpConn->PortDest = pck->PortDest;
    tcpConn->HeadPtr = 0;
    tcpConn->TailPtr = 0;
    tcpConn->NumBytes = 0;
    tcpConn->SeqNumber = rcvAck;
    tcpConn->AckSeqNumber = rcvSeq;
    tcpConn->Protocol = TCP_PROTOCOL;
    tcpConn->TxWindowSize = pck->WindowSize;
    tcpConn->RealTxWindowSize = pck->WindowSize;
    tcpConn->AutoDataSize = 0;
    }
  else
    if (pck->TcpOption != 0x11)
      return;
  }
                                        // Refresh connection Timeout.
tcpConn->ConnectionTimeOut = ConnectionTimeOut;

len = SwapHalf(pck->TotalLen);
                                        // ACK bit received.
if ((pck->TcpOption == 0x10)&&(tcpConn->ConnStatus == 2)){
  if (tcpConn->SeqNumber > rcvAck)
    tcpConn->RealTxWindowSize = pck->WindowSize - (tcpConn->SeqNumber - rcvAck);
  else
    if (tcpConn->SeqNumber < rcvAck)
      tcpConn->RealTxWindowSize = pck->WindowSize - (rcvAck - tcpConn->SeqNumber);
  else
    tcpConn->RealTxWindowSize = pck->WindowSize;

  if (len > 0x28){
    if (tcpConn->AckSeqNumber == rcvSeq){
      rcvSeq += (len - 0x28);
      TCP_AddToBuffer(cId,&pck->Options[2],(unsigned short )(len-0x28));
      if ((tcpConn->AutoDataSize > 0)&&(--tcpConn->AckTimeout >= 0)){
        tcpConn->AckSeqNumber = rcvSeq;
        return;
        }
      else
        tcpConn->AckTimeout = 4;
      }
    else{
      printf("?");
      rcvSeq = tcpConn->AckSeqNumber;
      }
    TCP_Rpck.TcpOption = 0x10;          // Answer with ACK.
    len = 0x28;
    }
  else{
    tcpConn->TxWindowSize = pck->WindowSize;
    if (pck->HeaderLen != 0x50)
      tcpConn->RetrySend = rcvAck;
    else{
      tcpConn->SeqNumberAck = rcvAck;
      }
    return;
    }
  }

if (pck->TcpOption & 0x04){             // Connection Reset received.
  tcpConn->ConnStatus = 0;
  return;
  }

if (pck->TcpOption == 0x02){            // Synchronization bit active.
  TCP_Rpck.TcpOption = 0x12;
  TCP_Rpck.Options[2] = 2;
  TCP_Rpck.Options[3] = 4;
  TCP_Rpck.Data[0] = 0x05;
  TCP_Rpck.Data[1] = 0xB4;
  TCP_Rpck.Data[2] = 1;
  TCP_Rpck.Data[3] = 1;
  TCP_Rpck.Data[4] = 4;
  TCP_Rpck.Data[5] = 2;
  TCP_Rpck.HeaderLen = 0x70;
  len = 0x30;
  rcvSeq++;
  tcpConn->SeqNumber++;
  }

/*----------------------------------------------------------------------------*/
/* In this case we have data from the Host system.                            */
/*----------------------------------------------------------------------------*/
else{
  TCP_Rpck.HeaderLen = 0x50;            // TCP Header to 20 bytes.
  
  if (pck->TcpOption == 0x18){          // Push bit active.
    if (tcpConn->AckSeqNumber == rcvSeq){
      rcvSeq += (len - 0x28);
      TCP_AddToBuffer(cId,&pck->Options[2],(unsigned short )(len-0x28));
      if ((tcpConn->AutoDataSize > 0)&&(--tcpConn->AckTimeout >= 0)){
        tcpConn->AckSeqNumber = rcvSeq;
        return;
        }
      else
        tcpConn->AckTimeout = 4;
      }
    else{
      printf("?");
      rcvSeq = tcpConn->AckSeqNumber;
      }
    TCP_Rpck.TcpOption = 0x10;          // Answer with ACK.
    len = 0x28;
//edo1    if ((TCP_BUFF_SIZE - tcpConn->NumBytes) < 16384)    //edo was 1500
//edo1      TCP_Rpck.WindowSize = SwapHalf(TCP_BUFF_SIZE - tcpConn->NumBytes);
    }
  else
    if (pck->TcpOption == 0x10){        // ACK bit.
      if (tcpConn->ConnStatus != 2){
        tcpConn->ConnStatus = 2;
        TCP_Rpck.TcpOption = 0x18;
        memcpy(&TCP_Rpck.Options[2],(char *)WelcomeMsg,WelcomeMsgLen+1);
        len = 0x28 + WelcomeMsgLen;
        tcpConn->SeqNumberAck = rcvAck;
        tcpConn->SeqNumber += WelcomeMsgLen;
        }
      }
  else
    if (pck->TcpOption == 0x11){        // FIN bit.
      tcpConn->ConnStatus = 0;
      TCP_Rpck.TcpOption = 0x10;
      len = 0x28;
      rcvSeq++;
      }
  }
                                        // Answer packet generation.
MemCopyH(TCP_Rpck.Mac0,pck->Mac1,6);
MemCopyH(TCP_Rpck.Mac1,MacAddress,6);

TCP_Rpck.Type = 0x0008;

/*----------------------------------------------------------------------------*/
/* This operation clear the following packet fields:                          */
/*  - TCP_Rpck.IpVer = 0x45                                                   */
/*  - TCP_Rpck.ServiceFields = 0                                              */
/*----------------------------------------------------------------------------*/
*(unsigned short *)&TCP_Rpck.IpVer = 0x0045;
TCP_Rpck.Identification = SwapHalf(++TcpSeqNumber);

/*----------------------------------------------------------------------------*/
/* This operation clear the following packet fields:                          */
/*  - TCP_Rpck.Flags = 0x40                                                   */
/*  - TCP_Rpck.FragOff = 0                                                    */
/*----------------------------------------------------------------------------*/
*(unsigned short *)&TCP_Rpck.Flags = 0x0040;

/*----------------------------------------------------------------------------*/
/* This operation clear the following packet fields:                          */
/*  - TCP_Rpck.TimeToLive = 0x80                                              */
/*  - TCP_Rpck.Protocol = 0x06                                                */
/*  - TCP_Rpck.HeaderChecksum = 0                                             */
/*----------------------------------------------------------------------------*/
*(unsigned *)&TCP_Rpck.TimeToLive = 0x00000680;

/*----------------------------------------------------------------------------*/
/* This operation clear the following packet fields:                          */
/*  - TCP_Rpck.Checksum = 0                                                   */
/*  - TCP_Rpck.Options[0] = 0                                                 */
/*  - TCP_Rpck.Options[1] = 0                                                 */
/*----------------------------------------------------------------------------*/
*(unsigned *)&TCP_Rpck.Checksum = 0;

TCP_Rpck.ProtSrc = IpAddress;
TCP_Rpck.ProtDest = tcpConn->ProtSrc;
TCP_Rpck.PortSrc = tcpConn->PortDest;
TCP_Rpck.PortDest = tcpConn->PortSrc;
TCP_Rpck.WindowSize = SwapHalf(48000);  //was 32000;

TCP_Rpck.TcpSeqNumber = SwapWord(rcvAck);
TCP_Rpck.AckSeqNumber = SwapWord(rcvSeq);
tcpConn->AckSeqNumber = rcvSeq;

TCP_Rpck.TotalLen = SwapHalf(len);

/*----------------------------------------------------------------------------*/
/* Checksum calculation.                                                      */
/*----------------------------------------------------------------------------*/
                                        // IP checksum.
TCP_Rpck.HeaderChecksum =
                     SwapHalf(IpCheckSum((unsigned short *)&TCP_Rpck.IpVer,20));
                                        // TCP checksum.
TCPchksum = TcpCheckSum(0,&TCP_Rpck.PortSrc,len-20);
TCPchksum = TcpCheckSum(TCPchksum,(unsigned short *)&TCP_Rpck.ProtSrc,8);
TCPchksum += (6 + len - 20);
TCP_Rpck.Checksum = SwapHalf((unsigned short )~(TCPchksum + (TCPchksum >> 16)));

Ethernet_SendFrame(TCP_Rpck.Mac0,len+14);
}


/******************************************************************************/
/* TCP_StopConnection                                                         */
/* This function stops the TCP connection. One of the reason could be the     */
/* timeout event.                                                             */
/* TCP and IP Checksum are calculated by the Hardware.                        */
/******************************************************************************/
void TCP_StopConnection(int cId)
{
unsigned short len;
TCP_packet     TCP_Pck;
TCPIP_conn     *tcpConn=&TCPIP_Conn[cId];

MemCopyH(TCP_Pck.Mac0,tcpConn->Mac,6);
MemCopyH(TCP_Pck.Mac1,MacAddress,6);
TCP_Pck.Type = 0x0008;
TCP_Pck.IpVer = 0x45;
TCP_Pck.ServiceFields = 0;
TCP_Pck.Identification = SwapHalf(++TcpSeqNumber);
TCP_Pck.Flags = 0x40;
TCP_Pck.FragOff = 0;
TCP_Pck.TimeToLive = 0x80;
TCP_Pck.Protocol = 0x06;
TCP_Pck.HeaderChecksum = 0;

TCP_Pck.ProtSrc = IpAddress;
TCP_Pck.ProtDest = tcpConn->ProtSrc;
TCP_Pck.PortSrc = tcpConn->PortDest;
TCP_Pck.PortDest = tcpConn->PortSrc;
                                        // TCP Header to 20 bytes.
TCP_Pck.HeaderLen = 0x50;
TCP_Pck.Checksum = 0;
TCP_Pck.Options[0] = 0;
TCP_Pck.Options[1] = 0;
TCP_Pck.WindowSize = SwapHalf(48000);   //was 32000;

TCP_Pck.TcpOption = 0x11;               // Send Finish bit.

TCP_Pck.TcpSeqNumber = SwapWord(tcpConn->SeqNumber);
TCP_Pck.AckSeqNumber = SwapWord(tcpConn->AckSeqNumber);

len = 0x28;
TCP_Pck.TotalLen = len;

Ethernet_SendFrame(TCP_Pck.Mac0,len+14);
}


/******************************************************************************/
/* TCP_TimeOutManagement                                                      */
/* This function manages the connection timeout.                              */
/******************************************************************************/
void TCP_TimeOutManagment(void)
{
int i;

for (i = 0;i < 2;i++)
  if (TCPIP_Conn[i].ConnStatus != 0){
    if (TCPIP_Conn[i].ConnectionTimeOut > 0)
      TCPIP_Conn[i].ConnectionTimeOut--;
    if (TCPIP_Conn[i].ConnectionTimeOut == 0){
      if (TCPIP_Conn[i].ConnStatus == 2){
        TCP_StopConnection(i);          // send disconnection packet.
        TCPIP_Conn[i].ConnStatus = 3;
        TCPIP_Conn[i].ConnectionTimeOut = 10;
        }
      else
        TCPIP_Conn[i].ConnStatus = 0;
      }
    }
}


/******************************************************************************/
/* UDP_Management function.                                                   */
/* This function is able to manage the UDP packets.  Of course we cannot ma-  */
/* nage all the UDP packets coming from the network.                          */
/* The MAC source address is inserted automatically by the Ethernet control-  */
/* ler.                                                                       */
/******************************************************************************/
void UDP_Management(struct udp_packet *pck)
{

if (pck->DstIp != IpAddress)            // discard extraneous packets
  return;
if (IpCheckSum((unsigned short *)pck,10) != 0)
  return;

//------------------------------------------------------------------------------
// process last fragment of multi-packet
//------------------------------------------------------------------------------
if ((pck->IpFrag & 0x2000) == 0){
  if (pck->PackId == CurPackId ){       // last fragment
    if (pck->IpFrag != (BytesReceived >> 3))
      return;
    memcpy(EthBuff+(BytesReceived-8),(unsigned char *)&pck->SrcPort,pck->PackLen - 20);
    EthBuffLen += BytesReceived + pck->PackLen -28;
    BytesReceived = 0;                  // reset multi-packet byte counter
    }
  else{
//------------------------------------------------------------------------------
// process single-packet
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Check for proper port (0x222x where x is the Host VimeExerciser Ethernet port
// number).
//------------------------------------------------------------------------------
    UDPport = pck->SrcPort;
    if ((pck->SrcPort & 0xFFF0) != 0x2220)
      return;
    if ((pck->DstPort & 0xFFF0) != 0x2220)
      return;

    CurPackId = pck->PackId;            // single packet ID
    memcpy(EthBuff,&pck->Data,pck->UdpPackLen - 8); // save data
    EthBuffLen += pck->UdpPackLen - 8;  // strip 8bytes (UDP hdr)
    memcpy(EthReqMacAddr,pck->Mac1,6); // save requestor's MAC & IP
    EthReqIpAddr = pck->SrcIp;          // for replies
    }
/*  if ((pck->PackLen & 1) == 0){   // checksum of UDP packet ( unclear )
      if (IpCheckSum((void *)pck,pck->PackLen/2) != 0)
        return;
      }
    else{
      *(unsigned char *)((unsigned )pck+pck->PackLen) = 0;
      if (IpCheckSum((void *)pck,1+pck->PackLen/2) != 0)
        return;
      }*/

//------------------------------------------------------------------------------
// process multiple PDUs
//------------------------------------------------------------------------------
  EthBuff += EthBuffLen;
  if (EthBuff + EMAC_PDU_SIZE > (unsigned char *) TX_BASE_BUF)
    EthBuff = (unsigned char *) RX_BASE_BUF;
  }
else

//------------------------------------------------------------------------------
// check for first fragment of a multi-packet
//------------------------------------------------------------------------------
  if (pck->IpFrag == 0x2000){           // check for proper UDP ports

//------------------------------------------------------------------------------
// Check for proper port (0x222x where x is the Host VimeExerciser Ethernet port
// number).
//------------------------------------------------------------------------------
    UDPport = pck->SrcPort;
    if ((pck->SrcPort & 0xFFF0) != 0x2220)
      return;
    if ((pck->DstPort & 0xFFF0) != 0x2220)
      return;

    if (pck->PackId == CurPackId)       // check for single ID != to multi-pack ID
      return;
    CurPackId = pck->PackId;            // preset packet ID filter for next fragments
    BytesToRec = pck->UdpPackLen-8;     // setup total bytes to receive
                                        // save UDP header
    memcpy(UDPheader,(unsigned char *)&pck->SrcPort,8);
                                        //strip 8+20 bytes (IP+UDP hdr)
    memcpy(EthBuff,&pck->Data,pck->PackLen - 28);
    BytesReceived += pck->PackLen -20;
    }
//------------------------------------------------------------------------------
// check for next fragment of a multi-packet
//------------------------------------------------------------------------------
else
  if ((pck->PackId == CurPackId)&&(pck->IpFrag & 0x2000)){  // next fragments
    if (pck->IpFrag != ((BytesReceived >> 3) | 0x2000))
      return;
    memcpy((char *)(EthBuff+(BytesReceived-8)),(char *)&pck->SrcPort,
                                                             pck->PackLen - 20);
    BytesReceived += pck->PackLen -20;
    }
}


/*----------------------------------------------------------------------------*/
/* ProcessUnicastEthernetPacket                                               */
/* Process all unicast packets received on Ethernet channel.                  */
/* (called by Rx_EOB IRQ handler on every incoming ethernet packet)           */
/*----------------------------------------------------------------------------*/
void ProcessUnicastEthernetPacket(TCP_packet *pck,unsigned len){

printf("\nUnicast received\n");        //edo

if (pck->Type != 0x0008){               // check for IP packet type.
  if (pck->Type == 0x0608){             // only unicast ARP packet type are
    printf("\nArp received_\n");        //edo
    ARP_Replay((struct arp_packet *)pck); // managed ouside IP packet.
    return;
    }
  return;
  }
                                        // check for IP header version & length
if (pck->IpVer != 0x45)
  return;

if (pck->Protocol == ICMP_PROTOCOL){    // ICMP protocol.
  printf("\nicmp received\n");        //edo
  ICMP_Replay((struct icmp_packet *)pck);
  }
else
  if (pck->Protocol == TCP_PROTOCOL){   // TCP packet.
    printf("\ntcp received\n");        //edo
    TCP_Management((struct tcp_packet *)pck);
    }
else
  if (pck->Protocol == UDP_PROTOCOL){   // UDP protocol.
    printf("\nudp received\n");        //edo
    UDP_Management((struct udp_packet *)pck);
    }
}


/*----------------------------------------------------------------------------*/
/* ProcessBroadcastEthernetPacket                                             */
/* Process all broadcast packets received on Ethernet channel.                */
/* (called by Rx_EOB IRQ handler on every incoming ethernet packet)           */
/*----------------------------------------------------------------------------*/
void ProcessBroadcastEthernetPacket(TCP_packet *pck,unsigned len){

if (pck->Type == 0x0608){               // check for ARP packet type.
  printf("\nArp received\n");        //edo
  ARP_Replay((struct arp_packet *)pck);
  }
}


/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
