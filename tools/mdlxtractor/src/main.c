#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "util.h"
#include "scene.h"

static void _scene_debug_node(const struct node *n, const u8 depth)
{
	for (u8 i = 0; i < depth; i++)
		printf("\t");
	printf("[NODE] %s (Mesh %d)\n", n->name, n->mesh_ind);
	for (u16 i = 0; i < n->num_children; i++)
		_scene_debug_node(n->children + i, depth + 1);

}

/**
 * _scene_debug_assimp - Debugs everything for Scene Structure
 * @s: Scene to Debug
 */
static void _scene_debug_assimp(const struct scene *s)
{
	printf("[SCENE]\n");
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct mesh *m = s->meshes + i;

		printf("\t[MESH %d] %s\n", i, m->name);
		for (u16 j = 0; j < m->num_verts; j++)
		{
			const struct vertex *v = m->verts + j;
			const f32 *pos = v->pos;
			const f32 *uv = v->uv;
			const u8 *col = v->col;

			printf("\t\t[POS %d] %.3f, %.3f, %.3f\n",
				j, pos[0], pos[1], pos[2]);
			printf("\t\t[UV  %d] %.3f, %.3f\n", j, uv[0], uv[1]);
			printf("\t\t[COL %d] %u, %u, %u, %u\n\n",
				j, col[0], col[1], col[2], col[3]);
		}
	}

	_scene_debug_node(&s->root_node, 0);
}

static void _scene_convert_node(const struct aiNode *ni,
				struct node *no, u8 depth)
{
	strncpy(no->name, ni->mName.data, CONF_NAME_MAX);

	if (!ni->mNumMeshes)
		no->mesh_ind = 0xFFFF;
	else
		no->mesh_ind = ni->mMeshes[0];

	for (u8 i = 0; i < depth; i++)
		printf("\t");
	printf("[NODE] %s (Mesh %d)\n", no->name, no->mesh_ind);

	no->num_children = ni->mNumChildren;
	no->children = malloc(sizeof(struct node) * no->num_children);
	for (u16 i = 0; i < no->num_children; i++)
		_scene_convert_node(ni->mChildren[i], no->children + i,
		      depth + 1);
}

/**
 * _scene_convert_assimp - Converts an Assimp Scene into a Scene Structure
 * @si: Assimp Scene
 * @so: Output Scene
 */
static void _scene_convert_assimp(const struct aiScene *si, struct scene *so)
{
	assert(si->mNumMeshes);
	so->num_meshes = si->mNumMeshes;
	so->meshes = malloc(sizeof(struct mesh) * so->num_meshes);
	for (u16 i = 0; i < so->num_meshes; i++)
	{
		const struct aiMesh *aimesh = si->mMeshes[i];
		struct mesh *mesh = so->meshes + i;
		const u32 name_len = strlen(aimesh->mName.data);

		strncpy(mesh->name, aimesh->mName.data, name_len);
		mesh->num_verts = aimesh->mNumVertices;
		mesh->verts = malloc(sizeof(struct vertex) * mesh->num_verts);
		for (u16 j = 0; j < mesh->num_verts; j++)
		{
			struct vertex *v = mesh->verts + j;

			v->pos[0] = aimesh->mVertices[j].x;
			v->pos[1] = aimesh->mVertices[j].y;
			v->pos[2] = aimesh->mVertices[j].z;

			v->uv[0] = aimesh->mTextureCoords[0][j].x;
			v->uv[1] = aimesh->mTextureCoords[0][j].y;

			v->col[0] = aimesh->mColors[0][j].r * 255;
			v->col[1] = aimesh->mColors[0][j].g * 255;
			v->col[2] = aimesh->mColors[0][j].b * 255;
			v->col[3] = aimesh->mColors[0][j].a * 255;
		}

		mesh->num_indis = aimesh->mNumFaces * 3;
		mesh->indis = malloc(sizeof(u16) * mesh->num_indis);
		for (u16 j = 0; j < mesh->num_indis / 3; j++)
		{
			for (u16 k = 0; k < 3; k++)
			{
				mesh->indis[j * 3 + k] =
					aimesh->mFaces[j].mIndices[k];
			}
		}
	}

	_scene_convert_node(si->mRootNode, &so->root_node, 0);
}

static void _scene_write_mesh_file(const struct mesh *m, const char *scnpath,
				   FILE *scnfile)
{
	const u32 strlength = strlen(scnpath);
	char mdlpath[512];
	char *scnpath_crop = malloc(strlength - 2);

	strncpy(scnpath_crop, scnpath, strlength - 3);
	scnpath_crop[strlength - 4] = 0;
	sprintf(mdlpath, "%s.%s.mdl", scnpath_crop, m->name);

	FILE *f = fopen(mdlpath, "wb");

	fwrite(m->name, sizeof(char), CONF_NAME_MAX, f);
	fwriteflipu16(&m->num_verts, f);
	fwriteflipu16(&m->num_indis, f);
	for (u16 j = 0; j < m->num_verts; j++)
	{
		const struct vertex *v = m->verts + j;

		fwriteflipf32(v->pos + 0, f);
		fwriteflipf32(v->pos + 1, f);
		fwriteflipf32(v->pos + 2, f);

		fwriteflipf32(v->uv + 0, f);
		fwriteflipf32(v->uv + 1, f);

		fwrite(v->col, sizeof(u32), 1, f);
	}

	for (u16 j = 0; j < m->num_indis; j++)
		fwriteflipu16(m->indis + j, f);

	fclose(f);

	fwrite(mdlpath, sizeof(char), CONF_NAME_MAX, scnfile);
}

/**
 * _scene_write_file - Writes Converted Scene to File
 * @s: Scene to Write
 * @outpath: Path to Write To
 */
static void _scene_write_file(const struct scene *s, const char *outpath)
{
	FILE *f = fopen(outpath, "wb");

	if (f)
	{
		fclose(f);
		remove(outpath);
		f = fopen(outpath, "wb");
	}

	fwriteflipu16(&s->num_meshes, f);
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		_scene_write_mesh_file(s->meshes + i, outpath, f);
	}

	fclose(f);
}

static void _scene_import_test_mesh(struct scene *s, const char *mpath, u8 i)
{
	struct mesh *m = s->meshes + i;
	FILE *mf = fopen(mpath, "rb");

	if (!mf)
	{
		fprintf(stderr, "Couldn't find mesh from '%s'\n", mpath);
		exit(1);
	}

	fread(m->name, sizeof(char), CONF_NAME_MAX, mf);
	freadflipu16(&m->num_verts, mf);
	freadflipu16(&m->num_indis, mf);
	m->verts = malloc(sizeof(struct vertex) * m->num_verts);
	m->indis = malloc(sizeof(u16) * m->num_indis);
	for (u16 j = 0; j < m->num_verts; j++)
	{
		struct vertex *v = m->verts + j;

		freadflipf32(v->pos + 0, mf);
		freadflipf32(v->pos + 1, mf);
		freadflipf32(v->pos + 2, mf);

		freadflipf32(v->uv + 0, mf);
		freadflipf32(v->uv + 1, mf);

		fread(v->col, sizeof(u32), 1, mf);
	}

	for (u16 j = 0; j < m->num_indis; j++)
		freadflipu16(m->indis + j, mf);

	fclose(mf);
}

/**
 * _scene_import_test - Test Scene Importing
 * @s: Scene to Import To
 * @path: Path to Import From
 */
static void _scene_import_test(struct scene *s, const char *path)
{
	FILE *sf = fopen(path, "rb");

	if (!sf)
	{
		fprintf(stderr, "Couldn't find scene at '%s'\n", path);
		exit(1);
	}

	freadflipu16(&s->num_meshes, sf);
	s->meshes = malloc(sizeof(struct mesh) * s->num_meshes);
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		char pathbuf[512];

		fread(pathbuf, sizeof(char), CONF_NAME_MAX, sf);
		_scene_import_test_mesh(s, pathbuf, i);
	}

	fclose(sf);
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

	const u32 flags = aiProcess_Triangulate | aiProcess_FlipUVs |
		aiProcess_ImproveCacheLocality |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure;
	const struct aiScene *aiscene = aiImportFile(pathin, flags);

	if (!aiscene)
	{
		fprintf(stderr, "Failed to load scene from '%s'\n", pathin);
		exit(1);
	}

	struct scene s;

	_scene_convert_assimp(aiscene, &s);
	_scene_write_file(&s, pathout);
	_scene_import_test(&s, pathout);
	_scene_debug_assimp(&s);

	return (0);
}
