/**
 * @file include/util.h
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include "../../../include/engine/types.h"

size_t fwriteflipu16(const u16 *ptr, FILE *stream);
size_t fwriteflipf32(const f32 *ptr, FILE *stream);
void freadflipu16(u16 *ptr, FILE *stream);
void freadflipf32(f32 *ptr, FILE *stream);
void matrix_transpose(const f32 *in, f32 *out);

#endif /* _UTIL_H_ */
