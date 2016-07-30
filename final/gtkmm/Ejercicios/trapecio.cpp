/*
 * Implemente una rutina (en Windows o Linux) que dibuje un trapecio 
 * regular en la ventana.
 */


#include <gtkmm.h>

#define ANCHO_LINEA 0.5

class Trapecio : public Gtk::DrawingArea {
	private:
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& ctx) {
				Gtk::Allocation alloc = get_allocation();
				const int height = alloc.get_height();
				const int width = alloc.get_width();

				ctx->scale(width, height);
				ctx->set_line_width(ANCHO_LINEA);
				

				ctx->move_to(0.33, 0);
				ctx->line_to(0.66, 0);
				ctx->line_to(1, 1);
				ctx->line_to(0, 1);
				ctx->close_path();

				ctx->fill();
				ctx->stroke();
			return true;			
		}
};

int main(int argc, char* argv[]) {
	Gtk::Main kit(argc, argv);	
	Trapecio trapecio;
	Gtk::Window v;
	v.add(trapecio);
	v.show_all();
	Gtk::Main::run(v);
	return 0;
}	
