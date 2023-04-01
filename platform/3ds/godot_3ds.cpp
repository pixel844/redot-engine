#define FT_DEBUG_AUTOFIT
#include "platform_config.h"
#include <unistd.h>
#include "main/main.h"
#include "os_3ds.h"
#include <sys/resource.h>
#include "servers/rendering/citro3d/display_server_citro3d.h"
#include <filesystem>

struct args {
	int argc;
	char **argv;
};

void stacked_main(void *arg)
{
	args *argobject = (args*)arg;
	char **argv = argobject->argv;
	char argc = argobject->argc;
	OS_3DS os;
	std::string without_filename = std::filesystem::path(argv[0]).remove_filename();
	chdir(without_filename.c_str());
	setlocale(LC_CTYPE,"");
	printf("%s\n",argv[0]);
	char *path[2] = {"--main-pack","sdmc:/3ds/godot.3ds.template_debug.arm32.elf.pck"};
	Error err = Main::setup(argv[0],2, path);
	os.setup();
	if (err != OK) {
		printf("The error of setup was %i\n", err);
		while(!aptShouldClose());
		if (err == ERR_HELP) { // Returned by --help and --version, so success.
			return;
		}
		return;
	}
	if(Main::start())
	{
		os.set_exit_code(EXIT_SUCCESS);
		os.run();
	}
	Main::cleanup();
    printf("Exit code : %d\n",os.get_exit_code());
	return;
}

int main(int argc, char *argv[])
{
	Handle thread;
	args argobject;
	argobject.argc = argc;
	argobject.argv = argv;
	CTRThread mainThread = threadCreate(stacked_main,&argobject,64 * 1024,0x30,0,true);
	Handle threadHandle = threadGetHandle(mainThread);
	threadJoin(mainThread,U64_MAX);
	return 0;
}

extern "C" {
	uint32_t htonl(uint32_t hostlong)
	{
		return __builtin_bswap32(hostlong);
	}

	uint16_t htons(uint16_t hostshort)
	{
		return __builtin_bswap16(hostshort);
	}

	uint32_t ntohl(uint32_t netlong)
	{
		return __builtin_bswap32(netlong);
	}

	uint16_t ntohs(uint16_t netshort)
	{
		return __builtin_bswap16(netshort);
	}
}