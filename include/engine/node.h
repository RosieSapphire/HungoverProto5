#ifndef _ENGINE_NODE_H_
#define _ENGINE_NODE_H_

/**
 * @file include/engine/node.h
 */

#include "engine/types.h"
#include "engine/config.h"

/**
 * Node Structure
 */
struct node
{
	char mesh_path[CONF_PATH_MAX]; ///< Path to Mesh for Node
	u16 num_children; ///< Number of Node Children
	u16 mesh_ind; ///< Mesh Index
	f32 trans[4][4]; ///< Transformation Matrix
	struct node *children; ///< Node Children Array
};

struct node *node_from_mesh_ind(struct node *n, const u16 mid);

#endif /* _ENGINE_NODE_H_ */
