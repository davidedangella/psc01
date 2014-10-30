//============================================================================
// Name        : txt_to_bin.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/**
 * Helper functions for reading from input data file
 *
 * @author E. Xue, V. Petkov
 * @date 22-May-2009, 22-Oct-2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}


void binconv(char *filename, char *output)
{
    int i, j, ivar, nintci, nintcf, nextci, nextcf;
    double dvar;
    FILE *fp = fopen(filename, "r");
    FILE *fo = fopen( output, "w");

    if (fp == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    //4 variables in total!!!
    fscanf(fp, "%d", &nintci);
    fscanf(fp, "%d", &nintcf);
    fscanf(fp, "%d", &nextci);
    fscanf(fp, "%d", &nextcf);

    fwrite(&nintci, sizeof(int), 1, fo);
    fwrite(&nintcf, sizeof(int), 1, fo);
    fwrite(&nextci, sizeof(int), 1, fo);
    fwrite(&nextcf, sizeof(int), 1, fo);

    //start reading lcc
    //note that c array index starts from 0 while fortran starts from 1!
    for (i = nintci; i <= nintcf; i++)
    {
	for(j=0; j<6; j++){
        	fscanf(fp, "%d", &ivar);
        	fwrite( &ivar, sizeof(int), 1, fo );
	}
    }


    // read the other arrays
    for (i = nintci; i <= nintcf; i++)
    {
	for(j=0; j<8; j++){
		fscanf(fp, "%lf", &dvar);
		fwrite( &dvar, sizeof(double), 1, fo );
	}
    }

    fclose(fp);
    fclose(fo);
}

int main(int argc, char *argv[]) {

	char *input_file, *output_file;

	/* the program needs to be called as:
	 * binconv <input_file> <output_file>
	 */

	if (argc != 3){
			printf("Wrong parameters! Exit!!\n");
			exit(1);
			}

	if(strcmp(get_filename_ext(argv[1]), "dat")!=0){
			printf("First argument must be a dat file\n");
			exit(1);
			}
	if(strcmp(get_filename_ext(argv[2]), "bin")!=0){
			printf("Second argument must be a bin file\n");
			exit(1);
			}
	
	input_file = argv[1];
	output_file = argv[2];
	


	binconv(input_file, output_file);

	printf("%s created\n", output_file);
	return 0;
}

