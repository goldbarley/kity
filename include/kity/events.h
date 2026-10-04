#ifndef KITY_EVENTS_H_
#define KITY_EVENTS_H_ 1

#include "common.h"
#include "attr.h"
#include "types.h"

KITY_BEGIN_DECLS

KITY_API kity_fnret_t kity_poll(const kity_window_t *KITY_RESTRICT window,
				kity_bool_t change);

KITY_API int kity_get_char(const kity_window_t *KITY_RESTRICT window,
			   register struct kity_event *KITY_RESTRICT event);

KITY_END_DECLS

#endif /* KITY_EVENTS_H_ */
