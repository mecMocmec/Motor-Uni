#include <cstddef>
#include <new>

template<typename T>
class vector {
	T* beginPtr = nullptr;
	T* endPtr = nullptr;
	T* capacityPtr = nullptr;

	
	void Realloc(std::size_t newCapacity) {

		T* tmp = new T[newCapacity]; //fa un de nou per a copiar-hi l'altre
		
		for (std::size_t i = 0; i < Size(); ++i) { //copiar els elements vells a el vector nou
			tmp[i] = beginPtr[i];
		}
		beginPtr = tmp; //assignar el nou a beginPtr
		delete[] beginPtr; //esborrar el vector vell
		
	}
	
	/*		//metode mes eficient de la funcio de dalt. 
	void Realloc(std::size_t newCapacity) {

		T* tmp = static_cast<T*>(::operator new(sizeof(T) * newCapacity));

		for (std::size_t i = 0; i < Size(); ++i) { //copiar els elements vells a el vector nou
			
			new(&tmp[i]) T(beginPtr[i]);
		}
		beginPtr = tmp; //assignar el nou a beginPtr
		::operator delete[](tmp);
		
	}
	*/

public:
	std::size_t Size() const { //mida que tenim d'elements allocats
		return endPtr - beginPtr;
	}

	std::size_t Capacity() const { //
		return capacityPtr - beginPtr;
	}

	T& push_back(const T& data) {

		if (Size() >= Capacity()) { //mira si els elements que tinc cap al vector, sino es fa Realloc
			Realloc(1 + Capacity() * 2); //size que teniem mes el doble, li sumes 1 per si es 0		//es podria fer amb un if(), pero aixi millora la performance
		}

		*(endPtr++) = data; //si ++ va davant, l'incrementa i es retorna a si mateix	//si ++ va despres, fa una copia i ho incrementa (mantenint el valor vell (en algun lloc)
	}

	//void operator=(const vector<T>& other) {}			//copy constructor

	//new crida al constructor, malloc nomes canvia la memoria
};


#include <vector>

int main() {
	
	std::vector<int> a;
	a.emplace_back(static_cast<std::vector<int>&&>(a);



	return 0;
}