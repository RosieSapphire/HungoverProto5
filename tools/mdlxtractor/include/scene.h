#ifndef _SCENE_H_
#define _SCENE_H_

#include <stdio.h>
#include <assimp/scene.h>

#include "mesh.h"
#include "node.h"
#include "animation.h"
#include "../../../include/engine/config.h"

/**
 * Scene Structure
 */
struct scene
{
	struct node root_node; ///< Root node
	u16 num_meshes; ///< Number of meshes
	struct mesh *meshes; ///< Meshes array
	u16 num_anims; ///< Number of animations
	struct animation *anims; ///< Animations array
	u16 num_tex_paths; ///< Number of Texture Paths
	char tex_paths[MAX_SCENE_TEXS][TEX_PATH_MAX_LEN]; ///< Tex paths array
};

/*
 * Converting
 */
void scene_convert_node(const struct scene *s, const char *scnpath,
			const struct aiNode *ni, struct node *no);
void scene_convert_mesh(const struct aiMesh *mi, struct mesh *mo);
void scene_convert_anim(const struct scene *so,
			const struct aiAnimation *ai,
			const struct aiNodeAnim *ci,
			struct animation *ao);
void scene_convert_assimp(const struct aiScene *si, struct scene *so,
			  const char *scnpath);

/*
 * Debugging
 */
void scene_debug_node(const struct node *n, const u8 depth);
void scene_debug_texture(const char *tpath, const u16 ind);
void scene_debug_anim_keys(const struct animation *a);
void scene_debug_assimp(const struct scene *s);

/*
 * Writing
 */
void scene_write_mesh_file(const struct mesh *m);
void scene_write_node(const struct scene *s, const struct node *n,
		      const char *scnpath, FILE *scnfile);
void scene_write_anim(const struct animation *a, FILE *f);
void scene_write_file(const struct scene *s, const char *outpath);

/*
 * Reading
 */
void scene_read_mesh(struct mesh *m, const char *mpath);
void scene_read_node(struct scene *s, struct node *n, FILE *f);
void scene_read_anim(struct animation *a, FILE *f);
void scene_read_file(struct scene *s, const char *path);

/*
 * Freeing
 */

#endif /* _SCENE_H_ */
