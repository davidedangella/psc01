/**
 * Main GCCG program
 *
 * @author E. Xue, V. Petkov
 * @date 22-May-2009, 22-Oct-2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <papi.h>
#include "mpi.h"

#include "initialization.h"
#include "compute_solution.h"
#include "finalization.h"

#include "vol2mesh.h"
#include "util_write_files.h"


#ifdef MISSES
#define N 4
#endif

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}


int main(int argc, char *argv[]) {
    int i;

    const int max_iters = 10000;    /// maximum number of iteration to perform

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

    double residual_ratio;    /// the ratio between the reference and the current residual
    double *var;    /// the variation vector -> keeps the result in the end

    /** Additional vectors required for the computation */
    double *cgup, *oc, *cnorm;
	long_long readingtime;

	int nodeCnt, **points, **elems;

   	/*
	gccg needs 3 arguments:
	gccg <format> <input file> <output prefix>
	*/
	if (argc!=4){
			printf("wrong parameters!!\ncall: gccg <format> <input file> <output prefix>\n-format:	bin or text\n-input file:	cojack.dat (or .bin) - tjunc.dat (or .bin) - pent.dat (or .bin)\nRemember to create the .bin files first.\n");
			exit(1);
			}
	


	char *format = argv[1];
	char *file_in = argv[2];
	char *out_pref = argv[3];
	char *outFileName=malloc(sizeof(char)*50);

	if( strcmp(format,"text")!=0 && strcmp(format,"bin")!=0 ){
		printf("wrong format! Insert <bin> or <text>\n");
		exit(1);
	}
	if( ( strcmp(format,"text")==0 && strcmp(get_filename_ext(file_in),"dat")!=0 ) || ( strcmp(format,"bin")==0 && strcmp(get_filename_ext(file_in),"bin")!=0 ) ){
		printf("wrong pair of <format> and <input file>!\n");
		exit(1);
	}	
	
    /* measuring variables */
#ifdef MISSES
	long_long start_usec, end_usec;
	long_long counters[N];
	int PAPI_events[N] = {	PAPI_L2_TCM,
				PAPI_L2_TCA,
				PAPI_L3_TCM,
				PAPI_L3_TCA };
#else
	float rtime, ptime, mflops;
	long_long flpops;
#endif
 
    /********** START INITIALIZATION **********/
    // read-in the input file
	int init_status = initialization(file_in, &nintci, &nintcf, &nextci, &nextcf, &lcc,
                                     &bs, &be, &bn, &bw, &bl, &bh, &bp, &su, &var, &cgup, &oc,
                                     &cnorm, format, &readingtime);

    if ( init_status != 0 ) {
        fprintf(stderr, "Failed to initialize data!\n");
        abort();
    }

if ( PAPI_library_init( PAPI_VER_CURRENT ) != PAPI_VER_CURRENT ) { 
      printf("Could not PAPI_library_init \n");
        exit(1);
     }

    /********** END INITIALIZATION **********/

    /********** START COMPUTATIONAL LOOP **********/

#ifdef MISSES
if (PAPI_start_counters( PAPI_events, N ) != PAPI_OK) { 
      printf("Could not PAPI_start_counters \n");
        exit(1);
     }

start_usec = PAPI_get_real_usec();
#else
 PAPI_flops( &rtime, &ptime, &flpops, &mflops );
#endif

    int total_iters = compute_solution(max_iters, nintci, nintcf, nextcf, lcc, bp, bs, bw, bl, bn,
                                       be, bh, cnorm, var, su, cgup, &residual_ratio);

#ifdef MISSES
end_usec = PAPI_get_real_usec();

if (PAPI_stop_counters( counters, N ) != PAPI_OK) { 
      printf("Could not PAPI_stop_counters \n");
        exit(1);
     }

//	printf("measurements misses: %f;%f;%f;%f\n", (double)(end_usec-start_usec)/1e6, (double)counters[4]/(double)(end_usec-start_usec), (double)counters[0] / (double)counters[1], (double)counters[2] / (double)counters[3]);
printf("measurements_misses;%f;%f;%f;\n",  (double)counters[0] / (double)counters[1], (double)counters[2] / (double)counters[3],(double)readingtime/1e6);
#else
 PAPI_flops( &rtime, &ptime, &flpops, &mflops );
printf("measurements_mflops;%f;%f;%f;\n", rtime, mflops, (double)readingtime/1e6);
#endif

PAPI_shutdown();
	
    /********** END COMPUTATIONAL LOOP **********/

    /********** START FINALIZATION **********/
    finalization(file_in, total_iters, residual_ratio, nintci, nintcf, var, cgup, su);
    /********** END FINALIZATION **********/

	/**************** Writing VTK files ************/
	
	if( vol2mesh( nintci, nintcf, lcc, &nodeCnt, &points, &elems) !=0 ){
		printf("vol2mesh error\n");	
	}
	
	strcpy(outFileName, out_pref);
	write_result_vtk( strcat(outFileName,"SU.vtk") , nintci, nintcf, nodeCnt, points, elems, su);
	strcpy(outFileName, out_pref);
	write_result_vtk( strcat(outFileName,"VAR.vtk") , nintci, nintcf, nodeCnt, points, elems, var);
	strcpy(outFileName, out_pref);
	write_result_vtk( strcat(outFileName,"CGUP.vtk") , nintci, nintcf, nodeCnt, points,  elems, cgup);

	/************** END writing VTK ****************/
	

    free(cnorm);
    free(var);
    free(cgup);
    free(su);
    free(bp);
    free(bh);
    free(bl);
    free(bw);
    free(bn);
    free(be);
    free(bs);
	
	free(outFileName);

for ( i = 0; i < 6; ++i)
{
   printf("lcc[1][%d] = %d\n", i, lcc[1][i] );
}

   for ( i = nintci; i <= nintcf; i++ ) {
        free(lcc[i]);
    }
   free(lcc);

    return 0;
}


