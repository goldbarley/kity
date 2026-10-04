#include "handles.h"
#include "kity/screen.h"
#include "kity/types.h"

#include <curses.h>

KITY_API kity_fnret_t kity_create_screen(kity_screen_t **KITY_RESTRICT screen,
					 FILE *inf, FILE *outf)
{
	if (!screen || !*screen || !inf || !outf)
		return KITY_ERROR_INVALID_ARGUMENT;

	SCREEN *term = newterm(NULL, outf, inf);
	if (!term)
		return KITY_ERROR_FAILURE;

	((struct kity_screen_s *)(*screen))->handle = term;

	return KITY_SUCCESS;
}

KITY_API void kity_destroy_screen(kity_screen_t *screen)
{
	if (!screen)
		return;

	delscreen(((struct kity_screen_s *)(screen))->handle);
}

KITY_API kity_fnret_t kity_set_screen(kity_screen_t *screen)
{
	if (!screen)
		return KITY_ERROR_INVALID_ARGUMENT;

	return !set_term(((struct kity_screen_s *)(screen))->handle) ?
		KITY_ERROR_FAILURE : KITY_SUCCESS;
}
