#ifndef _NODE_H_
#define _NODE_H_

#include "../../../include/engine/types.h"
#include "../../../include/engine/config.h"

struct node
{
	char mesh_path[CONF_PATH_MAX];
	u16 num_children, mesh_ind;
	f32 trans[4][4];
	struct node *children;
};

#endif /* _NODE_H_ */
