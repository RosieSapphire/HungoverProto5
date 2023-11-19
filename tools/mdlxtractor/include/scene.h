#ifndef _SCENE_H_
#define _SCENE_H_

#include "mesh.h"
#include "node.h"
#include "animation.h"

/**
 * struct scene - Scene Structure
 * @num_meshes: Number of Meshes
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

#endif /* _SCENE_H_ */
