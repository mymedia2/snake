#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>

namespace utils {

/* Координаты. Общий шаблон класса для представления координат */
template <typename T>
struct Coordinates
{
	/* Типы для обобщённых алгоритмов. Просто так */
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	/* Инициализирует координаты значениями по умолчанию. Для встроенных
	 * арифметических типов нулём */
	inline Coordinates();
	/* Инициализирует координаты переданными значениями */
	inline Coordinates(T x, T y);
	/* Собственно сами координаты */
	T x;
	T y;
};

/* Координаты на игровом поле. Используется для задания положения объектов
 * на игровом поле. */
typedef Coordinates<std::size_t> Point;

/* Класс ложного инварианта времени выполнения */
class Assertion
	: public std::logic_error
	, public std::bad_exception
{
public:
	/* Создаёт исключение с информацией о провале */
	inline Assertion(std::string filename, std::size_t lineno, std::string expression);
};

/* Макрос, вызывающий служебную функцию проверки инварианта. Выражение
 * вычисляется только в случае если не объявлен макрос NDEBUG */
#ifndef NDEBUG
#define assert(expr) ( ::utils::_assert((expr), __FILE__, __LINE__, #expr) )
#else
#define assert(expr) ( static_cast<void>(false) )
#endif

/* Служебная функция. Не предназначена для самостоятельного вызова из
 * клиентского кода */
inline void _assert(bool, std::string, std::size_t, std::string);

}	// namespace utils

template <typename T>
inline utils::Coordinates<T>::Coordinates()
	: Coordinates(T(), T())
{
}

template <typename T>
inline utils::Coordinates<T>::Coordinates(T x, T y)
	: x(x)
	, y(y)
{
}

inline utils::Assertion::Assertion(std::string filename, std::size_t lineno, std::string expression)
	: std::logic_error("Assertion '" + expression + "' at " + filename + ":" + std::to_string(lineno) + " failed")
{
}

inline void utils::_assert(bool invariant, std::string filename, std::size_t lineno, std::string expression)
{
	if (!invariant) {
		throw Assertion(filename, lineno, expression);
	}
}

#endif	// UTILS_HPP
