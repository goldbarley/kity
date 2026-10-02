#ifndef KITY_HANDLES_H_
#define KITY_HANDLES_H_ 1

#include "kity/common.h"
#include "kity/types.h"

#include <stdint.h>

#include <ncurses.h>

KITY_BEGIN_DECLS

struct kity_window_s
{
	WINDOW *handle;
	struct kity_window_s *parent;
	uint16_t width;
	uint16_t height;
	kity_layout_mode_t layoutmode;
	union kity_layout layout;
	kity_align_t t_align;
	uint16_t x;
	uint16_t y;
	kity_bool_t derived;
};
KITY_ASSERT_SIZE_N_ALIGNMENT(struct kity_window_s,  kity_window_t);

KITY_END_DECLS

#endif /* KITY_HANDLES_H_ */
