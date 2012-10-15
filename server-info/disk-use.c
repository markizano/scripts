
#include <libgen.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <limits.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>

#define unless(cond) if(!(cond))

#define statfs_fstype_tr(fstype) (\
    fstype == 0xadf5? "ADFS_SUPER_MAGIC" :\
    fstype == 0xADFF? "AFFS_SUPER_MAGIC" :\
    fstype == 0x42465331? "BEFS_SUPER_MAGIC" :\
    fstype == 0x1BADFACE? "BFS_MAGIC" :\
    fstype == 0xFF534D42? "CIFS_MAGIC_NUMBER" :\
    fstype == 0x73757245? "CODA_SUPER_MAGIC" :\
    fstype == 0x012FF7B7? "COH_SUPER_MAGIC" :\
    fstype == 0x28cd3d45? "CRAMFS_MAGIC" :\
    fstype == 0x1373? "DEVFS_SUPER_MAGIC" :\
    fstype == 0x00414A53? "EFS_SUPER_MAGIC" :\
    fstype == 0x137D? "EXT_SUPER_MAGIC" :\
    fstype == 0xEF51? "EXT2_OLD_SUPER_MAGIC" :\
    fstype == 0xEF53? "EXT2_SUPER_MAGIC" :\
    fstype == 0xEF53? "EXT3_SUPER_MAGIC" :\
    fstype == 0xEF53? "EXT4_SUPER_MAGIC" :\
    fstype == 0x4244? "HFS_SUPER_MAGIC" :\
    fstype == 0xF995E849? "HPFS_SUPER_MAGIC" :\
    fstype == 0x958458f6? "HUGETLBFS_MAGIC" :\
    fstype == 0x9660? "ISOFS_SUPER_MAGIC" :\
    fstype == 0x72b6? "JFFS2_SUPER_MAGIC" :\
    fstype == 0x3153464a? "JFS_SUPER_MAGIC" :\
    fstype == 0x137F? "MINIX_SUPER_MAGIC" :\
    fstype == 0x138F? "MINIX_SUPER_MAGIC2" :\
    fstype == 0x2468? "MINIX2_SUPER_MAGIC" :\
    fstype == 0x2478? "MINIX2_SUPER_MAGIC2" :\
    fstype == 0x4d44? "MSDOS_SUPER_MAGIC" :\
    fstype == 0x564c? "NCP_SUPER_MAGIC" :\
    fstype == 0x6969? "NFS_SUPER_MAGIC" :\
    fstype == 0x5346544e? "NTFS_SB_MAGIC" :\
    fstype == 0x9fa1? "OPENPROM_SUPER_MAGIC" :\
    fstype == 0x9fa0? "PROC_SUPER_MAGIC" :\
    fstype == 0x002f? "QNX4_SUPER_MAGIC" :\
    fstype == 0x52654973? "REISERFS_SUPER_MAGIC" :\
    fstype == 0x7275? "ROMFS_MAGIC" :\
    fstype == 0x517B? "SMB_SUPER_MAGIC" :\
    fstype == 0x012FF7B6? "SYSV2_SUPER_MAGIC" :\
    fstype == 0x012FF7B5? "SYSV4_SUPER_MAGIC" :\
    fstype == 0x01021994? "TMPFS_MAGIC" :\
    fstype == 0x15013346? "UDF_SUPER_MAGIC" :\
    fstype == 0x00011954? "UFS_MAGIC" :\
    fstype == 0x9fa2? "USBDEVICE_SUPER_MAGIC" :\
    fstype == 0xa501FCF5? "VXFS_SUPER_MAGIC" :\
    fstype == 0x012FF7B4? "XENIX_SUPER_MAGIC" :\
    fstype == 0x58465342? "XFS_SUPER_MAGIC" :\
    fstype == 0x012FD16D? "_XIAFS_SUPER_MAGIC" : "OTHER"\
)

int main( int argc, char **argv ) {
    struct statfs fpData;
    char fnPath[PATH_MAX], suf[8];
    int i, fd;
    float len;
    memset(suf, 0, sizeof suf);

    if( argc < 2 ) {
        printf("Usage, %s [/dev/sda]\n", basename(argv[0]));
        return 2;
    }

/*
	for ( i = 1; i < argc; ++i ) {
		fd = open(argv[i], O_APPEND);
		len = lseek(fd, 0, SEEK_END);
		if ( len < 0 ) {
			fprintf(stderr, "Cannot open `%s': (%01.0f) %m\n", argv[i], len);
			close(fd);
			continue;
		}

		close(fd);
		strcpy(suf, "bytes");

		unless ( strcmp(getenv("H"), "K") ) { // H=K
			len = len / 1024;
			memset(suf, 0, sizeof suf);
			strcpy(suf, "Kib");
		} else unless ( strcmp(getenv("H"), "M") ) { // H=M
			len = len / 1048576;
			memset(suf, 0, sizeof suf);
			strcpy(suf, "Mib");
		} else unless ( strcmp(getenv("H"), "G") ) { // H=G
			len = len / 1073741824;
			memset(suf, 0, sizeof suf);
			strcpy(suf, "Gib");
		}

		fd = printf("Sizeof `%s': %01.2f%s\n", argv[i], len, suf);
	}

	return 0;
/*/
    memset(fnPath, 0, sizeof fnPath);
    fd = 1;

	unless ( strcmp(getenv("H"), "K") ) { // H=K
		len = len / 1024;
		memset(suf, 0, sizeof suf);
		strcpy(suf, "Kib");
	} else unless ( strcmp(getenv("H"), "M") ) { // H=M
		len = len / 1048576;
		memset(suf, 0, sizeof suf);
		strcpy(suf, "Mib");
	} else unless ( strcmp(getenv("H"), "G") ) { // H=G
		len = len / 1073741824;
		memset(suf, 0, sizeof suf);
		strcpy(suf, "Gib");
	}

    for( i = 1 ; i < argc; i++ ) {

        strcpy(fnPath, *(argv + i) );
        memset(&fpData, 0, sizeof fpData);

        if( statfs(fnPath, &fpData) < 0 ) {
            fprintf(stderr, "Failed to stat %s: %m\n", fnPath);
        } else {
            printf("fnPath %s: \n", fnPath);
            printf("fpData.{\n\
  f_type:    %s\n\
  f_bsize:   %u\n\
  f_blocks:  %u\n\
  f_bfree:   %u\n\
  f_bavail:  %u\n\
  f_files:   %u\n\
  f_ffree:   %u\n\
  f_fsid:    %u\n\
  f_namelen: %u\n\
  f_frsize:  %u\n\
}\n",
                statfs_fstype_tr(fpData.f_type), fpData.f_bsize, fpData.f_blocks, fpData.f_bfree, fpData.f_bavail,
                fpData.f_files, fpData.f_ffree, fpData.f_fsid, fpData.f_namelen, fpData.f_frsize
            );
        }
    }
//*/
}

