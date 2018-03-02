/*
 * DATE 2016.12
 * writer:Eggsy Pang(Pangjiahua@ti.com)
 * Uboot LCD
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include "../drivers/lcd/rasterDisplay.h"
#include "../board/ti/am335x/board.h"

int get_int(char*num)
{
	int len = strlen(num);
	int i, result=0;
	for(i=0; i<len; i++){
		result = result * 10 + ( num[i] - '0' );
	}
	return result;

}



int get_lcd_cmd(char *var)
{
	if (strcmp(var, "off") == 0)
		return 0;

	if (strcmp(var, "on") == 0)
		return 1;

	if (strcmp(var, "reset") == 0)
		return 2;

	if (strcmp(var, "set") == 0)
		return 3;

    return -1;
}
static int do_lcd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
      int cmd;
      /* Validate arguments  */
      if ((argc <= 1) || (argc > 2))
		return CMD_RET_USAGE;

      cmd = get_lcd_cmd(argv[1]);
      if (cmd < 0) {
		return CMD_RET_USAGE;
      }
#ifdef CONFIG_CMDLINE
       if (cmd==0){
			printf("lcd off \n");
			lcdbacklight_off(7);
      }
      else if (cmd==1){
			printf("lcd on\n");
			lcdbacklight_on(7);

      }  
      else if (cmd==2){
			printf("lcd reset\n");
			board_lcd_reset(7);
      }

	return 0;
#else
	return 1;
#endif
}

U_BOOT_CMD(
	ULCD,	2,	1,	do_lcd,
	"lcd open or close or set\n",
	"ULCD [on|off|reset|set]\n"
);
