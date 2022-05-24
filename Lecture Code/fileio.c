/**
 * demonstrate fileio with C stdio library.  Reads pairs of numbers from
 * input file and writes their products to output file.
 */

#include <stdio.h> // fopen, fclose, fprintf, fscanf, perror, printf

int main( int argc, char *argv[] ) {
	if(argc != 3) {
		printf("usage: %s <in_file> <out_file>\n", argv[0]);
		return 1;
	}

	FILE *fin = fopen(argv[1], "r");
	if(fin == NULL) {
		perror("fopen fin error");
		return 1;
	}

	FILE *fout = fopen(argv[2], "w");
	if(fout == NULL) {
		fclose(fin);
		perror("fopen fout error");
		return 1;
	}

	int i1, i2;
	while(fscanf(fin,"%d %d", &i1, &i2) != EOF) // read pairs; note EOF != 0
		fprintf(fout,"%d\n", i1*i2); // write product

	fclose(fin);
	fclose(fout);

    return 0;
}

