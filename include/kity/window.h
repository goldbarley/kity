#ifndef KITY_WINDOW_H_
#define KITY_WINDOW_H_ 1

#include "attr.h"
#include "common.h"
#include "types.h"

KITY_BEGIN_DECLS

extern kity_window_t Kity_Terminal_Window;

KITY_API kity_fnret_t kity_init(kity_screen_t *screen);

KITY_API void kity_shutdown(kity_screen_t *screen);

KITY_API kity_fnret_t kity_create_window(const uint16_t width, const uint16_t height,
					 const char *KITY_RESTRICT title,
					 const kity_layout_mode_t layout_mode,
					 const union kity_layout *KITY_RESTRICT layout,
					 kity_window_t *KITY_RESTRICT window,
					 kity_window_t *KITY_RESTRICT parent);

KITY_API kity_fnret_t kity_draw_window_border(kity_window_t *KITY_RESTRICT window,
					      struct kity_border_info *KITY_RESTRICT border_info);

KITY_API kity_fnret_t kity_refresh_window(const kity_window_t *window);

KITY_API void kity_destroy_window(kity_window_t *window);

KITY_API kity_fnret_t kity_writesn(kity_window_t *KITY_RESTRICT window,
				  const uint16_t x, const uint16_t y,
				  const char *KITY_RESTRICT s,
				  const uint32_t n);

KITY_INLINE kity_fnret_t kity_writes(kity_window_t *KITY_RESTRICT window,
				    const uint16_t x, const uint16_t y,
				    const char *KITY_RESTRICT s)
{
	return kity_writesn(window, x, y, s, -1);
}

KITY_API kity_fnret_t kity_writeat(kity_window_t *KITY_RESTRICT window,
				   const uint16_t x, const uint16_t y,
				   const char *KITY_RESTRICT format, ...);

KITY_API kity_fnret_t kity_write(kity_window_t *KITY_RESTRICT window,
				 const char *KITY_RESTRICT format, ...);

KITY_API kity_fnret_t kity_write_window_title(const kity_window_t *KITY_RESTRICT window,
					      const char *KITY_RESTRICT title,
					      kity_align_t align, const uint16_t off,
					      const uint32_t len);

KITY_END_DECLS

#endif /* KITY_H_ */
