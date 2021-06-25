#include "appwindow.h"
#include <direct.h> // _getcwd
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <gdk/gdkwin32.h>


AppWindow::AppWindow(BaseObjectType* cobject,
	const Glib::RefPtr<Gtk::Builder>& refBuilder)
	: Gtk::ApplicationWindow(cobject),
	m_refBuilder(refBuilder)
{
	m_video_drawing_area = (Gtk::DrawingArea*)associate_widget("video_drawing_area");
	m_video_drawing_area->signal_realize().connect(sigc::mem_fun(*this, &AppWindow::on_video_area_realize));
}

AppWindow* AppWindow::create(PipelineData* pipeline_data)
{
	// Load the Builder file and instantiate its widgets.
	auto refBuilder = Gtk::Builder::create_from_file("app_view.glade");
	AppWindow* window = nullptr;
	refBuilder->get_widget_derived("app_window", window);
	if (!window)
		throw std::runtime_error("No \"app_window\" object in app_view.glade");

	window->m_pipeline_data = pipeline_data;
	// Get the bus from the pipeline:
	GstBus* bus = gst_element_get_bus(pipeline_data->pipeline);
	// Enable synchronous message emission 
	gst_bus_enable_sync_message_emission(bus);
	// Connect to bus's synchronous message signal 
	g_signal_connect(G_OBJECT(bus), "sync-message", (GCallback)AppWindow::on_bus_message_sync, pipeline_data);
	// Add a bus watch to receive messages from the pipeline's bus
	gst_bus_add_signal_watch(bus);
	g_signal_connect(G_OBJECT(bus), "message", (GCallback)on_bus_message, pipeline_data);
	return window;
}

void AppWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file */)
{
}

void AppWindow::on_video_area_realize() {
	guintptr window_handler = (guintptr)GDK_WINDOW_HWND(m_video_drawing_area->get_window()->gobj());
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(m_pipeline_data->video_sink), window_handler);
	gst_element_set_state(m_pipeline_data->pipeline, GST_STATE_PLAYING);
}

void AppWindow::on_bus_message_sync(GstBus* bus, GstMessage* message, PipelineData* pipeline_data) {
}

void AppWindow::on_bus_message(GstBus* bus, GstMessage* message, PipelineData* pipeline_data) {
}

Gtk::Widget* AppWindow::associate_widget(const Glib::ustring& name)
{
	std::ostringstream error_message;
	Gtk::Widget* widget = nullptr;

	m_refBuilder->get_widget(name, widget);
	if (!widget)
	{
		error_message << "No " << name << " object in app_view.glade ";
		throw std::runtime_error(error_message.str());
	}
	return widget;
}

AppWindow::~AppWindow()
{
	// reset image format
	GstBus* bus = gst_element_get_bus(m_pipeline_data->pipeline);
	gst_bus_disable_sync_message_emission(bus);
	gst_bus_remove_signal_watch(bus);
	gst_element_set_state(m_pipeline_data->pipeline, GST_STATE_NULL);
	gst_object_unref(m_pipeline_data->pipeline);
}