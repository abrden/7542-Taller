#include <gtkmm.h>

int main(int argc, char *argv[]) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Gtk::Window window;
    window.set_border_width(40);
    Gtk::Button close("Close");

    window.add(close);
    close.signal_clicked().connect(sigc::mem_fun(window, &Gtk::Window::unset_application));
    
    window.show_all();
    app->run(window);
    return 0;
}
