/*
 * File      : image.c
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
#include <rtgui/image.h>
#include <rtgui/image_xpm.h>
#include <rtgui/image_hdc.h>

#include <string.h>

#ifdef RTGUI_IMAGE_XPM
#include <rtgui/image_xpm.h>
#endif

#ifdef RTGUI_IMAGE_BMP
#include <rtgui/image_bmp.h>
#endif

static rtgui_list_t _rtgui_system_image_list = {RT_NULL};

/* init rtgui image system */
void rtgui_system_image_init(void)
{
	/* always support HDC image */
	rtgui_image_hdc_init();

#ifdef RTGUI_IMAGE_XPM
	rtgui_image_xpm_init();
#endif

#ifdef RTGUI_IMAGE_BMP
	rtgui_image_bmp_init();
#endif

#ifdef RTGUI_IMAGE_CONTAINER
	/* initialize image container */
	rtgui_system_image_container_init(RT_FALSE);
#endif
}

static struct rtgui_image_engine* rtgui_image_get_engine(const char* type)
{
	rtgui_list_t *node;
	struct rtgui_image_engine *engine;

	rtgui_list_foreach(node, &_rtgui_system_image_list)
	{
		engine = rtgui_list_entry(node, struct rtgui_image_engine, list);

		if(strncasecmp(engine->name, type, strlen(engine->name)) ==0)
			return engine;
	}

	return RT_NULL;
}


#if defined(RTGUI_USING_DFS_FILERW)
rtgui_image_t* rtgui_image_create_from_file(const char* type, const char* filename, rt_bool_t load)
{
	struct rtgui_filerw* file;
	struct rtgui_image_engine* engine;
	rtgui_image_t* image = RT_NULL;

	/* create filerw context */
	file = rtgui_filerw_create_file(filename, "rb");
	if(file == RT_NULL) return RT_NULL;

	/* get image engine */
	engine = rtgui_image_get_engine(type);
	if(engine == RT_NULL)
	{
		/* close filerw context */
		rtgui_filerw_close(file);
		return RT_NULL;
	}

	if(engine->image_check(file) == RT_TRUE)
	{
		image = (rtgui_image_t*) rt_malloc(sizeof(rtgui_image_t));
		if(image == RT_NULL)
		{
			/* close filerw context */
			rtgui_filerw_close(file);
			return RT_NULL;
		}

		if(engine->image_load(image, file, load) != RT_TRUE)
		{
			/* close filerw context */
			rtgui_filerw_close(file);
			return RT_NULL;
		}
		/* set image engine */
		image->engine = engine;
	}
	else
	{
		rtgui_filerw_close(file);
	}

	return image;
}

#endif

rtgui_image_t* rtgui_image_create_from_mem(const char* type, const rt_uint8_t* data, rt_size_t length, rt_bool_t load)
{
	struct rtgui_filerw* file;
	struct rtgui_image_engine* engine;
	rtgui_image_t* image = RT_NULL;


	/* create filerw context */
	file = rtgui_filerw_create_mem(data, length);
	if(file == RT_NULL) return RT_NULL;


	/* get image engine */
	engine = rtgui_image_get_engine(type);
	if(engine == RT_NULL)
	{
		/* close filerw context */
		rtgui_filerw_close(file);
		return RT_NULL;
	}

	if(engine->image_check(file) == RT_TRUE)
	{
		image = (rtgui_image_t*)rt_malloc(sizeof(rtgui_image_t));
		if(image == RT_NULL)
		{
			/* close filerw context */
			rtgui_filerw_close(file);
			return RT_NULL;
		}

		if(engine->image_load(image, file, load) != RT_TRUE)
		{
			/* close filerw context */
			rtgui_filerw_close(file);
			return RT_NULL;
		}

		/* set image engine */
		image->engine = engine;
	}
	else
	{
		rtgui_filerw_close(file);
	}

	return image;
}

void rtgui_image_destroy(rtgui_image_t* image)
{
	RT_ASSERT(image != RT_NULL);

	image->engine->image_unload(image);
	rt_free(image);
}

/* register an image engine */
void rtgui_image_register_engine(struct rtgui_image_engine* engine)
{
	RT_ASSERT(engine!= RT_NULL);

	rtgui_list_append(&_rtgui_system_image_list, &(engine->list));
}

void rtgui_image_blit(rtgui_image_t *image, rtgui_dc_t *dc, rtgui_rect_t *rect)
{
	RT_ASSERT(rect	!= RT_NULL);

	if(image != RT_NULL && image->engine != RT_NULL)
	{
		/* use image engine to blit */
		image->engine->image_blit(image, dc, rect);
	}
}


void rtgui_image_paste(rtgui_image_t *image, rtgui_dc_t *dc, rtgui_rect_t *rect, rtgui_color_t shield_color)
{
	RT_ASSERT(rect != RT_NULL);

	if(image != RT_NULL && image->engine != RT_NULL)
	{
		/* use image engine to blit */
		image->engine->image_paste(image, dc, rect, shield_color);
	}
}


struct rtgui_image_palette* rtgui_image_palette_create(rt_uint32_t ncolors)
{
	struct rtgui_image_palette* palette = RT_NULL;

	if(ncolors > 0)
	{
		palette = (struct rtgui_image_palette*) rt_malloc(sizeof(struct rtgui_image_palette) + sizeof(rt_uint32_t) * ncolors);
		if(palette != RT_NULL) palette->colors = (rt_uint32_t*)(palette + 1);
	}

	return palette;
}

rtgui_image_t* rtgui_image_zoom(rtgui_image_t* image, float scalew, float scaleh, rt_uint32_t mode)  
{ 
	if (image != RT_NULL && image->engine != RT_NULL)
	{
		return image->engine->image_zoom(image, scalew, scaleh, mode);
	}		
	return RT_NULL;
} 
RTM_EXPORT(rtgui_image_zoom);

rtgui_image_t* rtgui_image_rotate(rtgui_image_t* image, float angle)
{
	if (image != RT_NULL && image->engine != RT_NULL)
	{
		return image->engine->image_rotate(image, angle);
	}
	return RT_NULL;
}
RTM_EXPORT(rtgui_image_rotate);
