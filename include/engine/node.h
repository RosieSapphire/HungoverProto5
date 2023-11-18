#ifndef _ENGINE_NODE_H_
#define _ENGINE_NODE_H_

#include "engine/types.h"
#include "engine/config.h"

struct node
{
	char mesh_path[CONF_PATH_MAX];
	u16 num_children, mesh_ind;
	f32 trans[4][4];
	struct node *children;
};

#endif /* _ENGINE_NODE_H_ */
