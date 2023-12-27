#ifndef _NODE_H_
#define _NODE_H_

#include "../../../include/engine/types.h"
#include "../../../include/engine/config.h"

/**
 * Node Structure
 */
struct node
{
	char name[CONF_NAME_MAX]; ///< Name for Node
	char mesh_path[CONF_PATH_MAX]; ///< Mesh Path
	u16 num_children; ///< Number of Children
	u16 mesh_ind; ///< Mesh Index
	f32 trans[4][4]; ///< Transformation Matrix
	struct node *children; ///< Children array
};

#endif /* _NODE_H_ */
