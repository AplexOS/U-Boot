#include <common.h>
#include <net.h>
#include <command.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <config_cmd_default.h>
#include <asm/io.h>

#define GPIO1_OE *((volatile unsigned int *)0x4804C134)
#define GPIO1_DATAOUT *((volatile unsigned int *)0x4804C13C)
#define GPIO1_SETDATAOUT *((volatile unsigned int *)0x4804C194)
#define GPIO1_CLEARDATAOUT *((volatile unsigned int *)0x4804C190)

extern int g_on;

void update_led()
{
	static u32 n = 0;
	u32 l = 0;
#if defined(CONFIG_OK335XD)
	l = 1 << (16 + n);
	__raw_writel(l,0x4804C194);	//Ãð
	n = (n + 1)%4;
	l = 1 << (16 + n);
	__raw_writel(l, 0x4804C190);//ÁÁ
#elif defined(CONFIG_OK335XS) 
	l = 1 << 16;
	if(n)
		__raw_writel(l,0x481AE194);	//Ãð
	else
		__raw_writel(l, 0x481AE190);//ÁÁ
	n = !n;
#elif defined(CONFIG_OK335XS2)	
	l = 1 << (12 + n);
	__raw_writel(l,0x4804C194);	//Ãð
	n = (n + 1)%4;
	l = 1 << (12 + n);
	__raw_writel(l, 0x4804C190);//ÁÁ

#endif

}

int do_led(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])  
{ 
	u32 l = 0;
	u32 n = 0;

	switch(argc) { 
		case 3:
			if(strcmp(argv[1], "flash") == 0) 
			{
				if(strcmp(argv[2], "all") == 0) 
				{
#if defined(CONFIG_OK335XD)
					__raw_writel(1<<16,0x4804C190);	//ÁÁ
					__raw_writel(1<<17,0x4804C190);	//ÁÁ
					__raw_writel(1<<18,0x4804C190);	//ÁÁ
					__raw_writel(1<<19,0x4804C190);	//ÁÁ
					
					while(1);

#elif defined(CONFIG_OK335XS)
					__raw_writel(1<<16, 0x481AE190);//ÁÁ

					while(1)
					{
					__raw_writel(1<<7, 0x44E07190);
					udelay(200);
					__raw_writel(1<<7,0x44E07194);
					udelay(200);
					}
#elif defined(CONFIG_OK335XS2)
					__raw_writel(1<<12,0x4804C190);	//ÁÁ
					__raw_writel(1<<13,0x4804C190);	//ÁÁ
					__raw_writel(1<<14,0x4804C190);	//ÁÁ
					__raw_writel(1<<15,0x4804C190);	//ÁÁ
		
					while(1)
					{
					__raw_writel(1<<7, 0x44E07190);
					udelay(200);
					__raw_writel(1<<7,0x44E07194);
					udelay(200);
					}
#endif
				} 
			}
			else if(strcmp(argv[1], "1") == 0
				 || strcmp(argv[1], "2") == 0
				 || strcmp(argv[1], "3") == 0
				 || strcmp(argv[1], "4") == 0) 
			{
				n = *argv[1] - 0x30;
				l = 1 << (16 + (n - 1));//1~4¶ÔÓ¦gpio1_16~gpio1_19
				
				if(strcmp(argv[2], "on") == 0) 
				{
					
					__raw_writel(l,0x4804C190);

				} 
				else if(strcmp(argv[2], "off") == 0) 
				{
					
					__raw_writel(l, 0x4804C194);
				}
			}
			else if(strcmp(argv[1], "all") == 0) 
			{
				if(strcmp(argv[2], "on") == 0) 
				{
#if defined(CONFIG_OK335XD)
					__raw_writel(1<<16,0x4804C190);	//ÁÁ
					__raw_writel(1<<17,0x4804C190);	//ÁÁ
					__raw_writel(1<<18,0x4804C190);	//ÁÁ
					__raw_writel(1<<19,0x4804C190);	//ÁÁ

					/*lcd backlight enable*/
					__raw_writel(1<<7, 0x44E07194);
#elif defined(CONFIG_OK335XS) 
					__raw_writel(1<<16,0x481AE190);	//ÁÁ
					
					/*lcd backlight enable*/
					__raw_writel(1<<17, 0x481AE194);
#elif defined(CONFIG_OK335XS2) 
					__raw_writel(1<<12,0x4804C190);	//ÁÁ
					__raw_writel(1<<13,0x4804C190);	//ÁÁ
					__raw_writel(1<<14,0x4804C190);	//ÁÁ
					__raw_writel(1<<15,0x4804C190);	//ÁÁ
					
					/*lcd backlight enable*/
					__raw_writel(1<<17, 0x481AE194);
#endif
					
					g_on = 1;

				} 
				else if(strcmp(argv[2], "off") == 0) 
				{
#if defined(CONFIG_OK335XD)
					__raw_writel(1<<16,0x4804C194);	//Ãð
					__raw_writel(1<<17,0x4804C194);	//Ãð
					__raw_writel(1<<18,0x4804C194);	//Ãð
					__raw_writel(1<<19,0x4804C194);	//Ãð
#elif defined(CONFIG_OK335XS) 
					__raw_writel(1<<16,0x481AE194);	//Ãð
#elif defined(CONFIG_OK335XS2) 
					__raw_writel(1<<12,0x4804C194);	//Ãð
					__raw_writel(1<<13,0x4804C194);	//Ãð
					__raw_writel(1<<14,0x4804C194);	//Ãð
					__raw_writel(1<<15,0x4804C194);	//Ãð
#endif
					g_on = 0;

				}
			}
			break;
		case 1:          
		default:  
			printf ("wrong argv, see help led!\n");
	}    
}

U_BOOT_CMD(
        led ,      3,      1,      do_led,
        "    led control\n",
		"1/2/3/4 on/off\n"
		"all on/off\n"
);


