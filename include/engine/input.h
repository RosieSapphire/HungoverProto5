/**
 * @file include/engine/input.h
 */

#ifndef _ENGINE_INPUT_H_
#define _ENGINE_INPUT_H_

#include <libdragon.h>

/**
 *  Input Parameters
 */
struct input_parms
{
	joypad_buttons_t press; ///< Buttons Just Pressed
	joypad_buttons_t held; ///< Buttons Held Down
	joypad_buttons_t up; ///< Buttons Released
	joypad_inputs_t stick; ///< Stick Inputs
};

#endif /* _ENGINE_INPUT_H_ */
