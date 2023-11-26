#include "engine/vector.h"
#include "engine/collision_mesh.h"
#include "engine/raycast.h"

/**
 * raycast_triangle - Does a Raycast from eye to dir
 * @eye: Eye Position
 * @dir: Direction Vector
 * @positions: Array of Vec3s for the triangle
 * @distance: Hit Distance Return
 *
 * Return: Whether or not the ray hit the triangle
 */
u8 raycast_triangle(vec3 eye, vec3 dir, vec3 positions[3], f32 *distance)
{
	f32 edge1[3], edge2[3], p[3], t[3], q[3];

	vector_sub(positions[1], positions[0], edge1, 3);
	vector_sub(positions[2], positions[0], edge2, 3);
	vector3_cross(dir, edge2, p);

	f32 det = vector_dot(edge1, p, 3);
	const f32 epsilon = 0.001f;

	if (det > -epsilon && det < epsilon)
		return (0);

	f32 inv_det = 1.0f / det;

	vector_sub(eye, positions[0], t, 3);
	f32 u = inv_det * vector_dot(t, p, 3);

	if (u < 0.0f || u > 1.0f)
		return (0);

	vector3_cross(t, edge1, q);

	f32 v = inv_det * vector_dot(dir, q, 3);

	if (v < 0.0f || u + v > 1.0f)
		return (0);

	f32 dist = inv_det * vector_dot(edge2, q, 3);

	if (!distance)
		return (dist > epsilon);

	*distance = dist;

	return (dist > epsilon);
}

