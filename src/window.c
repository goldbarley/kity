#include "handles.h"
#include "kity/screen.h"
#include "kity/types.h"
#include "kity/window.h"

#include <string.h>

#include <curses.h>

kity_window_t Kity_Terminal_Window = {0};
static struct kity_window_s *Kity_Terminal_Window_S =
	(struct kity_window_s *) &Kity_Terminal_Window;

static inline void kity_get_padded_dim(uint16_t *width, uint16_t *height,
				       const union kity_layout *KITY_RESTRICT layout,
				       struct kity_window_s *parent)
{
	uint16_t w = getmaxx(parent->handle);
	uint16_t h = getmaxy(parent->handle);

	w -= (layout->padinfo.left + layout->padinfo.right);
	h -= (layout->padinfo.top + layout->padinfo.bottom);

	*width = w;
	*height = h;
}

static inline void kity_get_padded_pos(uint16_t *x, uint16_t *y,
				       const union kity_layout *KITY_RESTRICT layout,
				       struct kity_window_s *parent)
{
	uint16_t x0 = getbegx(parent->handle);
	uint16_t y0 = getbegy(parent->handle);

	*x = x0 + layout->padinfo.left;
	*y = y0 + layout->padinfo.top;
}

static inline kity_fnret_t kity_get_aligned_pos(uint16_t *x, uint16_t *y,
						const uint16_t width,
						const uint16_t height,
						const uint16_t off_x,
						const uint16_t off_y,
						const kity_align_t align,
						struct kity_window_s *KITY_RESTRICT parent)
{
	uint16_t x0 = parent->x;
	uint16_t y0 = parent->y;
	uint16_t xm = x0 + parent->width;
	uint16_t ym = y0 + parent->height;

	if (parent->width < (width + off_x) || parent->height < (height + off_y))
		return KITY_ERROR_INVALID_ARGUMENT;

	switch (align)
	{
		case KITY_ALIGN_NONE:
			return KITY_SUCCESS;
		case KITY_ALIGN_LEFT:
			*x = x0 + off_x;
			*y = y0 + ((ym - y0 - height) >> 1);
			break;
		case KITY_ALIGN_RIGHT:
			*x = xm - width - off_x;
			*y = y0 + ((ym - y0 - height) >> 1);
			break;
		case KITY_ALIGN_TOP:
			*x = x0 + ((xm - x0 - width) >> 1);
			*y = y0 + off_y;
			break;
		case KITY_ALIGN_BOTTOM:
			*x = x0 + ((xm - x0 - width) >> 1);
			*y = ym - height - off_y;
			break;
		case KITY_ALIGN_CENTRE:
			*x = x0 + ((xm - x0 - width) >> 1);
			*y = y0 + ((ym - y0 - height) >> 1);
			break;
		case KITY_ALIGN_TOPLEFT:
			*x = x0 + off_x;
			*y = y0 + off_y;
			break;
		case KITY_ALIGN_TOPRIGHT:
			*x = xm - width - off_x;
			*y = y0 + off_y;
			break;
		case KITY_ALIGN_BOTTOMLEFT:
			*x = x0 + off_x;
			*y = ym - height - off_y;
			break;
		case KITY_ALIGN_BOTTOMRIGHT:
			*x = xm - width - off_x;
			*y = ym - height - off_y;
			break;
		default:
			return KITY_ERROR_INVALID_ARGUMENT;
	}

	return KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_init(kity_screen_t *screen)
{
	kity_fnret_t error = kity_create_default_screen(&screen);
	if (error)
		return error;

	error = kity_set_screen(screen);
	if (error)
		return error;

	Kity_Terminal_Window_S = (struct kity_window_s *) &Kity_Terminal_Window;

	Kity_Terminal_Window_S->handle = stdscr;
	Kity_Terminal_Window_S->parent = NULL;
	Kity_Terminal_Window_S->width = COLS;
	Kity_Terminal_Window_S->height = LINES;
	Kity_Terminal_Window_S->x = getbegx(stdscr);
	Kity_Terminal_Window_S->y = getbegy(stdscr);
	Kity_Terminal_Window_S->derived = KITY_FALSE;

	return KITY_SUCCESS;
}

KITY_API void kity_shutdown(kity_screen_t *screen)
{
	endwin();
	kity_destroy_screen(screen);
}

KITY_API kity_fnret_t kity_create_window(const uint16_t width, const uint16_t height,
					 const char *KITY_RESTRICT title,
					 const kity_layout_mode_t layout_mode,
					 const union kity_layout *KITY_RESTRICT layout,
					 kity_window_t *KITY_RESTRICT window,
					 kity_window_t *KITY_RESTRICT parent)
{
	if (!layout || (layout_mode == KITY_LAYOUT_MODE_MANUAL && !(width && height)) ||!window)
		return KITY_ERROR_INVALID_ARGUMENT;

	register struct kity_window_s *win = (struct kity_window_s *) window;

	memset(win, 0, sizeof(struct kity_window_s));

	win->parent = parent ?
		(struct kity_window_s *) parent
		: (struct kity_window_s *) &Kity_Terminal_Window;
	win->derived = parent != &Kity_Terminal_Window && parent != NULL;

	win->layout = *layout;

	kity_fnret_t error = KITY_SUCCESS;

	switch (layout_mode)
	{
		case KITY_LAYOUT_MODE_MANUAL:
			win->width = width;
			win->height = height;
			error = kity_get_aligned_pos(&win->x, &win->y,
					     win->width, win->height,
					     win->layout.manual.off_x,
					     win->layout.manual.off_y,
					     win->layout.manual.w_align,
					     win->parent);
			if (error)
				return KITY_ERROR_FAILURE;
			break;
		case KITY_LAYOUT_MODE_PAD:
			kity_get_padded_dim(&win->width, &win->height,
					    layout, win->parent);
			kity_get_padded_pos(&win->x, &win->y, layout, win->parent);
			break;
		default:
			return KITY_ERROR_INVALID_ARGUMENT;
	}

	win->handle = win->derived ?
		derwin(win->parent->handle, win->height, win->width, win->y, win->x) :
		newwin(win->height, win->width, win->y, win->x);
	if (!win->handle)
		return KITY_ERROR_FAILURE;

	return KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_draw_window_border(kity_window_t *KITY_RESTRICT window,
					      struct kity_border_info *KITY_RESTRICT border_info)
{
	if (!window || !border_info)
		return KITY_ERROR_INVALID_ARGUMENT;

	int error = wborder(((struct kity_window_s *)(window))->handle,
			    border_info->left,
			    border_info->right,
			    border_info->top,
			    border_info->bottom,
			    border_info->topleft,
			    border_info->topright,
			    border_info->bottomleft,
			    border_info->bottomright);

	return error ? KITY_ERROR_FAILURE : KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_refresh_window(const kity_window_t *window)
{
	return wrefresh(((struct kity_window_s *)(window))->handle) != OK ?
		KITY_ERROR_FAILURE : KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_writesn(kity_window_t *KITY_RESTRICT window,
				  const uint16_t x, const uint16_t y,
				  const char *KITY_RESTRICT s,
				  const uint32_t n)
{
	if (!window)
		return KITY_ERROR_INVALID_ARGUMENT;

	if (!s)
		return KITY_SUCCESS;

	if (mvwaddnstr(((struct kity_window_s *)(window))->handle, y, x, s, n) != OK)
		return KITY_ERROR_OUT_OF_BOUNDS;

	return KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_writeat(kity_window_t *KITY_RESTRICT window,
				   const uint16_t x, const uint16_t y,
				   const char *KITY_RESTRICT format, ...)
{
	if (!window)
		return KITY_ERROR_INVALID_ARGUMENT;

	if (!format)
		return KITY_SUCCESS;

	va_list args;
	va_start(args, format);

	struct kity_window_s *win = (struct kity_window_s *) window;

	wmove(win->handle, y, x);
	if (vw_printw(win->handle, format, args) != OK)
		return KITY_ERROR_FAILURE;

	va_end(args);

	return KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_write(kity_window_t *KITY_RESTRICT window,
				 const char *KITY_RESTRICT format, ...)
{
	if (!window)
		return KITY_ERROR_INVALID_ARGUMENT;

	if (!format)
		return KITY_SUCCESS;

	va_list args;
	va_start(args, format);

	struct kity_window_s *win = (struct kity_window_s *) window;
	if (vw_printw(win->handle, format, args) != OK)
		return KITY_ERROR_FAILURE;

	va_end(args);

	return KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_write_window_title(const kity_window_t *KITY_RESTRICT window,
					      const char *KITY_RESTRICT title,
					      kity_align_t align, const uint16_t off,
					      const uint32_t len)
{
	if (!window || !(title || len))
		return KITY_ERROR_INVALID_ARGUMENT;

	const uint32_t halflen = len >> 1;
	const struct kity_window_s *win = (struct kity_window_s *) window;
	const uint16_t x0 = win->x;
	const uint16_t y0 = win->y;
	const uint16_t width = win->width;
	const uint16_t height = win->height;
	uint16_t x = 0;
	uint16_t y = 0;

	switch (align)
	{
		case KITY_ALIGN_NONE:
			x = off;
			y = off;
			break;
		case KITY_ALIGN_LEFT:
			x = off;
			y = height >> 1;
			break;
		case KITY_ALIGN_RIGHT:
			x = width - off;
			y = height >> 1;
			break;
		case KITY_ALIGN_TOP:
			x = ((width - len) >> 1);
			y = off;
			break;
		case KITY_ALIGN_BOTTOM:
			x = ((width - len) >> 1);
			y = height - off;
			break;
		case KITY_ALIGN_CENTRE:
			x = ((width - len) >> 1);
			y = height >> 1;
			break;
		case KITY_ALIGN_TOPLEFT:
			x = off;
			y = off;
			break;
		case KITY_ALIGN_TOPRIGHT:
			x = width - off;
			y = off;
			break;
		case KITY_ALIGN_BOTTOMLEFT:
			x = off;
			y = height - off;
			break;
		case KITY_ALIGN_BOTTOMRIGHT:
			x = width - off;
			y = height - off;
			break;
		default:
			return KITY_ERROR_INVALID_ARGUMENT;
	}

	if (mvwaddstr(win->handle, y, x, title) != OK)
		return KITY_ERROR_OUT_OF_BOUNDS;

	return KITY_SUCCESS;
}

KITY_API void kity_destroy_window(kity_window_t *window)
{
	if (!window)
		return;

	delwin(((struct kity_window_s *)(window))->handle);
}
