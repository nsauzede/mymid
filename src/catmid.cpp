#include <stdlib.h>
#include <stdio.h>

#include "midifile.h"

static FILE* in = 0;
int mygetc() {
	return fgetc(in);
}
void myerror(char *msg) {
	printf("%s: %s\n", __func__, msg);
	exit(1);
}
void myheader(int format, int ntrks, int division) {
	printf("%s: format=%d ntrks=%d division=%d\n", __func__, format, ntrks, division);
}
void myon(int chan, int pitch, int vol) {
	printf("%s: chan=%d pitch=%d vol=%d\n", __func__, chan, pitch, vol);
}

int main(int argc, char *argv[]) {
	int arg = 1;
	char *f = 0;
	if (arg < argc) {
		f = argv[arg++];
	}
	if (!f) {
		printf("Usage: %s <midifile.mid>\n", argv[0]);
		exit(1);
	}
	Mf_getc = mygetc;
	Mf_error = myerror;
	Mf_header = myheader;
	Mf_on = myon;

	printf("BEGIN\n");
	in = fopen(f, "rb");
	mfread();
	fclose(in);
	printf("END\n");
	return 0;
}
