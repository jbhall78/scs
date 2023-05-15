#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifdef UNIX
#include <time.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h> // read
#endif

#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "advapi32.lib")
#endif

#include "random.h"

// saved hints which were passed to rnd_open()
static int open_hints;

// driver selection
#ifdef UNIX
static rnd_driver_t *rnd_driver = &rnd_driver_dev_random;	
#else
#ifdef WIN32
static rnd_driver_t *rnd_driver = &rnd_driver_win32_crypt;	
#else
static rnd_driver_t *rnd_driver = &rnd_driver_rand;	
#endif
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Windows CryptGenRandom() driver
//
//////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
static HCRYPTPROV hCryptProv;

static void
win32_crypt_open(int hints)
{
    CryptAcquireContext(&hCryptProv, NULL, NULL,
		PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
}

static void
win32_crypt_bytes(int num, int8_t *buf)
{
    CryptGenRandom(hCryptProv, num, buf);
}

static void
win32_crypt_close(void)
{
    CryptReleaseContext(hCryptProv, 0);
}

static void
win32_crypt_seed(void)
{
    // RNG is automatically seeded with seed stored at:
    // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography\RND\Seed
}

rnd_driver_t rnd_driver_win32_crypt = {
	.open = win32_crypt_open,
	.bytes = win32_crypt_bytes,
	.close = win32_crypt_close,
	.seed = win32_crypt_seed,
};
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Linux/Unix /dev/random driver
//
//////////////////////////////////////////////////////////////////////////////
#ifdef UNIX
static int fd;
static char *filename;

static void
dev_random_open(int hints)
{
    if (hints & RANDOM_DEV_FAST)
	filename = "/dev/urandom";	
    else if (hints & RANDOM_DEV_SLOW)
	filename = "/dev/random";
    else
	filename = "/dev/urandom";	

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "ERROR: could not open file: %s: %s\n", filename, strerror(errno));
	exit(1); 
    }

    //	printf("opened: %s: %d\n", filename, fd);
}

static void
dev_random_bytes(int num, int8_t *buf)
{
    int ret;

    ret = read(fd, buf, num);
    if (ret != num) {
	fprintf(stderr, "ERROR: could not read %d bytes from: %s: %s\n", num, filename, strerror(errno));
	exit(1);
    }

    //	printf("read %d bytes from %s\n", num, filename);
}

static void
dev_random_close(void)
{
    close(fd);
}

static void
dev_random_seed(void)
{
}

rnd_driver_t rnd_driver_dev_random = {
	.open = dev_random_open,
	.bytes = dev_random_bytes,
	.close = dev_random_close,
	.seed = dev_random_seed,
};
#endif

//////////////////////////////////////////////////////////////////////////////
//
// C89/C99/POSIX srand() & rand() driver
//
//////////////////////////////////////////////////////////////////////////////

static unsigned int seed;

static void
rand_open(int hints)
{
}

static void
rand_close(void)
{
}

static void
rand_bytes(int num, int8_t *buf)
{
    int rnd;
    int8_t *p1, *p2;
    int i, j;

    p2 = buf;

    i = 0;
    while (i < num) {
	rnd = rand_r(&seed);
	p1 = (int8_t *)&rnd;

	for (j = 0; j < sizeof(int) && i < num; i++, j++) {
	    *p2 = *p1;
	    p1 += 1;
	    p2 += 1;	
	}
    }
}

void
rand_seed(void)
{
    seed = (unsigned int)time(NULL);
}

rnd_driver_t rnd_driver_rand = {
    .open = rand_open,
    .close = rand_close,
    .bytes = rand_bytes,
    .seed  = rand_seed,
};

//////////////////////////////////////////////////////////////////////////////
//
// POSIX srandom() & random() driver
//
//////////////////////////////////////////////////////////////////////////////
#ifdef UNIX

static char state[256];

static void
random_open(int hints)
{
    if (hints & RANDOM_DEV_SLOW)
	initstate(seed, state, 256);
    else if (hints & RANDOM_DEV_FAST)
	initstate(seed, state, 8);
}

static void
random_close(void)
{

}

static void
random_bytes(int num, int8_t *buf)
{
    long int rnd;
    int8_t *p1, *p2;
    int i, j;

    p2 = buf;

    i = 0;
    while (i < num) {
	rnd = random();
	p1 = (int8_t *)&rnd;

	for (j = 0; j < sizeof(long int) && i < num; i++, j++) {
	    *p2 = *p1;
	    p1 += 1;
	    p2 += 1;	
	}
    }
}

void
random_seed(void)
{
    rnd_driver_t *dev_random = &rnd_driver_dev_random;
    dev_random->open(open_hints);
    dev_random->bytes(sizeof(unsigned int), (int8_t *)&seed);
    dev_random->close();
}

rnd_driver_t rnd_driver_random = {
    .open = random_open,
    .close = random_close,
    .bytes = random_bytes,
    .seed  = random_seed,
};
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Exported API
//
//////////////////////////////////////////////////////////////////////////////

void
rnd_open(int hints)
{
    open_hints = hints;
    rnd_driver->seed();
    rnd_driver->open(hints);
}

void
rnd_close(void)
{
    rnd_driver->close();
}

void
rnd_bytes(int num, int8_t *buf)
{
    rnd_driver->bytes(num, buf);
}

int32_t
rnd_number(int min, int max)
{
    int32_t num;
    assert(max > min);
    int range = max - min + 1;
    rnd_driver->bytes(4, (int8_t *)&num);
    num = abs(num);
    num = num % range;
    num += min;
    return num;
}

void
rnd_seed(void)
{
    rnd_driver->seed();
}

//////////////////////////////////////////////////////////////////////////////

#ifdef RANDOM_MAIN
int
main(int argc, char **argv)
{
    int num;

    rnd_open(RANDOM_DEV_FAST);
    num = rnd_number(0, 10);
    printf("random number: %d\n", num);
    rnd_close();

    return 0;
}
#endif
