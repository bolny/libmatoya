#include "matoya.h"

// Your top level application context
struct context {
	MTY_App *app;
	bool quit;
};

// This function will fire for each event
static void event_func(const MTY_Event *evt, void *opaque)
{
	struct context *ctx = opaque;

	MTY_PrintEvent(evt);

	if (evt->type == MTY_EVENT_CLOSE)
		ctx->quit = true;
}

// This function fires once per "cycle", either blocked by a
// call to MTY_WindowPresent or limited by MTY_AppSetTimeout
static bool app_func(void *opaque)
{
	struct context *ctx = opaque;

	MTY_WindowPresent(ctx->app, 0, 1);

	return !ctx->quit;
}

int main(int argc, char **argv)
{
	// Set up the application object and attach it to your context
	struct context ctx = {0};
	ctx.app = MTY_AppCreate(app_func, event_func, &ctx);
	if (!ctx.app)
		return 1;

	// Create a window
	MTY_WindowDesc desc = {
		.title = "My Window",
		.api = MTY_GFX_GL,
		.width = 800,
		.height = 600,
	};

	MTY_WindowCreate(ctx.app, &desc);

	// In the case of MTY_GFX_GL, the GL context has to be made current on the
	// thread that's doing the drawing
	MTY_WindowMakeCurrent(ctx.app, 0, true);

	// Run the app -- blocks until your app_func returns false
	MTY_AppRun(ctx.app);
	MTY_AppDestroy(&ctx.app);

	return 0;
}
