/*
    lsmid - Standard MIDI File dumper
    Copyright (C) 2018  Nicolas Sauzede <nsauzede@laposte.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Refer to Standard MIDI File format specifications for more information :
	https://www.midi.org/specifications
*/

#include <inttypes.h>	// uint32_t
#include <stdlib.h>
#include <stdio.h>
#include <string.h>	// memset
#include <arpa/inet.h>	// ntohX

#include <string>

class Midi {
private:
	int m_format;
	int m_ntrks;
	int m_division;
public:
	Midi() :
		m_format(0) {
	}
	void load(const std::string& midifile) {
		FILE *f = fopen(midifile.c_str(), "rb");
		if (f) {
			while (!feof(f)) {
				char chtyp[5];
				uint32_t chlen = 0;
				memset(&chtyp, 0, sizeof(chtyp));
				if (1 != fread(&chtyp, 4, 1, f))
					break;
				if (1 != fread(&chlen, sizeof(chlen), 1, f))
					break;
				chlen = ntohl(chlen);
				printf("Chunk type [%s] (%" PRIu32 " bytes)\n", chtyp, chlen);
				if (0 == strcmp(chtyp, "MThd")) {
					uint16_t header[3];
					if (chlen == sizeof(header)) {
						if (1 != fread(&header, sizeof(header), 1, f))
							break;
						m_format = ntohs(header[0]);
						if (m_format != 1) {
							printf("Unsupported MIDI file format %d\n", m_format);
							exit(1);
						}
						m_ntrks = ntohs(header[1]);
						m_division = ntohs(header[2]);
						if (m_division >= 0x8000) {
							printf("Unsupported MIDI division %d\n", m_division);
							exit(1);
						}
						printf("Header : format=%d ntrks=%d division=%d\n", m_format, m_ntrks, m_division);
					} else {
						printf("Invalid header size (%" PRIu32 ")\n", chlen);
						exit(1);
					}
				} else {
					// skip alien chunk
					if (0 != fseek(f, chlen, SEEK_CUR))
						break;
					static int count = 0;
					if (count >= 5)
						exit(0);
					count++;
				}
			}
			fclose(f);
		} else {
			perror("fopen");
		}
	}
	void dump() {
		printf("Format : %d\n", m_format);
	}
};

int main(int argc, char *argv[]) {
	printf("Hello MIDI\n");
	int arg = 1;
	std::string file;
	if (argc == 2) {
		file = argv[arg++];
	} else {
		printf("Usage: %s <file.mid>\n", argv[0]);
		exit(1);
	}
	Midi m;
	m.load(file);
	m.dump();
	return 0;
}
