/*
 * file.h
 *
 *  Created on: Sep 26, 2013
 *      Author: jholdsclaw
 */

#ifndef FILE_H
#define FILE_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// initial packet to be sent to client with file info
struct SFileHeader {
	unsigned int	uiSize;				// total file size
	unsigned int	nChunks;			// number of chunks
	char			szFileName[256];	// file name (assuming 256 max file name size)
};

// inidividual packet chunks to be sent to client
struct SFileChunk {
	unsigned int	id;					// numerical id
	unsigned int	uiChunkSize;		// size of individual chunk
	char*			buffer;				// data buffer

};

class file{
private:
	unsigned int		m_uiMaxChunkSize;
	SFileHeader			m_fhHeader;
	unsigned int		m_uiActiveChunk;

	SFileChunk			chunk;

	ifstream*			pFileReader;

public:
	file();
	file(const char*);
	file(const char*, unsigned int);
	~file();

	bool processFile(const char*, unsigned int);
	bool getNextSplit(char*&);
	bool getSplit(unsigned int, char*);
	bool parseSplit(const char*, char*&, unsigned int&);

	unsigned int getMaxChunkSize();
	void setMaxChunkSize(unsigned int);
};



#endif /* FILE_H_ */
