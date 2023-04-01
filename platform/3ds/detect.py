# detect_godot4.py
#
# FRT - A Godot platform targeting single board computers
# Copyright (c) 2017-2023  Emanuele Fornara
# SPDX-License-Identifier: MIT
#

import os
import sys
import platform
import version

# TODO factor out common bits


def is_active():
    return True


def get_name():
    return "3ds"


def can_build():
    if os.getenv("DEVKITPRO") != None:
        return True
    print("Cannot build for 3DS due to missing DEVKITPRO environment variable!")
    return False


def get_opts():
    from SCons.Variables import BoolVariable
    return [
        BoolVariable("disable_advanced_gui", "", True),
        BoolVariable("openxr", "", False),
        BoolVariable(
            "dlink_enabled", "Enable WebAssembly dynamic linking (GDExtension support). Produces bigger binaries", False
        ),
        BoolVariable("use_assertions",
                     "Use Emscripten runtime assertions", False),
        BoolVariable(
            "use_ubsan", "Use Emscripten undefined behavior sanitizer (UBSAN)", False),
        BoolVariable(
            "use_asan", "Use Emscripten address sanitizer (ASAN)", False),
        BoolVariable(
            "use_lsan", "Use Emscripten leak sanitizer (LSAN)", False),
        BoolVariable("use_safe_heap",
                     "Use Emscripten SAFE_HEAP sanitizer", False),
    ]


def get_flags():
    return [
        ("optimize", "size"),

        ("arch", "arm32"),
        ('tools', False),
        ('editor', False),
        ('debug_symbols', True),
        ('target', 'template_debug'),
        ("module_glslang_enabled", False),
        ('module_dds_enabled', False),
        ('module_gltf_enabled', False),
        ('module_csg_enabled', False),
        ('module_gridmap_enabled', False),
        ('module_freetype_enabled', True),
        ('module_ogg_enabled', False),
        ('module_tga_enabled', False),
        ('module_denoise_enabled', False),
        ('module_jsonrpg_enabled', False),
        ('module_minimp3_enabled', False),
        ('module_raycast_enabled', False),
        ('module_noise_enabled', False),
        ('module_squish_enabled', False),
        ('module_vorbis_enabled', False),
        ('module_webp_enabled', False),
        ('module_camera_enabled', False),
        ('module_bmp_enabled', False),
        ('module_lightmapper_rd_enabled', False),
        ('module_text_server_adv_enabled', False),
        ('module_text_server_fb_enabled', True),
        ('module_svg_enabled', False),
        ('module_mono_enabled', False),
        ('module_upnp_enabled', False),
        ('module_mobile_vr_enabled', False),
        ('module_multiplayer_enabled', False),
        ('module_websocket_enabled', False),
        ('module_webxr_enabled', False),
        ('module_hdr_enabled', False),
        ('module_mbedtls_enabled', False),
        ('module_vhacd_enabled', False),
        ('module_astcenc_enabled', False),
        ('module_basis_universal_enabled', False),
        ('module_msdfgen_enabled', False),
        ('module_minimp3d_enabled', False),
        ('module_tinyexr_enabled', False),
        ('module_zip_enabled', False),
        ('module_xatlas_unwrap_enabled', False),
        ("builtin_pcre2_with_jit", False),
        ('builtin_mbedtls', False),
        ('builtin_miniupnpc', False),
        ('builtin_libpng', True),
        ('builtin_freetype', True),
        ('vulkan', False),
        ('gles3', False)
    ]


def build_shader_gen(target, source, env, for_signature):
    return "picasso -o {} {}".format(target[0], source[0])


def build_shader_header(target, source, env):
    import os
    data = source[0].get_contents()
    data_str = ",".join([str(x) for x in data])
    name = os.path.basename(str(target[0]))[:-2]
    target[0].prepare()
    with open(str(target[0]), 'w') as f:
        f.write("/* Auto-generated from {} */\n".format(str(source[0])))
        f.write("static uint8_t shader_builtin_{}[] =\n{{{}}};".format(
            name, data_str))


def configure_arch(env):
    env.Append(CCFLAGS=['-march=armv6k', '-mtp=soft',
               '-mfpu=vfp', '-mfloat-abi=hard'])


def configure_cross(env):
    dkp_path = os.getenv("DEVKITPRO")
    triple = dkp_path + '/devkitARM/bin/arm-none-eabi'
    env['CC'] = triple + '-gcc'
    env['CXX'] = triple + '-g++'
    env["LD"] = triple + "-g++"
    env["AR"] = triple + "-ar"
    env["RANLIB"] = triple + "-ranlib"
    env["AS"] = triple + "-as"
    env["STRIP"] = triple + "-strip"
    env["OBJCOPY"] = triple + "-objcopy"
    env["PROGSUFFIX"] = ".elf"
    env["SHLIBSUFFIX"] = ".so"
    dkp_path = os.getenv("DEVKITPRO")
    env.Append(CPPPATH=['#platform/3ds'])
    env.Append(CCFLAGS=['-Wall', '-mword-relocations', '-ffunction-sections', '-fconserve-stack',
               '-fdata-sections', '-fno-exceptions', '-Wl,-dead_strip'])
    env.Append(CCFLAGS=['-D__3DS__', '-DLIBC_FILEIO_ENABLED', '-DNO_SAFE_CAST',
               '-DNEED_LONG_INT', '-D_XOPEN_SOURCE=500', '-DRW_LOCK_H'])
    env.Append(CCFLAGS=['-U__INT32_TYPE__', '-U__UINT32_TYPE__', '-D__INT32_TYPE__=int',
               '-D__UINT32_TYPE__=unsigned int', '-DUNIX_SOCKET_UNAVAILABLE', '-DPAD_ALIGN=16'])
    env.Append(CPPPATH=[dkp_path + "/portlibs/3ds/include", dkp_path + "/portlibs/3ds/include/freetype2", dkp_path +
               "/portlibs/armv6k/include", dkp_path + "/libctru/include", dkp_path + "/devkitARM/arm-none-eabi/include"])
    env.Append(LIBPATH=[dkp_path+"/portlibs/armv6k/lib", dkp_path +
                        "/portlibs/3ds/lib", dkp_path + "/libctru/lib", dkp_path + "/arm-none-eabi/lib/armv6k/fpu"])
    env.Append(LINKFLAGS=['-specs=3dsx.specs', '-march=armv6k',
               '-mtp=soft', '-mfpu=vfp', '-mfloat-abi=hard'])
    env.Append(LIBS=['ctru'])


def configure_misc(env):
    if env["PLATFORM"] == "win32":
        env.use_windows_spawn_fix()
    env.Append(BUILDERS={'PICA': env.Builder(
        generator=build_shader_gen, suffix='.shbin', src_suffix='.pica')})
    env.Append(BUILDERS={'PICA_HEADER': env.Builder(
        action=build_shader_header, suffix='.h', src_suffix='.shbin')})


def configure(env):
    configure_arch(env)
    configure_cross(env)
    configure_misc(env)
