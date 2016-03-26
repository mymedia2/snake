#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

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

#endif	// UTILS_H
