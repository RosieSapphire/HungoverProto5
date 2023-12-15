/**
 * @file include/engine/raycast.h
 */

#ifndef _ENGINE_RAYCAST_H_
#define _ENGINE_RAYCAST_H_

#include "engine/types.h"
#include "engine/collision_mesh.h"

u8 raycast_triangle(vec3 eye, vec3 dir, vec3 *positions, f32 *distance);

#endif /* _ENGINE_RAYCAST_H_ */
