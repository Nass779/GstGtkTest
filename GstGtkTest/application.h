#pragma once

#include <gtkmm.h>
extern "C" {
#include <gst/gst.h>
}
class AppWindow;

class Application : public Gtk::Application
{
protected:
	Application();

public:
	static Glib::RefPtr<Application> create();

protected:
	void on_activate() override;
	void on_open(const Gio::Application::type_vec_files& files,
		const Glib::ustring& hint) override;

private:
	AppWindow* create_appwindow();
	void on_hide_window(Gtk::Window* window);
};

