#ifndef _VAR_H
#define _VAR_H

/* note: need to think about string types more
 *       also need to determine if i should allow for
 *       additional data types to be included in this
 *       api.
 */

// merge this with the stuff in shell.[ch]
//typedef enum {
#define    VAR_NONE		0x00
#define    VAR_BOOL		0x01
#define    VAR_CHAR8		0x02
#define    VAR_UCHAR8		0x03
#define    VAR_STRING	 	0x04
#define    VAR_INT8		0x05
#define    VAR_UINT8		0x06
#define    VAR_INT16		0x07
#define    VAR_UINT16		0x08
#define    VAR_INT32		0x09
#define    VAR_UINT32		0x0A
#define    VAR_INT64		0x0B
#define    VAR_UINT64		0x0C
#define    VAR_FLOAT		0x0D
#define    VAR_DOUBLE		0x0E
#define    VAR_PTR		0x0F
//} var_type_t;

typedef struct {
    int8_t type;
    int16_t size;		// sizeof(data_element) (for 8bit string types this is 1)
    int32_t len;		// length of data, for string & pointer types
    bool error;			// error flag which tells us if there was an error during type conversion
    bool ptr_requires_free;	// if the ptr or char * value requires free()ing
    union {
	bool b;
	char c8;
	char uc8;
	char *s8;
	int8_t i8;
	uint8_t ui8;
	int16_t i16;
	uint16_t ui16;
	int32_t i32;
	uint32_t ui32;
	int64_t i64;
	uint64_t ui64;
	float flt;
	double dbl;
	void *ptr;
    };
} var_t;

/* declare inline functions like this:
 * (i probably need to put these all in a header don't i)
// Microsoft C compiler
// __forceinline var_t * int32(int32_t);
*/


// initialization / self tests
void var_init(void);

// stack allocated
static inline var_t *bools(int8_t val) __attribute__((always_inline));
static inline var_t *char8s(int8_t val) __attribute__((always_inline));
static inline var_t *strs(char *val) __attribute__((always_inline));
static inline var_t *uchar8s(uint8_t val) __attribute__((always_inline));
static inline var_t *int8s(int8_t val) __attribute__((always_inline));
static inline var_t *uint8s(uint8_t val) __attribute__((always_inline));
static inline var_t *int16s(int16_t val) __attribute__((always_inline));
static inline var_t *uint16s(uint16_t val) __attribute__((always_inline));
static inline var_t *int32s(int32_t val) __attribute__((always_inline));
static inline var_t *uint32s(uint32_t val) __attribute__((always_inline));
static inline var_t *int64s(int64_t val) __attribute__((always_inline));
static inline var_t *uint64s(uint64_t val) __attribute__((always_inline));
static inline var_t *floats(float val) __attribute__((always_inline));
static inline var_t *doubles(double val) __attribute__((always_inline));

// heap allocated
static inline var_t *boolh(int8_t val) __attribute__((always_inline));
static inline var_t *char8h(int8_t val) __attribute__((always_inline));
static inline var_t *string8h(char *val) __attribute__((always_inline));
static inline var_t *uchar8h(uint8_t val) __attribute__((always_inline));
static inline var_t *int8h(int8_t val) __attribute__((always_inline));
static inline var_t *uint8h(uint8_t val) __attribute__((always_inline));
static inline var_t *int16h(int16_t val) __attribute__((always_inline));
static inline var_t *uint16h(uint16_t val) __attribute__((always_inline));
static inline var_t *int32h(int32_t val) __attribute__((always_inline));
static inline var_t *uint32h(uint32_t val) __attribute__((always_inline));
static inline var_t *int64h(int64_t val) __attribute__((always_inline));
static inline var_t *uint64h(uint64_t val) __attribute__((always_inline));
static inline var_t *floath(float val) __attribute__((always_inline));
static inline var_t *doubleh(double val) __attribute__((always_inline));

// string conversion
static void var_to_str(var_t *var, char *buf, int len);
static void bool_to_str(var_t *v, char *buf, int len);
static void char8_to_str(var_t *v, char *buf, int len);
static void uchar8_to_str(var_t *v, char *buf, int len);
static void str_to_str(var_t *v, char *buf, int len);
static void int8_to_str(var_t *v, char *buf, int len);
static void int16_to_str(var_t *v, char *buf, int len);
static void int32_to_str(var_t *v, char *buf, int len);
static void int64_to_str(var_t *v, char *buf, int len);
static void uint8_to_str(var_t *v, char *buf, int len);
static void uint16_to_str(var_t *v, char *buf, int len);
static void uint32_to_str(var_t *v, char *buf, int len);
static void uint64_to_str(var_t *v, char *buf, int len);
static void float_to_str(var_t *v, char *buf, int len);
static void double_to_str(var_t *v, char *buf, int len);

#include "var_auto.c"

#endif
