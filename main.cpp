#include <cstring>
#include <iostream>
#include "actor.h"
#include "tty.h"
#include "unistd.h"
#include "ansitty.h"

using std::cout;
using std::endl;
using std::memset;

//extern int fireMain();
extern "C" {
	void fireMain(void);
	void ansitty_canvas_setdirty(bool state);
	};


int main(int argc, char *argv[])
{

    TTY *myTTY = NULL;
    char buffer[2048];
    int bytes =0;
    bool running = false;

    myTTY = new TTY();
    myTTY->Init();

    sleep(1);

		/* temporary splash screen */

		myTTY->puts("\x1b\x5B""?25l");
    myTTY->puts("\n\n");

    FILE *myfile = NULL;
    myfile = fopen("data/ansi/frogue.ans", "rb");
    while (!feof(myfile) && !ferror(myfile)) {
        memset(&buffer, 0, 2048);
        bytes = fread(&buffer, 1, 2047, myfile);
        myTTY->puts(buffer);
    }
    fclose(myfile);

    myTTY->puts("                         (development version only)\n");
 
/*
    while (myTTY->getchar() == 0) {
    }
*/

		myTTY->puts("\x1b\x5B""2J");

		ansitty_canvas_setdirty(true);

		//myTTY->puts("\x1b\x5B""?25l");

		running = true;
		/*
    while (running) {
				//
       	if(myTTY->getchar() != 0) {
					running = false;
        }

    }
		*/
		fireMain();

    exit(0);
}

