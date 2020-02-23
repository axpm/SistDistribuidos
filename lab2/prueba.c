#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "lines.h"

#define MAX_LINE	256



int  main(int argc, char **argv)
{
	char buffer[MAX_LINE];
	int err = 0;
	int n;
	
	while (err != -1) {
		n = readLine(0, buffer, MAX_LINE);
		if (n!=-1)
			writeLine(1, buffer, n);
	}



	return (0);
}

	
