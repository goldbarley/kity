#ifndef KITY_WINDOW_H_
#define KITY_WINDOW_H_ 1

#include "attr.h"
#include "common.h"
#include "types.h"

KITY_BEGIN_DECLS

extern kity_window_t Kity_Terminal_Window;

KITY_API kity_fnret_t kity_init(void);

KITY_API void kity_shutdown(void);

KITY_API kity_fnret_t kity_create_window(const uint16_t width, const uint16_t height,
					 const char *KITY_RESTRICT title,
					 const kity_layout_mode_t layout_mode,
					 const union kity_layout *KITY_RESTRICT layout,
					 kity_window_t *KITY_RESTRICT window,
					 kity_window_t *KITY_RESTRICT parent);

KITY_API kity_fnret_t kity_draw_window_border(kity_window_t *window,
					      struct kity_border_info *border_info);

KITY_API void kity_destroy_window(kity_window_t *window);

KITY_END_DECLS

#endif /* KITY_H_ */
