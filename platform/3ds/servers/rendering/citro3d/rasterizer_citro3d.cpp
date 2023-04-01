#include "rasterizer_citro3d.h"
#include <citro3d.h>
#include "2d.h"
#include "scene/main/scene_tree.h"

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

RasterizerCitro3D::RasterizerCitro3D() : target_bottom(nullptr),target_top_left(nullptr),target_top_right(nullptr)
{
}



void RasterizerCitro3D::shader_init(void)
{
	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)shader_builtin_2d, sizeof(shader_builtin_2d) / 4);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1=tex
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=color

	// Compute the projection matrix
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);

	// Configure the first fragment shading substage to just pass through the vertex color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

void RasterizerCitro3D::finalize()
{
	/*shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
	C3D_RenderTargetDetachOutput(target_top_left);
	//C3D_RenderTargetDetachOutput(target_top_right);
	C3D_RenderTargetDelete(target_top_left);
	//C3D_RenderTargetDelete(target_top_right);
	C3D_Fini();*/
}
void RasterizerCitro3D::initialize()
 {
	print_line("rasterizer initialized");
	bool success = C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	if (!success)
	{
		SceneTree::get_singleton()->quit();
	}
	target_top_left = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target_top_left, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	/*target_top_right = C3D_RenderTargetCreate(200,400,GPU_RB_RGBA8,GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target_top_right,GFX_TOP,GFX_RIGHT,DISPLAY_TRANSFER_FLAGS);*/
	shader_init();
}

void RasterizerCitro3D::begin_frame(double frame_step)
 {
	print_line("frame begin");
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C3D_RenderTargetClear(target_top_left,C3D_CLEAR_ALL,CLEAR_COLOR,0);
	//canvas.set_time(time_total);
	scene.set_time(time, frame_step);
	//C3D_RenderTargetClear(target_top_right,C3D_CLEAR_ALL,CLEAR_COLOR,0);
	frame++;
	delta = frame_step;
	time += frame_step;
}

void RasterizerCitro3D::end_frame(bool p_swap_buffers) 
{
	C3D_FrameEnd(0);
	DisplayServer::get_singleton()->swap_buffers();
	/*print_line("frame end");
    if (p_swap_buffers) {
        
    }*/
}
