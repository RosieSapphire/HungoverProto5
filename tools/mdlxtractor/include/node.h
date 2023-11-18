#ifndef _NODE_H_
#define _NODE_H_

#include "../../../include/engine/types.h"
#include "../../../include/engine/config.h"

struct node
{
	char name[CONF_NAME_MAX];
	u16 mesh_ind, num_children;
	f32 trans[4][4];
	struct node *children;
};

#endif /* _NODE_H_ */
