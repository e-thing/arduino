#ifndef _ETHING_UTILS_H_
#define _ETHING_UTILS_H_


#include <Client.h>
#include <Request.h>
#include <Id.h>

namespace EThing {
	
	
	/*!
	 *  Returns a null terminated string in which all non-alphanumeric characters except -_. have been replaced with 
	 *  a percent (%) sign followed by two hex digits and spaces encoded as plus (+) signs.
	 *
	 *  \param string the string to be encoded
	 *  \param buf the buffer where to store the encoded string
	 *  \param bufSize the size of the buffer
	 *  \return the buffer pointer.
	 */
	char * urlencode(const char * string, char * buf, size_t bufSize);
	
	
	Id getIdFromEeprom(int address = 0);
	
	void storeIdInEeprom(const Id& id, int address = 0);
	
	
}



#endif /*_ETHING_UTILS_H_*/
