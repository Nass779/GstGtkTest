# GstGtkTest


Simple Gtk3 and gstreamer project to illustrate a gstreamer issue on windows (see https://gitlab.freedesktop.org/gstreamer/gstreamer/-/issues/712). 

## The app 
  * Simple gstreamer pipeline composed of videotestsrc and d3d11videosink.
  * Video is displayed on a drawingArea (Gtk)
 
## Programming environment
  * Windows 10
  * Visual studio 
  * gstreamer 3.18
  * gtkmm 3.22.2.1 (vcpkg)
  
## Bugs 
  1.  glimagesink and d3dvideosink :  you can try to use it, video is not displayed.
  1.  d3d11videosink :
      * on app start   
      ![GitHub Logo](/images/onstart.png)
      
      * on resize : 
       ![GitHub Logo](/images/onresize.png)
       
       Here we see that the video is not displayed over all the available space.
