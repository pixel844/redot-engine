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
		BoolVariable("use_ubsan", "Use LLVM/GCC compiler undefined behavior sanitizer (UBSAN)", True),
        BoolVariable("use_asan", "Use LLVM/GCC compiler address sanitizer (ASAN)", True),
        BoolVariable("use_lsan", "Use LLVM/GCC compiler leak sanitizer (LSAN)", True),
        BoolVariable("use_tsan", "Use LLVM/GCC compiler thread sanitizer (TSAN)", True)
	]

def get_flags():
	return [
		("arch", "arm32"),
		('tools', False),
		('editor',False),
		('debug_symbols',True),
		('target','template_release'),
		("module_glslang_enabled",False),
		('module_svg_enabled',False),
		('module_upnp_enabled',False),
		('module_mbedtls_enabled',False),
		('module_vhacd_enabled',False),
		('module_astcenc_enabled', False),
		('module_basis_universal_enabled', False),
		('module_tinyexr_enabled',False),
		('module_zip_enabled',False),
		('module_xatlas_unwrap_enabled',False),
		("builtin_pcre2_with_jit", False),
		('builtin_mbedtls', False),
		('builtin_miniupnpc',False),
		('vulkan',False),
		('gles3', False)
	]


def configure_arch(env):
		env.Append(CCFLAGS=['-march=armv6k','-mtp=soft', '-mfpu=vfp', '-mfloat-abi=hard'])

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

def configure_target(env):
	pass # use engine default

def configure_misc(env):
	env["SHLIBSUFFIX"] = ".so"
	dkp_path = os.getenv("DEVKITPRO")
	if env["PLATFORM"] == "win32":
		env.use_windows_spawn_fix()
	env.Append(CPPPATH=['#platform/3ds'])
	env.Append(CCFLAGS=['-Wall','-mword-relocations','-ffunction-sections', '-fno-exceptions', '-std=gnu++17'])
	env.Append(CCFLAGS=['-D__3DS__', '-DLIBC_FILEIO_ENABLED','-DNO_SAFE_CAST','-DNEED_LONG_INT','-D_XOPEN_SOURCE=500'])
	env.Append(CCFLAGS=['-U__INT32_TYPE__','-U__UINT32_TYPE__','-U__INT64_TYPE__','-U__UINT64_TYPE__','-D__INT32_TYPE__=int','-D__UINT32_TYPE__=unsigned int','-D__UINT64_TYPE__=long unsigned int','-DUNIX_SOCKET_UNAVAILABLE'])
	env.Append(CPPPATH=[dkp_path +"/portlibs/3ds/include",dkp_path +"/portlibs/armv6k/include", dkp_path + "/libctru/include", dkp_path + "/devkitARM/arm-none-eabi/include"])
	env.Append(LIBPATH=[dkp_path+"/portlibs/armv6k/lib", dkp_path +
               "/portlibs/3ds/lib", dkp_path + "/libctru/lib", dkp_path + "/arm-none-eabi/lib/armv6k/fpu"])
	env.Append(LINKFLAGS=['-specs=3dsx.specs','-march=armv6k','-mtp=soft', '-mfpu=vfp', '-mfloat-abi=hard'])
	env.Append(LIBS=['citro3d','ctru','bz2','png','z'])

def configure(env):
	configure_arch(env)
	configure_cross(env)
	configure_target(env)
	configure_misc(env)
