/*
 * file.h
 *
 *  Created on: Sep 2, 2013
 *      Author: lucky
 */
#include "packet.h"
#include <fstream>
#ifndef FILE_H_
#define FILE_H_

class file{
public:
//Finished and assigned
	bool m_bInitialized;
	bool m_bWriting;
	int m_iFileSize;
	char* m_FilePath;
	std::fstream m_FileStream;
//Not finished
	int m_iNumberOfPackets;
	/* Usage: file(FSTREAM, Is it writing[true] or reading [false])
	*/
	file(char* FilePath, bool Writing){
		m_bInitialized = false;
		m_FilePath = FilePath;
		m_bWriting = Writing;
		if(!m_bWriting){
			m_FileStream(m_FilePath, ifstream::binary);
			if(m_FileStream.good() && m_FileStream.is_open()){
				m_FileStream.seekg(0, m_FileStream.beg);
				m_iFileSize = m_FileStream.tellg();
				m_bInitialized = true;
			}
		}else{
			
			//Maybe needs to check if it is already existing?
			m_FileStream(m_FilePath, ofstream::binary);
			if(m_FileStream.good() && m_FileStream.is_open()){
			m_bInitialized = true;
			}
		}
	};
	int getNumberOfPackets();
	~file(){ if(m_FileStream.is_open()) { m_FileStream.close(); } };
};


#endif /* FILE_H_ */
