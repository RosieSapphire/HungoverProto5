#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "../../../include/engine/types.h"

/**
 * struct vertex - Vertex Structure
 * @pos: Position Component
 * @uv: Texture Coordinate Component
 * @col: 32-bit Color Component
 */
struct vertex
{
	f32 pos[3];
	f32 uv[2];
	u8 col[4];
};

#endif /* _VERTEX_H_ */
