#pragma once
#include "pipelinedata.h"
#include <gtkmm.h>
#include <gdk/gdk.h>
#include <gdk/gdkwin32.h>
extern "C" {
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
}



class AppWindow : public Gtk::ApplicationWindow
{
public:
	AppWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
	virtual ~AppWindow();
	static AppWindow* create(PipelineData* pipeline_data);
	void open_file_view(const Glib::RefPtr<Gio::File>& file);

protected:
	void on_video_area_realize();
	static void on_bus_message_sync(GstBus* bus, GstMessage* message, PipelineData* pipeline_data);
	static void on_bus_message(GstBus* bus, GstMessage* message, PipelineData* pipeline_data);

	// Members
	Glib::RefPtr<Gtk::Builder> m_refBuilder;
	PipelineData* m_pipeline_data;	
	Gtk::DrawingArea* m_video_drawing_area;
	Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();

private:
	Gtk::Widget* associate_widget(const Glib::ustring& name);
};
