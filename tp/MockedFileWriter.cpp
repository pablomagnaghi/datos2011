#include "MockedFileWriter.h"


using namespace util;
using namespace std;

MockedFileWriter::MockedFileWriter():IFileWriter() {

}

MockedFileWriter::~MockedFileWriter(){

}

void MockedFileWriter::write(char c){
	data << c;
}

std::string MockedFileWriter::get(){
	return data.str();
}