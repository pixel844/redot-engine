#include "material_storage.h"
#include "../2d.h"
#include <citro3d.h>

using namespace Renderer3DS;

RID MaterialStorage::shader_allocate() {
	return shader_owner.allocate_rid();
}
static C3D_Mtx projection;
void MaterialStorage::shader_initialize(RID p_rid) {
	Shader shader;
	shader.program = (shaderProgram_s *)malloc(sizeof(shaderProgram_s));
	shader.data = (u32 *)shader_builtin_2d;
	shader.dvlb = DVLB_ParseFile((u32 *)shader_builtin_2d, sizeof(shader_builtin_2d) / 4);
	shaderProgramInit(shader.program);
	shaderProgramSetVsh(shader.program, &shader.dvlb->DVLE[0]);
	C3D_BindProgram(shader.program);
	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo *attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1=color

	// Compute the projection matrix
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);

	// Configure the first fragment shading substage to just pass through the vertex color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv *env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

	shader.uniform_projection = shaderInstanceGetUniformLocation(shader.program->vertexShader, "projection");
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, shader.uniform_projection, &projection);
	shader_owner.initialize_rid(p_rid, shader);
}

void MaterialStorage::shader_free(RID p_rid) {
	Shader *shader = shader_owner.get_or_null(p_rid);
	ERR_FAIL_COND(!shader);

	while (shader->owners.size()) {
		material_set_shader((*shader->owners.begin())->self, RID());
	}

	if (shader->data) {
		shaderProgramFree(shader->program);
		free(shader->program);
		memdelete(shader->data);
		DVLB_Free(shader->dvlb);
	}
	shader_owner.free(p_rid);
}