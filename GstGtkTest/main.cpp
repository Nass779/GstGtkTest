// Gstreamer simple GUI

#include "application.h"

int main(int argc, char* argv[])
{
    auto application = Application::create();
    gst_init(&argc, &argv);
    // Start the application, showing the initial window,
    // run() will return when the last window has been closed by the user.
    return application->run(argc, argv);
}
