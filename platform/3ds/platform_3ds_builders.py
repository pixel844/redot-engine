"""Functions used to generate source files during build time

All such functions are invoked in a subprocess on Windows to prevent build flakiness.

"""
import os
from platform_methods import subprocess_main


def make_debug_3ds(target, source, env):
    print("3ds debug build setup")
    os.system("arm-none-eabi-objcopy --only-keep-debug {0} {0}.debugsymbols".format(target[0]))
    os.system("arm-none-eabi-strip --strip-debug --strip-unneeded {0}".format(target[0]))


if __name__ == "__main__":
    subprocess_main(globals())
