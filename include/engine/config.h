#ifndef _ENGINE_CONFIG_H_
#define _ENGINE_CONFIG_H_

/**
 * @file include/engine/config.h
 */

#define CONF_DEBUG ///< If debugging is enabled

#define CONF_WIDTH 320 ///< Viewport width
#define CONF_HEIGHT 240 ///< Viewport height
#define CONF_ASPECT ((float)CONF_WIDTH / (float)CONF_HEIGHT) ///< Aspect ratio

#define CONF_DECAL_MAX 128 ///< Maximum number of decals

#define CONF_TICKRATE 24 ///< Update tickrate
#define CONF_DELTATIME (1.0f / (float)CONF_TICKRATE) ///< Update deltatime
#define CONF_DELTATICKS (CONF_DELTATIME * TICKS_PER_SECOND) ///< Update dticks

#define CONF_NAME_MAX 128 ///< Max name length
#define CONF_PATH_MAX 128 ///< Max path length
#define CONF_MAX_NUM_AREAS 128 ///< Max number of areas in a scene

#define CONF_NEAR  0.1f ///< Near plane
#define CONF_FAR  256 ///< Far plane

#define CONF_AUDIO_FREQ 32000 ///< Base audio frequency
#define CONF_NUM_AUDIO_BUF 3 ///< Number of audio buffers

#define MAX_SCENE_TEXS 16 ///< Max number of textures in a scene
#define MAX_TOTAL_TEXS 128 ///< Max number of textures total
#define TEX_PATH_MAX_LEN 128 ///< Max length for texture path

#endif /* _ENGINE_CONFIG_H_ */
