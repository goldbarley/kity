#ifndef KITY_SCREEN_H_
#define KITY_SCREEN_H_ 1

#include "attr.h"
#include "common.h"
#include "types.h"

#include <stdio.h>

KITY_BEGIN_DECLS

KITY_API kity_fnret_t kity_create_screen(kity_screen_t **KITY_RESTRICT screen,
					 FILE *inf, FILE *outf);

KITY_API void kity_destroy_screen(kity_screen_t *screen);

KITY_INLINE kity_fnret_t kity_create_default_screen(kity_screen_t **KITY_RESTRICT screen)
{
	return kity_create_screen(screen, stdin, stdout);
}

KITY_API kity_fnret_t kity_set_screen(kity_screen_t *screen);

KITY_END_DECLS

#endif /* KITY_SCREEN_H_ */
