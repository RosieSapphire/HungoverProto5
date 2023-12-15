/**
 * @file src/engine/node.c
 */

#include <stddef.h>

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
