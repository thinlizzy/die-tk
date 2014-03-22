/** explicitly converts a dimension type to another, converting the dimension values to DimType
* @param d	the source dimension
* @note useful for converting int dimensions to size_t dimensions
*/
template<typename T>
template<typename U>
void basic_dimension<T>::assign(basic_dimension<U> const & d)
{
	width = DimType(d.width);
	height = DimType(d.height);
}

/** @return true if it is an empty dimension */
template<typename T>
bool basic_dimension<T>::empty() const
{
	return width == 0 && height == 0;
}

/** @return true if all dimensions are positive */
template<typename T>
bool basic_dimension<T>::isRectangle() const
{
	return width > 0 && height > 0;
}

/** @return dimension area */
template<typename T>
size_t basic_dimension<T>::area() const
{
	return size_t(width)*size_t(height);
}

template<typename T>
basic_dimension<T> basic_dimension<T>::setWidth(DimType width) const
{
    return basic_dimension<T>(width, this->height);
}

template<typename T>
basic_dimension<T> basic_dimension<T>::setHeight(DimType height) const
{
    return basic_dimension<T>(this->width, height);
}

template<typename T>
basic_dimension<T> basic_dimension<T>::addWidth(DimType width) const
{
    return basic_dimension<T>(this->width + width, this->height);
}

template<typename T>
basic_dimension<T> basic_dimension<T>::addHeight(DimType height) const
{
    return basic_dimension<T>(this->width, this->height + height);
}


/** vector sum (a,b) += (c,d) == (a+c,b+d)
* @param d	the dimension object to be added
* @return reference to *this
*/
template<typename T>
basic_dimension<T> & basic_dimension<T>::operator+=(basic_dimension<T> const & d)
{
	width += d.width;
	height += d.height;
	return *this;
}

/** vector minus (a,b) -= (c,d) == (a-c,b-d)
* @param d	the dimension object to be subtracted
* @return reference to *this
*/
template<typename T>
basic_dimension<T> & basic_dimension<T>::operator-=(basic_dimension<T> const & d)
{
	width -= d.width;
	height -= d.height;
	return *this;
}

/** performs a sort of dot product (a,b) *= (c,d) == (a*c,b*d)
* @param d	the dimension object to be multiplied
* @return reference to *this
*/
template<typename T>
basic_dimension<T> & basic_dimension<T>::operator*=(basic_dimension<T> const & d)
{
	width *= d.width;
	height *= d.height;
	return *this;
}

/** performs a sort of dot product (a,b) /= (c,d) == (a/c,b/d)
* @param d	the dimension object to be divided
* @return reference to *this
*/
template<typename T>
basic_dimension<T> & basic_dimension<T>::operator/=(basic_dimension<T> const & d)
{
	width /= d.width;
	height /= d.height;
	return *this;
}

/** scales the dimensions by a factor
* @param f	the scale factor
* @return reference to *this
*/
template<typename T>
basic_dimension<T> & basic_dimension<T>::operator*=(DimType f)
{
	width *= f;
	height *= f;
	return *this;
}

/** divides the dimensions by a factor
* @param f	the shrinking factor
* @return reference to *this
*/
template<typename T>
basic_dimension<T> & basic_dimension<T>::operator/=(DimType f)
{
	width /= f;
	height /= f;
	return *this;
}


/** @return true if dimension values are equal */
template<typename T>
bool operator==(basic_dimension<T> const & d1, basic_dimension<T> const & d2)
{
	return d1.width == d2.width && d1.height == d2.height;
}

/** @return true if dimension values are different */
template<typename T>
bool operator!=(basic_dimension<T> const & d1, basic_dimension<T> const & d2)
{
	return ! operator==(d1,d2);
}

/** @return vector sum of the two operators: (a,b) + (c,d) == (a+c,b+d) */
template<typename T>
basic_dimension<T> const operator+(basic_dimension<T> const & d1, basic_dimension<T> const & d2)
{
	return basic_dimension<T>(d1)+=d2;
}

/** @return vector difference of the two operators: (a,b) - (c,d) == (a-c,b-d) */
template<typename T>
basic_dimension<T> const operator-(basic_dimension<T> const & d1, basic_dimension<T> const & d2)
{
	return basic_dimension<T>(d1)-=d2;
}

/** @return vector product of the two operators: (a,b) * (c,d) == (a*c,b*d) */
template<typename T>
basic_dimension<T> const operator*(basic_dimension<T> const & d1, basic_dimension<T> const & d2)
{
	return basic_dimension<T>(d1)*=d2;
}

/** @return vector product of the two operators: (a,b) / (c,d) == (a/c,b/d) */
template<typename T>
basic_dimension<T> const operator/(basic_dimension<T> const & d1, basic_dimension<T> const & d2)
{
	return basic_dimension<T>(d1)/=d2;
}

template<typename T>
basic_dimension<T> const operator*(basic_dimension<T> const & d1, typename basic_dimension<T>::DimType f)
{
	return basic_dimension<T>(d1)*=f;
}

template<typename T>
basic_dimension<T> const operator*(typename basic_dimension<T>::DimType f, basic_dimension<T> const & d1)
{
	return basic_dimension<T>(d1)*=f;
}


/** @return dimension scaled: (a,b) * s == (a*s,b*s)
*/
template<typename T, typename U>
constexpr basic_dimension<T> const operator*(basic_dimension<T> const & d1, U f)
{
	return basic_dimension<T>(d1.width*f,d1.height*f);
}

/** @return dimension scaled: s * (a,b) == (a*s,b*s)
*/
template<typename T, typename U>
constexpr basic_dimension<T> const operator*(U f, basic_dimension<T> const & d1)
{
	return d1 * f;
}

/** @return dimension scaled: (a,b) / f == (a/f,b/f)
*/
template<typename T>
basic_dimension<T> const operator/(basic_dimension<T> const & d1, typename basic_dimension<T>::DimType f)
{
	return basic_dimension<T>(d1)/=f;
}

/** @return dimension scaled: (a,b) / f == (a/f,b/f)
* @note parameter f defines the type of resulting basic_dimension object
*/
template<typename T, typename U>
basic_dimension<U> const operator/(basic_dimension<T> const & d1, typename basic_dimension<U>::DimType f)
{
	return basic_dimension<U>(d1)/=f;
}


