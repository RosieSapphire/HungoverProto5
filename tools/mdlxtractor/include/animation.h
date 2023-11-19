#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "../../../include/engine/types.h"
#include "../../../include/engine/config.h"

struct vec3_key
{
	f32 val[3];
};

struct vec4_key
{
	f32 val[4];
};

struct animation
{
	char name[CONF_NAME_MAX];
	u16 num_pos, num_rot, num_sca;
	struct vec3_key *pos_keys;
	struct vec4_key *rot_keys;
	struct vec3_key *sca_keys;
	u16 frame, length, mesh_ind;
};

#endif /* _ANIMATION_H_ */
