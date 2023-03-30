#include "platform_config.h"
#include <unistd.h>
#include "main/main.h"
#include "os_3ds.h"
#include "renderer/CitroRenderer.h"
#include <sys/resource.h>

int main(int argc, char *argv[])
{
	// Note: Set stack size to be at least 30 MB (vs 8 MB default) to avoid overflow, address sanitizer can increase stack usage up to 3 times.
    OS_3DS os;
	setlocale(LC_CTYPE,"");
    TEST_MAIN_OVERRIDE;
	//char *ret = getcwd(cwd, PATH_MAX);
	Error err = Main::setup("sdmc:/godot.3dsx",0, NULL);
	if (err != OK) {
		printf("The error of setup was %i\n", err);

		if (err == ERR_HELP) { // Returned by --help and --version, so success.
			return 0;
		}
		return 255;
	}
	if (Main::start()) {
		os.set_exit_code(EXIT_SUCCESS);
		os.run();
	}
	Main::cleanup();
    printf("%d",os.get_exit_code());
	return os.get_exit_code();
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