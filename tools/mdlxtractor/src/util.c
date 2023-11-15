#include "util.h"

/**
 * fwriteflipu16 - Wrapper for fwrite that flips endianness of u16
 * @ptr: Pointer to Write
 * @stream: Filestream to write to
 *
 * Return: Size Written
 */
size_t fwriteflipu16(const u16 *ptr, FILE *stream)
{
	u16 flip = ((*ptr >> 8) | (*ptr << 8));

	return (fwrite(&flip, sizeof(u16), 1, stream));
}

/**
 * fwriteflipf32 - Wrapper for fwrite that flips endianness of f32
 * @ptr: Pointer to Write
 * @stream: Filestream to write to
 *
 * Return: Size Written
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
 * freadflipu16 - Wrapper for fread that flips endianness of u16
 * @ptr: Pointer to Write
 * @stream: Filestream to write to
 *
 * Return: Size Written
*/
void freadflipu16(u16 *ptr, FILE *stream)
{
	fread(ptr, sizeof(u16), 1, stream);
	*ptr = ((*ptr >> 8) | (*ptr << 8));
}

/**
 * freadflipf32 - Wrapper for fread that flips endianness of f32
 * @ptr: Pointer to Write
 * @stream: Filestream to write to
 *
 * Return: Size Written
 */
void freadflipf32(f32 *ptr, FILE *stream)
{
	fread(ptr, sizeof(f32), 1, stream);

	u32 tmp = *((u32 *)ptr);

	tmp = ((tmp << 8) & 0xFF00FF00) | ((tmp >> 8) & 0x00FF00FF);
	tmp = (tmp << 16) | (tmp >> 16);

	*ptr = (*((f32 *)&tmp));
}
