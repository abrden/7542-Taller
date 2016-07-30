/*
 * Escriba una rutina (para ambiente gráfico Windows o Linux) que pinte 
 * un rectángulo rojo de la 1/3 parte del alto y ancho de la ventana.
 */


#include <gtkmm.h>

class Rectangle : public Gtk::DrawingArea {
	private:
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& ctx) {
			Gtk::Allocation alloc = get_allocation();
			const int height = alloc.get_height();
			const int width = alloc.get_width();
			
			ctx->scale(width, height);
					
			ctx->rectangle(1.0/3.0, 1.0/3.0, 1.0/3.0, 1.0/3.0);
			ctx->set_source_rgb(1.0, 0.0, 0.0);		
			ctx->fill();	

			return true;			
		}
};

int main(int argc, char* argv[]) {
	Gtk::Main kit(argc, argv);	
	Rectangle r;
	Gtk::Window w;
	w.add(r);
	w.show_all();
	Gtk::Main::run(w);
	return 0;
}
