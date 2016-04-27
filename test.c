#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stropts.h>

#include "sErrors.h"


#define  CHAR_BS                      8
#define  CHAR_LF                      10
#define  CHAR_CR                      13
#define  CHAR_COMMA                   44

#define INPUT_BUFFER                  100
 
extern int errno;

/******************************************************************************/
/* PrintHelpPanel                                                             */
/* This function prints the help panel on serial port to facilitate the usage */
/* of the kernel debugger commands.                                           */
/******************************************************************************/
void PrintHelpPanel(void)
{
int           i;
unsigned char mData;

printf("\n\re         - Ethernet Statistics");
printf("\n\rE         - Ethernet initialization");
printf("\n\rh         - Help Panel");
printf("\n\rmdxxx,yy  - Memory Dump (xxx=addr, yy=size)");
printf("\n\rmwxxx,yy  - Memory Write (xxx=addr, yy=data)");
printf("\n\rn         - Phy read registers");
printf("\n\rNx,yy,zz  - Phy write register");
printf("\n\rq         - Exit");
printf("\n\rr         - Reset PL logic");
printf("\n\rre        - Reset Ethernet RX");
printf("\n\rR         - PL register dump");
printf("\n\rv         - Get versions");
}


/******************************************************************************/
/* AsciiToHex                                                                 */
/* This function converts ascii characters in Hex value. The same function is */
/* able to manage 32bits values.                                              */
/******************************************************************************/
unsigned char *AsciiToHex(unsigned char *ptr,unsigned *Data)
{
int i;
  
*Data = 0;
  
if ((ptr[0] == '0')&&(ptr[1] == 'x'))
  ptr += 2;
  
for (i = 0;i < 8;i++){
  if ((ptr[i] >= '0')&&(ptr[i] <= '9'))
    *Data = (*Data << 4) + ptr[i] - '0';
  else if ((ptr[i] >= 'A')&&(ptr[i] <= 'F'))
    *Data = (*Data << 4) + ptr[i] - 55;
  else if ((ptr[i] >= 'a')&&(ptr[i] <= 'f'))
    *Data = (*Data << 4) + ptr[i] - 87;
  else if ((ptr[i] == ' ')||(ptr[i] == 0)||(ptr[i] == CHAR_COMMA))
    break;
  else
    return 0;
  }
return ptr + i;
}


/******************************************************************************/
/* DumpData                                                                   */
/* This function dumps ascii character on the UART port.                      */
/******************************************************************************/
void DumpData(unsigned char *ptr,unsigned Address,unsigned Data)
{
int i;

for (i = 0;i < Data;i++){
  if ((i & 0x3) == 0)
    printf("\n\r%08X ",Address);
  printf(" %08X",*(unsigned *)ptr);
  Address += 4;
  ptr += 4;
  }
}


/******************************************************************************/
int main()
{
unsigned char  buffer[INPUT_BUFFER];
unsigned char  *TmRegArea,*I2CArea,*TEMAC0Area,*TEMAC1Area,*TEMAC2Area,*BdArea,*ptr,EthControllerState=0;;
int            i,fd,Exit=0;
unsigned       rc,Address,Data,Offset,I2C_slave_address,tmp1;
struct termios oldt, newt;
int            ch;

tcgetattr( STDIN_FILENO, &oldt );
newt = oldt;

newt.c_cc[VMIN] = 0;
newt.c_cc[VTIME] = 1;
newt.c_lflag &= ~( ICANON | ECHO );
tcsetattr( STDIN_FILENO, TCSANOW, &newt );

fd = open("/dev/mem",O_RDWR|O_SYNC);
if(fd < 0){
  printf("Can't open /dev/mem\n");
  return 1;
  }
TmRegArea = (unsigned char *) mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x44000000);
if(TmRegArea == NULL){
  printf("Can't mmap\n");
  return 1;
  }
printf("\nTmRegArea %08X",TmRegArea);     //edo

TEMAC0Area = (unsigned char *) mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x47000000);
if(TEMAC0Area == NULL){
  printf("Can't mmap\n");
  return 1;
  }
printf("\nTEMAC0Area %08X",TEMAC0Area);     //edo

TEMAC1Area = (unsigned char *) mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x48000000);
if(TEMAC1Area == NULL){
  printf("Can't mmap\n");
  return 1;
  }
printf("\nTEMAC1Area %08X",TEMAC1Area);     //edo

TEMAC2Area = (unsigned char *) mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x49000000);
if(TEMAC2Area == NULL){
  printf("Can't mmap\n");
  return 1;
  }
printf("\nTEMAC2Area %08X",TEMAC2Area);     //edo

BdArea = (unsigned char *) mmap(0, 0x20000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x45000000);
if(BdArea == NULL){
  printf("Can't mmap\n");
  return 1;
  }
printf("\nBdArea %08X",BdArea);     //edo

printf("Tm>\n");
i = 0;
do{
/*----------------------------------------------------------------------------*/
/* Ethernet Rx frame management.                                              */
/*----------------------------------------------------------------------------*/
  if (EthControllerState != 0)
    Ethernet_GetRxFrameManagement();

  buffer[i] = getchar();
  if (buffer[i] != 0xFF){
    printf("%c",buffer[i]);
    if (buffer[i] == CHAR_LF){
      printf("\n");
      buffer[i] = 0;
      rc = 0;

      switch (buffer[0]){

/*----------------------------------------------------------------------------*/  
/* I2C                                                                        */
/*----------------------------------------------------------------------------*/ 
		case 'c':
			if (buffer[1]=='1')
			{
				*(unsigned *) (TEMAC0Area+0x0404) = 0xD0000000;
				*(unsigned *) (TEMAC0Area+0x0408) = 0xD0000000;
				*(unsigned *) (TEMAC0Area+0x0500) = 0x58;
				*(unsigned *) (TEMAC0Area+0x0508) = 0x1140;
				*(unsigned *) (TEMAC0Area+0x0504) = 0x4800;
				delay(0x1000000);
				while (( ((*(unsigned *) (TEMAC0Area+0x050C)) & 0x10000) != 0x10000));
				*(unsigned *) (TEMAC0Area+0x0508) = 0x1340;
				*(unsigned *) (TEMAC0Area+0x0504) = 0x4800;
				delay(0x1000000);
				while (( ((*(unsigned *) (TEMAC0Area+0x050C)) & 0x10000) != 0x10000));

				*(unsigned *) (TEMAC0Area+0x0504) = 0x18800;
				delay(0x1000000);
				while (( ((*(unsigned *) (TEMAC0Area+0x050C)) & 0x10000) != 0x10000));
				printf("\n\rStatus register: ");
				printf("%08X",((*(unsigned *) (TEMAC0Area+0x050C)) & 0xFFFF));

				*(unsigned *) (TEMAC0Area+0x0504) = 0x18800;
				delay(0x1000000);
				while (( ((*(unsigned *) (TEMAC0Area+0x050C)) & 0x10000) != 0x10000));
				printf("\n\rStatus register: ");
				printf("%08X",((*(unsigned *) (TEMAC0Area+0x050C)) & 0xFFFF));
				
				*(unsigned *) (TmRegArea+0x1004) = 0x1;
				*(unsigned *) (TmRegArea+0x1204) = 0x1;
				*(unsigned *) (TmRegArea+0x1304) = 0x1;
				*(unsigned *) (TmRegArea+0x0010) = 0x1;

				*(unsigned *) (TmRegArea+0xC000) = 0x00010203;
				*(unsigned *) (TmRegArea+0xC004) = 0x04060000;
				*(unsigned *) (TmRegArea+0xC008) = 0xFFFFFFFF;
				*(unsigned *) (TmRegArea+0xC00C) = 0xFFFF0000;
				*(unsigned *) (TmRegArea+0xC010) = 0x15;

			}
			else if (buffer[1]=='2')
			{
				
			}
			else if (buffer[1]=='3')
			{
				
			}
			else
				rc = ERROR_SYNTAX;
			break;

/*----------------------------------------------------------------------------*/
/* Ethernet statistics command.                                               */
/*----------------------------------------------------------------------------*/
        case 'e':
            break;

/*----------------------------------------------------------------------------*/
/* Ethernet controller initialization command.                                */
/*----------------------------------------------------------------------------*/
        case 'E':
            rc = EthernetControllerInit(TmRegArea,BdArea);
            EthControllerState = 1;
            break;

/*----------------------------------------------------------------------------*/
/* Help panel command.                                                        */
/*----------------------------------------------------------------------------*/
        case 'h':
            PrintHelpPanel();
            rc = 0;
            break;

/*----------------------------------------------------------------------------*/
/* Memory Dump.                                                               */
/*----------------------------------------------------------------------------*/
        case 'm':
          rc = 0;
          if (buffer[1] == 'd'){
            ptr = AsciiToHex(&buffer[2],&Address);
            Data = 0x40;
            if ((ptr != 0)&&(*ptr == ',')){
              ptr = AsciiToHex(++ptr,&Data);
              if (Data == 0)
                Data = 0x40;
              }
            if ((Address & 0xFFF00000) == 0x83C00000){
              Offset = Address - 0x83C00000;
              DumpData(TmRegArea+Offset,Address,Data);
              }
            else if ((Address & 0xFF000000) == 0x84000000){
              Offset = Address - 0x84000000;
              DumpData(BdArea+Offset,Address,Data);
              }
            }
          else if (buffer[1] == 'w'){
            ptr = AsciiToHex(&buffer[2],&Address);
            if ((ptr == 0)||(*ptr != ','))
              rc = ERROR_SYNTAX;
            else{
              ptr = AsciiToHex(++ptr,&Data);
              if ((Address & 0xFFF00000) == 0x83C00000){
                Address = Address - 0x83C00000;
                *(unsigned *)(TmRegArea+Address) = Data;
                }
              else if ((Address & 0xFF000000) == 0x84000000){
                Address = Address - 0x84000000;
                *(unsigned *)(BdArea+Address) = Data;
                }
              }
            }
          else
            rc = ERROR_SYNTAX;
          break;

/*----------------------------------------------------------------------------*/
/* Exit command.                                                              */
/*----------------------------------------------------------------------------*/
        case 'q':
            Exit = 1;
            rc = 0;
            break;

/*----------------------------------------------------------------------------*/
/* Error in case commands are not found.                                      */
/*----------------------------------------------------------------------------*/
        default:
          rc = ERROR_SYNTAX;
          printf("\n\rSyntax Error in last command");
          break;
        }

      i = 0;
      if (rc != 0)
        printf("\n\rERROR: %08X",rc);
      else
        printf("\n\rOK\n\r");
        
      printf("\n\rTm>");
      }
    else
      if (buffer[i] == CHAR_BS){
        if (i > 0)
          i--;
        }
    else
      if (++i > INPUT_BUFFER - 1)
        i = INPUT_BUFFER - 1;
    }
  } while (Exit == 0);

tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

}
