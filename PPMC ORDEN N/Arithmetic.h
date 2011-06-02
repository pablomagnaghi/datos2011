#ifndef _ppmc_arithmetic_h_
#define _ppmc_arithmetic_h_

#include <vector>
#include "PPMC.h"
#include "Model.h"
#include "ContextSelector.h"
#include "algorithm"
#include "FileWriter.h"
#include "FileReader.h"

namespace ppmc {
class Arithmetic {
public:
	Arithmetic(){

	}
	Arithmetic(util::FileReader* r, util::FileWriter* w);
	u_int32_t getTop();
	u_int32_t getBottom();
	void setNewLimits(u_int32_t, u_int32_t);
	void print_in_bin(u_int32_t);
	void clean_buffer();
	// metodo solo para ver progreso debe eliminarse
	void show();
	virtual ~Arithmetic();
protected:
	std::vector<Model*> models;
	ContextSelector contextSelector;
	FrequencyTable frequencyTable;
	util::FileReader * reader;
	util::FileWriter * writer;
	u_int32_t top;
	u_int32_t bottom;
	u_int8_t bits_in_buffer;
private:
	u_int32_t buffer;
	u_int8_t underflow_counter;
	// Agrego el bit al buffer

	void addBitToBuffer(u_int8_t);

	// Funcion que se usa para mandar el buffer de bits al fileWriter

	void putBufferInFileWriter();
	void solve_underflow();
	void solve_overflow();

};
}
#endif //_ppmc_arithmetic_h_
