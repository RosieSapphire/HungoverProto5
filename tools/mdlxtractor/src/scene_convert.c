#include <assert.h>
#include <malloc.h>
#include <assimp/cimport.h>

#include "util.h"
#include "scene.h"

void scene_convert_node(const struct scene *s, const char *scnpath,
			const struct aiNode *ni, struct node *no)
{
	assert(s);
	assert(ni);
	assert(no);

	strncpy(no->name, ni->mName.data, ni->mName.length + 1);
	if (!ni->mNumMeshes)
	{
		strcpy(no->mesh_path, "N/A");
		no->mesh_ind = 0xFFFF;
	}
	else
	{
		snprintf(no->mesh_path, ni->mName.length + 1,
	   		 "%s.mdl", ni->mName.data);
		no->mesh_ind = ni->mMeshes[0];
	}

	matrix_transpose((f32 *)&ni->mTransformation, (f32 *)no->trans);
	no->num_children = ni->mNumChildren;
	no->children = malloc(sizeof(struct node) * no->num_children);
	for (u16 i = 0; i < no->num_children; i++)
		scene_convert_node(s, scnpath, ni->mChildren[i],
				   no->children + i);
}

void scene_convert_mesh(const struct aiMesh *mi, struct mesh *mo)
{
	assert(mi);
	assert(mo);

	const u32 name_len = strlen(mi->mName.data) + 1;

	strncpy(mo->name, mi->mName.data, name_len);
	mo->num_verts = mi->mNumVertices;
	mo->verts = malloc(sizeof(struct vertex) * mo->num_verts);
	for (u16 j = 0; j < mo->num_verts; j++)
	{
		struct vertex *v = mo->verts + j;

		v->pos[0] = mi->mVertices[j].x;
		v->pos[1] = mi->mVertices[j].y;
		v->pos[2] = mi->mVertices[j].z;

		v->uv[0] = mi->mTextureCoords[0][j].x;
		v->uv[1] = mi->mTextureCoords[0][j].y;

		v->col[0] = mi->mColors[0][j].r * 255;
		v->col[1] = mi->mColors[0][j].g * 255;
		v->col[2] = mi->mColors[0][j].b * 255;
		v->col[3] = mi->mColors[0][j].a * 255;
	}

	mo->num_indis = mi->mNumFaces * 3;
	mo->indis = malloc(sizeof(u16) * mo->num_indis);
	for (u16 j = 0; j < mo->num_indis / 3; j++)
		for (u16 k = 0; k < 3; k++)
			mo->indis[j * 3 + k] = mi->mFaces[j].mIndices[k];

	mo->tind = mi->mMaterialIndex;
}

void scene_convert_anim(const struct scene *so,
			const struct aiAnimation *ai,
			const struct aiNodeAnim *ci,
			struct animation *ao)
{
	assert(so);
	assert(ai);
	assert(ci);
	assert(ao);

	const u32 name_len = strlen(ai->mName.data) + 1;

	strncpy(ao->name, ai->mName.data, name_len);
	ao->num_pos = ci->mNumPositionKeys;
	ao->pos_keys = malloc(sizeof(struct vec3_key) * ao->num_pos);
	assert(ao->pos_keys);
	for (u16 j = 0; j < ao->num_pos; j++)
	{
		struct vec3_key *poskey = ao->pos_keys + j;

		poskey->val[0] = ci->mPositionKeys[j].mValue.x;
		poskey->val[1] = ci->mPositionKeys[j].mValue.y;
		poskey->val[2] = ci->mPositionKeys[j].mValue.z;
	}

	ao->num_rot = ci->mNumRotationKeys;
	ao->rot_keys = malloc(sizeof(struct vec4_key) * ao->num_rot);
	assert(ao->rot_keys);
	for (u16 j = 0; j < ao->num_rot; j++)
	{
		struct vec4_key *rotkey = ao->rot_keys + j;

		rotkey->val[0] = ci->mRotationKeys[j].mValue.x;
		rotkey->val[1] = ci->mRotationKeys[j].mValue.y;
		rotkey->val[2] = ci->mRotationKeys[j].mValue.z;
		rotkey->val[3] = ci->mRotationKeys[j].mValue.w;
	}

	ao->num_sca = ci->mNumScalingKeys;
	ao->sca_keys = malloc(sizeof(struct vec3_key) * ao->num_sca);
	assert(ao->sca_keys);
	for (u16 j = 0; j < ao->num_sca; j++)
	{
		struct vec3_key *scakey = ao->sca_keys + j;

		scakey->val[0] = ci->mScalingKeys[j].mValue.x;
		scakey->val[1] = ci->mScalingKeys[j].mValue.y;
		scakey->val[2] = ci->mScalingKeys[j].mValue.z;
	}

	ao->length = (u16)((ai->mDuration / ai->mTicksPerSecond) * 24);
	ao->mesh_ind = mesh_ind_by_name(ci->mNodeName.data,
					so->meshes, so->num_meshes);
}

/**
 * scene_convert_assimp - Converts an Assimp Scene into a Scene Structure
 * @si: Assimp Scene
 * @so: Output Scene
 */
void scene_convert_assimp(const struct aiScene *si, struct scene *so,
			  const char *scnpath)
{
	scene_convert_node(so, scnpath, si->mRootNode, &so->root_node);

	assert(si->mNumMaterials < MAX_SCENE_TEXS);
	so->num_tex_paths = si->mNumMaterials;
	u8 num_tex_excluded = 0;

	for (int i = 0; i < so->num_tex_paths; i++)
	{
		struct aiString tex_path;
		u8 do_continue = 0;

		aiGetMaterialString(si->mMaterials[i],
				    AI_MATKEY_NAME, &tex_path);
		for (int j = 0; j < so->num_tex_paths; j++)
		{
			if (j == i)
				continue;

			do_continue = !strncmp(tex_path.data,
	     				       so->tex_paths[j],
					       tex_path.length);
			num_tex_excluded += do_continue;
		}

		if (do_continue)
			continue;

		u8 new_ind = i - num_tex_excluded;

		strncpy(so->tex_paths[new_ind], tex_path.data,
	  		tex_path.length + 1);
	}

	so->num_tex_paths -= num_tex_excluded;

	assert(si->mNumMeshes);
	so->num_meshes = si->mNumMeshes;
	so->meshes = malloc(sizeof(struct mesh) * so->num_meshes);
	for (u16 i = 0; i < so->num_meshes; i++)
	{
		const struct aiMesh *mi = si->mMeshes[i];
		struct mesh *mesh = so->meshes + i;

		scene_convert_mesh(mi, mesh);
	}

	so->num_anims = si->mNumAnimations;
	so->anims = malloc(sizeof(struct animation) * so->num_anims);
	for (u16 i = 0; i < so->num_anims; i++)
	{
		const struct aiAnimation *aianim = si->mAnimations[i];
		const struct aiNodeAnim *ci = si->mAnimations[i]->mChannels[0];
		struct animation *anim = so->anims + i;

		scene_convert_anim(so, aianim, ci, anim);
	}
}
