/*
 * file.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: jholdsclaw
 */

#include "file.h"
#include <cstring>
#include <string>
#ifndef SPLIT_SIZE
#define SPLIT_SIZE 512
#endif

file::file(void)
{
	m_uiMaxChunkSize = SPLIT_SIZE;

}

file::file(const char* _filename)
{
	int i = SPLIT_SIZE;
	processFile(_filename,SPLIT_SIZE);

}

file::file(const char* _filename, unsigned int _size)
{
	processFile(_filename, _size);

}

file::~file()
{
}

bool file::processFile(const char* _filename, unsigned int _size = SPLIT_SIZE)
{
	m_uiMaxChunkSize = _size;

	// clear out any ugly memory in szFileName
	memset(m_fhHeader.szFileName,0,sizeof(m_fhHeader.szFileName));
	strcpy(m_chHeaderBuffer,"00");
	strcat(m_chHeaderBuffer,".");
	// open file for reading	
	pFileReader = new ifstream(_filename, ios::binary);

	// check that file opened and exists
	if(pFileReader->is_open()){

		// seek to end for file size
		pFileReader->seekg(0, pFileReader->end);
		m_fhHeader.uiSize = pFileReader->tellg();


		// determine # of chunks needed
		m_fhHeader.nChunks = (pFileReader->tellg()/m_uiMaxChunkSize);


		// store filename for transmission
		memcpy(m_fhHeader.szFileName,_filename,strlen(_filename));

		// return pointer to start of file
		pFileReader->seekg(0, pFileReader->beg);

		// set active chunk to 0
		m_uiActiveChunk = 0;


	}

	return true;
}

// gets the next chunk of data and builds a "split" struct with the id, pure data chunk size, and data buffer
bool file::getNextSplit(char* &dest)
{
	static char* _buffer;
	if(m_uiActiveChunk > 0)
	{
		delete chunk.buffer;
		delete _buffer;
		chunk.buffer = NULL;
		_buffer = NULL;
	}

	if(m_uiActiveChunk <= m_fhHeader.nChunks)
	{
		//memcpy(&chunk,0,sizeof(chunk));

		chunk.id = m_uiActiveChunk;

		// determine if this split will be max chunk size or remainder for last packet
		chunk.uiChunkSize = (m_uiMaxChunkSize>=(pFileReader->end - pFileReader->tellg())?m_uiMaxChunkSize:(pFileReader->end - pFileReader->tellg()));
	
		chunk.buffer = new char[chunk.uiChunkSize];
		pFileReader->read(chunk.buffer, chunk.uiChunkSize);

		// copy all of the elements into a new memory chunk
		unsigned int packet_size = (sizeof(chunk.id) + sizeof(chunk.uiChunkSize) + chunk.uiChunkSize);

		_buffer = new char[packet_size];
		memcpy(_buffer,&chunk.id,sizeof(chunk.id));
		memcpy(&_buffer[sizeof(chunk.id)],&chunk.uiChunkSize,sizeof(chunk.uiChunkSize));
		memcpy(&_buffer[sizeof(chunk.id)+sizeof(chunk.uiChunkSize)],chunk.buffer,chunk.uiChunkSize);

		dest = _buffer;

		m_uiActiveChunk++;
		return true;
	}
	return false;
}

// returns the pure data buffer, based on the chunk size of this split
bool file::parseSplit(const char* source, char* &dest, unsigned int &size)
{
	static char* _buffer;

	if(_buffer)
		delete _buffer;

	unsigned int _id,_chunksize;

	memcpy(&_id,source,sizeof(_id));
	memcpy(&_chunksize,&source[sizeof(_id)],sizeof(_id));

	size = _chunksize;

	_buffer = new char[_chunksize];
	memcpy(_buffer,&source[sizeof(_id)+sizeof(_chunksize)],_chunksize);

	dest = _buffer;

	return true;
}

bool file::getHeader(char* &header){
	header = m_chHeaderBuffer;
	return true;
}
void file::setMaxChunkSize(unsigned int _size)
{
	m_uiMaxChunkSize = _size;
}



unsigned int file::getMaxChunkSize()
{
	return m_uiMaxChunkSize;
}
