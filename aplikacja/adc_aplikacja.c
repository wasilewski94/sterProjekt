/*
----------------------------MAX1202 DATASHEET------------------------------------------------------------
1)	 Set up the control byte for external clock mode and call it TB1. TB1’s format should be: 1XXXXX11 binary,
 	 where the Xs denote the particular channel and conversion mode selected.
2)	 Use a general-purpose I/O line on the CPU to pull CS on the MAX1202/MAX1203 low.
3)	 Transmit TB1 and simultaneously receive a byte and
 	 call it RB1. Ignore RB1.
4)	 Transmit a byte of all zeros ($00 hex) and simultaneously receive byte RB2.
5)	 Transmit a byte of all zeros ($00 hex) and simultaneously receive byte RB3.
6)	 Pull CS on the MAX1202/MAX1203 high

clock frequency from 100kHz to 2MHz.
----------------------------------------------------------------------------
*/


#define SPI_CPHA    0x01   /* clock phase */
#define SPI_CPOL    0x02   /* clock polarity */
#define SPI_MODE_0  (0|0)  /* (original MicroWire) */
#define SPI_MODE_1  (0|SPI_CPHA)
#define SPI_MODE_2  (SPI_CPOL|0)
#define SPI_MODE_3  (SPI_CPOL|SPI_CPHA)
#define SPI_CS_HIGH 0x04  /* chipselect active high? */
#define SPI_LSB_FIRST 0x08  /* per-word bits-on-wire */
#define SPI_3WIRE   0x10    /* SI/SO signals shared */
#define SPI_LOOP    0x20    /* loopback mode */
#define SPI_NO_CS   0x40   /* 1 dev/bus, no chipselect */
#define SPI_READY   0x80   /* slave pulls low to pause */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fcntl.h>
#include "rspriv.h"
#include <stdlib.h>

void blad(char * fmt,...)
{
  va_list ap;
  va_start(ap,fmt);
  vfprintf(stderr,fmt,ap);
  va_end(ap);
  exit(1);
}


int f=-1; //Deskryptor pliku urz±dzenia

void cleanup()
{
  if(f>=0) {
    ioctl(f,IOCTL_STOP,0);
    close(f);
  }
}


void main(int argc, char * argv[])
{
   printf("funkcja main()\n");
   int div=0;
   if(argc < 2)  blad("Prawid³owe wywo³anie: rs_test plik_specjalny dzielnik\n");
         f = open(argv[1],O_RDWR);
   div=atoi(argv[2]);
   if(f<0) blad("Nie mogê otworzyæ pliku: %s\n",argv[1]);
         atexit(cleanup);
   div=atoi(argv[2]);
   if(ioctl(f,IOCTL_SET_DIV,(long) div)<0) {
         perror("B³±d IOCTL:");
   exit(1);
}
    int res;
   //Czekamy albo na dane z urz±dzenia sygnalizuj±cego przerwanie,
   //albo na naci¶niêcie klawisza
 exit(1);
 }



/*
struct spi_device {
  struct device      dev;
  struct spi_master  *master;
  u32  max_speed_hz;
  u8   chip_select;
  u8   mode;
  u8    bits_per_word;
  int   irq;
  void  *controller_state;
  void  *controller_data;
  char  modalias[SPI_NAME_SIZE];
};


//tablica zdefiniowana dla ADC

static struct spi_board_info bf_spi_devices[] = {
  {  // DataFlash chip 
    .modalias       = “mtd_dataflash”,
    .chip_select    = 0,
    .max_speed_hz   = 15 * 1000 * 1000,
  },
  {
    .modalias = “spidev”,
    .chip_select  = 1,
    .max_speed_hz = 10 * 1000 * 1000,
    .mode = SPI_MODE_3
  }
};



//definicja transferu

struct spi_ioc_transfer {
  __u64           tx_buf;
  __u64           rx_buf;
  __u32           len;
  __u32           speed_hz;
  __u16           delay_usecs;
  __u8            bits_per_word;
  __u8            cs_change;
  __u32           pad;
};


*/
