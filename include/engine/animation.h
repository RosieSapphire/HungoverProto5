#ifndef _ENGINE_ANIMATION_H_
#define _ENGINE_ANIMATION_H_

#include "engine/types.h"
#include "engine/config.h"

/**
 * struct vec3_key - Vector 3 Keyframe
 * @val: Value
 */
struct vec3_key
{
	f32 val[3];
};

/**
 * struct vec4_key - Vector 4 Keyframe
 * @val: Value
 */
struct vec4_key
{
	f32 val[4];
};

/**
 * enum animation_flags - Animation Flags
 * @ANIM_FLAGS_NONE: No Flags
 * @ANIM_IS_PLAYING: Animation is Playing
 * @ANIM_IS_BACKWARD: Animation is Backwards
 * @ANIM_LOOPS: Whether Animation Loops
 */
enum animation_flags
{
	ANIM_FLAGS_NONE  = 0x0,
	ANIM_IS_PLAYING  = 0x1,
	ANIM_IS_BACKWARD = 0x2,
	ANIM_LOOPS       = 0x4,
};

/**
 * struct animation - Animation Structure
 * @name: Name
 * @num_pos: Number of Position Keyframes
 * @num_rot: Number of Rotation Keyframes
 * @num_sca: Number of Scaling Keyframes
 * @pos_keys: Position Keyframes Array
 * @rot_keys: Rotation Keyframes Array
 * @sca_keys: Scaling Keyframes Array
 * @frame_last: Last Frame
 * @frame: Current Frame
 * @mesh_ind: Index for Mesh it Affects
 * @length: Length in Frames (24 FPS usually)
 * @flags: Flags, Obviously
 */
struct animation
{
	char name[CONF_NAME_MAX];
	u16 num_pos, num_rot, num_sca;
	struct vec3_key *pos_keys;
	struct vec4_key *rot_keys;
	struct vec3_key *sca_keys;
	s16 frame_last, frame;
	u16 mesh_ind, length;
	enum animation_flags flags;
};

void animation_update(struct animation *a);
void animation_matrix_multiply(const struct animation *a, const f32 subtick);

#endif /* _ENGINE_ANIMATION_H_ */
