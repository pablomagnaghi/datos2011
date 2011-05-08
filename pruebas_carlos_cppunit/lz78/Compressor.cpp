#include <string>
#include "Compressor.hpp"
#include <cmath>

#include <sstream>
#include <iostream>

using namespace lz78;
using namespace std;

Compressor::Compressor(size_t mb):bits(9){
	if (mb < 9) {
		throw 1;
	}
	max_bits=mb;
	//pow(2,max_bits)
	code_table = new vector<string>();
	add("eof");
	cache = 0;
}

/**
 * @todo handle empty streams
 */
void Compressor::compress(util::IFileReader* reader, util::IFileWriter* writer){
	char c = reader->read();
	string context;
	context = c;
	while (!reader->eof()) {
		c = reader->read();
		if ( find (context + c)) {
			context += c;
		} else {
			writer->write(encode(context));
			if (true) { // it checks that code_table not full, use exception instead
				add (context + c );
				context=c;
			}
		}
	}
	
	writer->write(encode(context));
	writer->write("'256");
}

std::string Compressor::compressedRead(util::IFileReader* reader) {
	string result;
	result = reader->read();
	if ("'" == result) {
		result += reader->read();
		result += reader->read();
		result += reader->read();
	}
	return result;
}

/**
 * @todo handle empty streams
 */
void Compressor::decompress(util::IFileReader* reader, util::IFileWriter* writer){
	string old_one;
	old_one = compressedRead(reader);
	writer->write(old_one);
	while (!reader->eof()) {
		string new_one = decode(compressedRead(reader));
		writer->write(new_one);
		string match = old_one + new_one.substr(0,1);
		if (!find(match)) {
			add(match);
		}
		old_one = new_one;
	}
}

bool Compressor::find(std::string match) {
	if (match.size() == 1) {
		return true;
	}
	
	//if (code_table->at(cache) == match) return true;
	
	cache = 256;
	vector<string>::iterator it = code_table->begin();
	for ( ; it != code_table->end(); it++) {
		if (match == *it) {
			return true;
		}
		cache++;
	}
	return false;
}

void Compressor::add(std::string match) {
	code_table->push_back(match);
}

std::string Compressor::decode(std::string match) {
	if ("'" != match.substr(0,1)) {
		last_match = match;
		return match;
	}
	if ("'256" == match) {
		return "";
	}
	stringstream ss;
	size_t pos ;
	ss << match.substr(1,3);
	ss >> pos;
	if (pos >= code_table->size() + 256) {
		last_match += last_match.substr(0,1);
		return last_match;
	}
	return code_table->at(pos - 256);
}

std::string Compressor::encode(std::string match) {
	if (!find(match)) throw 1;
	if (match.size() == 1) {
		return match;
	}
	stringstream ss;
	ss << "'" << cache ;
	return ss.str();
}