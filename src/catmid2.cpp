#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "MidiFile.h"

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
	printf("BEGIN\n");
	using namespace std;
	using namespace smf;
	MidiFile midifile;
	midifile.read(f);

	midifile.joinTracks();	// join tracks
	midifile.doTimeAnalysis();	// times in seconds
	midifile.linkNotePairs();	// calculate durations

	int tracks = midifile.getTrackCount();
	for (int track=0; track<tracks; track++) {
		if (tracks > 1) cout << "\nTrack " << track << endl;
		cout << "SEC\tDUR\tTRACK\tNOTE" << endl;
		for (int event=0; event<midifile[track].size(); event++) {
			MidiEvent* mev;
			mev = &midifile[track][event];
			if (!mev->isNoteOn()) {
				continue;
			}
			double duration = mev->getDurationInSeconds();

			cout << mev->seconds << '\t';
			cout << duration << '\t';
			cout << mev->track << '\t';
			cout << mev->getKeyNumber();
			cout << endl;
		}
	}
	printf("END\n");
	return 0;
}
