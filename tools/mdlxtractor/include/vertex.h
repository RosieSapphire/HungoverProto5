/**
 * @file include/vertex.h
 */

#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "../../../include/engine/types.h"

/**
 * Vertex Structure
 */
struct vertex
{
	f32 pos[3]; ///< @param pos Position Component
	f32 uv[2]; ///< @param uv Texture Coordinate Component
	u8 col[4]; ///< @param col 32-bit Color Component
};

#endif /* _VERTEX_H_ */
