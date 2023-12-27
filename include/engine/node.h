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
	char name[CONF_NAME_MAX]; ///< Name for Node
	char mesh_path[CONF_PATH_MAX]; ///< Path to Mesh for Node
	u16 num_children; ///< Number of Node Children
	u16 mesh_ind; ///< Mesh Index
	f32 trans[4][4]; ///< Transformation Matrix
	struct node *children; ///< Node Children Array
};

/**
 * @defgroup engine Engine Functions
 * @{
 * @defgroup node Node Functions
 * @{
 */
struct node *node_from_mesh_ind(struct node *n, const u16 mid);
struct node *node_children_find(struct node *start, const char *name);
void node_tree_toggle_meshes(struct mesh *meshes, const u16 num_meshes,
			     struct node *start, const u8 toggle);
/**
 * @}
 * @}
 */

#endif /* _ENGINE_NODE_H_ */
