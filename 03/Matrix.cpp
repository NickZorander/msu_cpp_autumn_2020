#include <iostream>
#include <fstream>
#include <iomanip>

#define assert(condition){if(!(condition)){std::cout << "fail"<<std::endl;}else{std::cout << "success"<<std::endl;}}


using namespace std;

class Proxy 
{
	int* row;
	size_t rowLen;
public:
	Proxy(int* d, size_t l)
	{
		row = d;
		rowLen = l;
	}

	const int &operator[](size_t i) const
	{
		if (i >= rowLen)
			throw std::out_of_range("");
		return row[i];
	}

	int &operator[](size_t i)
	{
		if (i >= rowLen)
			throw std::out_of_range("");
		return row[i];
	}
};



class Matrix
{
	size_t numRows, numCols;
	int* data;

public:
	Matrix(size_t rows, size_t cols)
	{
		numRows = rows;
		numCols = cols;

		data = new int[rows*cols];

		for (size_t i = 0; i < numCols; ++i)
			for (size_t j = 0; j < numRows; ++j)
				data[i*numCols + j] =  i==j? 1:0;
	}

	~Matrix()
	{
		delete data;
	}

	size_t getRows() const
	{
		return numRows;
	}

	size_t getCols() const 
	{
		return numCols;
	}

	const Proxy &operator[] (size_t i) const
	{
		if (i>=numRows)
			throw std::out_of_range("");
		return Proxy(data + i * numCols, numCols);
	}

	Proxy &operator[] (size_t i)
	{
		return const_cast<Proxy&>( static_cast<const Matrix&>(*this)[i]);
	}

	void operator*=(int x)
	{
		for (size_t i = 0; i < numRows*numCols; ++i)
		{
			data[i] *= x;
		}
	}

	bool operator==(const Matrix& m)
	{
		if (m.getRows() != numRows || m.getCols() != numCols)
			throw std::out_of_range("");

		for (size_t i = 0; i < numCols; ++i)
		{
			for (size_t j = 0; j < numRows; ++j)
			{
				if (data[i*numCols + j] != m.data[i*numCols + j])
					return false;
			}			
		}

		return true;
	}

	bool operator!=(const Matrix& m)
	{
		if (m.getRows() != numRows || m.getCols() != numCols)
			throw std::out_of_range("");

		for (size_t i = 0; i < numCols; ++i)
		{
			for (size_t j = 0; j < numRows; ++j)
			{
				if (data[i*numCols + j] != m.data[i*numCols + j])
					return true;
			}
		}
		return false;
	}


	Matrix &operator= (const Matrix& m)
	{
		if (m.getRows() != numRows || m.getCols() != numCols)
			throw std::out_of_range("");

		for (size_t i = 0; i < numCols; ++i)
		{
			for (size_t j = 0; j < numRows; ++j)
			{
				data[i*numCols + j] = m.data[i*numCols + j];
			}
		}

		return *this;
	}

	Matrix operator+(const Matrix& m)
	{
		if (m.getRows() != numRows || m.getCols() != numCols)
			throw std::out_of_range("");

		Matrix res_m(numRows, numCols);
		//cout << res_m;

		for (size_t i = 0; i < numCols; ++i)
		{
			for (size_t j = 0; j < numRows; ++j)
			{
				res_m.data[i*numCols + j] = data[i*numCols + j] + m.data[i*numCols + j];
			}
		}

		//cout << res_m;
		return res_m;
	}
};

std::ostream& operator<< (std::ostream &out, const Matrix& m)
{
	for (size_t i = 0; i < m.getCols(); ++i)
	{
		for (size_t j = 0; j < m.getRows(); ++j)
		{
			out << setw(10) << m[i][j];
		}
		out << endl;
	}

	return out;
}




void TestMethod()
{
	Matrix m1(2, 2), m2(2, 2), m3(2, 2);

	m1[0][0] = 2;
	assert(m1[0][0] == 2);
	assert(m1.getCols() == 2);
	assert(m1.getRows() == 2);
	m1[0][0] = 1;
	
	m3 *= 2;
	assert(m3[0][0] == 2 && m3[1][1] == 2 && m3[0][1] == 0 && m3[1][0] == 0);

	m3 = m1 + m2;
	cout << m3;
	assert(m3[0][0] == 2 && m3[1][1] == 2 && m3[0][1] == 0 && m3[1][0] == 0);

	assert(m1 == m2);
	assert(!(m1 != m2));

	try
	{
		Matrix w1(2, 2), w2(3, 3);
		w1 + w2;
	}
	catch (const std::out_of_range&)
	{
		cout << "SUCCESS" << endl;
	}

	try
	{
		Matrix w1(2, 2), w2(3, 3);
		w1 != w2;
	}
	catch (const std::out_of_range&)
	{
		cout << "SUCCESS" << endl;
	}

	try
	{
		Matrix w1(2, 2), w2(3, 3);
		w1 == w2;
	}
	catch (const std::out_of_range&)
	{
		cout << "SUCCESS" << endl;
	}
}


int main()
{
	TestMethod();
}


