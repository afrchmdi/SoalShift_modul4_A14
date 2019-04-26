#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
// AFSHiaAP_A14.c
void decryptit(char *fromthis);
void encryptit(char *this);

int shifit;

char charlist[]="qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
char list[]="qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

static const char *dirpath = "/home/Penunggu/shift4";

static int xmp_getattr(const char *path, struct stat *stbuf);
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);


static struct fuse_operations xmp_oper = {
  .readdir	= xmp_readdir,
  .read		= xmp_read,
	.getattr	= xmp_getattr,
};


int main(int argc, char *argv[])
{
	umask(0);

  printf("shift is ... : ");
  scanf("%d", &shifit);

  char cut[shifit+1];

  strncpy(cut, charlist, shifit);
  cut[shifit] = 0;
  sprintf(list, "%s%s", charlist+shifit, cut);

	return fuse_main(argc, argv, &xmp_oper, NULL);
}


static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
  char hmm[1000];
	char fpath[1000];


  sprintf(hmm, "%s", path);

  encryptit(hmm);

	sprintf(fpath,"%s%s",dirpath,hmm);

	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char hmm[1000];
  char fpath[1000];

  sprintf(hmm, "%s", path);
  encryptit(hmm);

	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
  else
  {
    sprintf(fpath, "%s%s", dirpath, hmm);
  }

	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {

      struct stat st;
  		memset(&st, 0, sizeof(st));

      st.st_ino = de->d_ino;
  		st.st_mode = de->d_type << 12;

      decryptit(de->d_name);

      res = (filler(buf, de->d_name, &st, 0));

      if(res!=0) break;

	}

	closedir(dp);

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char hmm[1000];
  char fpath[1000];

  sprintf(hmm, "%s", path);
  encryptit(hmm);

	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
  else
  {
    sprintf(fpath, "%s%s", dirpath, hmm);
  }

	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}


void encryptit(char *this)
{
  int x = 0;
  int whichchar;
  
  while(this[x] != '\0')
  {
      if (this[x] != '/')
      {
          whichchar = strchr(charlist, this[x]) - charlist;
          this[x] = list[whichchar];
      }

      x++;
  }

}

void decryptit(char *fromthis)
{
  int x = 0;
  int whichlist;

  while(fromthis[x] != '\0')
  {
      if(fromthis[x] != '/')
      {
          whichlist = strchr(list, fromthis[x]) - list;
          fromthis[x] = charlist[whichlist];
      }

      x++;
  }
}
