
/*
################################################################################
##                  diskstat.c                    ##
################################################################################
## diskstat returns (or will soon return to be more precice) webified diskstat for specific disk, should change to     ##
## XML'ified info at some point.....                      ##
################################################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statfs.h>
#include <limits.h>

int main( int argc, char **argv ) {
    struct statfs fpData;
    char fnPath[PATH_MAX];
    int i;

    if( argc < 2 ) {
        printf("Usage, webdisk DEVICE0 ..... DEVICEX\n");
        return(2);
    }

    memset(fnPath, 0, sizeof fnPath);

    for( i = 1 ; i < argc; i++ ) {

        strcpy(fnPath, *(argv + i) );
        memset(&fpData, 0, sizeof fpData);

        if( statfs(fnPath, &fpData) < 0 ) {
            fprintf(stderr, "Failed to stat %s: %m\n", fnPath);
        } else {
            printf("fnPath %s: \n", fnPath);
            printf("fpData.{\n\
\tf_type:	%u\n\
\tf_bsize:	%u\n\
\tf_blocks:	%u\n\
\tf_bfree:	%u\n\
\tf_bavail:	%u\n\
\tf_files:	%u\n\
\tf_ffree:	%u\n\
\tf_fsid:	%u\n\
\tf_namelen:	%u\n\
\tf_frsize:	%u\n\
}\n",
                fpData.f_type, fpData.f_bsize, fpData.f_blocks, fpData.f_bfree, fpData.f_bavail,
                fpData.f_files, fpData.f_ffree, fpData.f_fsid, fpData.f_namelen, fpData.f_frsize
            );
        }
    }
}

