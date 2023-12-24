#ifndef _ENGINE_SCENE_H_
#define _ENGINE_SCENE_H_

/**
 * @file include/engine/scene.h
 */

#include "engine/mesh.h"
#include "engine/node.h"
#include "engine/animation.h"
#include "engine/config.h"

/**
 * Scene Index
 */
enum scene_index
{
	SCENE_UNKNOWN = 0xFFFF, ///< Unknown Scene
	SCENE_TITLE = 0, ///< Title Screen
	SCENE_TESTROOM, ///< Test Room
	NUM_SCENES, ///< Number of Scenes
};

/**
 * Which scene is currently active (global)
 */
extern enum scene_index scene_index;

/**
 * Scene Structure
 */
struct scene
{
	struct node root_node; ///< Root Node for Scene
	u16 num_tex_ids; ///< Number of OpenGL Texture IDs
	u32 *tex_ids; ///< OpenGL Texture IDs Array
	u16 num_meshes; ///< Number of Meshes
	struct mesh *meshes; ///< Meshes Array
	u16 num_anims; ///< Number of Animations
	struct animation *anims; ///< Animations Array
};

/**
 * @defgroup engine Engine Functions
 * @{
 * @defgroup scene Scene Functions
 * @{
 */

/**
 * @defgroup scene_main Scene Main Functions
 * @{
 */
void scene_global_spin_timer_update(void);
void scene_read_file(struct scene *s, const char *path);
void scene_draw(const struct scene *s, const f32 subtick, const u16 anim_set);
void scene_destroy(struct scene *s);
struct node *node_from_mesh_ind(struct node *n, const u16 mid);
/**
 * @}
 */

/**
 * @defgroup scene_anims Scene Animation Functions
 * @{
 */
void scene_anims_update(struct scene *s, const u16 index);
void scene_anims_set_flags(struct scene *s, u8 flags);
void scene_anims_set_frame(struct scene *s, u16 frame);
/**
 * @}
 * @}
 * @}
 */

#endif /* _ENGINE_SCENE_H_ */
