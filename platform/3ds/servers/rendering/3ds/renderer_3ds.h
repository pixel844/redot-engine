/**************************************************************************/
/*  rasterizer_3ds.h                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef RASTERIZER_3DS_H
#define RASTERIZER_3DS_H

#include "core/templates/rid_owner.h"
#include "core/templates/self_list.h"
#include "environment/fog.h"
#include "environment/gi.h"
#include "renderer_canvas_3ds.h"
#include "renderer_scene_3ds.h"
#include "scene/resources/mesh.h"
#include "servers/rendering/renderer_compositor.h"
#include "servers/rendering_server.h"
#include "storage/light_storage.h"
#include "storage/material_storage.h"
#include "storage/mesh_storage.h"
#include "storage/particles_storage.h"
#include "storage/texture_storage.h"
#include "storage/utilities.h"
#include <citro3d.h>

class RendererCompositor3DS : public RendererCompositor {
private:
	uint64_t frame = 1;
	double delta = 0;
	double time = 0.0;
	C3D_RenderTarget *target_top_left;
	C3D_RenderTarget *target_top_right;
	C3D_RenderTarget *target_bottom;
	DVLB_s *vshader_dvlb {nullptr};
	shaderProgram_s program;
	int uLoc_projection;
	C3D_Mtx projection;

protected:
	RendererCanvas3DS canvas;
	Renderer3DS::Utilities utilities;
	Renderer3DS::LightStorage light_storage;
	Renderer3DS::MaterialStorage material_storage;
	Renderer3DS::MeshStorage mesh_storage;
	Renderer3DS::ParticlesStorage particles_storage;
	Renderer3DS::TextureStorage texture_storage;
	Renderer3DS::GI gi;
	Renderer3DS::Fog fog;
	RendererScene3DS scene;

public:
	RendererUtilities *get_utilities() override { return &utilities; };
	RendererLightStorage *get_light_storage() override { return &light_storage; };
	RendererMaterialStorage *get_material_storage() override { return &material_storage; };
	RendererMeshStorage *get_mesh_storage() override { return &mesh_storage; };
	RendererParticlesStorage *get_particles_storage() override { return &particles_storage; };
	RendererTextureStorage *get_texture_storage() override { return &texture_storage; };
	RendererGI *get_gi() override { return &gi; };
	RendererFog *get_fog() override { return &fog; };
	RendererCanvasRender *get_canvas() override { return &canvas; }
	RendererSceneRender *get_scene() override { return &scene; }

	void set_boot_image(const Ref<Image> &p_image, const Color &p_color, bool p_scale, bool p_use_filter = true) override {}

	void initialize() override;
	void begin_frame(double frame_step) override;

	void shader_init();

	void blit_render_targets_to_screen(int p_screen, const BlitToScreen *p_render_targets, int p_amount) override;
	void _blit_render_target_to_screen(RID p_render_target, int p_screen, const Rect2 &p_screen_rect, uint32_t p_layer, bool p_first);
	void end_frame(bool p_swap_buffers) override;
	void end_viewport(bool p_swap_buffers) override {}
	void finalize() override;

	static RendererCompositor *_create_current() {
		return memnew(RendererCompositor3DS);
	}

	static void make_current() {
		_create_func = _create_current;
		low_end = true;
	}

	uint64_t get_frame_number() const override { return frame; }
	double get_frame_delta_time() const override { return delta; }
	double get_total_time() const override { return time; }

	RendererCompositor3DS();
	~RendererCompositor3DS() {}
};

#endif // RASTERIZER_3DS_H
