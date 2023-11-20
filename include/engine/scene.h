#ifndef _ENGINE_SCENE_H_
#define _ENGINE_SCENE_H_

#include "engine/mesh.h"
#include "engine/node.h"
#include "engine/animation.h"

/**
 * enum scene_index - Scene Index
 * @SCENE_TITLE: Title Screen
 * @SCENE_TESTROOM: Test Room
 * @NUM_SCENES: Number of Scenes
 */
enum scene_index
{
	SCENE_TITLE,
	SCENE_TESTROOM,
	NUM_SCENES,
};

extern enum scene_index scene_index;

/**
 * struct scene - Scene Structure
 * @num_meshes: Number of Meshes
 * @root_node: Root Node for the Scene
 * @meshes: Meshes Array
 */
struct scene
{
	u16 num_meshes;
	struct node root_node;
	struct mesh *meshes;
	u16 num_anims;
	struct animation *anims;
};

void scene_read_file(struct scene *s, const char *path);
void scene_anims_update(struct scene *s);
void scene_draw(const struct scene *s, const f32 subtick);
void scene_destroy(struct scene *s);

#endif /* _ENGINE_SCENE_H_ */
