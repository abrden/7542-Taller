class Digitos100 {
	private:
		long int numero;
	public:
		Digitos100();
		Digitos100(long int numero);
		Digitos100(const Digitos100& otro);
		bool operator<(const Digitos100& otro);
		bool operator==(const Digitos100& otro);
		Digitos100& operator=(const Digitos100& otro);
		operator int const ();
		friend std::ostream& operator<<(std::ostream& o, const Digitos100& d);
};

std::ostream& operator<<(std::ostream& o, const Digitos100& d) {
	o << d.numero;
	return o;
}
