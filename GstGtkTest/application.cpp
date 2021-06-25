#include "application.h"
#include "appwindow.h"
#include <iostream>
#include <exception>
#include <shlobj_core.h> 

Application::Application()
	: Gtk::Application("org.gtkmm.gstgui.application", Gio::APPLICATION_HANDLES_OPEN)
{
}

Glib::RefPtr<Application> Application::create()
{
	return Glib::RefPtr<Application>(new Application());
}

AppWindow* Application::create_appwindow()
{
	// Build gstreamer pipeline
	PipelineData* pipeline_data = new PipelineData;
	pipeline_data->pipeline = gst_pipeline_new("video-test-pipeline");
	pipeline_data->video_source = gst_element_factory_make("videotestsrc", "video_source");
	pipeline_data->video_sink = gst_element_factory_make("d3d11videosink", "video_sink");
	if (!pipeline_data->pipeline || !pipeline_data->video_source || !pipeline_data->video_sink) {
		std::cerr << "One of the pipeline elements could not be created." << std::endl;
		exit(-1);
	}


	// add elements to pipeline
	gst_bin_add_many(GST_BIN(pipeline_data->pipeline), pipeline_data->video_source, pipeline_data->video_sink, NULL);

	// link elements
	if ((gst_element_link_many(pipeline_data->video_source, pipeline_data->video_sink, NULL)) != TRUE){
		gst_object_unref(pipeline_data->pipeline);
		exit(-1);
	}

	auto appwindow = AppWindow::create(pipeline_data);

	// Make sure that the application runs for as long this window is still open.
	add_window(*appwindow);

	// Gtk::Application::add_window() connects a signal handler to the window's
	// signal_hide(). That handler removes the window from the application.
	// If it's the last window to be removed, the application stops running.
	// Gtk::Window::set_application() does not connect a signal handler, but is
	// otherwise equivalent to Gtk::Application::add_window().
	// Delete the window when it is hidden.
	appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this,
		&Application::on_hide_window),
		appwindow));

	return appwindow;
}

void Application::on_activate()
{
	try
	{
		std::cout << "Application::on_activate()  " << std::endl;
		// The application has been started, so let's show a window.
		auto appwindow = create_appwindow();
		appwindow->present();
	}
	// If create_appwindow() throws an exception (perhaps from Gtk::Builder),
	// no window has been created, no window has been added to the application,
	// and therefore the application will stop running.
	catch (const Glib::Error& ex)
	{
		std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
	}
}

void Application::on_open(const Gio::Application::type_vec_files& files,
	const Glib::ustring& /* hint */)
{
	// The application has been asked to open some files,
	// so let's open a new view for each one.
	std::cout << "Application::on_open()  " << std::endl;
	AppWindow* appwindow = nullptr;
	auto windows = get_windows();
	if (windows.size() > 0)
		appwindow = dynamic_cast<AppWindow*>(windows[0]);

	try
	{
		if (!appwindow)
			appwindow = create_appwindow();

		for (const auto& file : files)
			appwindow->open_file_view(file);

		appwindow->present();
	}
	catch (const Glib::Error& ex)
	{
		std::cerr << "Application::on_open(): " << ex.what() << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Application::on_open(): " << ex.what() << std::endl;
	}
}

void Application::on_hide_window(Gtk::Window* window)
{
	delete window;
}