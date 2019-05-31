/*
 *
 */
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/imx-common/mxc_i2c.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/imx-common/video.h>
#include <mmc.h>
#include <i2c.h>

#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include "eeprom_info.h"

extern struct mmc *find_mmc_device(int dev_num);
extern int mmc_init(struct mmc *mmc);
extern int fat_register_device(struct blk_desc *dev_desc, int part_no);
extern int file_fat_read(const char *filename, void *buffer, int maxsize);
extern void puts(const char *str);
extern void udelay(unsigned long usec);
//#define CONFIG_GALCORE_VIVANTE 

#ifdef CONFIG_EDID_EEPROM_I2C2
// variable
struct edid_eeprom_info edid_eeprom;
// function
static void update_displays_array(void);
static void combine_panel_name(void);
#endif

struct eeprom_info AT24c02_eeprom;
struct udevice *dev;

#ifdef CONFIG_EEPROM_GPIO_I2C4
#define GPIO_I2C_SCL	IMX_GPIO_NR(1, 28)
#define GPIO_I2C_SDA	IMX_GPIO_NR(1, 29)
/*	gpio i2c speed 
	(5) 100k, 
	(50) 10k
 */
#define I2C_DELAY_TIME		50
#define I2C_DELAY			udelay(I2C_DELAY_TIME)
#define GPIO_REG_RW	1
#ifdef GPIO_REG_RW
#ifndef GPIO_GDIR
#define GPIO_GDIR		4
#endif
#ifndef GPIO_DR
#define GPIO_DR			0
#endif
#ifndef GPIO_PSR
#define GPIO_PSR		8
#endif
#else
#define GPIO_I2C_SCL	IMX_GPIO_NR(1, 28)
#define GPIO_I2C_SDA	IMX_GPIO_NR(1, 29)
#endif

static int i2c_gpio_sda_get(void)
{
#ifdef GPIO_REG_RW
	unsigned int reg=0;
	reg=readl(GPIO1_BASE_ADDR+ GPIO_PSR);

	reg &= (1<<29);
	return(reg==0?0:1);
#else
	return gpio_get_value(GPIO_I2C_SDA);
#endif
}

static void i2c_gpio_sda_set(int bit)
{
#ifdef GPIO_REG_RW
	unsigned int reg=0;
	if (bit)
	{
		reg=readl(GPIO1_BASE_ADDR+ GPIO_GDIR);
		reg &= ~(1<<29);
		writel(reg, GPIO1_BASE_ADDR+ GPIO_GDIR);
	}
	else
	{
		reg=readl(GPIO1_BASE_ADDR+ GPIO_GDIR);
		reg |= (1<<29);
		writel(reg, GPIO1_BASE_ADDR+ GPIO_GDIR);

		reg=readl(GPIO1_BASE_ADDR+ GPIO_DR);
		reg &= ~(1<<29);
		writel(reg, GPIO1_BASE_ADDR+ GPIO_DR);
	}
#else
	if (bit) {
		gpio_direction_input(GPIO_I2C_SDA);
	} else {
		gpio_direction_output(GPIO_I2C_SDA,bit);
	}
#endif
}

static void i2c_gpio_sda_wset(int bit)
{
#ifdef GPIO_REG_RW
	unsigned int reg=0;
	reg=readl(GPIO1_BASE_ADDR+ GPIO_GDIR);
	reg |= (1<<29);
	writel(reg, GPIO1_BASE_ADDR+ GPIO_GDIR);
	if(bit==0)
	{
		reg=readl(GPIO1_BASE_ADDR+ GPIO_DR);
		reg &= ~(1<<29);
		writel(reg, GPIO1_BASE_ADDR+ GPIO_DR);
	}
	else
	{
		reg=readl(GPIO1_BASE_ADDR+ GPIO_DR);
		reg |= (1<<29);
		writel(reg, GPIO1_BASE_ADDR+ GPIO_DR);
	}
#else
	gpio_direction_output(GPIO_I2C_SDA,bit);
#endif
}

static void i2c_gpio_scl_set(int bit)
{
#ifdef GPIO_REG_RW
	unsigned int reg=0;
	reg=readl(GPIO1_BASE_ADDR+ GPIO_GDIR);
	reg |= (1<<28);
	writel(reg, GPIO1_BASE_ADDR+ GPIO_GDIR);
	if(bit==0)
	{
		reg=readl(GPIO1_BASE_ADDR+ GPIO_DR);
		reg &= ~(1<<28);
		writel(reg, GPIO1_BASE_ADDR+ GPIO_DR);
	}
	else
	{
		reg=readl(GPIO1_BASE_ADDR+ GPIO_DR);
		reg |= (1<<28);
		writel(reg, GPIO1_BASE_ADDR+ GPIO_DR);
	}
#else
	gpio_direction_output(GPIO_I2C_SCL,bit);
#endif
}

static void i2c_gpio_write_bit(uchar bit)
{
	i2c_gpio_scl_set(0);
	I2C_DELAY;
	i2c_gpio_sda_set(bit);
	I2C_DELAY;
	i2c_gpio_scl_set(1);
	I2C_DELAY;
	I2C_DELAY;
}

static void i2c_gpio_wwrite_bit(uchar bit)
{
	i2c_gpio_scl_set(0);
	I2C_DELAY;
	i2c_gpio_sda_wset(bit);
	I2C_DELAY;
	i2c_gpio_scl_set(1);
	I2C_DELAY;
	I2C_DELAY;
}

static int i2c_gpio_check_nack(void)
{
	int count;
	int value;

	i2c_gpio_scl_set(1);
	//I2C_DELAY;
	count=0;
	while(count<=(I2C_DELAY_TIME*2))
	{
		count++;
		value = i2c_gpio_sda_get();
		if(value==0)break;
		udelay(1);
	}
	I2C_DELAY;
	i2c_gpio_scl_set(0);
	I2C_DELAY;
	I2C_DELAY;
	return value;
}

static int i2c_gpio_read_bit(void)
{
	int value;

	i2c_gpio_scl_set(1);
	I2C_DELAY;
	value = i2c_gpio_sda_get();
	I2C_DELAY;
	i2c_gpio_scl_set(0);
	I2C_DELAY;
	I2C_DELAY;
	return value;
}

/* START: High -> Low on SDA while SCL is High */
static void i2c_gpio_send_start(void)
{
	I2C_DELAY;
	i2c_gpio_sda_set(1);
	I2C_DELAY;
	i2c_gpio_scl_set(1);
	I2C_DELAY;
	i2c_gpio_sda_set(0);
	I2C_DELAY;
}

/* STOP: Low -> High on SDA while SCL is High */
static void i2c_gpio_send_stop(void)
{
	i2c_gpio_scl_set(0);
	I2C_DELAY;
	i2c_gpio_sda_set(0);
	I2C_DELAY;
	i2c_gpio_scl_set(1);
	I2C_DELAY;
	i2c_gpio_sda_set(1);
	I2C_DELAY;
}

/* ack should be I2C_ACK or I2C_NOACK */
static void i2c_gpio_send_ack(int ack)
{
	i2c_gpio_write_bit(ack);
	i2c_gpio_scl_set(0);
	I2C_DELAY;
}

static void i2c_gpio_send_reset(void)
{
	int j;
	for (j = 0; j < 9; j++)
		i2c_gpio_write_bit(1);
	i2c_gpio_send_stop();
}

/* Set sda high with low clock, before reading slave data */
static void i2c_gpio_sda_high(void)
{
	i2c_gpio_scl_set(0);
	I2C_DELAY;
	i2c_gpio_sda_set(1);
	I2C_DELAY;
}

/* Send 8 bits and look for an acknowledgement */
static int i2c_gpio_write_byte(uchar data)
{
	int j;
	int nack;

	for (j = 0; j < 8; j++) {
		i2c_gpio_write_bit(data & 0x80);
		data <<= 1;
	}
	I2C_DELAY;
	/* Look for an <ACK>(negative logic) and return it */
	i2c_gpio_sda_high();
	nack = i2c_gpio_check_nack();
	return nack;	/* not a nack is an ack */
}

static int i2c_gpio_wwrite_byte(uchar data)
{
	int j;
	int nack;

	for (j = 0; j < 8; j++) {
		i2c_gpio_wwrite_bit(data & 0x80);
		data <<= 1;
	}
	I2C_DELAY;
	/* Look for an <ACK>(negative logic) and return it */
	i2c_gpio_sda_high();
	nack = i2c_gpio_check_nack();
	return nack;	/* not a nack is an ack */
}

static uchar i2c_gpio_read_byte(int ack)
{
	int  data;
	int  j;

	i2c_gpio_sda_high();
	data = 0;
	for (j = 0; j < 8; j++) {
		data <<= 1;
		data |= i2c_gpio_read_bit();
	}
	i2c_gpio_send_ack(ack);
	return data;
}

unsigned int i2c_gpio_write_eeprom(uint addr, int alen, uchar *buffer, int len)
{
	int shift, failures = 0;
	int index=0;
	char showt[32]={0};
	uchar wrbyte=0x0;

	i2c_gpio_send_start();
	if(i2c_gpio_write_byte(EEPROM_I2C_ADDRESS << 1)) {	/* write cycle */
		i2c_gpio_send_stop();
		puts("i2c_write, no chip responded\n");
		return(1);
	}
	shift = (alen-1) * 8;
	while(alen-- > 0) {
		if(i2c_gpio_write_byte(addr >> shift)) {
			puts("i2c_write, address not <ACK>ed\n");
			return(1);
		}
		shift -= 8;
	}

	for(index=0;index<len;index++)
	{
		wrbyte=buffer[index];
		if(i2c_gpio_wwrite_byte(wrbyte)) 
		{
			failures++;
		}
	}
	i2c_gpio_send_stop();
	udelay(100);
	i2c_gpio_send_reset();
	if(failures)
	{
		sprintf(showt," Error count:%d\n",failures);
		puts(showt);
	}
	udelay( 11000 );
	return(failures);
}

unsigned int i2c_gpio_read_eeprom(uint addr, int alen, uchar *buffer, int len)
{
	int shift;
	i2c_gpio_send_start();
	if(alen > 0) 
	{
		if(i2c_gpio_write_byte(EEPROM_I2C_ADDRESS << 1)) 
		{ /* write cycle */
			i2c_gpio_send_stop();
			puts("i2c_gpio_write_byte, Error !\n");
			return(1);
		}
		shift = (alen-1) * 8;
		while(alen-- > 0) 
		{
			if(i2c_gpio_write_byte(addr >> shift)) 
			{
				puts("i2c_read, address not <ACK>ed\n");
				return(1);
			}
			shift -= 8;
		}
	
		/* Some I2C chips need a stop/start sequence here,
		 * other chips don't work with a full stop and need
		 * only a start.  Default behaviour is to send the
		 * stop/start sequence.
		 */
#ifdef CONFIG_SOFT_I2C_READ_REPEATED_START
		i2c_gpio_send_start();
#else
		i2c_gpio_send_stop();
		i2c_gpio_send_start();
#endif
	}
	/*
	 * Send the chip address again, this time for a read cycle.
	 * Then read the data.	On the last byte, we do a NACK instead
	 * of an ACK(len == 0) to terminate the read.
	 */
	i2c_gpio_write_byte((EEPROM_I2C_ADDRESS << 1) | 1);	/* read cycle */
	while(len-- > 0) {
		*buffer++ = i2c_gpio_read_byte(len == 0);
	}
	i2c_gpio_send_stop();
	return(0);
}

static int i2c_gpio_eeprom_init(void)
{
	int ret;
	
	 i2c_gpio_send_start();
	 ret = i2c_gpio_write_byte((EEPROM_I2C_ADDRESS << 1) | 0);
	 i2c_gpio_send_stop();
	return ret;
}

#else
unsigned int eeprom_i2c_read( unsigned int addr, int alen, uint8_t *buffer, int len )
{
	i2c_set_bus_num( EEPROM_I2C_BUS );

	if ( i2c_read( EEPROM_I2C_ADDRESS, addr, alen, buffer, len ) )
	{
		puts( "I2C read failed in eeprom_i2c_read()\n" );
	}

	udelay( 10 );
	return(0);
}

unsigned int eeprom_i2c_write( unsigned int addr, int alen, uint8_t *buffer, int len )
{
	i2c_set_bus_num( EEPROM_I2C_BUS );

	if ( i2c_write( EEPROM_I2C_ADDRESS, addr, alen, buffer, len ) )
	{
		puts( "I2C write failed in eeprom_i2c_write()\n" );
	}
	udelay( 11000 );
	return(0);
}
#endif
int eeprom_i2c_parse_data(void)
{
	uchar buffer[128]={0};
	int i=0;
	int datalength=0;

	//if (AT24c02_eeprom.read( 0xfe, 1, buffer, 2)) {
#ifdef CONFIG_EEPROM_GPIO_I2C4
		if (i2c_gpio_read_eeprom( 0xfe, 1, buffer, 2)) {
#else
		if (eeprom_i2c_read( 0xfe, 1, buffer, 2)) {
#endif
		puts("I2C read failed in eeprom 0xf8()\n");
		AT24c02_eeprom.version=0x00;
		return 0;
	}

//	sprintf(buffer,"bAT24c02_eeprom.version(0x%04x)\n",AT24c02_eeprom.version);
	AT24c02_eeprom.version=0+buffer[0]+(buffer[1]<<8);
	if(AT24c02_eeprom.version<0x0 || AT24c02_eeprom.version>0xf000)
	{
		AT24c02_eeprom.version=0x00;
	}
	else
	{
		//AT24c02_eeprom.read( 0x00, 1, AT24c02_eeprom.content, 0x40);
#ifdef CONFIG_EEPROM_GPIO_I2C4
		i2c_gpio_read_eeprom( 0x00, 1, AT24c02_eeprom.content, 0x40);
#else
		eeprom_i2c_read( 0x00, 1, AT24c02_eeprom.content, 0x40);
#endif
	}
//	sprintf(buffer,"aAT24c02_eeprom.version(0x%04x)\n",AT24c02_eeprom.version);
//	puts(buffer);
	memset(&AT24c02_eeprom.data,0,sizeof(AT24c02_eeprom.data));
	AT24c02_eeprom.data.version=3;		
	i=0;
	datalength=0;
	while(i<0x40)
	{
		datalength=AT24c02_eeprom.content[i+1]+2;
		switch(AT24c02_eeprom.content[i])
		{
			default:
			case 0x00:
				return 0;
			case 0x01:
				memcpy(AT24c02_eeprom.data.mac1,AT24c02_eeprom.content+i,datalength);
				break;
			case 0x02:
				memcpy(AT24c02_eeprom.data.mac2,AT24c02_eeprom.content+i,datalength);
				break;
			case 0x03:
				memcpy(AT24c02_eeprom.data.softid,AT24c02_eeprom.content+i,datalength);
				break;
			case 0x04:
				memcpy(AT24c02_eeprom.data.backlight,AT24c02_eeprom.content+i,datalength);
				break;
			case 0x10:
				memcpy(AT24c02_eeprom.data.display,AT24c02_eeprom.content+i,datalength);
				break;
			case 0x11:
				memcpy(AT24c02_eeprom.data.logo,AT24c02_eeprom.content+i,datalength);
				break;
		}
		AT24c02_eeprom.size=i+datalength;
		i=i+datalength;
	}	
	return AT24c02_eeprom.version;
}

uchar datatransfer(uchar h4b,uchar l4b)
{
	uchar dtr=0x0;
	if(h4b>='0' && h4b<='9')
		dtr=(h4b-'0')<<4;
	else if(h4b>='a' && h4b<='f')
		dtr=(h4b-'a'+10)<<4;
	else if(h4b>='A' && h4b<='F')
		dtr=(h4b-'A'+10)<<4;
	
	if(l4b>='0' && l4b<='9')
		dtr+=(l4b-'0');
	else if(l4b>='a' && l4b<='f')
		dtr+=(l4b-'a'+10);
	else if(l4b>='A' && l4b<='F')
		dtr+=(l4b-'A'+10);
	return dtr;
}

int atoi(char *string)
{
	int length;
	int retval = 0;
	int i;
	int sign = 1;

	length = strlen(string);
	for (i = 0; i < length; i++) {
		if (0 == i && string[0] == '-') {
			sign = -1;
			continue;
		}
		if (string[i] > '9' || string[i] < '0') {
			break;
		}
		retval *= 10;
		retval += string[i] - '0';
	}
	retval *= sign;
	return retval;
}

u32 atoilength(char *string,int len)
{
	int length;
	u64 retval = 0;
	int i;
	u64 sign = 1;

	length = strlen(string);
	if(len<length)length=len;
	for (i = 0; i < length; i++) {
		if (0 == i && string[0] == '-') {
			sign = -1;
			continue;
		}
		if (string[i] > '9' || string[i] < '0') {
			break;
		}
		retval *= 10;
		retval += string[i] - '0';
	}
	retval *= sign;
	return retval;
}

int eeprom_i2c_synthesis_data(void)
{
	char tmp[128];
	uchar eepromtmp[10];

	puts("nMAC: ");
	sprintf(tmp,"write_systeminfo_eeprom:%02x:%02x:%02x:%02x:%02x:%02x\n",AT24c02_eeprom.data.mac1[2],AT24c02_eeprom.data.mac1[3],AT24c02_eeprom.data.mac1[4],AT24c02_eeprom.data.mac1[5],AT24c02_eeprom.data.mac1[6],AT24c02_eeprom.data.mac1[7]);
	puts(tmp);
	int write_size_offset=0;
	AT24c02_eeprom.size=0;
	memset(AT24c02_eeprom.content,0,sizeof(AT24c02_eeprom.content));
	if(AT24c02_eeprom.data.mac1[0]!=0 && AT24c02_eeprom.data.mac1[1]!=0)
	{
		memcpy(AT24c02_eeprom.content+AT24c02_eeprom.size,AT24c02_eeprom.data.mac1,AT24c02_eeprom.data.mac1[1]+2);
		AT24c02_eeprom.size+=(AT24c02_eeprom.data.mac1[1]+2);
	}
	if(AT24c02_eeprom.data.mac2[0]!=0 && AT24c02_eeprom.data.mac2[1]!=0)
	{
		memcpy(AT24c02_eeprom.content+AT24c02_eeprom.size,AT24c02_eeprom.data.mac2,AT24c02_eeprom.data.mac2[1]+2);
		AT24c02_eeprom.size+=(AT24c02_eeprom.data.mac2[1]+2);
	}
	if(AT24c02_eeprom.data.softid[0]!=0 && AT24c02_eeprom.data.softid[1]!=0)
	{
		memcpy(AT24c02_eeprom.content+AT24c02_eeprom.size,AT24c02_eeprom.data.softid,AT24c02_eeprom.data.softid[1]+2);
		AT24c02_eeprom.size+=(AT24c02_eeprom.data.softid[1]+2);
	}
	if(AT24c02_eeprom.data.backlight[0]!=0 && AT24c02_eeprom.data.backlight[1]!=0)
	{
		memcpy(AT24c02_eeprom.content+AT24c02_eeprom.size,AT24c02_eeprom.data.backlight,AT24c02_eeprom.data.backlight[1]+2);
		AT24c02_eeprom.size+=(AT24c02_eeprom.data.backlight[1]+2);
	}
	if(AT24c02_eeprom.data.display[0]!=0 && AT24c02_eeprom.data.display[1]!=0)
	{
		memcpy(AT24c02_eeprom.content+AT24c02_eeprom.size,AT24c02_eeprom.data.display,AT24c02_eeprom.data.display[1]+2);
		AT24c02_eeprom.size+=(AT24c02_eeprom.data.display[1]+2);
	}
	if(AT24c02_eeprom.data.logo[0]!=0 && AT24c02_eeprom.data.logo[1]!=0)
	{
		memcpy(AT24c02_eeprom.content+AT24c02_eeprom.size,AT24c02_eeprom.data.logo,AT24c02_eeprom.data.logo[1]+2);
		AT24c02_eeprom.size+=(AT24c02_eeprom.data.logo[1]+2);
	}

	while(write_size_offset<AT24c02_eeprom.size)
	{
		//AT24c02_eeprom.write(write_size_offset, 1, AT24c02_eeprom.content+write_size_offset, 8);
#ifdef CONFIG_EEPROM_GPIO_I2C4
		i2c_gpio_write_eeprom(write_size_offset, 1, AT24c02_eeprom.content+write_size_offset, 8);
#else
		eeprom_i2c_write(write_size_offset, 1, AT24c02_eeprom.content+write_size_offset, 8);
#endif
		write_size_offset+=8;
	}
	AT24c02_eeprom.version=AT24c02_eeprom.data.version;
	eepromtmp[0]=0xff & AT24c02_eeprom.data.version;
	eepromtmp[1]=0xff & (AT24c02_eeprom.data.version>>8);
	//AT24c02_eeprom.write(0xfe, 1, eepromtmp, 2);
#ifdef CONFIG_EEPROM_GPIO_I2C4
	i2c_gpio_write_eeprom(0xfe, 1, eepromtmp, 2);
#else
	eeprom_i2c_write(0xfe, 1, eepromtmp, 2);
#endif
	sprintf(tmp,"AT24c02_eeprom.size:%d\n",AT24c02_eeprom.size);
	puts(tmp);

	return AT24c02_eeprom.version;
}

int Load_config_from_mmc(void)
{
	struct mmc *mmc;

	mmc = find_mmc_device(1);
	if (mmc) 
	{
		mmc_init(mmc);
	}
	else
	{
		puts("No sdcrd found!!!\n");
	}
	if (mmc) 
	{
		long size;
		//int i;
		char tmp[128];
		uchar tbuffer[512]={0};
		//uchar buffer[128]={0};
		char * ptr;
		struct blk_desc *dev_desc=NULL;
		unsigned int read_version=0;
		//unsigned int tmpversion=AT24c02_eeprom.data.version;
		unsigned int prm_check=0;
				
		//dev_desc=get_dev("mmc",1);
		dev_desc = blk_get_dev("mmc", 1);
		if (dev_desc!=NULL) 
		{
			if (fat_register_device((struct blk_desc *)dev_desc,1)==0) 
			{
				size = file_fat_read ("jo6wu06.cfg", (void *) tbuffer, sizeof(tbuffer));
				if(size<=0)return -1;
				puts((char *)tbuffer);
				puts("\n");
				ptr = strstr((const char *)tbuffer, "Version=");
				if (ptr != NULL) 
				{
					read_version=atoi(ptr+8);
					if(read_version<0 || read_version>0xF000)
						read_version=AT24c02_eeprom.version;
				}
				else
				{
					read_version=AT24c02_eeprom.version;
				}
				sprintf(tmp,"Version=0x%04x\n",read_version);
				puts(tmp);
				if(read_version!=0x03)
				{
					return -2;
				}
				ptr = strstr((const char *)tbuffer, "MAC1=");
				if (ptr != NULL)
				{
					//70:B3:D5:10:6F:56 
					//70:B3:D5:10:6F:57
					//00:11:22:33:44:55
					AT24c02_eeprom.show_pass_logo++;
					ptr+=5;
					memset(tmp,0x0,sizeof(tmp));
					tmp[0]=datatransfer(ptr[0], ptr[1]);
					tmp[1]=datatransfer(ptr[3], ptr[4]);
					tmp[2]=datatransfer(ptr[6], ptr[7]);
					tmp[3]=datatransfer(ptr[9], ptr[10]);
					tmp[4]=datatransfer(ptr[12],ptr[13]);
					tmp[5]=datatransfer(ptr[15],ptr[16]);
					if (is_valid_ethaddr((const u8 *)tmp))
					{
						AT24c02_eeprom.data.mac1[0]=0x01;
						AT24c02_eeprom.data.mac1[1]=0x06;
						memcpy(AT24c02_eeprom.data.mac1+2,tmp,6);
					}
					//sprintf(tmp,"%02x:%02x:%02x:%02x:%02x:%02x",AT24c02_eeprom.data.mac1[2],AT24c02_eeprom.data.mac1[3],AT24c02_eeprom.data.mac1[4],AT24c02_eeprom.data.mac1[5],AT24c02_eeprom.data.mac1[6],AT24c02_eeprom.data.mac1[7]);
					//puts("nMAC: ");
					//puts(tmp);
					//puts("\n");
				}
				ptr = strstr((const char *)tbuffer, "Software_part_number=");
				if (ptr != NULL) 
				{
					u32 softidhigh=0;
					u32 softidlow=0;
					AT24c02_eeprom.show_pass_logo++;
					softidhigh=(u32)(atoilength(ptr+21,4));
					softidlow=(u32)(atoilength(ptr+21+4,8));
					sprintf(tmp,"Software_part_number=(%04u)(%08u)\n",softidhigh,softidlow);
					puts(tmp);
					if(softidhigh<=9999 && softidhigh>0 && softidlow<=99999999 && softidlow>0)
					{
						AT24c02_eeprom.data.softid[0]=0x03;
						AT24c02_eeprom.data.softid[1]=0x06;
						AT24c02_eeprom.data.softid[2]=(softidhigh>>8)&0xff;
						AT24c02_eeprom.data.softid[3]=(softidhigh>>0)&0xff;
						AT24c02_eeprom.data.softid[4]=(softidlow>>24)&0xff;
						AT24c02_eeprom.data.softid[5]=(softidlow>>16)&0xff;
						AT24c02_eeprom.data.softid[6]=(softidlow>> 8)&0xff;
						AT24c02_eeprom.data.softid[7]=(softidlow>> 0)&0xff;
					}
				}

				//for backlight pwm parameter
				prm_check=0;
				ptr = strstr((const char *)tbuffer, "Backlight_polarity=");
				if (ptr != NULL) 
				{
					prm_check++;
					AT24c02_eeprom.data.backlight[2]=(uchar)(atoi(ptr+19)&0xff)==1?1:0;
				}
				ptr = strstr((const char *)tbuffer, "Backlight_min=");
				if (ptr != NULL) 
				{
					prm_check++;
					AT24c02_eeprom.data.backlight[3]=(uchar)(atoi(ptr+14)&0xff);
				}
				ptr = strstr((const char *)tbuffer, "Backlight_frequency=");
				if (ptr != NULL) 
				{
					int read_backlight_frequency=atoi(ptr+20);
					prm_check++;
					AT24c02_eeprom.data.backlight[4]=(read_backlight_frequency>>8)&0xff;
					AT24c02_eeprom.data.backlight[5]=(read_backlight_frequency)&0xff;
				}
#ifdef BACKLIGHT_MAX
				ptr = strstr((const char *)tbuffer, "Backlight_max=");
				if (ptr != NULL) 
				{
					prm_check++;
					AT24c02_eeprom.data.backlight[6]=(uchar)(atoi(ptr+14)&0xff);
				}
#endif
				if(prm_check)
				{
					int check_backlight_frequency=AT24c02_eeprom.data.backlight[4]*256+AT24c02_eeprom.data.backlight[5];
					AT24c02_eeprom.show_pass_logo++;
					AT24c02_eeprom.data.backlight[0]=0x04;
#ifdef BACKLIGHT_MAX
					AT24c02_eeprom.data.backlight[1]=0x05;
#else
					AT24c02_eeprom.data.backlight[1]=0x04;
#endif
					if(AT24c02_eeprom.data.backlight[2]!=1 &&AT24c02_eeprom.data.backlight[2]!=0)
						AT24c02_eeprom.data.backlight[2]=0;
#ifdef BACKLIGHT_MAX
					if(AT24c02_eeprom.data.backlight[6]<70 || AT24c02_eeprom.data.backlight[6]>236)
						AT24c02_eeprom.data.backlight[6]=236;
					if(AT24c02_eeprom.data.backlight[6]<AT24c02_eeprom.data.backlight[3])
						AT24c02_eeprom.data.backlight[3]=AT24c02_eeprom.data.backlight[6]/2;
#endif
					if(AT24c02_eeprom.data.backlight[3]<0 || AT24c02_eeprom.data.backlight[3]>100)//default :0
						AT24c02_eeprom.data.backlight[3]=0;
					if(check_backlight_frequency<50 || check_backlight_frequency>50000)
					{
						check_backlight_frequency=50000;
						AT24c02_eeprom.data.backlight[4]=(check_backlight_frequency>>8)&0xff;
						AT24c02_eeprom.data.backlight[5]=(check_backlight_frequency)&0xff;
					}
				}

				//for display parameter
				prm_check=0;
				ptr = strstr((const char *)tbuffer, "Resolution_ID=");
				if (ptr != NULL) 
				{
					prm_check++;
					AT24c02_eeprom.data.display[2]=(uchar)(atoi(ptr+14)&0xff);
				}
				ptr = strstr((const char *)tbuffer, "Color_depth=");
				if (ptr != NULL) 
				{
					prm_check++;
					AT24c02_eeprom.data.display[3]=(uchar)(atoi(ptr+12)&0xff);
				}
				ptr = strstr((const char *)tbuffer, "Frame_rate=");
				if (ptr != NULL) 
				{
					prm_check++;
					AT24c02_eeprom.data.display[4]=(uchar)(atoi(ptr+11)&0xff);
				}
				ptr = strstr((const char *)tbuffer, "Interface=");
				if (ptr != NULL) 
				{
					prm_check++;
					AT24c02_eeprom.data.display[5]=(uchar)(atoi(ptr+10)&0xff);
				}
				if(prm_check)
				{
					AT24c02_eeprom.show_pass_logo++;
					AT24c02_eeprom.data.display[0]=0x10;
					AT24c02_eeprom.data.display[1]=0x04;
					if(AT24c02_eeprom.data.display[2]<RESOLUTION_640X480 || AT24c02_eeprom.data.display[2]>RESOLUTION_1920X1080)
						AT24c02_eeprom.data.display[2]=RESOLUTION_800X480;
					if(AT24c02_eeprom.data.display[3]!=0x12 && AT24c02_eeprom.data.display[3]!=0x18)
						AT24c02_eeprom.data.display[3]=0x12;
					if(AT24c02_eeprom.data.display[4]<0x28 || AT24c02_eeprom.data.display[4]>0x64)
						AT24c02_eeprom.data.display[4]=0x3c;
					//
					if(AT24c02_eeprom.data.display[5]<1 || AT24c02_eeprom.data.display[5]>3)
						AT24c02_eeprom.data.display[5]=1;
				}
				ptr = strstr((const char *)tbuffer, "Logo=");
				if (ptr != NULL) 
				{
					uchar read_boot_logo=AT24c02_eeprom.data.logo[2];
					AT24c02_eeprom.show_pass_logo++;
					read_boot_logo=(uchar)(atoi(ptr+5)&0xff);
					if(read_boot_logo<1 || read_boot_logo>5)
					{
						read_boot_logo=0;
					}
					AT24c02_eeprom.data.logo[0]=0x11;
					AT24c02_eeprom.data.logo[1]=0x01;
					AT24c02_eeprom.data.logo[2]=0xff&read_boot_logo;
				}
				ptr = strstr((const char *)tbuffer, "Halt");
				if (ptr != NULL) 
				{
					AT24c02_eeprom.mHalt=1;
				}
				ptr = strstr((const char *)tbuffer, "bootdelay=");
				if (ptr != NULL) 
				{
					AT24c02_eeprom.mbootdelay=(uchar)(atoi(ptr+10)&0xff);
				}
				if(AT24c02_eeprom.show_pass_logo)
					eeprom_i2c_synthesis_data();
			}
		}
	}
	return 0;
}

unsigned char eeprom_i2c_get_type(void)
{
	return AT24c02_eeprom.data.display[5];
}

unsigned char eeprom_i2c_get_color_depth(void)
{
	if(AT24c02_eeprom.data.display[3]!=0x18)
		AT24c02_eeprom.data.display[3]=0x12;
	return AT24c02_eeprom.data.display[3];
}

unsigned char eeprom_i2c_pass_logo(void)
{
	return AT24c02_eeprom.show_pass_logo;
}

unsigned char eeprom_i2c_check_logo(void)
{
	return AT24c02_eeprom.data.logo[2];
}

unsigned char eeprom_i2c_get_EDID(void)
{
	return AT24c02_eeprom.data.display[2];
}
//#define CMDLINE_ADD_QUIET
#define CONSOLE_READWRITE_ABLE
void set_kernel_env(int width, int height)
{
	char videoprm[128]={0};
	char Backlightprm[128]={0};
#ifdef MX6_SABRE_ANDROID_COMMON_H
	char consoleprm[128]={0};
#endif
	char envprm[512]={0};

	setenv("bootdelay","0");
	if(AT24c02_eeprom.data.display[4]<30 || AT24c02_eeprom.data.display[4]>100)
		AT24c02_eeprom.data.display[4]=60;
	if(AT24c02_eeprom.data.display[3]!=18 && AT24c02_eeprom.data.display[3]!=24)
		AT24c02_eeprom.data.display[3]=18;
//
	switch(AT24c02_eeprom.data.display[5])
	{//0x01:lvds, 0x02:hdmi, 0x03:RGB
		case 0x02:
#ifdef CONFIG_GALCORE_VIVANTE
			sprintf(videoprm,"video=mxcfb0:dev=hdmi,%dx%dM@%u,if=RGB%d,bpp=32 video=mxcfb1:off video=mxcfb2:off vmalloc=384M galcore.gpuProfiler=1 ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24);
#else
			sprintf(videoprm,"video=mxcfb0:dev=hdmi,%dx%dM@%u,if=RGB%d,bpp=32 video=mxcfb1:off video=mxcfb2:off vmalloc=384M ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24);
#endif
//			sprintf(videoprm,"video=mxcfb0:dev=hdmi,%dx%dM@%u,if=RGB24,bpp=32 video=mxcfb1:off video=mxcfb2:off vmalloc=%dM", width, height, AT24c02_eeprom.data.display[4],192+24*AT24c02_eeprom.data.display[2]);
			break;
		case 0x01:
		default:
			AT24c02_eeprom.data.display[5]=0x01;
#ifndef CONFIG_EDID_EEPROM_I2C2
			if(eeprom_i2c_get_EDID()==RESOLUTION_1920X1080)
			{
	#ifdef CONFIG_GALCORE_VIVANTE
				sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=spl%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M galcore.gpuProfiler=1 ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#else
				sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=spl%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#endif
			}
			else
			{
	#ifdef CONFIG_GALCORE_VIVANTE
				sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=sin%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M galcore.gpuProfiler=1 ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#else
				sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=sin%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#endif
			}
#else
			// use EDID config
			if(edid_eeprom.efficient_config == 0) 
			{
				if(get_edid_eeprom_resolution_num() == RESOLUTION_1920X1080)
	#ifdef CONFIG_GALCORE_VIVANTE
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=spl%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M galcore.gpuProfiler=1 ", width, height, edid_eeprom.mode.refresh, edid_eeprom.color_depth == 18 ? 666 : 24, LVDS_PORT);
	#else
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=spl%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M ", width, height, edid_eeprom.mode.refresh, edid_eeprom.color_depth == 18 ? 666 : 24, LVDS_PORT);
	#endif
				else
	#ifdef CONFIG_GALCORE_VIVANTE
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=sin%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M galcore.gpuProfiler=1 ", width, height, edid_eeprom.mode.refresh, edid_eeprom.color_depth == 18 ? 666 : 24, LVDS_PORT);
	#else
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=sin%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M ", width, height, edid_eeprom.mode.refresh, edid_eeprom.color_depth == 18 ? 666 : 24, LVDS_PORT);
	#endif
			}
			else 
			{ 
				// use SD card config
				if(eeprom_i2c_get_EDID()==RESOLUTION_1920X1080)
	#ifdef CONFIG_GALCORE_VIVANTE
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=spl%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M galcore.gpuProfiler=1 ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#else
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=spl%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#endif
				else
	#ifdef CONFIG_GALCORE_VIVANTE
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=sin%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M galcore.gpuProfiler=1 ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#else
					sprintf(videoprm,"video=mxcfb0:dev=ldb,%dx%dM@%u,if=RGB%d,bpp=32 ldb=sin%d video=mxcfb1:off video=mxcfb2:off vmalloc=384M ", width, height, AT24c02_eeprom.data.display[4], AT24c02_eeprom.data.display[3]==18?666:24, LVDS_PORT);
	#endif
			}
#endif
			break;
	}
#ifdef MX6_SABRE_ANDROID_COMMON_H
	#ifdef CONSOLE_READWRITE_ABLE
		#ifdef CMDLINE_ADD_QUIET
	sprintf(consoleprm,"androidboot.console=ttymxc0 androidboot.selinux=disabled consoleblank=0 quiet");
		#else
	sprintf(consoleprm,"androidboot.console=ttymxc0 androidboot.selinux=disabled consoleblank=0");
		#endif
	#else
		#ifdef CMDLINE_ADD_QUIET
	sprintf(consoleprm,"androidboot.console=ttymxc0 consoleblank=0 quiet");
		#else
	sprintf(consoleprm,"androidboot.console=ttymxc0 consoleblank=0");
		#endif
	#endif
	if(AT24c02_eeprom.data.backlight[0]==0x04 && AT24c02_eeprom.data.backlight[1]==0x04)
	{
		int check_backlight_frequency=AT24c02_eeprom.data.backlight[4]*256+AT24c02_eeprom.data.backlight[5];
	#ifdef CONFIG_SBC7819
		check_backlight_frequency=200;
	#endif
	#ifndef BACKLIGHT_MAX
		sprintf(Backlightprm," Backlight_polarity=%d,Backlight_min=%d,Backlight_frequency=%d",AT24c02_eeprom.data.backlight[2]?1:0,AT24c02_eeprom.data.backlight[3],check_backlight_frequency);
	#else
		sprintf(Backlightprm," Backlight_polarity=%d,Backlight_min=%d,Backlight_frequency=%d,Backlight_max=%d",AT24c02_eeprom.data.backlight[2]?1:0,AT24c02_eeprom.data.backlight[3],check_backlight_frequency,AT24c02_eeprom.data.backlight[6]);
	#endif
		sprintf(envprm,"console=ttymxc0,115200 init=/init %s %s %s androidboot.hardware=freescale cma=384M usbcore.autosuspend=-1",videoprm,consoleprm,Backlightprm);
	}
	else
	{
		sprintf(envprm,"console=ttymxc0,115200 init=/init %s %s androidboot.hardware=freescale cma=384M usbcore.autosuspend=-1",videoprm,consoleprm);
	}
	setenv("bootargs",envprm);
#else
	//mmcargs=setenv bootargs console=${console},${baudrate} ${smp}  root=${mmcroot}\0"	
	if(AT24c02_eeprom.data.backlight[0]==0x04 && AT24c02_eeprom.data.backlight[1]>=0x04)
	{
		int check_backlight_frequency=AT24c02_eeprom.data.backlight[4]*256+AT24c02_eeprom.data.backlight[5];
	#ifdef CONFIG_SBC7819
		check_backlight_frequency=200;
	#endif
	#ifndef BACKLIGHT_MAX
		sprintf(Backlightprm," Backlight_polarity=%d,Backlight_min=%d,Backlight_frequency=%d",AT24c02_eeprom.data.backlight[2]?1:0,AT24c02_eeprom.data.backlight[3],check_backlight_frequency);
	#else
		sprintf(Backlightprm," Backlight_polarity=%d,Backlight_min=%d,Backlight_frequency=%d,Backlight_max=%d",AT24c02_eeprom.data.backlight[2]?1:0,AT24c02_eeprom.data.backlight[3],check_backlight_frequency,AT24c02_eeprom.data.backlight[6]);
	#endif
	#ifdef CMDLINE_ADD_QUIET
		sprintf(envprm,"setenv bootargs console=${console},${baudrate} ${smp} root=${mmcroot} %s %s cma=384M usbcore.autosuspend=-1 quiet",videoprm,Backlightprm);
	#else
		sprintf(envprm,"setenv bootargs console=${console},${baudrate} ${smp} root=${mmcroot} %s %s cma=384M usbcore.autosuspend=-1",videoprm,Backlightprm);
	#endif
	}
	else
	{
	#ifdef CMDLINE_ADD_QUIET
		sprintf(envprm,"setenv bootargs console=${console},${baudrate} ${smp} root=${mmcroot} %s cma=384M usbcore.autosuspend=-1 quiet",videoprm);
	#else
		sprintf(envprm,"setenv bootargs console=${console},${baudrate} ${smp} root=${mmcroot} %s cma=384M usbcore.autosuspend=-1",videoprm);
	#endif
	}
	setenv("mmcargs",envprm);
#endif
	if(is_valid_ethaddr(AT24c02_eeprom.data.mac1+2))
	{
		sprintf(envprm,"%02x:%02x:%02x:%02x:%02x:%02x", AT24c02_eeprom.data.mac1[2], AT24c02_eeprom.data.mac1[3], AT24c02_eeprom.data.mac1[4], AT24c02_eeprom.data.mac1[5], AT24c02_eeprom.data.mac1[6], AT24c02_eeprom.data.mac1[7]);
		setenv("ethaddr",envprm);
		setenv("fec_addr",envprm);
	}
	if(AT24c02_eeprom.mHalt>0)
		setenv("Halt","y");
	else
	{
		if(AT24c02_eeprom.mbootdelay>0)
		{
			sprintf(envprm,"%d", AT24c02_eeprom.mbootdelay&0xff);
			setenv("mbootdelay",envprm);
		}
		else
			setenv("mbootdelay","0");
	}
}

int eeprom_i2c_init(void)
{
	memset(&AT24c02_eeprom,0,sizeof(AT24c02_eeprom));
 	AT24c02_eeprom.accessable_size = EEPROM_ACCESSABLE_SIZE;
	AT24c02_eeprom.max_size = EEPROM_MAX_SIZE;
	AT24c02_eeprom.i2c_bus = EEPROM_I2C_BUS;
	AT24c02_eeprom.device_addr = EEPROM_I2C_ADDRESS;
	AT24c02_eeprom.address_length = EEPROM_ADDRESS_LENGTH;
	AT24c02_eeprom.mmc_bus = MMC_DEVICE_BUS;

//#ifdef CONFIG_EEPROM_GPIO_I2C4
//	AT24c02_eeprom.read = i2c_gpio_read_eeprom;
//	AT24c02_eeprom.write = i2c_gpio_write_eeprom;
//#else
//	AT24c02_eeprom.read = eeprom_i2c_read;
//	AT24c02_eeprom.write = eeprom_i2c_write;
//#endif
	AT24c02_eeprom.parse_data = eeprom_i2c_parse_data;
	AT24c02_eeprom.synthesis_data = eeprom_i2c_synthesis_data;
	AT24c02_eeprom.load_config = Load_config_from_mmc;

	AT24c02_eeprom.show_pass_logo=0;
	AT24c02_eeprom.mHalt=0;
	AT24c02_eeprom.mbootdelay=0;

#ifdef CONFIG_EEPROM_GPIO_I2C4
	i2c_gpio_eeprom_init();
#endif
	eeprom_i2c_parse_data();
#ifdef CONFIG_EDID_EEPROM_I2C2
	// Hertz 20180524:
	edid_eeprom_i2c_parse_data();
#endif
	return 0;
}

#ifdef CONFIG_EDID_EEPROM_I2C2
static unsigned int edid_eeprom_i2c_read( unsigned int addr, int alen, uint8_t *buffer, int len )
{
#ifdef CONFIG_SYS_I2C
	i2c_set_bus_num( EDID_EEPROM_I2C_BUS );

	if ( i2c_read( EDID_EEPROM_I2C_ADDRESS, addr, alen, buffer, len ) ){
		printf( "I2C read failed in edid_eeprom_i2c_read()\n" );
		return 1;
	}
#else
	int ret;
	ret = i2c_get_chip_for_busnum(EDID_EEPROM_I2C_BUS, EDID_EEPROM_I2C_ADDRESS, 1, &dev);
	if (ret) {
		printf("Cannot find EDID_EEPROM_I2C_ADDRESS: %d\n", ret);
		return 0;
	}

	ret = dm_i2c_read(dev, addr, buffer, len);
	if (ret) {
		printf("Failed to read IO expander value via I2C\n");
		return -EIO;
	}
#endif
	udelay( 10 );
	return(0);
}

#if 0
static unsigned int edid_eeprom_i2c_write( unsigned int addr, int alen, uint8_t *buffer, int len )
{
	i2c_set_bus_num( EDID_EEPROM_I2C_BUS );

	if ( i2c_write( EDID_EEPROM_I2C_ADDRESS, addr, alen, buffer, len ) ){
		printf( "I2C write failed in edid_eeprom_i2c_write()\n" );
	}

	udelay( 11000 );
	return(0);
}
#endif

int edid_eeprom_i2c_parse_data(void)
{
	uchar buffer[128] = {0};
	int i = 0, j = 0;
	//int data_length = 0;
	u32 checksum = 0;
	u32 hblank = 0, vblank = 0;
	
	if (edid_eeprom_i2c_read(0, 1, buffer, 128) == 0) {	
		// print EDID content in debug console.
		printf("\n");
		printf("EDID eeprom memory content at address[0, 127]:\n");
		for(i = 0; i <= 0x07; i++){
			for(j = 0; j <= 0x0f; j++){
				printf("%02X ", buffer[i*0x10 + j]);
			}
			printf("\n");
		}
	}
	
	// verify the checksum
	for(i = 0; i < 127; i++) 
		checksum += buffer[i];
	checksum = 256 - checksum % 256;
	printf(" EDID checksum: %02X\n", checksum);
	
	memset(&edid_eeprom, 0, sizeof(edid_eeprom));
	if(checksum == buffer[127]) // checksum pass
	{
		printf(" EDID checksum okay!\n");
		edid_eeprom.efficient_config = 0;

		// parse useful information
		// Header information.
		edid_eeprom.manufacturer_name = (buffer[9] << 8) + buffer[8]; // 8-9
		edid_eeprom.product_code = (buffer[11] << 8) + buffer[10]; // 10-11
		edid_eeprom.serial_number = (buffer[15] << 24) + (buffer[14] << 16) + (buffer[13] << 8) + buffer[12]; // 12-15
		edid_eeprom.manufacturer_week = buffer[16]; // 16
		edid_eeprom.manufacturer_year = buffer[17]; // 17
		edid_eeprom.edid_major_version = buffer[18]; // 18
		edid_eeprom.edid_minor_version = buffer[19]; // 19
		
		// Video input parameters bitmap.
		edid_eeprom.bit_depth = (buffer[20] & 0x70) >> 4; // 20: Bits 6??. 000=undefined, 001=6, 010=8, 011=10.
		if(edid_eeprom.bit_depth == 1)
                        edid_eeprom.color_depth = 18;
                else
                        edid_eeprom.color_depth = 24;

		// Established timing bitmap. Supported bitmap for (formerly) very common timing modes.
		//edid_eeprom.timing_mode[0] = buffer[35]; // 35-37: 36-Bit3-1024?768 @ 60 Hz
		//edid_eeprom.timing_mode[1] = buffer[36];

        	edid_eeprom.mode.xres = ((buffer[58] & 0xf0) << 4) + buffer[56]; // pixel
        	edid_eeprom.mode.yres = ((buffer[61] & 0xf0) << 4) + buffer[59]; // line
        	edid_eeprom.pixel_frequency = ((buffer[55] << 8) + buffer[54]); // *100 MHz
		edid_eeprom.mode.pixclock = 100000000 / edid_eeprom.pixel_frequency;
        	hblank = ((buffer[58] & 0x0f) << 8) + buffer[57]; // pixel
        	vblank = ((buffer[61] & 0x0f) << 8) + buffer[60]; // line
        	edid_eeprom.mode.hsync_len = ((buffer[65] & 0x30) << 4) + buffer[63]; // pixel
        	edid_eeprom.mode.vsync_len = ((buffer[65] & 0x03) << 4) + (buffer[64] & 0x0f); // line
        	edid_eeprom.mode.left_margin = ((buffer[65] & 0xc0) << 2) + buffer[62]; // pixel
        	edid_eeprom.mode.right_margin = hblank - edid_eeprom.mode.hsync_len - edid_eeprom.mode.left_margin;// pixel
        	edid_eeprom.mode.upper_margin = ((buffer[65] & 0x0c) << 2) + ((buffer[64] & 0xf0) >> 4); // line
        	edid_eeprom.mode.lower_margin = vblank - edid_eeprom.mode.vsync_len - edid_eeprom.mode.upper_margin; // line
        	//edid_eeprom.mode.sync = 0;
        	//edid_eeprom.mode.vmode = 0;
		// calculate frame rate: round(x)
		edid_eeprom.mode.refresh = (1000000 / ((edid_eeprom.mode.xres + hblank) * (edid_eeprom.mode.yres + vblank) / edid_eeprom.pixel_frequency) + 50) /100;
		
		// Detailed timing descriptor
                combine_panel_name(); // edid_eeprom.mode.name = "panel1024x768d24";
	} else {
		printf(" EDID checksum error!\n");
		 
		// if keep " edid_eeprom.efficient_config = 0", we will use defalt EDID config; else we will use the SD card config or defualt SD card config.
		edid_eeprom.efficient_config = 1;
		//edid_eeprom.efficient_config = 0;

		// set default value
		// Header information.
                edid_eeprom.manufacturer_name = 0; // 8-9
                edid_eeprom.product_code = 0; // 10-11
                edid_eeprom.serial_number = 0; // 12-15
                edid_eeprom.manufacturer_week = 0; // 16
                edid_eeprom.manufacturer_year = 0; // 17
                edid_eeprom.edid_major_version = 1; // 18
                edid_eeprom.edid_minor_version = 4; // 19
                #if 0
		// Video input parameters bitmap.
                edid_eeprom.color_depth = 24;
                edid_eeprom.mode.xres = 1024; // pixel
                edid_eeprom.mode.yres = 768; // line
                edid_eeprom.pixel_frequency = 6350; // *100 MHz
                edid_eeprom.mode.pixclock = 15748;
                edid_eeprom.mode.hsync_len = 104; // pixel
                edid_eeprom.mode.vsync_len = 4; // line
                edid_eeprom.mode.left_margin = 48; // pixel
		edid_eeprom.mode.right_margin = 152;// pixel
                edid_eeprom.mode.upper_margin = 3; // line
                edid_eeprom.mode.lower_margin = 23; // line
                edid_eeprom.mode.refresh = 60;
        	sprintf(edid_eeprom.panel_name, "panel1024x768d24");
        	edid_eeprom.mode.name = edid_eeprom.panel_name;
                edid_eeprom.resolution_num = RESOLUTION_1024X768;
		#endif
		#if 1
                // Video input parameters bitmap.
                edid_eeprom.color_depth = 24;
                edid_eeprom.mode.xres = 1920; // pixel
                edid_eeprom.mode.yres = 1080; // line
                edid_eeprom.pixel_frequency = 13850; // 6925; // *100 MHz 13850/2=6925 
                edid_eeprom.mode.pixclock = 7220; // 14440; // 7220;
                edid_eeprom.mode.hsync_len = 32; // pixel
                edid_eeprom.mode.vsync_len = 5; // line
                edid_eeprom.mode.left_margin = 48; // pixel
                edid_eeprom.mode.right_margin = 80;// pixel 160-32-48=80
                edid_eeprom.mode.upper_margin = 3; // line
                edid_eeprom.mode.lower_margin = 23; // line 31-5-3=23
                edid_eeprom.mode.refresh = 60;
                sprintf(edid_eeprom.panel_name, "panel1920x1080d24");
                edid_eeprom.mode.name = edid_eeprom.panel_name;
                edid_eeprom.resolution_num = RESOLUTION_1920X1080;
		#endif
	}

	printf(" efficient_config: %u\n", edid_eeprom.efficient_config);

	if(edid_eeprom.efficient_config == 0) 
	{
		// update the displays[0].
        update_displays_array();
#if 1
        // print all information.
 		printf(" manufacturer_name: %X\n product_code: %X\n serial_number: %X\n manufacturer_week: %u\n\
 				manufacturer_year: %u\n edid_major_version: %u\n edid_minor_version: %u\n color_depth: %u\n\
 				resolution_num: %u\n name: %s\n refresh: %u\n xres: %u\n yres: %u\n\
 				pixel_frequency: %u\n pixclock: %u\n left_margin: %u\n right_margin: %u\n upper_margin: %u\n lower_margin: %u\n\
 				hsync_len: %u\n vsync_len: %u\n",
	                edid_eeprom.manufacturer_name,
	                edid_eeprom.product_code,
	                edid_eeprom.serial_number,
	                edid_eeprom.manufacturer_week,
	                edid_eeprom.manufacturer_year,
	                edid_eeprom.edid_major_version,
	                edid_eeprom.edid_minor_version,
	                edid_eeprom.color_depth,
	                edid_eeprom.resolution_num,
	                edid_eeprom.mode.name,
	                edid_eeprom.mode.refresh,
	                edid_eeprom.mode.xres,
	                edid_eeprom.mode.yres,
	                edid_eeprom.pixel_frequency,
	                edid_eeprom.mode.pixclock,
	                edid_eeprom.mode.left_margin,
	                edid_eeprom.mode.right_margin,
	                edid_eeprom.mode.upper_margin,
	                edid_eeprom.mode.lower_margin,
	                edid_eeprom.mode.hsync_len,
	                edid_eeprom.mode.vsync_len
	        );
#endif
	}

 	return 0;
}
 
unsigned char get_eeprom_efficient_config(void)
{
	return edid_eeprom.efficient_config;
}

unsigned char get_edid_eeprom_color_depth(void)
{
	return edid_eeprom.color_depth;
}

unsigned char get_edid_eeprom_resolution_num(void)
{
	return edid_eeprom.resolution_num;
}

u32 get_edid_eeprom_pixel_frequency(void)
{
	return edid_eeprom.pixel_frequency;
}

u32 get_edid_eeprom_xres(void)
{
	return edid_eeprom.mode.xres;
}

u32 get_edid_eeprom_yres(void)
{
        return edid_eeprom.mode.yres;
}

char * get_edid_eeprom_panel_name(void)
{
	return edid_eeprom.panel_name;
}

static void combine_panel_name(void)
{
	char resolution_type[16] = {0};
	
	sprintf(resolution_type, "%dx%d", edid_eeprom.mode.xres, edid_eeprom.mode.yres);
	sprintf(edid_eeprom.panel_name, "panel%sd%d", resolution_type, edid_eeprom.color_depth);
	edid_eeprom.mode.name = edid_eeprom.panel_name;
	
	if(!strcmp(resolution_type, "640x480"))
		edid_eeprom.resolution_num = RESOLUTION_640X480;
	else if(!strcmp(resolution_type, "800x480"))
                edid_eeprom.resolution_num = RESOLUTION_800X480;
	else if(!strcmp(resolution_type, "800x600"))
                edid_eeprom.resolution_num = RESOLUTION_800X600;
	else if(!strcmp(resolution_type, "1024x600"))
                edid_eeprom.resolution_num = RESOLUTION_1024X600;
	else if(!strcmp(resolution_type, "1024x768"))
                edid_eeprom.resolution_num = RESOLUTION_1024X768;
	else if(!strcmp(resolution_type, "1280x800"))
                edid_eeprom.resolution_num = RESOLUTION_1280X800;
	else if(!strcmp(resolution_type, "1366x768"))
                edid_eeprom.resolution_num = RESOLUTION_1366X768;
	else if(!strcmp(resolution_type, "1920x1080"))
                edid_eeprom.resolution_num = RESOLUTION_1920X1080;
	else {
		edid_eeprom.resolution_num = RESOLUTION_800X480; // set default value
		printf("Input resolution isn't in the list! Please check the resolution of x and y.\n");
	}
}

static void update_displays_array(void)
{
	//displays[0].bus = 1;
	//displays[0].addr = 0;
	if(edid_eeprom.color_depth == 18)
		displays[0].pixfmt = IPU_PIX_FMT_RGB666;
	else
		displays[0].pixfmt = IPU_PIX_FMT_RGB24;
	//displays[0].detect = NULL;
	//displays[0].enable = enable_lvds;
	//memset(displays[0].mode, edid_eeprom.mode, sizeof(edid_eeprom.mode));
	displays[0].mode.name = edid_eeprom.mode.name;
	displays[0].mode.refresh = edid_eeprom.mode.refresh;
	displays[0].mode.xres = edid_eeprom.mode.xres;
	displays[0].mode.yres = edid_eeprom.mode.yres;
	displays[0].mode.pixclock = edid_eeprom.mode.pixclock;
	displays[0].mode.left_margin = edid_eeprom.mode.left_margin;
	displays[0].mode.right_margin = edid_eeprom.mode.right_margin;
	displays[0].mode.upper_margin = edid_eeprom.mode.upper_margin;
	displays[0].mode.lower_margin = edid_eeprom.mode.lower_margin;
	displays[0].mode.hsync_len = edid_eeprom.mode.hsync_len;
	displays[0].mode.vsync_len = edid_eeprom.mode.vsync_len;
	//displays[0].mode.sync = 0;
	//displays[0].mode.vmode = FB_VMODE_NONINTERLACED;
}

#endif

