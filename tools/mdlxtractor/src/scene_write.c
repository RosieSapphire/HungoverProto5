#include "util.h"
#include "scene.h"

void scene_write_mesh_file(const struct mesh *m, const char *scnpath)
{
	char mdlpath[CONF_PATH_MAX];

	mesh_path_from_scene_path(mdlpath, m->name, scnpath);
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

		fwrite(v->col, sizeof(u8), 4, f);
	}

	for (u16 j = 0; j < m->num_indis; j++)
		fwriteflipu16(m->indis + j, f);

	fwriteflipu16(&m->tind, f);

	fclose(f);
}

void scene_write_node(const struct scene *s, const struct node *n,
		      const char *scnpath, FILE *scnfile)
{
	fwrite(n->mesh_path, sizeof(char), CONF_PATH_MAX, scnfile);
	fwriteflipu16(&n->num_children, scnfile);
	fwriteflipu16(&n->mesh_ind, scnfile);

	for (u8 i = 0; i < 16; i++)
		fwriteflipf32((f32 *)n->trans + i, scnfile);

	for (u16 i = 0; i < n->num_children; i++)
		scene_write_node(s, n->children + i, scnpath, scnfile);
}

void scene_write_anim(const struct animation *a, FILE *f)
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
 * scene_write_file - Writes Converted Scene to File
 * @s: Scene to Write
 * @outpath: Path to Write To
 */
void scene_write_file(const struct scene *s, const char *outpath)
{
	FILE *f = fopen(outpath, "wb");

	if (f)
	{
		fclose(f);
		remove(outpath);
		f = fopen(outpath, "wb");
	}

	scene_write_node(s, &s->root_node, outpath, f);

	fwriteflipu16(&s->num_tex_paths, f);
	for (u16 i = 0; i < s->num_tex_paths; i++)
		fwrite(s->tex_paths[i], sizeof(char), TEX_PATH_MAX_LEN, f);

	fwriteflipu16(&s->num_meshes, f);
	for (u16 i = 0; i < s->num_meshes; i++)
		scene_write_mesh_file(s->meshes + i, outpath);

	fwriteflipu16(&s->num_anims, f);
	for (u16 i = 0; i < s->num_anims; i++)
		scene_write_anim(s->anims + i, f);

	fclose(f);
}
