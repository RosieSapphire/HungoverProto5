#include "engine/scene.h"

enum scene_index scene_index = SCENE_TITLE;

/**
 * scene_destroy - Destroys a Scene
 * @s: Scene to Destroy
 */
void scene_destroy(struct scene *s)
{
	for (u16 i = 0; i < s->num_meshes; i++)
		mesh_destroy(s->meshes[i]);

	s->num_meshes = 0;
}
