
#include <stdio.h>

// ./a.out -x nice
int
main (int argc, char **argv)
{

	while (--argc > 0 && **++argv == '-') {
		while (*++*argv) {
            printf("**argv: %c\n", **argv);
		}
	}
}
