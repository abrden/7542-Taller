/*
 * Escriba una rutina (para ambiente gráfico Windows o Linux) que dibuje 
 * un línea discontinua horizontal en pantalla.
 *  _____
 * |_ _ _|
 * |_____|
 */


#include <gtkmm.h>
#include <vector>

#define ANCHO_LINEA 0.01
#define ESPACIADO_GUIONES 0.05

class Dash : public Gtk::DrawingArea {
	private:
		bool on_draw(const Cairo::RefPtr< Cairo::Context >& ctx) {
			Gtk::Allocation alloc = get_allocation();
			const int height = alloc.get_height();
			const int width = alloc.get_width();
			ctx->scale(width, height);
			ctx->set_line_width(ANCHO_LINEA);
				
			// fondo blanco
			ctx->save();
			ctx->set_source_rgb(1.0, 1.0, 1.0);
			ctx->rectangle(0.0, 0.0, 1.0, 1.0);
			ctx->fill();
			ctx->restore();
				
			// seteo el patrón de guiones
			std::vector< double > guiones(1);
			guiones[0] = ESPACIADO_GUIONES;
			ctx->set_dash(guiones, 0);
				
			// línea
			ctx->move_to(0.0, 0.5);
			ctx->line_to(1.0, 0.5);
			ctx->stroke();	

			return true;			
		}
};

int main(int argc, char* argv[]) {
	Gtk::Main kit(argc, argv);	
	Dash d;
	Gtk::Window w;
	w.add(d);
	w.show_all();
	Gtk::Main::run(w);
	return 0;
}	
