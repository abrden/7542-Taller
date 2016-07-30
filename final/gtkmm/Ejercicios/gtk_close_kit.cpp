#include <gtkmm.h>

void click_button() {
    Gtk::Main::quit();
}

bool quitting () {
    Gtk::MessageDialog("Goodbye").run();
    return true;
}

int main(int argc, char *argv[]) {
    Gtk::Main kit(argc, argv);

    Gtk::Window window;
    Gtk::Button close("Close");
    window.add(close);
    close.signal_clicked().connect(sigc::ptr_fun(&click_button));
    Gtk::Main::signal_quit().connect(sigc::ptr_fun(&quitting));

    window.show_all();
    Gtk::Main::run(window);

    return 0;
}
