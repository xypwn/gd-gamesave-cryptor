#pragma once

#include <fstream>
#include <zlib.h>
#include "Base64.hpp"

constexpr int CHUNK = 16384;
constexpr int WINDOW_BITS = 15; 

//Returns true if it was able to open the given file for reading
bool IsFile(const std::string& filename) {
	std::ifstream file(filename);
	const bool ret = file.good();
	file.close();
	return ret;
}

//Reads the contents of a given file into a string
std::string FileToStr(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.good())
		return "";
	std::string ret(std::istreambuf_iterator<char>(file), {});
	file.close();
	return ret;
}

//Writes the contents of a string to the given file
// returns false if it was unable to open the file
bool StrToFile(const std::string& filename, const std::string& data) {
	std::ofstream ofile(filename, std::ios::binary);
	if(!ofile.good())
		return false;
	ofile << data;
	ofile.close();
	return true;
}

//XORs each byte of a string reference
void Xor_Str(std::string& str, unsigned char key) {
	for (auto& c : str) {
		c = c^key;
	}
}

std::string GZipDecompress(const std::string& in) {
	//Initialize all members of strm to be 0
	z_stream strm = {};
	//Output buffer
  	char out[CHUNK];
	//Holds return value  
	std::string final_output;
	//Initialize GZip decompression; except on failure
	if(inflateInit2(&strm, WINDOW_BITS + 16) != Z_OK)
		throw(std::exception());
	//Tell zlib where to find the data we want to decompress
  	strm.avail_in = in.size();
  	strm.next_in = (Bytef*)in.data();

  	do {
        //Tell zlib where to store the decompressed output data
  	  	strm.avail_out = CHUNK;
  	  	strm.next_out = (Bytef*)out;
		//Decompress our input data; except on failure
  	  	switch (inflate(&strm, Z_FINISH)) {
  	  	  	case Z_NEED_DICT:
				  throw(std::exception());
  	  	  	case Z_DATA_ERROR:
				  throw(std::exception());
  	  	  	case Z_MEM_ERROR:
				  throw(std::exception());
  	  	}
		//How many bytes we received into our output buffer
  	  	unsigned int have = CHUNK - strm.avail_out;
		//Append received data to our return string
		final_output.append(reinterpret_cast<const char*>(out), have);
  	} while (strm.avail_out == 0);
	//Clean up and return
	inflateEnd(&strm);
	return final_output;
}

std::string GZipCompress(const std::string& in) {
	//Initialize all members of strm to be 0
    z_stream strm = {};
	//Output buffer
    unsigned char out[CHUNK];
	//Return value
	std::string final_output;
	//Initialize GZip compression; except on failure
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK)
		throw(std::exception());
	//Tell zlib where to find the data we want to compress
    strm.avail_in = in.size(); 
    strm.next_in = (Bytef*)in.data();

    do {
        //Tell zlib where to store the compressed output data
        strm.avail_out = CHUNK;
        strm.next_out = out;
		//Compress our input data; except on failure
        if(deflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
			throw(std::exception());
		//How many bytes we received into our output buffer
        unsigned int have = CHUNK - strm.avail_out;
		//Append received data to our return string
		final_output.append(reinterpret_cast<const char*>(out), have);
    } while (strm.avail_out == 0);
	//Clean up and return
    deflateEnd(&strm);
	return final_output;
}

/** @brief Decrypt GD save files or level data                   *
 * @param in_data encrypted input data                           *
 * @param xor11 whether or not the data should be xored with 11; *
 * select true for gamesave or false for level data              *
 * @retval decrypted data                                        **/
std::string Decrypt(const std::string& in_data, bool xor11) {
	//Retun value
	std::string ret = in_data;
	//Xor with value 11 if nescessary
	if(xor11)
		Xor_Str(ret, 11);
	//replace - with + and _ with /
	for (auto& i : ret) {
		if (i == '-')
			i = '+';
		if (i == '_')
			i = '/';
	}
	//Decrypt with Base64
	ret = base64::Decode(ret);
	//Decompress with GZip
	ret = GZipDecompress(ret);
	//Return
	return ret;
}

/** @brief Encrypt GD save files or level data                   *
 * @param in_data decrypted input data                           *
 * @param xor11 whether or not the data should be xored with 11; *
 * select true for gamesave or false for level data              * 
 * @retval encrypted data                                        **/
std::string Encrypt(const std::string& in_data, bool xor11) {
	//Calculate crc32 checksum and size
	const uLong crc = crc32(0L, Z_NULL, 0);
	const uint32_t crc32_sum = crc32(crc, (Bytef*)in_data.data(), in_data.size());
	const uint32_t dataSize = in_data.size();
	//Compress with GZip
	std::string ret = GZipCompress(in_data);
	//Remove first 2 and last 4 chars of string
	ret = ret.substr(2, ret.size() - 4 - 2);
	//Add header, checksum and size
	char header[10] = {'\x1f', '\x8b', '\x08', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x0b'};
	ret = std::string(header, sizeof(header)) +
			ret +
			std::string((const char*)&crc32_sum, sizeof(crc32_sum)) + //Use raw binary data
			std::string((const char*)&dataSize, sizeof(dataSize)); //Use raw binary data
	//Encrypt with Base64
	ret = base64::Encode(ret);
	//replace + with - and / with _
	for (auto& i : ret) {
		if (i == '+')
			i = '-';
		if (i == '/')
			i = '_';
	}
	//Xor with value 11 if nescessary
	if(xor11)
		Xor_Str(ret, 11);
	//Return
	return ret;
}