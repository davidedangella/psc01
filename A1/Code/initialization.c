/**
 * Initialization step - parse the input file, compute data distribution, initialize LOCAL computational arrays
 *
 * @date 22-Oct-2012
 * @author V. Petkov
 */

#include <stdlib.h>

#include "util_read_files.h"
#include "initialization.h"

int initialization(char* file_in, int* nintci, int* nintcf, int* nextci,
                   int* nextcf, int*** lcc, double** bs, double** be, double** bn, double** bw,
                   double** bl, double** bh, double** bp, double** su, double** var, double** cgup, 
                   double** oc, double** cnorm, char* format, long_long* readingtime) {
    /********** START INITIALIZATION **********/
    int i = 0;
	long_long start_usec, end_usec;
    // read-in the input file
    int f_status = 1;
	if(strcmp(format, "text")==0)	
	{	
		start_usec=PAPI_get_virt_usec();
		f_status = read_formatted(file_in, &*nintci, &*nintcf, &*nextci, &*nextcf, &*lcc, &*bs,
		                   &*be, &*bn, &*bw, &*bl, &*bh, &*bp, &*su);
		end_usec=PAPI_get_virt_usec();
	}
	else if(strcmp(format, "bin")==0)	
		{
		start_usec=PAPI_get_virt_usec();
		f_status = read_formatted_bin(file_in, &*nintci, &*nintcf, &*nextci, &*nextcf, &*lcc, &*bs,
				   &*be, &*bn, &*bw, &*bl, &*bh, &*bp, &*su);
		end_usec=PAPI_get_virt_usec();
		}
	*readingtime=end_usec-start_usec;	

    if ( f_status != 0 ) return f_status;

    *var = (double*) calloc( (*nextcf + 1), sizeof(double) );
    *cgup = (double*) calloc( (*nextcf + 1), sizeof(double) );
    *cnorm = (double*) calloc( (*nintcf + 1), sizeof(double) );

    // initialize the arrays
    for ( i = 0; i <= 10; i++ ) {
        (*cnorm)[i] = 1.0;
    }

    for ( i = (*nintci); i <= (*nintcf); i++ ) {
        (*var)[i] = 0.0;
    }

    for ( i = (*nintci); i <= (*nintcf); i++ ) {
        (*cgup)[i] = 1.0 / ((*bp)[i]);
    }

    for ( i = (*nextci); i <= (*nextcf); i++ ) {
        (*var)[i] = 0.0;
        (*cgup)[i] = 0.0;
        (*bs)[i] = 0.0;
        (*be)[i] = 0.0;
        (*bn)[i] = 0.0;
        (*bw)[i] = 0.0;
        (*bh)[i] = 0.0;
        (*bl)[i] = 0.0;
    }

    return 0;
}

