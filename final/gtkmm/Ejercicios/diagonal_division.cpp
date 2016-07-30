/*
 * Escriba una rutina (para ambiente gráfico Windows o Linux) que dibuje
 * 2 lineas diagonales dividiendo la ventana en 3 franjas de igual ancho.
 *  
 * Mas o menos asi
 *  | /  /|
 *  |/  / |
 * 
 */


#include <gtkmm.h>

#define LINE_WIDTH 0.01

class DiagonalDivision : public Gtk::DrawingArea {
	private:
		bool on_draw(const Cairo::RefPtr< Cairo::Context >& ctx) {
			Gtk::Allocation alloc = get_allocation();
			const int height = alloc.get_height();
			const int width = alloc.get_width();
			ctx->scale(width, height);
			ctx->set_line_width(LINE_WIDTH);
			ctx->move_to((2/3.0), 0);
			ctx->line_to(0, (2/3.0));
			ctx->move_to(1, (1/3.0));
			ctx->line_to((1/3.0), 1);
			ctx->stroke();

			return true;			
		}
};

int main(int argc, char* argv[]) {
	Gtk::Main kit(argc, argv);	
	DiagonalDivision dd;
	Gtk::Window w;
	w.add(dd);
	w.show_all();
	Gtk::Main::run(w);
	return 0;
}	
