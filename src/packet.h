/*
 * packet.h
 *
 *  Created on: Sep 2, 2013
 *      Author: lucky
 */
#include "cstring.h"
#ifndef PACKET_H_
#define PACKET_H_

class packet{
public:
//For now
	char* m_chAttribute; 
	//Take data of that file split
	char* m_chData;
	//Adds m_chAttribute + m_chData, this is what we send
	char* m_chPacket;
	


//Possible attributes
	int m_iPacketSize;
	int m_iNumberOfSplit;
	char* m_MD5;
	// What's Checksum? Maybe we can use that instead of MD5. I've always heard about it but haven't really checked it out
	
	
/* Still need to find out how packets will look like, which ones will be a request
 * or a file packet, how the client and server talk to each other.
 * 
 * For now this is just a skeleton
 */
	packet(char* m_chAttribute, char* chData){
		m_chData = chData;
		strcat(m_chPacket, m_chAttribute);
		strcat(m_chPacket, m_chData);
	};
	char* getPacket();
	~packet();

};



#endif /* PACKET_H_ */
