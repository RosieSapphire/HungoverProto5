/**
 * @file src/engine/node.c
 */

#include <stddef.h>

#include "engine/scene.h"
#include "engine/node.h"

/**
 * Gets Pointer to Node from Mesh Index
 * @param[in] n Node to start searching through
 * @param[in] mid Mesh Index
 * @return Node Found (or NULL)
 */
struct node *node_from_mesh_ind(struct node *n, const u16 mid)
{
	if (mid == n->mesh_ind)
		return (n);

	struct node *nf;

	for (u16 i = 0; i < n->num_children; i++)
	{
		nf = node_from_mesh_ind(n->children + i, mid);
		if (nf)
			return (nf);
	}

	return (NULL);
}

/**
 * Searches through node tree to find node by name
 * @param[in] start Starting Node
 * @param[in] name Desired Node's Name
 * @return Node with desired name
 */
struct node *node_children_find(struct node *start, const char *name)
{
	if (!strncmp(start->name, name, strlen(name)))
		return (start);

	struct node *n = NULL;

	for (u16 i = 0; i < start->num_children; i++)
		if ((n = node_children_find(start->children + i, name)))
			return (n);

	return (NULL);
}

void node_tree_toggle_meshes(struct mesh *meshes, const u16 num_meshes,
			     struct node *start, const u8 toggle)
{
	struct mesh *m;

	if (start->mesh_ind == 0xFFFF)
		goto check_children;

	if (!(m = mesh_get_ind(meshes, start->mesh_ind, num_meshes)))
		goto check_children;

	if (toggle)
		m->flags |= MESH_IS_ACTIVE;
	else
		m->flags &= ~(MESH_IS_ACTIVE);

check_children:
	for (u16 i = 0; i < start->num_children; i++)
		node_tree_toggle_meshes(meshes, num_meshes,
					start->children + i, toggle);
}
