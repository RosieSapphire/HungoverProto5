#include <stddef.h>

#include "engine/node.h"

/**
 * node_from_mesh_ind - Gets Pointer to Node from Mesh Index
 * @n: Node to start searching through
 * @mid: Mesh Index
 *
 * Return: Node Found (or NULL)
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
