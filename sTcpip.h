/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
/* File Name          : sTcpip.h                                              */
/* Author             : Edoardo Lotti, Paolo Torretta                         */
/* Version            : v1.0.0                                                */
/* Date               : 05/11/2015                                            */
/* Description        : Include file for sTcpip.c code.                       */
/******************************************************************************/

/******************************************************************************/
/* Tx & Rx Buffer Descriptors for MAL/EMAC processing                         */
/******************************************************************************/
#define NUM_TX_BUFF            32         // set-up number of BD's for Tx channel
#define NUM_RX_BUFF            32         // and Rx channel
#define EMAC_BUF_SIZE          0x00000800 // max data buffer size = 2Kb
#define EMAC_PDU_SIZE          0x0000FFE0 // max PDU size (frag'd blocks) 64k-32

#define IP_FRAME_SIZE          0x5C0

#define RX_BASE_BUF            0x00D00000 // reserve 1M for Eth Rx temp buffer
#define TX_BASE_BUF            0x00E00000 // reserve 1M for Eth Tx temp buffer

#define TCP_BUFF_SIZE          0x1000
#define TCP_BUFF_SIZE_MASK     0x0FFF


#define ETH_CMD_OK             0x00
#define ETH_ERR_NO_CMD         0x01
#define ETH_ERR_CMD_SIZE       0x02
#define ETH_ERR_CODE_TYPE      0x03
#define ETH_ERR_CMD_PARAM      0x04

#define CAMERA_READREG_CMD     0x01
#define CAMERA_WRITEREG_CMD    0x02
#define CAMERA_GETVPD_CMD      0x03
#define CAMERA_SETVPD_CMD      0x04
#define CAMERA_STARTACQ_CMD    0x05
#define CAMERA_STOPACQ_CMD     0x06
#define READ_SENSOR_TEMP_CMD   0x07
#define SET_AFE_REG_CMD        0x08
#define GET_STATUS_CMD         0x09
#define SET_STATUS_CMD         0x0A
#define EN_DIS_UART_CMD        0x0B
#define UART_STATUS_CMD        0x0C
#define CAMERA_LOADDATA_CMD    0x0D
#define CAMERA_CODEUPD_CMD     0x0E
#define GET_AFE_REG_CMD        0x0F

#define TCP_FRAME_SYNCH        0x10000
#define TCP_STREAMING_EN       0x2
#define TCP_FIFO_EMPTY_FLAG    0x20
#define TCP_FIFO_OVERFLOW_FLAG 0x40
/******************************************************************************/
/* Tunss application data structure.                                          */
/******************************************************************************/

typedef struct arp_packet{              // Arp header packet structure.
  unsigned short Padding;               // Padding to align paylod on word.
  unsigned char  Mac0[6];
  unsigned char  Mac1[6];
  unsigned short Type;
  unsigned short HwType;                // Hardware type.
  unsigned short ProtType;              // Protocol type.
  unsigned short HpLen;                 // Hardware and Protocol len.
  unsigned short OpCode;                // Operating code.
  unsigned char  SrcMac[6];             // Source Mac address.
  unsigned char  SrcIp[4];              // Source Ip address.
  unsigned char  DstMac[6];             // Destination Mac address.
  unsigned char  DstIp[4];              // Destination Ip address.
  } ARP_packet;


typedef struct icmp_packet{             /* Contents of an ICMP packet.        */
  unsigned short Padding;               // Padding to align paylod on word.
  unsigned char  Mac0[6];
  unsigned char  Mac1[6];
  unsigned short Type;
  unsigned char  IpVer;
  unsigned char  ServiceFields;
  unsigned short TotalLen;
  unsigned short Identification;
  unsigned char  Flags;
  unsigned char  FragOff;
  unsigned char  TimeToLive;
  unsigned char  Protocol;
  unsigned short HeaderChecksum;
  unsigned       ProtSrc;
  unsigned       ProtDest;
  unsigned char  IcmpType;
  unsigned char  IcmpCode;
  unsigned short Checksum;
  unsigned short Identifier;
  unsigned short SeqNumber;
  unsigned char  Data[32];
  } ICMP_packet;


typedef struct tcp_packet{
  unsigned short Padding;               // Padding to align paylod on word.
  unsigned char  Mac0[6];
  unsigned char  Mac1[6];
  unsigned short Type;
  unsigned char  IpVer;
  unsigned char  ServiceFields;
  unsigned short TotalLen;
  unsigned short Identification;
  unsigned char  Flags;
  unsigned char  FragOff;
  unsigned char  TimeToLive;
  unsigned char  Protocol;
  unsigned short HeaderChecksum;
  unsigned       ProtSrc;
  unsigned       ProtDest;
  unsigned short PortSrc;
  unsigned short PortDest;
  unsigned       TcpSeqNumber;
  unsigned       AckSeqNumber;
  unsigned char  HeaderLen;
  unsigned char  TcpOption;
  unsigned short WindowSize;
  unsigned short Checksum;
  unsigned char  Options[4];
  unsigned char  Data[1500];
  } TCP_packet;


typedef struct udp_packet{              // IP-UDP header packet structure.
  unsigned short Padding;               // Padding to align paylod on word.
  unsigned char  Mac0[6];
  unsigned char  Mac1[6];
  unsigned short Type;
  unsigned char  IpVer;                 // Ip version and length.
  unsigned char  TypeService;           // Type of service.
  unsigned short PackLen;               // Packet length.
  unsigned short PackId;                // Packet Id.
  unsigned short IpFrag;                // Ip fragment offset.
  unsigned char  TimeToLive;            // Time to live.
  unsigned char  TrasportProt;          // Transport protocol.
  unsigned short IpHdrCheckSum;         // Ip Header checksum.
  unsigned       SrcIp;                 // Ip source address.
  unsigned       DstIp;                 // Ip destination address.
  unsigned short SrcPort;               // Source port / Data pointer (multi-packet)
  unsigned short DstPort;               // Destination port.
  unsigned short UdpPackLen;            // UDP packet len
  unsigned short UdpCheckSum;           // UDP checksum.
  unsigned char  Data;                  // Data buffer pointer.
  } UDP_packet;


typedef struct tcpip_conn{              /* TCPIP connection structure         */
  unsigned       SeqNumber;             /* Sender seq number                  */
  unsigned       AckSeqNumber;
  unsigned       SeqNumberAck;
  unsigned       RetrySend;
  unsigned       ConnectionTimeOut;     /* Connection Timeout (in ms).        */
  unsigned       AutoDestAddr;
  int            AutoDataSize;
  int            AckTimeout;
  unsigned       ProtSrc;
  unsigned char  Mac[6];
  unsigned short PortSrc;
  unsigned short PortDest;
  unsigned char  Protocol;
  unsigned char  ConnStatus;
  unsigned short TxWindowSize;
  unsigned short RealTxWindowSize;
  unsigned short HeadPtr;
  unsigned short TailPtr;
  unsigned short NumBytes;
  unsigned char  buffer[TCP_BUFF_SIZE];
  } TCPIP_conn;

/******************************************************************************/
/* Function Prototypes.                                                       */
/******************************************************************************/
void TcpIpInit(unsigned TcpTimeout,unsigned IpAddr,unsigned char *MacAddr);
void TCP_TimeOutManagment(void);
void TCP_GetData(unsigned char *cId,unsigned char *pdata,unsigned short *num);
int  TCP_SendData(unsigned char cId,unsigned char *pdata,unsigned short len);
void ProcessUnicastEthernetPacket(TCP_packet *pck,unsigned len);
void ProcessBroadcastEthernetPacket(TCP_packet *pck,unsigned len);

/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
