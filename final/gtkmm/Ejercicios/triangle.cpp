/*
 * Escriba una rutina (para ambiente gráfico Windows o Linux) que pinte 
 * un triángulo rojo con las carcterísticas que muestra la siguiente figura:
 * Es algo así, pintado de rojo la parte derecha:
 *  
 * | /|
 * |/_|
 * 
 */


#include <gtkmm.h>

class Triangle : public Gtk::DrawingArea {
	private:
		bool on_draw(const Cairo::RefPtr< Cairo::Context >& ctx) {
			Gtk::Allocation alloc = get_allocation();
			const int height = alloc.get_height();
			const int width = alloc.get_width();
			ctx->scale(width, height);
				
			ctx->move_to(1, 0);
			ctx->line_to(1, 1);
			ctx->line_to(0, 1);
			ctx->close_path();
			ctx->set_source_rgb(1.0, 0.0, 0.0);
			ctx->fill();
			ctx->stroke();

			return true;			
		}
};

int main(int argc, char* argv[]) {
	Gtk::Main kit(argc, argv);	
	Triangle t;
	Gtk::Window v;
	v.add(t);
	v.show_all();
	Gtk::Main::run(v);
	return 0;
}	
