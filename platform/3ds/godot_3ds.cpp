#define __UINT32_TYPE__ unsigned int
#define __INT32_TYPE__ int
#define lock_guard base_lock_guard
#include "platform_config.h"
#include <unistd.h>
#include "main/main.h"
#include "os_3ds.h"

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

int main(int argc, char *argv[])
{
    OS_3DS os;
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    TEST_MAIN_OVERRIDE;

    char *cwd = (char *)malloc(PATH_MAX);
	ERR_FAIL_COND_V(!cwd, ERR_OUT_OF_MEMORY);
	char *ret = getcwd(cwd, PATH_MAX);

	Error err = Main::setup(argv[0], argc - 1, &argv[1]);
	if (err != OK) {
		free(cwd);

		if (err == ERR_HELP) { // Returned by --help and --version, so success.
			return 0;
		}
		return 255;
	}
    return 0;
}