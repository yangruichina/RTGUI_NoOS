/*
 * File      : textbox.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-16     Bernard      first version
 */
#ifndef __RTGUI_TEXTBOX_H__
#define __RTGUI_TEXTBOX_H__

#include <rtgui/widgets/widget.h>
#include <rtgui/widgets/container.h>

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_CLASS_TYPE(textbox);

/** Gets the type of a textbox */
#define RTGUI_TEXTBOX_TYPE       (RTGUI_TYPE(textbox))
/** Casts the object to a rtgui_textbox_t */
#define RTGUI_TEXTBOX(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_TEXTBOX_TYPE, rtgui_textbox_t))
/** Checks if the object is a rtgui_textbox_t */
#define RTGUI_IS_TEXTBOX(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_TEXTBOX_TYPE))

#define RTGUI_TEXTBOX_DEFAULT_WIDTH		80
#define RTGUI_TEXTBOX_DEFAULT_HEIGHT		20

#define RTGUI_TEXTBOX_NONE			0x00
#define RTGUI_TEXTBOX_MULTI			0x01 /* multiline */
#define RTGUI_TEXTBOX_MASK			0x02 /* ciphertext */
#define RTGUI_TEXTBOX_DIGIT  		0x04 /* digit */
#define RTGUI_TEXTBOX_CARET	0x10
#define RTGUI_TEXTBOX_CARET_STAT	0x20 /* unused */

#define RTGUI_TEXTBOX_LINE_MAX		128  /* text line cache */

struct rtgui_textbox
{
	/* inherit from widget */
	rtgui_widget_t parent;

	/* text box flag */
	rt_uint32_t flag;
	rt_uint32_t isedit;

	/* current line and position */
	rt_uint16_t line, line_begin, position, line_length;
	rt_uint16_t dis_length;	/*may be display length.*/
	char* text;
	rt_size_t font_width;

	struct rtgui_timer *caret_timer;
	rtgui_color_t *caret;
	rtgui_rect_t  caret_rect;

	/* textbox private data */
	rt_bool_t (*on_change)(pvoid wdt, rtgui_event_t* event);
	rt_bool_t (*on_enter) (pvoid wdt, rtgui_event_t* event);
};
typedef struct rtgui_textbox rtgui_textbox_t;

void _rtgui_textbox_constructor(rtgui_textbox_t *box);
void _rtgui_textbox_deconstructor(rtgui_textbox_t *textbox);

rtgui_textbox_t* rtgui_textbox_create(pvoid wdt,const char* text,int left,int top,int w,int h, rt_uint32_t flag);
void rtgui_textbox_destroy(rtgui_textbox_t* box);
void rtgui_textbox_ondraw(rtgui_textbox_t* box);
rt_bool_t rtgui_textbox_event_handler(pvoid wdt, rtgui_event_t* event);
void rtgui_textbox_set_value(rtgui_textbox_t* box, const char* text);
const char* rtgui_textbox_get_value(rtgui_textbox_t* box);

void rtgui_textbox_set_line_length(rtgui_textbox_t* box, rt_size_t length);

void rtgui_textbox_get_edit_rect(rtgui_textbox_t *box,rtgui_rect_t *rect);

#ifdef __cplusplus
}
#endif

#endif
