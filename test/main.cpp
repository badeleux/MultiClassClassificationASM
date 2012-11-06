
#include <iostream>
#include <cstdio>
typedef struct args {
	int jakasLiczba;
	int innaLiczba;
} args;

int main()
{
	args struktura = {69, 70};
	printf("%d", struktura);

	fflush(stdout);
}
