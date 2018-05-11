/* ------------- */
/* --- def.h --- */
/* ------------- */

#ifndef __DEF_H__
#define __DEF_H__

#ifdef __cplusplus
extern "C" {
#endif
    
typedef unsigned char byte;

typedef unsigned char uint8;
typedef   signed char sint8;

typedef unsigned short uint16;
typedef   signed short sint16;

typedef unsigned int uint32;
typedef   signed int sint32;

typedef float         float32;
typedef double        float64;
typedef struct { byte r; byte g; byte b;} rgb8;

#ifdef __cplusplus
}
#endif

#endif // __DEF_H__
