#ifndef _ENGINE_SCENE_H_
#define _ENGINE_SCENE_H_

#include "engine/mesh.h"
#include "engine/node.h"
#include "engine/animation.h"
#include "engine/config.h"

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
 * @root_node: Root Node for Scene
 * @num_tex_ids: Number of OpenGL Texture IDs
 * @tex_ids: OpenGL Texture IDs Array
 * @num_meshes: Number of Meshes
 * @meshes: Meshes Array
 * @num_anims: Number of Animations
 * @anims: Animations Array
 */
struct scene
{
	struct node root_node;
	u16 num_tex_ids;
	u32 *tex_ids;
	u16 num_meshes;
	struct mesh *meshes;
	u16 num_anims;
	struct animation *anims;
};

/*
 * Main
 */
void scene_global_spin_timer_update(void);
void scene_read_file(struct scene *s, const char *path);
void scene_draw(const struct scene *s, const f32 subtick);
void scene_destroy(struct scene *s);
struct node *node_from_mesh_ind(struct node *n, const u16 mid);

/*
 * Animations
 */
void scene_anims_update(struct scene *s);
void scene_anims_set_flags(struct scene *s, u8 flags);
void scene_anims_set_frame(struct scene *s, u16 frame);

#endif /* _ENGINE_SCENE_H_ */
