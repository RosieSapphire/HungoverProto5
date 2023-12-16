/**
 * @file include/animation.h
 */

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "../../../include/engine/types.h"
#include "../../../include/engine/config.h"

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
 * Animation Structure
 */
struct animation
{
	char name[CONF_NAME_MAX]; ///< Name
	u16 num_pos; ///< Number of Position Keys
	u16 num_rot; ///< Number of Rotation Keys
	u16 num_sca; ///< Number of Scaling Keys
	struct vec3_key *pos_keys; ///< Position Keys
	struct vec4_key *rot_keys; ///< Rotation Keys
	struct vec3_key *sca_keys; ///< Scaling Keys
	u16 frame; ///< Current Frame
	u16 length; ///< Animation Length
	u16 mesh_ind; ///< Mesh Index
};

#endif /* _ANIMATION_H_ */
