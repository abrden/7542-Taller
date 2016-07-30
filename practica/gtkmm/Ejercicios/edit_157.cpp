/*
 * 	Escriba el trozo de código necesario para cargar el texto ‘hola’
 *  en un edit cuyo ID es 157
 */

#include <gtkmm.h>

void hola_id_157(Gtk::VBox* vbox) {
	std::vector<Gtk::Widget*> widgets = vbox->get_children();
	for (unsigned int i = 0; i < widgets.size(); ++i) {
		Gtk::Widget* widget = widgets[i];
		Glib::ustring name = widget->get_name();
		if (!name.compare("157")) {
			Gtk::Entry* entry = (Gtk::Entry*) widget;
			entry->set_text("hola");
		}
	}
}

class Boton : public Gtk::Button {
	public:
		Boton(Gtk::VBox* vbox) : Gtk::Button("SALUDAME") {
			this->vbox = vbox;	
		}
	private:
		Gtk::VBox* vbox;
		void on_clicked() { 
			hola_id_157(vbox);
		}
};

int main(int argc, char* argv[]) {
	Gtk::Main kit(argc, argv);
	Gtk::VBox vbox;
	Gtk::Entry entry;
	entry.set_name("157");
	vbox.add(entry);
	Boton boton(&vbox);
	vbox.add(boton);
	Gtk::Window w;
	w.add(vbox);
	w.show_all();
	Gtk::Main::run(w);
	return 0;
}	
