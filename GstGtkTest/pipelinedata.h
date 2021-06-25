#pragma once
/* Structure to contain all our information, so we can pass it around */
extern "C" {
#include <gst/gst.h>
}

typedef struct _PipelineData
{
	GstElement* pipeline;
	GstElement* video_source;
	GstElement* video_sink;
} PipelineData;
