/*
 * Escriba una rutina gráfica para Windows o Linux que dibuje lo que 
 * muestra la siguiente figura: 
 * 
 * ...
 * 
 * https://7542.fi.uba.ar/examenes/pregunta/imagen/show/17 
 * 
 */


#include <gtkmm.h>

#define LINE_WIDTH 0.01

class Figure : public Gtk::DrawingArea {
	private:
		bool on_draw(const Cairo::RefPtr< Cairo::Context >& ctx) {
			Gtk::Allocation alloc = get_allocation();
			const int height = alloc.get_height();
			const int width = alloc.get_width();
			ctx->scale(width, height);
			ctx->set_line_width(LINE_WIDTH);
			
			// O
			ctx->arc((1/2.0), (3/8.0), (1/8.0), 0, 2 * M_PI);
			ctx->save();
			ctx->set_source_rgb(0.5, 0.5, 0.5);
			ctx->fill_preserve();
			ctx->restore();
			ctx->stroke();
			
			// Rectangle
			ctx->rectangle((1/6.0), (1/2.0), (4/6.0), (2/6.0));
			ctx->save();
			ctx->set_source_rgb(0.5, 0.5, 0.5);
			ctx->fill_preserve();
			ctx->restore();
			ctx->stroke();

			return true;			
		}
};

int main(int argc, char* argv[]) {
	Gtk::Main kit(argc, argv);	
	Figure f;
	Gtk::Window w;
	w.add(f);
	w.show_all();
	Gtk::Main::run(w);
	return 0;
}	
