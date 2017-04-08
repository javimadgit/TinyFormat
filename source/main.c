/*
 * TinyFormat v1.0
 * javiMaD
 *
 */

#include <stdio.h>
#include <string.h>
#include <3ds.h>

Result
FS_InitializeCtrFileSystem(Handle handle)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = 0x08430000;

	Result ret = 0;
	if((ret = svcSendSyncRequest(handle)))
		return ret;

	ret = cmdbuf[1];
	return ret;
}

int main(int argc, char** argv)
{
	// Initialize services
	gfxInitDefault();
	aptInit();
	fsInit();

	Result res;

	// Init console for text output
	consoleInit(GFX_BOTTOM, NULL);

	printf("TinyFormat\n");
	printf("----------\n\n");
	printf("Press START to exit.\n");
	printf("Press Y to start.\n\n");
	
	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
		{
			printf("CANCEL! Exiting...\n");
			break;
		}
		else if (kDown & KEY_Y)
		{ 
			printf("ALL YOUR DATA WILL BE DELETED (except on SD Card)\n")
		        printf("Press again Y format system , START to exit\n")
			if (kDown & KEY_START)
			{
				printf("CANCEL! Exiting...\n");
				break;
			}
			else if (kDown & KEY_Y)
			{ 
				printf("InitializeCtrFileSystem. Please wait...\n");
				res = FS_InitializeCtrFileSystem( *(fsGetSessionHandle()) );
				if (res == 0)
				printf("Done!\n");
				else
				printf("FAILED!\n");

				printf("Wait for system reboot...\n");
				svcSleepThread(3000000000);
				aptOpenSession();
				APT_HardwareResetAsync(NULL);
				aptCloseSession();
			}
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	// Exit services
	fsExit();
	aptExit();
	gfxExit();
	return 0;
}
