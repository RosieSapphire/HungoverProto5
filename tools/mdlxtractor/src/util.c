/**
 * @file src/util.c
 */

#include "util.h"

/**
 * Wrapper for fwrite that flips endianness of u16
 * @param[in] ptr Pointer to Write
 * @param[out] stream Filestream to write to
 * @return Size Written
 */
size_t fwriteflipu16(const u16 *ptr, FILE *stream)
{
	u16 flip = ((*ptr >> 8) | (*ptr << 8));

	return (fwrite(&flip, sizeof(u16), 1, stream));
}

/**
 * Wrapper for fwrite that flips endianness of f32
 * @param[in] ptr Pointer to Write
 * @param[out] stream Filestream to write to
 * @return Size Written
 */
size_t fwriteflipf32(const f32 *ptr, FILE *stream)
{
	u32 tmp = *((u32 *)ptr);

	tmp = ((tmp << 8) & 0xFF00FF00) | ((tmp >> 8) & 0x00FF00FF);
	tmp = (tmp << 16) | (tmp >> 16);

	f32 num = (*((f32 *)&tmp));

	return (fwrite(&num, sizeof(f32), 1, stream));
}

/**
 * Wrapper for fread that flips endianness of u16
 * @param[in] ptr Pointer to Write
 * @param[out] stream Filestream to write to
*/
void freadflipu16(u16 *ptr, FILE *stream)
{
	fread(ptr, sizeof(u16), 1, stream);
	*ptr = ((*ptr >> 8) | (*ptr << 8));
}

/**
 * Wrapper for fread that flips endianness of f32
 * @param[in] ptr Pointer to Write
 * @param[out] stream Filestream to write to
 */
void freadflipf32(f32 *ptr, FILE *stream)
{
	fread(ptr, sizeof(f32), 1, stream);

	u32 tmp = *((u32 *)ptr);

	tmp = ((tmp << 8) & 0xFF00FF00) | ((tmp >> 8) & 0x00FF00FF);
	tmp = (tmp << 16) | (tmp >> 16);

	*ptr = (*((f32 *)&tmp));
}

/**
 * Transposing a 4x4 Matrix
 * @param[in] in Input Matrix
 * @param[out] out Output Matrix
 */
void matrix_transpose(const f32 *in, f32 *out)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out[j * 4 + i] = in[i * 4 + j];
}
