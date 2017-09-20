#include<stdio.h>
extern "C"
{
#include "miniunz.h"
}

int main(){

	char* cmdUnzip[] = {"", "test", "-d", "./test/"};
	do_unzip(4, cmdUnzip);

}
