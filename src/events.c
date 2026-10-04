#include "handles.h"
#include "kity/types.h"
#include "kity/events.h"

#include <curses.h>

KITY_API kity_fnret_t kity_poll(const kity_window_t *KITY_RESTRICT window,
				kity_bool_t change)
{
	if (!window)
		return KITY_ERROR_INVALID_ARGUMENT;

	return nodelay(((struct kity_window_s *)(window))->handle, change) != OK ?
		KITY_ERROR_FAILURE : KITY_SUCCESS;
}

KITY_API int kity_get_char(const kity_window_t *KITY_RESTRICT window,
			   register struct kity_event *KITY_RESTRICT event)
{
	if (!window || !event)
		return KITY_ERROR_INVALID_ARGUMENT;

	event->key.id = wgetch(((struct kity_window_s *)(window))->handle);
	event->type = KITY_EVENT_KEY;

	return event->key.id;
}

KITY_API int kity_unget_char(int chr)
{
	return ungetch(chr);
}
