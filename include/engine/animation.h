/**
 * @file include/engine/animation.h
 */

#ifndef _ENGINE_ANIMATION_H_
#define _ENGINE_ANIMATION_H_

#include "engine/types.h"
#include "engine/config.h"

/**
 * Vector 3 Keyframe
 */
struct vec3_key
{
	f32 val[3]; ///< Value
};

/**
 * Vector 4 Keyframe
 */
struct vec4_key
{
	f32 val[4]; ///< Value
};

/**
 * Animation Flags
 */
enum animation_flags
{
	ANIM_FLAGS_NONE  = 0x0, ///< No Flags
	ANIM_IS_PLAYING  = 0x1, ///< Animation is Playing
	ANIM_IS_BACKWARD = 0x2, ///< Animation is Backwards
	ANIM_LOOPS       = 0x4, ///< Whether Animation Loops
};

/**
 * Animation Structure
 */
struct animation
{
	char name[CONF_NAME_MAX]; ///< Name
	u16 num_pos; ///< Number of Position Keyframes
	u16 num_rot; ///< Number of Rotation Keyframes
	u16 num_sca; ///< Number of Scaling Keyframes
	struct vec3_key *pos_keys; ///< Position Keyframes Array
	struct vec4_key *rot_keys; ///< Rotation Keyframes Array
	struct vec3_key *sca_keys; ///< Scaling Keyframes Array
	s16 frame_last; ///< Last Frame
	s16 frame; ///< Current Frame
	u16 mesh_ind; ///< Index for Mesh it Affects
	u16 length; ///< Length in Frames (24 FPS usually)
	enum animation_flags flags; ///< Flags, Obviously
};

void animation_update(struct animation *a);
void animation_matrix_multiply(const struct animation *a, const f32 subtick);

#endif /* _ENGINE_ANIMATION_H_ */
