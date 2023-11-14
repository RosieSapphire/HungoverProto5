#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "scene.h"

static size_t _fwriteflipu16(const u16 *ptr, FILE *stream)
{
	u16 flip = ((*ptr >> 8) | (*ptr << 8));

	return (fwrite(&flip, sizeof(u16), 1, stream));
}

static size_t _fwriteflipf32(const f32 *ptr, FILE *stream)
{
	u32 tmp = *((u32 *)ptr);

	tmp = ((tmp << 8) & 0xFF00FF00) | ((tmp >> 8) & 0x00FF00FF);
	tmp = (tmp << 16) | (tmp >> 16);

	f32 num = (*((f32 *)&tmp));

	return (fwrite(&num, sizeof(f32), 1, stream));
}

static void _freadflipu16(u16 *ptr, FILE *stream)
{
	fread(ptr, sizeof(u16), 1, stream);
	*ptr = ((*ptr >> 8) | (*ptr << 8));
}

static void _freadflipf32(f32 *ptr, FILE *stream)
{
	fread(ptr, sizeof(f32), 1, stream);

	u32 tmp = *((u32 *)ptr);

	tmp = ((tmp << 8) & 0xFF00FF00) | ((tmp >> 8) & 0x00FF00FF);
	tmp = (tmp << 16) | (tmp >> 16);

	*ptr = (*((f32 *)&tmp));
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
}

/**
 * _scene_convert_assimp - Converts an Assimp Scene into a Scene Structure
 * @si: Assimp Scene
 * @so: Output Scene
 */
static void _scene_convert_assimp(const struct aiScene *si, struct scene *so)
{
	so->num_meshes = si->mNumMeshes;
	so->meshes = malloc(sizeof(struct mesh *) * so->num_meshes);
	for (u16 i = 0; i < so->num_meshes; i++)
	{
		const struct aiMesh *aimesh = si->mMeshes[i];
		struct mesh *mesh = so->meshes + i;

		strncpy(mesh->name, aimesh->mName.data,
	  		strlen(aimesh->mName.data));

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

	_fwriteflipu16(&s->num_meshes, f);
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct mesh *m = s->meshes + i;

		fwrite(m->name, sizeof(char), CONF_NAME_MAX, f);
		_fwriteflipu16(&m->num_verts, f);
		_fwriteflipu16(&m->num_indis, f);
		for (u16 j = 0; j < m->num_verts; j++)
		{
			const struct vertex *v = m->verts + j;

			_fwriteflipf32(v->pos + 0, f);
			_fwriteflipf32(v->pos + 1, f);
			_fwriteflipf32(v->pos + 2, f);

			_fwriteflipf32(v->uv + 0, f);
			_fwriteflipf32(v->uv + 1, f);

			fwrite(v->col, sizeof(u32), 1, f);
		}

		for (u16 j = 0; j < m->num_indis; j++)
			_fwriteflipu16(m->indis + j, f);
	}

	fclose(f);
}

static void _scene_import_test(struct scene *s, const char *path)
{
	FILE *f = fopen(path, "rb");

	if (!f)
	{
		fprintf(stderr, "Couldn't find scene at '%s'\n", path);
		exit(1);
	}

	_freadflipu16(&s->num_meshes, f);
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		struct mesh *m = s->meshes + i;

		fread(m->name, sizeof(char), CONF_NAME_MAX, f);
		_freadflipu16(&m->num_verts, f);
		_freadflipu16(&m->num_indis, f);
		for (u16 j = 0; j < m->num_verts; j++)
		{
			struct vertex *v = m->verts + j;

			_freadflipf32(v->pos + 0, f);
			_freadflipf32(v->pos + 1, f);
			_freadflipf32(v->pos + 2, f);

			_freadflipf32(v->uv + 0, f);
			_freadflipf32(v->uv + 1, f);

			fread(v->col, sizeof(u32), 1, f);
		}
	}

	fclose(f);
}

/**
 * main - Main Function
 *
 * Return: 0 for Success
 */
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage %s: [.glb input] [.scn output]\n", argv[0]);
		return 1;
	}

	const char *pathin = argv[1];
	const char *pathout = argv[2];

	const u32 flags = aiProcess_Triangulate | aiProcess_FlipUVs |
		aiProcess_OptimizeGraph | aiProcess_ImproveCacheLocality |
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
