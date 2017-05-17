#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define __MYPID 1

extern char inbyte (void);
extern int outbyte (char x);

extern char __end[] __attribute__ ((aligned (4)));


/*
 * _exit -- stop the CPU as we want to exit
 */
    __attribute__((noreturn))
void _exit (int code)
{
    while (1)
        asm volatile ("stop #0x700");
}

/*
 * close -- close a file descriptor
 */
int close (int fd)
{
    return 0;
}

/*
 * fstat -- get file information
 */
int fstat (int fd, struct stat *buf)
{
    errno = ENOSYS;
    return -1;
}

/*
 * getpid -- only one process, so just return 1.
 */
int getpid()
{
    return __MYPID;
}

/*
 * isatty -- check if fd is a terminal
 */
int isatty (int fd)
{
    return 1;
}

/*
 * kill -- go out via exit...
 */
int kill (int pid, int sig)
{
    if(pid == __MYPID)
        _exit(sig);
    return 0;
}

/*
 * lseek --  Since a serial port is non-seekable, we return an error.
 */
off_t lseek (int fd, off_t offset, int whence)
{
    errno = ESPIPE;
    return (off_t)-1;
}

/*
 * open -- open a file descriptor. We don't have a filesystem, so
 *         we return an error.
 */
int open (const char *buf, int flags, int mode)
{
    errno = EIO;
    return -1;
}

/*
 * print -- do a raw print of a string
 */
void print (char *ptr)
{
    while (*ptr) {
        outbyte (*ptr++);
    }
}

/*
 * putnum -- print a 32 bit number in hex
 */
void putnum (unsigned int num)
{
    char  buf[9];
    int   cnt;
    char  *ptr;
    int   digit;

    ptr = buf;
    for (cnt = 7 ; cnt >= 0 ; cnt--) {
        digit = (num >> (cnt * 4)) & 0xf;

        if (digit <= 9)
            *ptr++ = (char) ('0' + digit);
        else
            *ptr++ = (char) ('a' - 10 + digit);
    }

    *ptr = (char) 0;
    print (buf);
}

/*
 * read  -- read bytes from the serial port. Ignore fd, since
 *          we only have stdin.
 */
int read (int fd, char *buf, int nbytes)
{
    int i = 0;

    for (i = 0; i < nbytes; i++) {
        *(buf + i) = inbyte();
        if ((*(buf + i) == '\n') || (*(buf + i) == '\r')) {
            i++;
            break;
        }
    }
    return (i);
}

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */
void *sbrk (int nbytes)
{
    static char *heap = __end;
    char *end = NULL;
    char *base = heap;
    char *new_heap = heap + nbytes;

    if (!end)
    {
        /* Use sp - 256 as the heap limit.  */
        __asm__ __volatile__ ("move.l %/sp,%0" : "=r"(end));
        end -= 256;
    }
    if (nbytes < 0 || (long)(end - new_heap) < 0)
    {
        errno = ENOMEM;
        return (void *)-1;
    }
    heap = new_heap;
    return base;
}

/*
 * stat -- Since we have no file system, we just return an error.
 */
int stat (const char *path, struct stat *buf)
{
    errno = EIO;
    return -1;
}

/*
 * unlink -- since we have no file system,
 *           we just return an error.
 */
int unlink (char * path)
{
    errno = EIO;
    return -1;
}

/*
 * write -- write bytes to the serial port. Ignore fd, since
 *          stdout and stderr are the same. Since we have no filesystem,
 *          open will only return an error.
 */
int write (int fd, char *buf, int nbytes)
{
    int i;

    for (i = 0; i < nbytes; i++) {
        if (*(buf + i) == '\n') {
            outbyte ('\r');
        }
        outbyte (*(buf + i));
    }
    return nbytes;
}
