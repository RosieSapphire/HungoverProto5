#ifndef _ENGINE_CONFIG_H_
#define _ENGINE_CONFIG_H_

#include "engine/types.h"

#define CONF_WIDTH 320
#define CONF_HEIGHT 240
#define CONF_ASPECT ((f32)CONF_WIDTH / (f32)CONF_HEIGHT)

#define CONF_TICKRATE 24
#define CONF_DELTATIME (1.0f / (f32)CONF_TICKRATE)
#define CONF_DELTATICKS (CONF_DELTATIME * TICKS_PER_SECOND)

#define CONF_NAME_MAX 128

#define CONF_NEAR  1
#define CONF_FAR  256

#define CONF_AUDIO_FREQ 32000
#define CONF_NUM_AUDIO_BUF 4

#endif /* _ENGINE_CONFIG_H_ */
