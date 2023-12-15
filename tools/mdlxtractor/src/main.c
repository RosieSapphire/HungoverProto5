#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "util.h"
#include "scene.h"

static void _scene_free_node_tree(const struct node *start)
{
	assert(start);

	for (u16 i = 0; i < start->num_children; i++)
		_scene_free_node_tree(start->children + i);

	if (start->num_children)
		free(start->children);
}

static void _scene_free(struct scene *s)
{
	_scene_free_node_tree(&s->root_node);

	for (u16 i = 0; i < s->num_meshes; i++)
	{
		struct mesh *m = s->meshes + i;

		memset(m->name, 0, CONF_NAME_MAX);
		free(m->verts);
		free(m->indis);
	}

	for (u16 i = 0; i < s->num_anims; i++)
	{
		struct animation *a = s->anims + i;

		memset(a->name, 0, CONF_NAME_MAX);
		free(a->pos_keys);
		free(a->rot_keys);
		free(a->sca_keys);
	}
}

/**
 * main - Main Function
 * @argc: Argument Count
 * @argv: Argument Values
 *
 * Return: 0 for Success
 */
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage %s: [.glb input] [.scn output]\n", argv[0]);
		return (1);
	}

	const char *pathin = argv[1];
	const char *pathout = argv[2];
	const u32 flags = aiProcess_Triangulate | aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
		| aiProcess_ImproveCacheLocality;
	const struct aiScene *aiscene = aiImportFile(pathin, flags);

	if (!aiscene)
	{
		fprintf(stderr, "Failed to load scene from '%s'\n", pathin);
		exit(1);
	}

	struct scene s;

	scene_convert_assimp(aiscene, &s, pathin);
	scene_debug_assimp(&s);
	scene_write_file(&s, pathout);
	_scene_free(&s);
	scene_read_file(&s, pathout);

	return (0);
}
