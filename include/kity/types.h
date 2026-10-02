#ifndef KITY_TYPES_H_
#define KITY_TYPES_H_ 1

#include "common.h"

#include <stdint.h>

KITY_BEGIN_DECLS

#define KITY_SIZEOF_KITY_WINDOW (64U)

typedef enum kity_fnret
{
	KITY_SUCCESS = 0,
	KITY_ERROR_FAILURE = -1,
	KITY_ERROR_OUT_OF_MEMORY = -2,
	KITY_ERROR_INVALID_ARGUMENT = -3
} kity_fnret_t;

typedef enum kity_align
{
	KITY_ALIGN_NONE = 0,
	KITY_ALIGN_LEFT = 1,
	KITY_ALIGN_RIGHT = 2,
	KITY_ALIGN_TOP = 3,
	KITY_ALIGN_BOTTOM = 4,
	KITY_ALIGN_CENTRE = 5,
	KITY_ALIGN_TOPLEFT = 6,
	KITY_ALIGN_TOPRIGHT = 7,
	KITY_ALIGN_BOTTOMLEFT = 8,
	KITY_ALIGN_BOTTOMRIGHT = 9,
	KITY_ALIGN_MAX_ENUM = 0x7FFFFFFF
} kity_align_t;

typedef enum kity_layout_mode
{
	KITY_LAYOUT_MODE_DEFAULT = 0,
	KITY_LAYOUT_MODE_MANUAL = KITY_LAYOUT_MODE_DEFAULT,
	KITY_LAYOUT_MODE_PAD = 1
} kity_layout_mode_t;

#define KITY_FALSE (0)
#define KITY_TRUE (1)

typedef uint8_t kity_bool_t;

typedef struct kity_window
{
	_Alignas(void *) uint8_t size[KITY_SIZEOF_KITY_WINDOW];
} kity_window_t;

union kity_layout
{
	struct
	{
		kity_align_t w_align;
	} manual;
	struct
	{
		uint16_t left;
		uint16_t right;
		uint16_t top;
		uint16_t bottom;
	} padinfo;
};

struct kity_border_info
{
	uint16_t topleft;
	uint16_t top;
	uint16_t topright;
	uint16_t left;
	uint16_t right;
	uint16_t bottomleft;
	uint16_t bottom;
	uint16_t bottomright;
};

struct kity_event
{
	kity_window_t *window;
	union
	{
		struct
		{
			uint16_t id;
		} key;
		struct
		{
			uint16_t id;
			uint16_t x;
			uint16_t y;
		} ptr;
		struct
		{
			float dx;
			float dy;
		} scroll;
	};
};


KITY_END_DECLS

#endif /* KITY_TYPES_H_ */
