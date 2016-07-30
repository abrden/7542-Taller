class Numero {
	private:
		long int _numero;
	public:
		Numero();
		Numero(const Numero& otro);
		Numero operator+(const Numero& otro);
		Numero operator++();
		Numero& operator++(int);
		bool operator>(const Numero& otro);
		Numero operator<<(int amount);
		Numero operator=(const Numero& otro);
		friend std::ostream& operator<<(std::ostream& o, const Numero& n);
};
