Documentation for "matrix.h"
	Author: Peter Amorese

How to use:

	matrix<typename> mat(int rows, int cols)
		Specify data type (int, float, double) in the template, then specify matrix size
		in constructor


Public member functions:
	matrix(unsigned int r_, unsigned int c_);
		Constructor, specify matrix size 

	const unsigned int rows, cols;
		Public const variables accessible for matrix size (set in the constructor)

	void set(std::vector<std::vector<T>>& set_vec);
		Set the matrix values using an external "vector<vector<pair<typename>>>". Dimensions
		have to match those specified in constructor

	void fill(T elem);
		Fill all values in the matrix with "elem"

	void scale(T scalar);
		Multiply all elements in the matrix by "scalar". I did not create an operator 
		overload for scalar multiplication because it would be slower

	void add(T scalar);
		Add "scalar" to all elements in the matrix.

	void print() const;
		Print all of the elements in the matrix row by row.

	T& operator() (int i, int j);
		Setter indexing. Set a single element using row and column indexing (0-start)

	T operator() (int i, int j) const;
		Getter indexing. Get a single element using row and column indexing (0-start)

	matrix<T> operator* (const matrix<T>& arg) const;
		Matrix multiplication. Returns a matrix

	void operator= (const matrix<T>& arg);
		Move elements from "arg" to this matrix

	matrix<T> operator+ (const matrix<T>& arg) const;
		Add two matrices element-wise. Returns a matrix

	matrix<T> operator- (const matrix<T>& arg) const;
		Subtract two matrices element-wise. Returns a matrix
	
