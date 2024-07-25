#include "renderer_scene_3ds.h"

typedef struct {
	float x, y, z;
} vertex;

static const vertex vertex_list[] = {
	{ 200.0f, 200.0f, 0.5f },
	{ 100.0f, 40.0f, 0.5f },
	{ 300.0f, 40.0f, 0.5f },
};

void RendererScene3DS::render_scene(const Ref<RenderSceneBuffers> &p_render_buffers, const CameraData *p_camera_data, const CameraData *p_prev_camera_data, const PagedArray<RenderGeometryInstance *> &p_instances, const PagedArray<RID> &p_lights, const PagedArray<RID> &p_reflection_probes, const PagedArray<RID> &p_voxel_gi_instances, const PagedArray<RID> &p_decals, const PagedArray<RID> &p_lightmaps, const PagedArray<RID> &p_fog_volumes, RID p_environment, RID p_camera_attributes, RID p_compositor, RID p_shadow_atlas, RID p_occluder_debug_tex, RID p_reflection_atlas, RID p_reflection_probe, int p_reflection_probe_pass, float p_screen_mesh_lod_threshold, const RenderShadowData *p_render_shadows, int p_render_shadow_count, const RenderSDFGIData *p_render_sdfgi_regions, int p_render_sdfgi_region_count, const RenderSDFGIUpdateData *p_sdfgi_update_data, RenderingMethod::RenderInfo *r_render_info) {
	Renderer3DS::TextureStorage *texture_storage = Renderer3DS::TextureStorage::get_singleton();
	Ref<RenderSceneBuffers3DS> rb;
	if (p_render_buffers.is_valid()) {
		rb = p_render_buffers;
		ERR_FAIL_COND(rb.is_null());
	}
	Renderer3DS::RenderTarget *rt = texture_storage->get_render_target(rb->render_target);
	ERR_FAIL_COND(!rt);
	RenderDataC3D render_data;
	{
		render_data.render_buffers = rb;
		render_data.transparent_bg = rb.is_valid() ? rb->is_transparent : false;
		// Our first camera is used by default
		render_data.cam_transform = p_camera_data->main_transform;
		render_data.inv_cam_transform = render_data.cam_transform.affine_inverse();
		render_data.cam_projection = p_camera_data->main_projection;
		render_data.cam_orthogonal = p_camera_data->is_orthogonal;
		render_data.camera_visible_layers = p_camera_data->visible_layers;

		render_data.view_count = p_camera_data->view_count;
		for (uint32_t v = 0; v < p_camera_data->view_count; v++) {
			render_data.view_eye_offset[v] = p_camera_data->view_offset[v].origin;
			render_data.view_projection[v] = p_camera_data->view_projection[v];
		}

		render_data.z_near = p_camera_data->main_projection.get_z_near();
		render_data.z_far = p_camera_data->main_projection.get_z_far();

		render_data.instances = &p_instances;
		render_data.lights = &p_lights;
		render_data.reflection_probes = &p_reflection_probes;
		render_data.environment = p_environment;
		render_data.camera_attributes = p_camera_attributes;
		render_data.reflection_probe = p_reflection_probe;
		render_data.reflection_probe_pass = p_reflection_probe_pass;

		// this should be the same for all cameras..
		render_data.lod_distance_multiplier = p_camera_data->main_projection.get_lod_multiplier();

		/*if (get_debug_draw_mode() == RS::VIEWPORT_DEBUG_DRAW_DISABLE_LOD) {
			render_data.screen_mesh_lod_threshold = 0.0;
		} else {
			render_data.screen_mesh_lod_threshold = p_screen_mesh_lod_threshold;
		}*/
		//render_data.render_info = r_render_info;
	}
	Color clear_color;
	if (p_render_buffers.is_valid()) {
		clear_color = texture_storage->render_target_get_clear_request_color(rb->render_target);
	} else {
		clear_color = texture_storage->get_default_clear_color();
	}
	//texture_storage->render_target_do_clear_request(rb->render_target);
	C3D_FrameDrawOn(rt->target);

	// Draw the triangle directly
	C3D_ImmDrawBegin(GPU_TRIANGLES);
	C3D_ImmSendAttrib(200.0f, 200.0f, 0.5f, 0.0f); // v0=position
	C3D_ImmSendAttrib(1.0f, 0.0f, 0.0f, 1.0f); // v1=color

	C3D_ImmSendAttrib(100.0f, 40.0f, 0.5f, 0.0f);
	C3D_ImmSendAttrib(0.0f, 1.0f, 0.0f, 1.0f);

	C3D_ImmSendAttrib(300.0f, 40.0f, 0.5f, 0.0f);
	C3D_ImmSendAttrib(0.0f, 0.0f, 1.0f, 1.0f);
	C3D_ImmDrawEnd();

	bool fb_cleared = true;
	Size2i screen_size;
	screen_size.x = rb->width;
	screen_size.y = rb->height;
}

Ref<RenderSceneBuffers> RendererScene3DS::render_buffers_create() {
	Ref<RenderSceneBuffers3DS> rb;
	rb.instantiate();
	return rb;
}