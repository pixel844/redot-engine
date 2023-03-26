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
	return os.getenv("DEVKITPRO") != None

def get_opts():
	from SCons.Variables import BoolVariable
	return [
	]

def get_flags():
	return [
	]


def configure_arch(env):
		env.Append(CCFLAGS=['-march=armv6k','-mtp=soft', '-mfpu=vfp', '-mfloat-abi=hard'])
		env.extra_suffix += '.arm32v6k'

def configure_cross(env):
	dkp_path = os.getenv("DEVKITPRO")
	triple = dkp_path + '/devkitARM/bin/arm-none-eabi'
	env['CC'] = triple + '-gcc'
	env['CXX'] = triple + '-g++'
	env["LD"] = triple + "-g++"
	env["AR"] = triple + "-ar"
	env["RANLIB"] = triple + "-ranlib"
	env["AS"] = triple + "-as"

def configure_target(env):
	pass # use engine default

def configure_misc(env):
	dkp_path = os.getenv("DEVKITPRO")
	env.Append(CPPPATH=['#platform/3ds'])
	env.Append(CCFLAGS=['-g','-Wall','-mword-relocations','-ffunction-sections', '-fno-exceptions', '-std=gnu++17'])
	env.Append(CCFLAGS=['-D__3DS__','-DNEED_LONG_INT', '-DLIBC_FILEIO_ENABLED','-DNO_SAFE_CAST'])
	env.Append(CPPPATH=[dkp_path +"/portlibs/armv6k/include", dkp_path +
               "/portlibs/3ds/include", dkp_path + "/libctru/include", dkp_path + "/devkitARM/arm-none-eabi/include"])
	env.Append(LIBPATH=[dkp_path+"/portlibs/armv6k/lib", dkp_path +
               "/portlibs/3ds/lib", dkp_path + "/libctru/lib", dkp_path + "/arm-none-eabi/lib/armv6k/fpu"])
	env.Append(LINKFLAGS=['-specs=3dsx.specs', '-g'])
	env.Append(LIBS=['pthread','citro3d','ctru','bz2','png', 'z'])

def configure(env):
	env.msvc = False
	configure_arch(env)
	configure_cross(env)
	configure_target(env)
	configure_misc(env)
