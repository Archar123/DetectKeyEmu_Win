#include <stdio.h>
#include "mouse.h"

int main(int argc, char *argv[])
{
	while (1)
	{
		MouseClick(500, 600, 0);


		getchar();// (1000);
	}

	return 0;
}