#ifndef _ENGINE_INPUT_H_
#define _ENGINE_INPUT_H_

#include <libdragon.h>

/**
 * struct input_parms - Input Parameters
 * @press: Buttons Just Pressed
 * @held: Buttons Held Down
 * @stick: Stick Inputs
 */
struct input_parms
{
	joypad_buttons_t press, held;
	joypad_inputs_t stick;
};

#endif /* _ENGINE_INPUT_H_ */
