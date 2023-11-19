#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

#include "engine/types.h"
#include "engine/input.h"

/**
 * struct camera - Camera Structure
 * @eye_last: Eye Position Last
 * @eye: Eye Position Now
 * @angles_last: Angles Last
 * @angles: Angles Now
 */
struct camera
{
	f32 eye_last[3], eye[3];
	f32 angles_last[2], angles[2];
};

void camera_init(struct camera *c);
void camera_update(struct camera *c, const struct input_parms iparms);
void camera_get_focus_last(const struct camera *c, f32 *out);
void camera_get_focus_now(const struct camera *c, f32 *out);
void camera_get_focus_lerp(f32 *eye_lerp, f32 *angles_lerp, f32 *out);
void camera_view_matrix_setup(const struct camera *c, f32 subtick);

#endif /* _ENGINE_CAMERA_H_ */
