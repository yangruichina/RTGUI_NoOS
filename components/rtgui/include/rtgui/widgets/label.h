/*
 * File      : label.h
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
#ifndef __RTGUI_LABEL_H__
#define __RTGUI_LABEL_H__

#include <rtgui/widgets/widget.h>
#include <rtgui/widgets/container.h>

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_CLASS_TYPE(label);
/** Gets the type of a label */
#define RTGUI_LABEL_TYPE       (RTGUI_TYPE(label))//(rtgui_label_type_get())
/** Casts the object to an rtgui_label_t */
#define RTGUI_LABEL(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_LABEL_TYPE, rtgui_label_t))
/** Checks if the object is an rtgui_label_t */
#define RTGUI_IS_LABEL(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_LABEL_TYPE))

/*
 * the label widget
 */
struct rtgui_label
{
	rtgui_widget_t parent;
	/* label */
	char* text;
};
typedef struct rtgui_label rtgui_label_t;

rtgui_label_t* rtgui_label_create(pvoid parent, const char* text, int left, int top, int w, int h);
void rtgui_label_destroy(rtgui_label_t* label);
void rtgui_label_ondraw(rtgui_label_t* label);
rt_bool_t rtgui_label_event_handler(pvoid wdt, rtgui_event_t* event);

void rtgui_label_set_text(rtgui_label_t* label, const char* text);
char* rtgui_label_get_text(rtgui_label_t* label);

#ifdef __cplusplus
}
#endif

#endif
