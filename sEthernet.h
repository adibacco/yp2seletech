/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
/* File Name          : sEthernet.h                                           */
/* Author             : Edoardo Lotti, Paolo Torretta                         */
/* Version            : v1.0.0                                                */
/* Date               : 05/11/2015                                            */
/* Description        : This file includes all the definitions for the code   */
/*                      included in sEthPhy.c.                                */
/******************************************************************************/

/******************************************************************************/
/* Code definitions.                                                          */
/******************************************************************************/
#define EMAC_DMABD_MIN_ALIGN 4          /* Minimum BD alignment.              */

#define EMAC_NUM_RX_BD       32
#define EMAC_NUM_TX_BD       8
#define EMAC_RX_BUFFER_ADDR  0xFFFE0000
#define EMAC_TX_BUFFER_ADDR  0xFFFF0400
#define EMAC_BUFFER_SIZE     0x00000800
#define EMAC_RX_BD_ADDR      0xFFFF0040
#define EMAC_TX_BD_ADDR      0xFFFF0140

#define EMAC_MDC_DIV_8       0
#define EMAC_MDC_DIV_16      1
#define EMAC_MDC_DIV_32      2
#define EMAC_MDC_DIV_48      3
#define EMAC_MDC_DIV_64      4
#define EMAC_MDC_DIV_96      5
#define EMAC_MDC_DIV_128     6
#define EMAC_MDC_DIV_224     7

#define EMAC_NWCTRL_OFF      0x00000000 // Network Control reg
#define EMAC_NWCFG_OFF       0x00000004 // Network Config reg
#define EMAC_NWSR_OFF        0x00000008 // Network Status reg

#define EMAC_DMACR_OFF       0x00000010 // DMA Control reg
#define EMAC_TXSR_OFF        0x00000014 // TX Status reg
#define EMAC_RXQBASE_OFF     0x00000018 // RX Q Base address reg
#define EMAC_TXQBASE_OFF     0x0000001C // TX Q Base address reg
#define EMAC_RXSR_OFF        0x00000020 // RX Status reg

#define EMAC_ISR_OFF         0x00000024 // Interrupt Status reg
#define EMAC_IER_OFF         0x00000028 // Interrupt Enable reg
#define EMAC_IDR_OFF         0x0000002C // Interrupt Disable reg
#define EMAC_IMR_OFF         0x00000030 // Interrupt Mask reg

#define EMAC_PHYMNTNC_OFF    0x00000034 // Phy Maintaince reg
#define EMAC_RXPAUSE_OFF     0x00000038 // RX Pause Time reg
#define EMAC_TXPAUSE_OFF     0x0000003C // TX Pause Time reg

#define EMAC_HASHL_OFF       0x00000080 // Hash Low address reg
#define EMAC_HASHH_OFF       0x00000084 // Hash High address reg

#define EMAC_LADDR1L_OFF     0x00000088 // Specific1 addr low reg
#define EMAC_LADDR1H_OFF     0x0000008C // Specific1 addr high reg
#define EMAC_LADDR2L_OFF     0x00000090 // Specific2 addr low reg
#define EMAC_LADDR2H_OFF     0x00000094 // Specific2 addr high reg
#define EMAC_LADDR3L_OFF     0x00000098 // Specific3 addr low reg
#define EMAC_LADDR3H_OFF     0x0000009C // Specific3 addr high reg
#define EMAC_LADDR4L_OFF     0x000000A0 // Specific4 addr low reg
#define EMAC_LADDR4H_OFF     0x000000A4 // Specific4 addr high reg

#define EMAC_MATCH1_OFF      0x000000A8 // Type ID1 Match reg
#define EMAC_MATCH2_OFF      0x000000AC // Type ID2 Match reg
#define EMAC_MATCH3_OFF      0x000000B0 // Type ID3 Match reg
#define EMAC_MATCH4_OFF      0x000000B4 // Type ID4 Match reg

#define EMAC_STRETCH_OFF     0x000000BC // IPG Stretch reg
#define EMAC_OCTTXL_OFF      0x00000100 // Octects transmitted Low reg
#define EMAC_OCTTXH_OFF      0x00000104 // Octects transmitted High reg
#define EMAC_TXCNT_OFF       0x00000108 // Error-free Frames transmitted counter
#define EMAC_TXBCCNT_OFF     0x0000010C // Error-free Broadcast Frames counter
#define EMAC_TXMCCNT_OFF     0x00000110 // Error-free Multicast Frame counter
#define EMAC_TXPAUSECNT_OFF  0x00000114 // Pause Frames Transmitted Counter
#define EMAC_TX64CNT_OFF     0x00000118 // Error-free 64 byte Frames Transmitted counter
#define EMAC_TX65CNT_OFF     0x0000011C // Error-free 65-127 byte Frames
                                        // Transmitted counter
#define EMAC_TX128CNT_OFF    0x00000120 // Error-free 128-255 byte Frames
                                        // Transmitted counter
#define EMAC_TX256CNT_OFF    0x00000124 // Error-free 256-511 byte Frames
                                        // transmitted counter
#define EMAC_TX512CNT_OFF    0x00000128 // Error-free 512-1023 byte Frames
                                        // transmitted counter
#define EMAC_TX1024CNT_OFF   0x0000012C // Error-free 1024-1518 byte Frames
                                        // transmitted counter
#define EMAC_TX1519CNT_OFF   0x00000130 // Error-free larger than 1519 byte
                                        // Frames transmitted counter
#define EMAC_TXURUNCNT_OFF   0x00000134 // TX under run error counter

#define EMAC_SNGLCOLLCNT_OFF 0x00000138 // Single Collision Frame Counter
#define EMAC_MULTICOLLCNT_OFF 0x0000013C // Multiple Collision Frame Counter
#define EMAC_EXCESSCOLLCNT_OFF 0x00000140 // Excessive Collision Frame Counter
#define EMAC_LATECOLLCNT_OFF 0x00000144 // Late Collision Frame Counter
#define EMAC_TXDEFERCNT_OFF  0x00000148 // Deferred Transmission Frame Counter
#define EMAC_TXCSENSECNT_OFF 0x0000014C // Transmit Carrier Sense Error Counter
#define EMAC_OCTRXL_OFF      0x00000150 // Octects Received register Low
#define EMAC_OCTRXH_OFF      0x00000154 // Octects Received register High
#define EMAC_RXCNT_OFF       0x00000158 // Error-free Frames Received Counter
#define EMAC_RXBROADCNT_OFF  0x0000015C // Error-free Broadcast Frames Received
                                        // Counter
#define EMAC_RXMULTICNT_OFF  0x00000160 // Error-free Multicast Frames Received
                                        // Counter
#define EMAC_RXPAUSECNT_OFF  0x00000164 // Pause Frames Received Counter
#define EMAC_RX64CNT_OFF     0x00000168 // Error-free 64 byte Frames Received Counter
#define EMAC_RX65CNT_OFF     0x0000016C // Error-free 65-127 byte Frames
                                        // Received Counter */
#define EMAC_RX128CNT_OFF    0x00000170 // Error-free 128-255 byte Frames
                                        // Received Counter */
#define EMAC_RX256CNT_OFF    0x00000174 // Error-free 256-512 byte Frames
                                        // Received Counter
#define EMAC_RX512CNT_OFF    0x00000178 // Error-free 512-1023 byte Frames
                                        // Received Counter
#define EMAC_RX1024CNT_OFF   0x0000017C // Error-free 1024-1518 byte Frames
                                        // Received Counter
#define EMAC_RX1519CNT_OFF   0x00000180 // Error-free 1519-max byte Frames
                                        // Received Counter
#define EMAC_RXUNDRCNT_OFF   0x00000184 // Undersize Frames Received Counter
#define EMAC_RXOVRCNT_OFF    0x00000188 // Oversize Frames Received Counter
#define EMAC_RXJABCNT_OFF    0x0000018C // Jabbers Received Counter
#define EMAC_RXFCSCNT_OFF    0x00000190 // Frame Check Sequence Error Counter
#define EMAC_RXLENGTHCNT_OFF 0x00000194 // Length Field Error Counter
#define EMAC_RXSYMBCNT_OFF   0x00000198 // Symbol Error Counter
#define EMAC_RXALIGNCNT_OFF  0x0000019C // Alignment Error Counter
#define EMAC_RXRESERRCNT_OFF 0x000001A0 // Receive Resource Error Counter
#define EMAC_RXORCNT_OFF     0x000001A4 // Receive Overrun Counter
#define EMAC_RXIPCCNT_OFF    0x000001A8 // IP header Checksum Error Counter
#define EMAC_RXTCPCCNT_OFF   0x000001AC // TCP Checksum Error Counter
#define EMAC_RXUDPCCNT_OFF   0x000001B0 // UDP Checksum Error Counter
#define EMAC_LAST_OFF        0x000001B4 // Last statistic counter offset, for
                                        // clearing
#define EMAC_1588_SEC_OFF    0x000001D0 // 1588 second counter
#define EMAC_1588_NANOSEC_OFF 0x000001D4 // 1588 nanosecond counter
#define EMAC_1588_ADJ_OFF    0x000001D8 // 1588 nanosecond adjustment counter
#define EMAC_1588_INC_OFF    0x000001DC // 1588 nanosecond increment counter
#define EMAC_PTP_TXSEC_OFF   0x000001E0 // 1588 PTP transmit second counter
#define EMAC_PTP_TXNANOSEC_OFF 0x000001E4 // 1588 PTP transmit nanosecond counter
#define EMAC_PTP_RXSEC_OFF   0x000001E8 // 1588 PTP receive second counter
#define EMAC_PTP_RXNANOSEC_OFF 0x000001EC // 1588 PTP receive nanosecond counter
#define EMAC_PTPP_TXSEC_OFF  0x000001F0 // 1588 PTP peer transmit second counter
#define EMAC_PTPP_TXNANOSEC_OFF 0x000001F4 // 1588 PTP peer transmit nanosecond counter
#define EMAC_PTPP_RXSEC_OFF  0x000001F8 // 1588 PTP peer receive second counter
#define EMAC_PTPP_RXNANOSEC_OFF 0x000001FC // 1588 PTP peer receive nanosecond counter
                                          // @name network control register bit definitions
#define EMAC_NWCTRL_ZEROPAUSETX 0x00000800 // Transmit zero quantum pause frame
#define EMAC_NWCTRL_PAUSETX  0x00000800 // Transmit pause frame
#define EMAC_NWCTRL_HALTTX   0x00000400 // Halt transmission after current frame
#define EMAC_NWCTRL_STARTTX  0x00000200 // Start tx (tx_go)

#define EMAC_NWCTRL_STATWEN  0x00000080 // Enable writing to stat counters
#define EMAC_NWCTRL_STATINC  0x00000040 // Increment statistic registers
#define EMAC_NWCTRL_STATCLR  0x00000020 // Clear statistic registers
#define EMAC_NWCTRL_MDEN     0x00000010 // Enable MDIO port
#define EMAC_NWCTRL_TXEN     0x00000008 // Enable transmit
#define EMAC_NWCTRL_RXEN     0x00000004 // Enable receive
#define EMAC_NWCTRL_LOOPEN   0x00000002 // local loopback
                                        // network configuration register
#define EMAC_NWCFG_BADPREAMBEN 0x20000000 // disable rejection of non-standard preamble
#define EMAC_NWCFG_IPDSTRETCH 0x10000000 // enable transmit IPG
#define EMAC_NWCFG_FCSIGNORE 0x04000000 // disable rejection of FCS error
#define EMAC_NWCFG_HDRXEN    0x02000000 // RX half duplex
#define EMAC_NWCFG_RXCHKSUMEN 0x01000000 // enable RX checksum offload
#define EMAC_NWCFG_PAUSECOPYDI 0x00800000 // Do not copy pause Frames to memory
#define EMAC_NWCFG_MDC_SHIFT 18         // shift bits for MDC
#define EMAC_NWCFG_MDCCLKDIV 0x001C0000 // MDC Mask PCLK divisor
#define EMAC_NWCFG_FCSREM    0x00020000 // Discard FCS from received frames
#define EMAC_NWCFG_LENGTHERRDSCRD 0x00010000
                                        // RX length error discard
#define EMAC_NWCFG_RXOFFS    0x0000C000 // RX buffer offset
#define EMAC_NWCFG_PAUSEEN   0x00002000 // Enable pause RX
#define EMAC_NWCFG_RETRYTESTEN 0x00001000 // Retry test
#define EMAC_NWCFG_EXTADDRMATCHEN 0x00000200
                                        // External address match enable
#define EMAC_NWCFG_1000      0x00000400 // 1000 Mbps
#define EMAC_NWCFG_1536RXEN  0x00000100 //Enable 1536 byte frames reception
#define EMAC_NWCFG_UCASTHASHEN 0x00000080 // Receive unicast hash frames
#define EMAC_NWCFG_MCASTHASHEN 0x00000040 // Receive multicast hash frames
#define EMAC_NWCFG_BCASTDI   0x00000020 // Do not receive broadcast frames
#define EMAC_NWCFG_COPYALLEN 0x00000010 // Copy all frames
#define EMAC_NWCFG_JUMBO     0x00000008 // Jumbo frames
#define EMAC_NWCFG_NVLANDISC 0x00000004 // Receive only VLAN frames
#define EMAC_NWCFG_FDEN      0x00000002 // full duplex
#define EMAC_NWCFG_100       0x00000001 // 100 Mbps
                                        // network status register bit definitaions
#define EMAC_NWSR_MDIOIDLE   0x00000004 // PHY management idle
#define EMAC_NWSR_MDIO       0x00000002 // Status of mdio_in

/*----------------------------------------------------------------------------*/
/* Configuration options                                                      */
/* Device configuration options. See the XEmacPs_SetOptions(),                */
/* XEmacPs_ClearOptions() and XEmacPs_GetOptions() for information on how to  */
/* use options.                                                               */
/* The default state of the options are noted and are what the device and     */
/* driver will be set to after calling XEmacPs_Reset() or XEmacPs_Initialize()*/
/*----------------------------------------------------------------------------*/
#define EMAC_PROMISC         0x00000001
#define EMAC_FRAME1536       0x00000002
#define EMAC_VLAN            0x00000004
#define EMAC_FLOW_CONTROL    0x00000010
#define EMAC_FCS_STRIP       0x00000020
#define EMAC_FCS_INSERT      0x00000040
#define EMAC_LENTYPE_ERR     0x00000080
#define EMAC_TRANSMITTER_ENABLE 0x00000100
#define EMAC_RECEIVER_ENABLE 0x00000200
#define EMAC_BROADCAST       0x00000400
#define EMAC_MULTICAST       0x00000800
#define EMAC_RX_CHKSUM_ENABLE 0x00001000
#define EMAC_TX_CHKSUM_ENABLE 0x00002000

#define EMAC_DEFAULT_OPTIONS                     \
    (EMAC_FLOW_CONTROL_OPTION |                  \
     EMAC_FCS_INSERT_OPTION |                    \
     EMAC_FCS_STRIP_OPTION |                     \
     EMAC_BROADCAST_OPTION |                     \
     EMAC_LENTYPE_ERR_OPTION |                   \
     EMAC_TRANSMITTER_ENABLE_OPTION |            \
     EMAC_RECEIVER_ENABLE_OPTION |               \
     EMAC_RX_CHKSUM_ENABLE_OPTION |              \
     EMAC_TX_CHKSUM_ENABLE_OPTION)
                                        // DMA control register bit definitions
#define EMAC_DMACR_RXBUF     0x00FF0000 // Mask bit for RX buffer size
#define EMAC_DMACR_RXBUF_SHIFT 16       // Shift bit for RX buffer size
#define EMAC_DMACR_TCPCKSUM  0x00000800 // enable/disable TX checksum offload
#define EMAC_DMACR_TXSIZE    0x00000400 // TX buffer memory size
#define EMAC_DMACR_RXSIZE    0x00000300 // RX buffer memory size
#define EMAC_DMACR_ENDIAN    0x00000080 // endian configuration
#define EMAC_DMACR_BLENGTH   0x0000001F // buffer burst length

/*----------------------------------------------------------------------------*/
/* Direction identifier.                                                      */
/*----------------------------------------------------------------------------*/
#define EMAC_SEND            1          // send direction
#define EMAC_RECV            2          // receive direction

/******************************************************************************/
/* Structure definition.                                                      */
/******************************************************************************/
typedef struct {
  unsigned BufferAddr;
  unsigned Control;
  } Emac_Bd;

typedef struct {
  unsigned BdAddr;                      /* Buffer Descriptors address.        */
  unsigned BufferAddr;                  /* First Buffer list address.         */
  Emac_Bd  *Current;                    /* Flag to indicate DMA is started    */
  Emac_Bd  *First;                      /* This is the first BD of the list.  */
  Emac_Bd  *Tail;                       /* This is the tail pointer.          */
  unsigned AllCnt;                      /* Total Number of BDs for channel    */
  } Emac_BdRing;


typedef struct {                        /* Hardware configuration             */
  unsigned    BaseAddress;              /* Physical address of IPIF registers */
  unsigned    PhyAddress;               /* PHY address.                       */
  unsigned    IsReady;                  /* Device is initialized and ready    */
  unsigned    Options;                  /* Current options word               */

  Emac_BdRing RxBdRing;                 /* Receive BD ring                    */
  Emac_BdRing TxBdRing;                 /* Transmit BD ring                   */

  unsigned    ManagementInts;           /* Management interrupts.             */
  unsigned    RxBufferOverrider;        /* Rx Buffer overrider by controller. */
  unsigned    RxNoBuffers;              /* Rx buffers not savailable.         */
  unsigned    CheckSumErrors;           /* Cumulated Checksum errors.         */
  unsigned    OtherErrors;              /* Other Generic errors.              */
  unsigned    TxFrameCorrupted;         /* Tx Frame corrupted errors.         */
  } Emac;



/******************************************************************************/
/* Function prototypes.                                                       */
/******************************************************************************/
int  Ethernet_SetMacAddress(void *AddressPtr,unsigned char Index);
int  Ethernet_GetMacAddress(void *AddressPtr,unsigned char Index);
int  Ethernet_SetHash(void *AddressPtr);
int  Ethernet_ClearHash(void);
int  Ethernet_GetHash(void *AddressPtr);
int  Ethernet_SendPausePacket(void);
unsigned short Ethernet_GetOperatingSpeed(void);
int  Ethernet_SetOperatingSpeed(unsigned short Speed);
int  Ethernet_SetMdioDivisor(int Divisor);
int  Ethernet_SetTypeIdCheck(unsigned Id_Check,unsigned char Index);
int  Ethernet_SetOptions(unsigned Options);
int  Ethernat_ClearOptions(unsigned Options);
int  EthernetPhyConfigure(void);
int  EthernetControllerInit(unsigned char *TmRegArea,unsigned char *BdArea);
int  EthernetControllerConfigure(void);
int  Ethernet_SetBufferDescriptors(Emac *xEmac,int NumRxBuffers,
                                                              int NumTxBuffers);
int  EthernetDmaInit(Emac *xEmac);
void Ethernet_SendFrame(unsigned char *ptr,int len);


/******************** (C) COPYRIGHT 2015 Seletech srl *************************/
