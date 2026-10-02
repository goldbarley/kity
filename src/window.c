#include "handles.h"
#include "kity/window.h"
#include "kity/types.h"

#include <ncurses.h>

kity_window_t Kity_Terminal_Window = {0};

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
						const kity_align_t align,
						struct kity_window_s *KITY_RESTRICT parent)
{
	uint16_t x0 = parent->x;
	uint16_t y0 = parent->y;
	uint16_t xm = parent->width;
	uint16_t ym = parent->height;

	if (xm < width || ym < height)
		return KITY_ERROR_INVALID_ARGUMENT;

	switch (align)
	{
		case KITY_ALIGN_NONE:
			return KITY_SUCCESS;
		case KITY_ALIGN_LEFT:
			*x = 0;
			*y = (ym >> 1) - height;
			break;
		case KITY_ALIGN_RIGHT:
			*x = xm - width;
			*y = (ym >> 1) - height;
			break;
		case KITY_ALIGN_TOP:
			*x = (xm >> 1) - width;
			*y = 0;
			break;
		case KITY_ALIGN_BOTTOM:
			*x = (xm >> 1) - width;
			*y = ym;
			break;
		case KITY_ALIGN_CENTRE:
			*x = (xm >> 1) - width;
			*y = (ym >> 1) - height;
			break;
		case KITY_ALIGN_TOPLEFT:
			*x = 0;
			*y = 0;
			break;
		case KITY_ALIGN_TOPRIGHT:
			*x = xm - width;
			*y = 0;
			break;
		case KITY_ALIGN_BOTTOMLEFT:
			*x = 0;
			*y = ym - height;
			break;
		case KITY_ALIGN_BOTTOMRIGHT:
			*x = xm - width;
			*y = ym - height;
			break;
		default:
			return KITY_ERROR_INVALID_ARGUMENT;
	}

	return KITY_SUCCESS;
}

KITY_API kity_fnret_t kity_init(void)
{
	if (initscr() != OK)
		return KITY_ERROR_FAILURE;

	register struct kity_window_s *win =
		(struct kity_window_s *) &Kity_Terminal_Window;

	win->handle = stdscr;
	win->parent = NULL;
	win->width = getmaxx(stdscr);
	win->height = getmaxy(stdscr);
	win->x = getbegx(stdscr);
	win->y = getbegy(stdscr);
	win->derived = KITY_FALSE;

	return KITY_SUCCESS;
}

KITY_API void kity_shutdown(void)
{
	endwin();
}

KITY_API kity_fnret_t kity_create_window(const uint16_t width, const uint16_t height,
					 const char *KITY_RESTRICT title,
					 const kity_layout_mode_t layout_mode,
					 const union kity_layout *KITY_RESTRICT layout,
					 kity_window_t *KITY_RESTRICT window,
					 kity_window_t *KITY_RESTRICT parent)
{
	if (!width || !height || !layout ||!window)
		return KITY_ERROR_INVALID_ARGUMENT;

	register struct kity_window_s *win = (struct kity_window_s *) window;
	register struct kity_window_s *pwin = (struct kity_window_s *) parent;

	win->parent = parent ?
		pwin : (struct kity_window_s *) &Kity_Terminal_Window;
	win->derived = parent != &Kity_Terminal_Window;

	kity_fnret_t error = KITY_SUCCESS;

	switch (layout_mode)
	{
		case KITY_LAYOUT_MODE_MANUAL:
			win->width = width;
			win->height = height;
			error = kity_get_aligned_pos(&win->x, &win->y,
					     win->width, win->height,
					     win->layout.manual.w_align,
					     win->parent);
			break;
		case KITY_LAYOUT_MODE_PAD:
			kity_get_padded_dim(&win->width, &win->height,
					    layout, pwin);
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

KITY_API kity_fnret_t kity_draw_window_border(kity_window_t *window,
					      struct kity_border_info *border_info)
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

KITY_API void kity_destroy_window(kity_window_t *window)
{
	if (!window)
		return;

	delwin(((struct kity_window_s *)(window))->handle);
}
