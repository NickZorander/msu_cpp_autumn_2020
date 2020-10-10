#include <iostream>
#include <stdio.h>

class Allocator
{
	char *base = nullptr;		//адрес нашего блока в памяти
	size_t offset = 0;			//смещение до свободной области
	size_t mem_size = 0;		//размер выделенной памяти

	public:
		void makeAllocator(size_t maxSize)
		{
			base = new char [maxSize];
			mem_size = maxSize;
		}
		char* alloc(size_t size)
		{
			if (offset + size > mem_size)
				return nullptr;

			size_t tmp = offset;
			offset += size;

			return base + tmp;
		}
		void reset()
		{
			offset = 0;
		}
};

using namespace std;

// В арргументах командной строки передается:
// HeapSize						numAllocs						alloc[0].size  ... alloc[numAllocs-1].size
//    ^								^									^^^^^^^^^^^^^^^^^^^^^^
// Размер кучи	 Сколько будет обращений на выделение памяти	Конкретные размеры каждой из аллокаций
int main(int argc, char **argv)
{
	Allocator myTestAllocator;

	size_t generalHeapSize = atoi(argv[1]);			//сколько памяти выделим
	size_t numAllocs = atoi(argv[2]);				//сколько раз будем вызывать аллокации
	
	size_t *allocsArray = new size_t[numAllocs];	//массив хранящий размер каждой аллокации 
	for (size_t i= 0; i < numAllocs; ++i)
	{
		allocsArray[i] = atoi(argv[3 + i]);
	}

	myTestAllocator.makeAllocator(generalHeapSize);	

	for (size_t i = 0; i < numAllocs; i++)
	{
		char* test_ptr = myTestAllocator.alloc(allocsArray[i]); //аллоцируем нашу память
		if (test_ptr != nullptr) 
			for (size_t j = 0; j < allocsArray[i]; ++j)
			{
				test_ptr[j] = 0; //пытаемся достучаться до каждого байта
			}
		else
			cout << "NULL POINTER (Out of space)" <<endl <<"tryed to alloc:"<< allocsArray[i] << endl; //проверяем, что кейс с превышением размера работает корреткно
	}

	return 0;
}


