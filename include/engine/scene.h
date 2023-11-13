#ifndef _ENGINE_SCENE_H_
#define _ENGINE_SCENE_H_

#include "engine/mesh.h"

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
 * @meshes: Meshes Array
 */
struct scene
{
	u16 num_meshes;
	struct mesh **meshes;
};

void scene_destroy(struct scene *s);

#endif /* _ENGINE_SCENE_H_ */
