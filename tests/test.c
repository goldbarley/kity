#include "kity/window.h"
#include <kity/kity.h>

#include <stdio.h>
#include <stddef.h>

#include <ncurses.h>

int main(void)
{
	kity_window_t window;
	kity_init();
	union kity_layout layout = {
		.manual.w_align = KITY_ALIGN_CENTRE
	};
	kity_create_window(10, 10, "A", KITY_LAYOUT_MODE_MANUAL,
			   &layout, &window, NULL);

	struct kity_border_info bordinfo = {0};

	kity_draw_window_border(&window, &bordinfo);

	getch();

	kity_destroy_window(&window);

	kity_shutdown();
}
