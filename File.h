#ifndef _ETHING_FILE_H_
#define _ETHING_FILE_H_


/*!
 * \file File.h
 * \brief File's functions
 * \author Adrien Mezerette
 * \version 0.1
 */

#include "Id.h"



namespace EThing {
	
	/*! \namespace File
     *
     *  This namespace handles File resource
     */
	namespace File {
		
		/*!
         *  Create a new File resource.
         *
         *  \param name the name of the file to create
		 *  \param expireAfter the amount of time (in seconds) after which this resource will be removed
		 *  \param statusCode an integer pointer receiving the HTTP request status code
		 *  \return the id of the created file. This id is invalid if an error occurs.
         */
		Id create(const char * name, unsigned long expireAfter = 0, int * statusCode = 0);
		
	
	}
	
	
}



#endif /*_ETHING_FILE_H_*/
