#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

/**
 * @file include/engine/camera.h
 */

#include "engine/types.h"
#include "engine/input.h"

/**
 * Camera Structure
 */
struct camera
{
	f32 eye[3]; ///< Current Eye Position
	f32 eye_last[3]; ///< Previous Eye Position
	f32 angles[2]; ///< Current Angles
	f32 angles_last[2]; ///< Previous Angles
};

/**
 * @defgroup engine Engine Functions
 * @{
 * @defgroup camera Camera Functions
 * @{
 */
void camera_init(struct camera *c);
void camera_update(struct camera *c, const struct input_parms iparms);
void camera_get_focus_now(const struct camera *c, f32 *out);
void camera_get_focus_lerp(f32 *eye_lerp, f32 *angles_lerp, f32 *out);
void camera_view_matrix_setup(const struct camera *c, f32 subtick);
/**
 * @}
 * @}
 */

#endif /* _ENGINE_CAMERA_H_ */
