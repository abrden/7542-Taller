/*
 * Implemente una rutina (en Windows o Linux) que dibuje un 
 * óvalo que ocupe toda la ventana.
 */

#include <gtkmm/drawingarea.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <cmath>

class Ovalo : public Gtk::DrawingArea {

	protected:
		virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
			Gtk::Allocation alloc = get_allocation();
			int ancho = alloc.get_width();
			int alto = alloc.get_height();
			int x = ancho / 2;
			int y = alto / 2;
			
			cr->translate(x, y);
			cr->scale(x, y);
			
			cr->arc(0, 0, 1.0, 0, 2 * M_PI);
			
			cr->set_source_rgb(1.0, 0, 1.0);
			cr->fill();

			cr->stroke();
			return true;
		}
};

int main(int argc, char* argv[]) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create
									(argc, argv, "org.gtkmm.example");
    Gtk::Window win;
	win.set_title("DrawingArea");
	Ovalo area;
	win.add(area);
	area.show();
    return app->run(win);
}
