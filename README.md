# SoalShift_modul4_A14

Soal Shift Modul 4
1. [Soal 1](#1-soal-1)
2. [Soal 2](#2-soal-2)
3. [Soal 3](#3-soal-3)
4. [Soal 4](#4-soal-4)
5. [Soal 5](#5-soal-5)

### 1. soal 1
##### Semua nama file dan folder harus terenkripsi
##### Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:

##### qE1~ YMUR2"\`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0

##### Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
##### “INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .
##### Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

```sh
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

```
Folder dan file yang berada di /home/Penunggu/shift4 ter-encrypt dengan Caesar Cipher yang dimodifikasi dengan character list yang diekspan.

Ditambahkan fungsi __encryptit__ dan __decryptit__ untuk melakukan encrypt dan decrypt pada program.
Pada fungsi readdir, read, dan getattr terdapat perubahan agar dapat memenuhi ketentuan soal.

fungsi __encryptit__
```sh
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
```

fungsi __decryptit__
```sh
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
```

fungsi __getattr__
```sh
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
```

fungsi __readdir__
```sh
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
```

fungsi __read__
```sh
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
```

### 2. soal 2
##### Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”
##### Urutan operasi dari kebutuhan ini adalah:
##### Tepat saat sebelum file system di-mount
##### Secara otomatis folder “Videos” terbuat di root directory file system
##### Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”
##### Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse
##### Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut
##### Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”
##### Jangan tampilkan file pecahan di direktori manapun
##### Tepat saat file system akan di-unmount
##### Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system
##### Hapus folder “Videos” 


### 3. soal 3
##### Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
##### Owner Name 	: ‘chipset’ atau ‘ic_controller’
##### Group Name	: ‘rusak’
##### Tidak dapat dibaca
##### Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.


### 4. soal 4
##### Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”. File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”


### 5. soal 5
##### Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi. Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss

### Catatan
##### Semua nama file dan folder yang terdapat pada soal harus memenuhi syarat soal no. 1 (terenkripsi saat belum di-mount dan terdekripsi saat di-mount)
##### Tidak diperkenankan menggunakan system() dan exec*(), kecuali ada pengecualian di butir soal.
##### Untuk soal nomor 3 diharapkan secara manual membuat user dan group-nya. Pengecekan file dilakukan setiap membuka direktori.
##### Untuk soal nomor 4 dan 5 diperbolehkan menggunakan exec*().
##### Pengerjaan hanya dilakukan dalam 1 file program C dengan format nama AFSHiaAP_[Kelompok].c . 
##### File System AFSHiaAP mengarah ke folder /home/[user]/shift4.


##### -- revisi soal 3 --
```sh
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
#include <pwd.h>
#include <grp.h>
// AFSHiaAP_A14.c
void decryptit(char *fromthis);
void encryptit(char *this);

int shifit;
int buka = 0;

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
  printf("charlist : %s\n", charlist);
  printf("list : %s\n", list);

	return fuse_main(argc, argv, &xmp_oper, NULL);
}


static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
  char hmm[1000];
	char fpath[1000];

  printf("%s\n", path);
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
  char tmp[1000];

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
  int check;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {

      struct stat st;
  		memset(&st, 0, sizeof(st));

      strcpy(hmm, de->d_name);
      sprintf(tmp, "%s/%s", fpath, hmm);

      stat(tmp, &st);
      check = st.st_mode;

      st.st_ino = de->d_ino;
  		st.st_mode = de->d_type << 12;

      struct group *wat_group = getgrgid(st.st_gid);
      struct passwd *who_own = getpwuid(st.st_uid);

      struct tm *timeis = localtime(&st.st_atime);

      if(strcmp(hmm, ".") != 0 && strcmp(hmm, "..") != 0)
      {
        if((strcmp(who_own->pw_name, "ic_controller") == 0 || strcmp(who_own->pw_name, "chipset") == 0) && strcmp(wat_group->gr_name, "rusak") == 0 && (!(check & S_IRUSR) && !(check & S_IRGRP) && !(check & S_IROTH)))
        {

            FILE *thisfile;
            int taim = timeis->tm_year + 1900;
            char miris[1000];
            char inside[1000];

            sprintf(miris, "%s/V[EOr[c[Y`HDH", dirpath);
            sprintf(inside, "%s - %d - %d %02d:%02d:%02d %02d-%02d-%04d \n", tmp, wat_group->gr_gid, who_own->pw_uid, timeis->tm_hour, timeis->tm_min, timeis->tm_sec, timeis->tm_mday, timeis->tm_mon, taim);

            thisfile = fopen(miris, "a+");

            fputs(inside, thisfile);
            fclose(thisfile);

            if(S_ISDIR(st.st_mode))
            {
              rmdir(tmp);
            }
            else
            {
              remove(tmp);
            }
          // }
        }
        else
        {
          decryptit(de->d_name);

          res = (filler(buf, de->d_name, &st, 0));

          if(res!=0) break;
        }
      }

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

```

pada soal 3, diminta untuk menghapus file yang memiliki owner 'ic_controller' atau 'chipset' dan grup 'rusak' serta jika file tersebut tidak dapat dibaca.

Pencarian file ini dilakukan pada fungsi readdir, dimana dilakukan pengecekan owner, grup, dan mode dari file:

```sh
if((strcmp(who_own->pw_name, "ic_controller") == 0 || strcmp(who_own->pw_name, "chipset") == 0) && strcmp(wat_group->gr_name, "rusak") == 0 && (!(check & S_IRUSR) && !(check & S_IRGRP) && !(check & S_IROTH)))
```
Asumsi saya pada kondisi file tidak dapat dibaca adalah ketika file permissionnya tidak ada permission 'read' nya, baik untuk owner, grup, maupun other.

```sh
FILE *thisfile;
            int taim = timeis->tm_year + 1900;
            char miris[1000];
            char inside[1000];

            sprintf(miris, "%s/V[EOr[c[Y`HDH", dirpath);
            sprintf(inside, "%s - %d - %d %02d:%02d:%02d %02d-%02d-%04d \n", tmp, wat_group->gr_gid, who_own->pw_uid, timeis->tm_hour, timeis->tm_min, timeis->tm_sec, timeis->tm_mday, timeis->tm_mon, taim);

            thisfile = fopen(miris, "a+");

            fputs(inside, thisfile);
            fclose(thisfile);

            if(S_ISDIR(st.st_mode))
            {
              rmdir(tmp);
            }
            else
            {
              remove(tmp);
            }
```
fungsi diatas digunakan untuk menginputkan nama file, grup, owner, serta waktu akhir diakses ke dalam file 'filemiris.txt' yang dalam mode enkrip dengan key 17 adalah 'V\[EOr\[c\[Y\`HDH'.
lalu setelah itu, file akan dihapus. apabila file berupa direktori, maka akan dijalankan rmdir(), dan selain itu, dijalankan remove().
