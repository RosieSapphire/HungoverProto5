/**
 * @file src/mesh.c
 */

#include <assert.h>
#include <string.h>
#include <malloc.h>

#include "mesh.h"

void mesh_path_from_scene_path(char *out, const char *mname,
			       const char *scnpath)
{
	assert(out);

	const u32 scnpathlen = strlen(scnpath);
	char *scnpath_crop = malloc(scnpathlen);

	strncpy(scnpath_crop, scnpath, scnpathlen);
	scnpath_crop[scnpathlen - 4] = 0;
	sprintf(out, "%s.%s.mdl", scnpath_crop, mname);
	free(scnpath_crop);
}

u16 mesh_ind_by_name(const char *name, const struct mesh *meshes,
		     const u16 num_meshes)
{
	for (u16 i = 0; i < num_meshes; i++)
	{
		const struct mesh *m = meshes + i;

		if (!strcmp(m->name, name))
			return (i);
	}

	return (0xFFFF);
}
