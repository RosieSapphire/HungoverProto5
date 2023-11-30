#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "util.h"
#include "scene.h"

static u16 _mesh_ind_by_name(const char *name, const struct mesh *meshes,
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

static void _matrix_transpose(f32 *in, f32 *out)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out[j * 4 + i] = in[i * 4 + j];
}

static void _scene_debug_node(const struct node *n, const u8 depth)
{
	for (u8 i = 0; i < depth; i++)
		printf("\t");
	printf("[NODE %d] '%s'\n", n->mesh_ind, n->mesh_path);
	for (u16 i = 0; i < n->num_children; i++)
		_scene_debug_node(n->children + i, depth + 1);
}

static void _scene_debug_texture(const char *tpath, const u16 ind)
{
	printf("[TEXTURE %d] '%s'\n", ind, tpath);
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

	printf("\n");

	for (u16 i = 0; i < s->num_anims; i++)
	{
		const struct animation *a = s->anims + i;

		printf("[ANIM %d] %s\n", i, a->name);
		for (u16 j = 0; j < a->num_pos; j++)
		{
			const f32 *pos = a->pos_keys[j].val;

			printf("\t[POS %d] %.3f, %.3f, %.3f\n",
				j, pos[0], pos[1], pos[2]);
		}

		printf("\n");

		for (u16 j = 0; j < a->num_rot; j++)
		{
			const f32 *rot = a->rot_keys[j].val;

			printf("\t[ROTATE %d] %.3f, %.3f, %.3f, %.3f\n",
				j, rot[0], rot[1], rot[2], rot[3]);
		}

		printf("\n");

		for (u16 j = 0; j < a->num_sca; j++)
		{
			const f32 *sca = a->sca_keys[j].val;

			printf("\t[SCALE %d] %.3f, %.3f, %.3f\n",
				j, sca[0], sca[1], sca[2]);
		}
	}

	for (u16 i = 0; i < s->num_tex_paths; i++)
		_scene_debug_texture(s->tex_paths[i], i);
}

static void _mesh_path_from_scene_path(char *out, const char *mname,
				       const char *scnpath)
{
	const u32 scnpathlen = strlen(scnpath);
	char *scnpath_crop = malloc(scnpathlen);

	strncpy(scnpath_crop, scnpath, scnpathlen);
	scnpath_crop[scnpathlen - 4] = 0;
	sprintf(out, "%s.%s.mdl", scnpath_crop, mname);
	free(scnpath_crop);
}

static void _scene_convert_node(const struct scene *s, const char *scnpath,
				const struct aiNode *ni,
				struct node *no)
{
	if (!ni->mNumMeshes)
	{
		strcpy(no->mesh_path, "N/A");
		no->mesh_ind = 0xFFFF;
	}
	else
	{
		_mesh_path_from_scene_path(no->mesh_path,
			     ni->mName.data, scnpath);
		no->mesh_ind = ni->mMeshes[0];
	}

	_matrix_transpose((f32 *)&ni->mTransformation, (f32 *)no->trans);
	no->num_children = ni->mNumChildren;
	no->children = malloc(sizeof(struct node) * no->num_children);
	for (u16 i = 0; i < no->num_children; i++)
		_scene_convert_node(s, scnpath,
		      ni->mChildren[i], no->children + i);
}

/**
 * _scene_convert_assimp - Converts an Assimp Scene into a Scene Structure
 * @si: Assimp Scene
 * @so: Output Scene
 */
static void _scene_convert_assimp(const struct aiScene *si, struct scene *so,
				  const char *scnpath)
{
	_scene_convert_node(so, scnpath, si->mRootNode, &so->root_node);

	assert(si->mNumMaterials < MAX_SCENE_TEXS);
	so->num_tex_paths = si->mNumMaterials;
	printf("%d\n", so->num_tex_paths);
	for (int i = 0; i < so->num_tex_paths; i++)
	{
		struct aiString tex_path;

		aiGetMaterialString(si->mMaterials[i],
		      AI_MATKEY_NAME, &tex_path);
		strncpy(so->tex_paths[i], tex_path.data, tex_path.length + 1);
		printf("%s\n", so->tex_paths[i]);
	}

	assert(si->mNumMeshes);
	so->num_meshes = si->mNumMeshes;
	so->meshes = malloc(sizeof(struct mesh) * so->num_meshes);
	for (u16 i = 0; i < so->num_meshes; i++)
	{
		const struct aiMesh *aimesh = si->mMeshes[i];
		struct mesh *mesh = so->meshes + i;
		const u32 name_len = strlen(aimesh->mName.data) + 1;

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
			for (u16 k = 0; k < 3; k++)
				mesh->indis[j * 3 + k] =
						aimesh->mFaces[j].mIndices[k];

		mesh->tind = aimesh->mMaterialIndex;
		if (mesh->tind >= so->num_tex_paths)
			mesh->tind = 0xFFFF;
	}

	so->num_anims = si->mNumAnimations;
	so->anims = malloc(sizeof(struct animation) * so->num_anims);
	for (u16 i = 0; i < so->num_anims; i++)
	{
		const struct aiAnimation *aianim = si->mAnimations[i];

		/*
		 * FIXME: This assumes that each animation only holds 1 mesh
		 */
		const struct aiNodeAnim *aichan = aianim->mChannels[0];
		struct animation *anim = so->anims + i;
		const u32 name_len = strlen(aianim->mName.data) + 1;

		strncpy(anim->name, aianim->mName.data, name_len);

		anim->num_pos = aichan->mNumPositionKeys;
		anim->pos_keys =
			malloc(sizeof(struct vec3_key) * anim->num_pos);
		for (u16 j = 0; j < anim->num_pos; j++)
		{
			struct vec3_key *poskey = anim->pos_keys + j;

			poskey->val[0] = aichan->mPositionKeys[j].mValue.x;
			poskey->val[1] = aichan->mPositionKeys[j].mValue.y;
			poskey->val[2] = aichan->mPositionKeys[j].mValue.z;
		}

		anim->num_rot = aichan->mNumRotationKeys;
		anim->rot_keys =
			malloc(sizeof(struct vec4_key) * anim->num_rot);
		for (u16 j = 0; j < anim->num_rot; j++)
		{
			struct vec4_key *rotkey = anim->rot_keys + j;

			rotkey->val[0] = aichan->mRotationKeys[j].mValue.x;
			rotkey->val[1] = aichan->mRotationKeys[j].mValue.y;
			rotkey->val[2] = aichan->mRotationKeys[j].mValue.z;
			rotkey->val[3] = aichan->mRotationKeys[j].mValue.w;
		}

		anim->num_sca = aichan->mNumScalingKeys;
		anim->sca_keys =
			malloc(sizeof(struct vec3_key) * anim->num_sca);
		for (u16 j = 0; j < anim->num_sca; j++)
		{
			struct vec3_key *scakey = anim->sca_keys + j;

			scakey->val[0] = aichan->mScalingKeys[j].mValue.x;
			scakey->val[1] = aichan->mScalingKeys[j].mValue.y;
			scakey->val[2] = aichan->mScalingKeys[j].mValue.z;
		}

		anim->length = (u16)((aianim->mDuration /
		       aianim->mTicksPerSecond) * 24);

		anim->mesh_ind = _mesh_ind_by_name(aichan->mNodeName.data,
				     so->meshes, so->num_meshes);
	}
}

static void _scene_write_mesh_file(const struct mesh *m, const char *scnpath)
{
	char mdlpath[CONF_PATH_MAX];

	_mesh_path_from_scene_path(mdlpath, m->name, scnpath);
	FILE *f = fopen(mdlpath, "wb");

	if (f)
	{
		fclose(f);
		remove(mdlpath);
		f = fopen(mdlpath, "wb");
	}

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

	fwriteflipu16(&m->tind, f);

	fclose(f);
}

static void _scene_write_node(const struct scene *s, const struct node *n,
			      const char *scnpath, FILE *scnfile)
{
	fwrite(n->mesh_path, sizeof(char), CONF_PATH_MAX, scnfile);
	fwriteflipu16(&n->num_children, scnfile);
	fwriteflipu16(&n->mesh_ind, scnfile);

	for (u8 i = 0; i < 16; i++)
		fwriteflipf32((f32 *)n->trans + i, scnfile);

	for (u16 i = 0; i < n->num_children; i++)
		_scene_write_node(s, n->children + i, scnpath, scnfile);
}

static void _scene_write_anim(const struct animation *a, FILE *f)
{
	fwrite(a->name, sizeof(char), CONF_NAME_MAX, f);
	fwriteflipu16(&a->num_pos, f);
	fwriteflipu16(&a->num_rot, f);
	fwriteflipu16(&a->num_sca, f);
	for (u16 j = 0; j < a->num_pos; j++)
	{
		struct vec3_key *poskey = a->pos_keys + j;

		fwriteflipf32(poskey->val + 0, f);
		fwriteflipf32(poskey->val + 1, f);
		fwriteflipf32(poskey->val + 2, f);
	}
	for (u16 j = 0; j < a->num_rot; j++)
	{
		struct vec4_key *rotkey = a->rot_keys + j;

		fwriteflipf32(rotkey->val + 0, f);
		fwriteflipf32(rotkey->val + 1, f);
		fwriteflipf32(rotkey->val + 2, f);
		fwriteflipf32(rotkey->val + 3, f);
	}
	for (u16 j = 0; j < a->num_sca; j++)
	{
		struct vec3_key *scakey = a->sca_keys + j;

		fwriteflipf32(scakey->val + 0, f);
		fwriteflipf32(scakey->val + 1, f);
		fwriteflipf32(scakey->val + 2, f);
	}

	fwriteflipu16(&a->length, f);
	fwriteflipu16(&a->mesh_ind, f);
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

	_scene_write_node(s, &s->root_node, outpath, f);

	fwriteflipu16(&s->num_tex_paths, f);
	for (u16 i = 0; i < s->num_tex_paths; i++)
		fwrite(s->tex_paths[i], sizeof(char), TEX_PATH_MAX_LEN, f);

	fwriteflipu16(&s->num_meshes, f);
	for (u16 i = 0; i < s->num_meshes; i++)
		_scene_write_mesh_file(s->meshes + i, outpath);

	fwriteflipu16(&s->num_anims, f);
	for (u16 i = 0; i < s->num_anims; i++)
		_scene_write_anim(s->anims + i, f);

	fclose(f);
}

static void _scene_read_mesh(struct mesh *m, const char *mpath)
{
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

	freadflipu16(&m->tind, mf);
	fclose(mf);
}

static void _scene_read_node(struct scene *s, struct node *n, FILE *f)
{
	fread(n->mesh_path, sizeof(char), CONF_PATH_MAX, f);
	freadflipu16(&n->num_children, f);
	freadflipu16(&n->mesh_ind, f);

	for (u8 i = 0; i < 16; i++)
		freadflipf32((f32 *)n->trans + i, f);

	for (u16 i = 0; i < n->num_children; i++)
		_scene_read_node(s, n->children + i, f);
}

static void _scene_read_anim(struct animation *a, FILE *f)
{
	fread(a->name, sizeof(char), CONF_NAME_MAX, f);
	freadflipu16(&a->num_pos, f);
	freadflipu16(&a->num_rot, f);
	freadflipu16(&a->num_sca, f);
	a->pos_keys = malloc(sizeof(struct vec3_key) * a->num_pos);
	a->rot_keys = malloc(sizeof(struct vec4_key) * a->num_rot);
	a->sca_keys = malloc(sizeof(struct vec3_key) * a->num_sca);
	for (u16 j = 0; j < a->num_pos; j++)
	{
		struct vec3_key *poskey = a->pos_keys + j;

		freadflipf32(poskey->val + 0, f);
		freadflipf32(poskey->val + 1, f);
		freadflipf32(poskey->val + 2, f);
	}
	for (u16 j = 0; j < a->num_rot; j++)
	{
		struct vec4_key *rotkey = a->rot_keys + j;

		freadflipf32(rotkey->val + 0, f);
		freadflipf32(rotkey->val + 1, f);
		freadflipf32(rotkey->val + 2, f);
		freadflipf32(rotkey->val + 3, f);
	}
	for (u16 j = 0; j < a->num_sca; j++)
	{
		struct vec3_key *scakey = a->sca_keys + j;

		freadflipf32(scakey->val + 0, f);
		freadflipf32(scakey->val + 1, f);
		freadflipf32(scakey->val + 2, f);
	}

	freadflipu16(&a->length, f);
	freadflipu16(&a->mesh_ind, f);
}

static const struct node *_node_from_mesh_ind(const struct node *n,
					      const u16 mid)
{
	if (mid == n->mesh_ind)
		return (n);

	const struct node *nf;

	for (u16 i = 0; i < n->num_children; i++)
	{
		nf = _node_from_mesh_ind(n->children + i, mid);
		if (nf)
			return (nf);
	}

	return (NULL);
}

/**
 * _scene_read - Test Scene Importing
 * @s: Scene to Import To
 * @path: Path to Import From
 */
static void _scene_read_file(struct scene *s, const char *path)
{
	FILE *sf = fopen(path, "rb");

	if (!sf)
	{
		fprintf(stderr, "Couldn't find scene at '%s'\n", path);
		exit(1);
	}

	_scene_read_node(s, &s->root_node, sf);

	freadflipu16(&s->num_tex_paths, sf);
	for (u16 i = 0; i < s->num_tex_paths; i++)
		fread(s->tex_paths[i], sizeof(char), TEX_PATH_MAX_LEN, sf);

	freadflipu16(&s->num_meshes, sf);
	s->meshes = malloc(sizeof(struct mesh) * s->num_meshes);
	/**
	 * FIXME: This will cause a crash later
	 */
	for (u16 i = 0; i < s->num_meshes; i++)
	{
		const struct node *n = _node_from_mesh_ind(&s->root_node, i);

		if (!n)
			continue;

		char path_correct[CONF_PATH_MAX];

		sprintf(path_correct, "filesystem/%s",
	  		n->mesh_path + strlen("assets/"));

		_scene_read_mesh(s->meshes + i, path_correct);
	}

	freadflipu16(&s->num_anims, sf);
	for (u16 i = 0; i < s->num_anims; i++)
		_scene_read_anim(s->anims + i, sf);

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

	_scene_convert_assimp(aiscene, &s, pathin);
	_scene_debug_assimp(&s);
	_scene_write_file(&s, pathout);
	_scene_read_file(&s, pathout);

	return (0);
}
