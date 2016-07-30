/*
 * Implemente una rutina (en Windows o Linux) que 
 * divida la ventana en 4 partes
 */

#include <gtkmm/drawingarea.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <cmath>
#define ANCHO_LINEA 0.001

class Area : public Gtk::DrawingArea {

	protected:
		virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
			Gtk::Allocation alloc = get_allocation();
			int ancho = alloc.get_width();
			int alto = alloc.get_height();
			
			cr->scale(ancho, alto);
			
			cr->set_line_width(ANCHO_LINEA);
			
			cr->move_to(0, 0.25);
			cr->line_to(1, 0.25);
			cr->move_to(0, 0.5);
			cr->line_to(1, 0.5);
			cr->move_to(0, 0.75);
			cr->line_to(1, 0.75);

			cr->stroke();
			return true;
		}
};

int main(int argc, char* argv[]) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create
									(argc, argv, "org.gtkmm.example");
    Gtk::Window win;
	win.set_title("DrawingArea");
	Area area;
	win.add(area);
	area.show();
    return app->run(win);
}
