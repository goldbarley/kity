#include "kity/types.h"
#include "kity/window.h"
#include <kity/kity.h>

#include <stdio.h>
#include <string.h>

int main(void)
{
	kity_fnret_t error = KITY_SUCCESS;
	kity_screen_t screen;
	kity_window_t window;
	kity_init(&screen);
	union kity_layout layout = {
		.manual.w_align = KITY_ALIGN_CENTRE,
		.manual.off_x = 5,
		.manual.off_y = 5
	};
	kity_create_window(60, 20, "A", KITY_LAYOUT_MODE_MANUAL,
			   &layout, &window, NULL);

	struct kity_border_info bordinfo = {0};

	error = kity_draw_window_border(&window, &bordinfo);
	if (error)
	{
		fputs("Could not draw window border.\n", stderr);
		goto cleanup;
	}

	const char *title = "A WINDOW!";
	error = kity_write_window_title(&window, title,
					KITY_ALIGN_CENTRE, 0, strlen(title));
	if (error)
	{
		fputs("Could not write window title.\n", stderr);
		goto cleanup;
	}

	kity_refresh_window(&window);

	struct kity_event event = {0};
	kity_get_char(&window, &event);

	cleanup:
	kity_destroy_window(&window);
	kity_shutdown(&screen);

	return error;
}
