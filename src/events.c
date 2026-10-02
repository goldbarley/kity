#include "handles.h"
#include "kity/types.h"
#include "kity/events.h"

#include <curses.h>

KITY_API kity_fnret_t kity_poll(kity_window_t *KITY_RESTRICT window,
				kity_bool_t change)
{
	if (!window)
		return KITY_ERROR_INVALID_ARGUMENT;

	return nodelay(((struct kity_window_s *)(window))->handle, change) != OK ?
		KITY_ERROR_FAILURE : KITY_SUCCESS;
}

KITY_API int kity_get_next_event(kity_window_t *KITY_RESTRICT window,
				 struct kity_event *KITY_RESTRICT event)
{
	if (!window || !event)
		return KITY_ERROR_INVALID_ARGUMENT;

	event-> = wgetch(((struct kity_window_s *)(window))->handle);
}

KITY_API int kity_unget_event(kity)
