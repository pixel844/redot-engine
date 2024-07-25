#include "renderer_3ds.h"
#include "2d.h"
#include "scene/main/scene_tree.h"
#include "storage/texture_storage.h"
#include <citro3d.h>

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS                                                                            \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |                      \
			GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
			GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

RendererCompositor3DS::RendererCompositor3DS() :
		target_bottom(nullptr), target_top_left(nullptr), target_top_right(nullptr) {
}

void RendererCompositor3DS::shader_init(void) {
	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32 *)shader_builtin_2d, sizeof(shader_builtin_2d) / 4);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo *attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1=tex
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=color

	// Compute the projection matrix
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);


	C3D_TexEnv *env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

void RendererCompositor3DS::finalize() {
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
	if (target_top_left)
	{
		C3D_RenderTargetDetachOutput(target_top_left);
		//C3D_RenderTargetDetachOutput(target_top_right);
		C3D_RenderTargetDelete(target_top_left);
		//C3D_RenderTargetDelete(target_top_right);
	}
	C3D_Fini();
}
void RendererCompositor3DS::initialize() {
	print_line("rasterizer initialized");
	bool success = C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	if (!success) {
		SceneTree::get_singleton()->quit();
	}
	//target_top_left = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	//C3D_RenderTargetSetOutput(target_top_left, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	//target_top_right = C3D_RenderTargetCreate(200, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	//C3D_RenderTargetSetOutput(target_top_right,GFX_TOP,GFX_RIGHT,DISPLAY_TRANSFER_FLAGS);
	shader_init();
}
void RendererCompositor3DS::blit_render_targets_to_screen(int p_screen, const BlitToScreen *p_render_targets, int p_amount) {
	for (int i = 0; i < p_amount; i++) {
		const BlitToScreen &blit = p_render_targets[i];

		RID rid_rt = blit.render_target;

		Rect2 dst_rect = blit.dst_rect;
		_blit_render_target_to_screen(rid_rt, p_screen, dst_rect, blit.multi_view.use_layer ? blit.multi_view.layer : 0, i == 0);
	}
}

void RendererCompositor3DS::_blit_render_target_to_screen(RID p_render_target, int p_screen, const Rect2 &p_screen_rect, uint32_t p_layer, bool p_first) {
	Renderer3DS::RenderTarget *rt = Renderer3DS::TextureStorage::get_singleton()->get_render_target(p_render_target);
	ERR_FAIL_COND(!rt);
	if (p_screen == 0) {
		//C3D_FrameBufTransfer(&rt->target->frameBuf, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
		C3D_RenderTargetSetOutput(rt->target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	} else if (p_screen == 1) {
		//C3D_FrameBufTransfer(&rt->target->frameBuf, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
		C3D_RenderTargetSetOutput(rt->target, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	}
}

void RendererCompositor3DS::begin_frame(double frame_step) {
	print_line("frame begin");
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	//canvas.set_time(time_total);
	scene.set_time(time, frame_step);
	frame++;
	delta = frame_step;
	time += frame_step;
}

void RendererCompositor3DS::end_frame(bool p_swap_buffers) {
	C3D_FrameEnd(0);
}
