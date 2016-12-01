#include <common.h>
#include <net.h>
#include <command.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <config_cmd_default.h>

int screen_type_menu()
{
	char c;

	do
	{
		printf("current screen type is `%s`\n",getenv("screentype"));
		printf("R means Resistor screen,C means Capacitor screen\n");
		printf("---------------------screen type----------------------\n");
		printf("0 -- exit to upper menu\n");
		printf("1 -- Resistor screen\n");
		printf("2 -- Capacitor screen\n");
		printf("------------------------------------------------------\n");

	HERE:
		printf(":");
		c = getc();

		printf("%c\n",c);
		
		switch(c)
		{
		case '0':
			return 0;
			break;
		case '1':
			setenv("screentype","R");
			break;
		case '2':
			setenv("screentype","C");
			break;
		default:
			printf("incorrect number\n");
			goto HERE;
		}	

		if(saveenv())
			printf("something error occured, please check the nand device!");

	}while(1);
}

int screen_size_menu()
{
	char c;

	do
	{
		printf("current screen size is `%s`\n",getenv("screensize"));
		printf("AAAxAAA-B means Binch screen with AAAxAAA pixels\n");
		printf("---------------------screen type----------------------\n");
		printf("0 -- exit to upper menu\n");
		printf("1 -- 480x272-4\n");
		printf("2 -- 800x480-5\n");
		printf("3 -- 800x480-7\n");
		printf("4 -- 800x600-8\n");
		printf("5 -- 800x600-10\n");
		printf("------------------------------------------------------\n");

	HERE:
		printf(":");
		c = getc();

		printf("%c\n",c);
		
		switch(c)
		{
		case '0':
			return 0;
			break;
		case '1':
			setenv("screensize","480x272-4");
			break;
		case '2':
			setenv("screensize","800x480-5");
			break;
		case '3':
			setenv("screensize","800x480-7");
			break;
		case '4':
			setenv("screensize","800x600-8");
			break;
		case '5':
			setenv("screensize","800x600-10");
			break;
		default:
			printf("incorrect number\n");
			goto HERE;
		}	

		if(saveenv())
			printf("something error occured, please check the nand device!");

	}while(1);
}

int lcd_menu()
{
	char c;
	do
	{
		printf("----------------------LCD Menu-----------------------\n");
		printf("0 -- exit to upper menu\n");
		printf("1 -- set screen type\n");
		printf("2 -- set screen size\n");
		printf("------------------------------------------------------\n");

	HERE:
		printf(":");
		c = getc();

		printf("%c\n",c);
		
		switch(c)
		{
		case '0':
			return 0;
			break;
		case '1':
			screen_type_menu();
			break;
		case '2':
			screen_size_menu();
			break;
		default:
			printf("incorrect number\n");
			goto HERE;

		}	
	}while(1);
}

int do_scr(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])  
{ 
	char c;

	if(argc >1)
		goto err;
	
	do
	{
		printf("----------------------Main Menu-----------------------\n");
		printf("0 -- exit to uboot shell\n");
		printf("1 -- set LCD parameters\n");
		printf("2 -- erase the whole nand\n");
		printf("------------------------------------------------------\n");
		
	HERE:
		printf(":");
		c = getc();

		printf("%c\n",c);
		
		switch(c)
		{
		case '0':
			return 0;
			break;
		case '1':
			lcd_menu();
			break;
		case '2':
			run_command("nand scrub.chip",0);
			break;
		default:
			printf("incorrect number\n");
			goto HERE;
		}
	}while(1);
	
err:
	printf ("wrong argv, see help scr!\n");
	return 1;
}

U_BOOT_CMD(
        scr ,      1,      1,      do_scr,
        "set extra parameters\n",
        ""
);


