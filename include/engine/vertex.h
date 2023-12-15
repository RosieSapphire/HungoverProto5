/**
 * @file include/engine/vertex.h
 */

#ifndef _ENGINE_VERTEX_H_
#define _ENGINE_VERTEX_H_

/**
 * Vertex Structure
 */
struct vertex
{
	f32 pos[3]; ///< pos Position Component
	f32 uv[2]; ///< uv Texture Coordinate Component
	u8 col[4]; ///< col 32-bit Color Component
};

#endif /* _ENGINE_VERTEX_H_ */
