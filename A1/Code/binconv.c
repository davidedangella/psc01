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


int read_formatted(char *filename, int *nintci, int *nintcf, int *nextci,
        int *nextcf, int ***lcc, double **bs, double **be, double **bn,
        double **bw, double **bl, double **bh, double **bp, double **su, char *output)
{
    int i;
    FILE *fp = fopen(filename, "r");
    FILE *fo = fopen( output, "w");

    if (fp == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    //4 variables in total!!!
    fscanf(fp, "%d", nintci);
    fscanf(fp, "%d", nintcf);
    fscanf(fp, "%d", nextci);
    fscanf(fp, "%d", nextcf);

    fwrite(nintci, sizeof(int), 1, fo);
    fwrite(nintcf, sizeof(int), 1, fo);
    fwrite(nextci, sizeof(int), 1, fo);
    fwrite(nextcf, sizeof(int), 1, fo);


    //allocating lcc
    if ((*lcc = (int**) malloc( ( (*nintcf) - (*nintci) + 1) * sizeof(int*) )) == NULL)
    {
        printf("malloc failed to allocate first dimension of lcc (nintcf)");
        return -1;
    }
    for (i = (*nintci); i <= (*nintcf); i++)
    {
        if (((*lcc)[i] = (int *) malloc( 6 * sizeof(int) )) == NULL)
        {
            printf("malloc(lcc) failed\n");
            return -1;
        }
    }

    //start reading lcc
    //note that c array index starts from 0 while fortran starts from 1!
    for (i = (*nintci); i <= (*nintcf); i++)
    {
        fscanf(fp, "%d", &(*lcc)[i][0]);
        fscanf(fp, "%d", &(*lcc)[i][1]);
        fscanf(fp, "%d", &(*lcc)[i][2]);
        fscanf(fp, "%d", &(*lcc)[i][3]);
        fscanf(fp, "%d", &(*lcc)[i][4]);
        fscanf(fp, "%d", &(*lcc)[i][5]);

        fwrite( (&(*lcc)[i][0]), sizeof(int), 1, fo );
        fwrite( (&(*lcc)[i][1]), sizeof(int), 1, fo );
        fwrite( (&(*lcc)[i][2]), sizeof(int), 1, fo );
        fwrite( (&(*lcc)[i][3]), sizeof(int), 1, fo );
        fwrite( (&(*lcc)[i][4]), sizeof(int), 1, fo );
        fwrite( (&(*lcc)[i][5]), sizeof(int), 1, fo );

    }


    // allocate other arrays
    if ((*bs = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }
    if ((*be = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }
    if ((*bn = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }
    if ((*bw = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }
    if ((*bl = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }
    if ((*bh = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }
    if ((*bp = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }
    if ((*su = (double *) malloc((*nextcf + 1) * sizeof(double))) == NULL)
    {
        printf("malloc() failed\n");
        return -1;
    }

    // read the other arrays
    for (i = (*nintci); i <= *nintcf; i++)
    {
        fscanf(fp, "%lf", &((*bs)[i]));
        fscanf(fp, "%lf", &((*be)[i]));
        fscanf(fp, "%lf", &((*bn)[i]));
        fscanf(fp, "%lf", &((*bw)[i]));
        fscanf(fp, "%lf", &((*bl)[i]));
        fscanf(fp, "%lf", &((*bh)[i]));
        fscanf(fp, "%lf", &((*bp)[i]));
        fscanf(fp, "%lf", &((*su)[i]));


        fwrite( (&((*bs)[i])), sizeof(double), 1, fo );
        fwrite( (&((*be)[i])), sizeof(double), 1, fo );
        fwrite( (&((*bn)[i])), sizeof(double), 1, fo );
        fwrite( (&((*bw)[i])), sizeof(double), 1, fo );
        fwrite( (&((*bl)[i])), sizeof(double), 1, fo );
        fwrite( (&((*bh)[i])), sizeof(double), 1, fo );
        fwrite( (&((*bp)[i])), sizeof(double), 1, fo );
        fwrite( (&((*su)[i])), sizeof(double), 1, fo );

    }

    fclose(fp);
    fclose(fo);


    /*
     * CHECK
     */

//    int nintci2, nintcf2, nextci2, nextcf2;
//    int **lcc2;
//    //allocating lcc2
//        if ((lcc2 = (int**) malloc( ( (*nintcf) - (*nintci) + 1) * sizeof(int*) )) == NULL)
//        {
//            printf("malloc failed to allocate first dimension of lcc2 (nintcf)");
//            return -1;
//        }
//        for (i = (*nintci); i <= (*nintcf); i++)
//        {
//            if (((lcc2)[i] = (int *) malloc( 6 * sizeof(int) )) == NULL)
//            {
//                printf("malloc(lcc) failed\n");
//                return -1;
//            }
//        }
//
//	printf("arriving here , point 2\n");
//
//    	fp = fopen(filename, "r");
//	if (!fp)
//		{
//			printf("Unable to open text file!");
//			return 1;
//		}
//    	fo = fopen( output , "r");
//	if (!fo)
//		{
//			printf("Unable to open bin file!");
//			return 1;
//		}
//
//	printf("arriving here , point 3\n");
//    	fscanf(fp, "%d", nintci);
//	fscanf(fp, "%d", nintcf);
//	fscanf(fp, "%d", nextci);
//	fscanf(fp, "%d", nextcf);
//printf("arriving here , point 4\n");
//
//	fread( &nintci2,sizeof(int),1, fo);
//printf("arriving here , point 5\n");
//
//	fread(&nintcf2,sizeof(int),1, fo);
//	fread(&nextci2,sizeof(int),1, fo);
//	fread(&nextcf2,sizeof(int),1, fo);
//
//	if( (*nintci)!=(nintci2) || *nintcf!=nintcf2 || *nextci!=nextci2 || *nextcf!=nextcf2){
//		 printf("ERROR! WRONG writing in the .bin file!! \n");
//		printf("nintci = %d,  nintci2 = %d!! \n", *nintci, nintci2);
//		printf("nintcf = %d,  nintcf2 = %d!! \n", *nintcf, nintcf2);
//		printf("nextci = %d,  nextci2 = %d!! \n", *nextci, nextci2);
//		printf("nextcf = %d,  nextcf2 = %d!! \n", *nextcf, nextcf2);
//
//		        return -1;
//	}
//
//	printf("nintci , nintcf, nextci, nextcf written correctly!! \n");
//
//	for (i = (*nintci); i <= (*nintcf); i++)
//	    {
//	        fscanf(fp, "%d", &(*lcc)[i][0]);
//	        fscanf(fp, "%d", &(*lcc)[i][1]);
//	        fscanf(fp, "%d", &(*lcc)[i][2]);
//	        fscanf(fp, "%d", &(*lcc)[i][3]);
//	        fscanf(fp, "%d", &(*lcc)[i][4]);
//	        fscanf(fp, "%d", &(*lcc)[i][5]);
//
//	        fread( &(lcc2)[i][0] ,sizeof(int),1, fo );
//	        fread( &(lcc2)[i][1] ,sizeof(int),1, fo );
//	        fread( &(lcc2)[i][2] ,sizeof(int),1, fo );
//	        fread( &(lcc2)[i][3] ,sizeof(int),1, fo );
//	        fread( &(lcc2)[i][4] ,sizeof(int),1, fo );
//	        fread( &(lcc2)[i][5] ,sizeof(int),1, fo );
//
//			if( 	(*lcc)[i][0]!=(lcc2)[i][0] || (*lcc)[i][1]!=(lcc2)[i][1] || (*lcc)[i][2]!=(lcc2)[i][2] ||
//					(*lcc)[i][3]!=(lcc2)[i][3] || (*lcc)[i][4]!=(lcc2)[i][4] || (*lcc)[i][5]!=(lcc2)[i][5] ){
//				printf("ERROR! WRONG writing in the .bin file!! \n");
//				printf("llc[%d][0] = %d, llc2[%d][0] = %d\n",i, (*lcc)[i][0], i, (lcc2)[i][0]);
//				printf("llc[%d][1] = %d, llc2[%d][1] = %d\n",i, (*lcc)[i][1], i ,(lcc2)[i][1]);
//				printf("llc[%d][2] = %d, llc2[%d][2] = %d\n",i, (*lcc)[i][2], i, (lcc2)[i][2]);
//				printf("llc[%d][3] = %d, llc2[%d][3] = %d\n",i, (*lcc)[i][3], i, (lcc2)[i][3]);
//				printf("llc[%d][4] = %d, llc2[%d][4] = %d\n",i, (*lcc)[i][4], i, (lcc2)[i][4]);
//				printf("llc[%d][5] = %d, llc2[%d][5] = %d\n",i, (*lcc)[i][5], i, (lcc2)[i][5]);
//						        return -1;
//			}
//
//	    }
//	printf("llc written correctly!! \n");
//
//	fclose(fp);
//	fclose(fo);


    return 0;
}

int main(int argc, char *argv[]) {

	/** Simulation parameters parsed from the input datasets */
	    int nintci, nintcf;    /// internal cells start and end index
	    /// external cells start and end index. The external cells are only ghost cells.
	    /// They are accessed only through internal cells
	    int nextci, nextcf;
	    int **lcc;    /// link cell-to-cell array - stores neighboring information
	    /// Boundary coefficients for each volume cell (South, East, North, West, High, Low)
	    double *bs, *be, *bn, *bw, *bh, *bl;
	    double *bp;    /// Pole coefficient
	    double *su;    /// Source values
	char *input_file, *output_file;

	/* the program needs to be called as:
	 * binconv <input_file> <output_file>
	 */

	if (argc != 3)
			{
			printf("Wrong parameters! Exit!!\n");
			exit(1);
			}	
	input_file = argv[1];
	output_file = argv[2];
	


	read_formatted(input_file, &nintci, &nintcf, &nextci,
	        &nextcf, &lcc, &bs, &be, &bn,
	        &bw, &bl, &bh, &bp, &su, output_file);

	printf("%s created\n", output_file);
	return 0;
}

