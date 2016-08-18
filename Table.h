#ifndef _ETHING_TABLE_H_
#define _ETHING_TABLE_H_


/*!
 * \file Table.h
 * \brief Table's functions
 * \author Adrien Mezerette
 * \version 0.1
 */

#include "Id.h"



namespace EThing {
	
	/*! \namespace Table
     *
     *  This namespace handles Table resource
     */
	namespace Table {
		
		/*!
         *  Create a new Table resource.
         *
         *  \param name the name of the table to create
		 *  \param expireAfter the amount of time (in seconds) after which a records will be automatically removed. Set it to 0 to disable this feature.
		 *  \param maxLength the maximum of records allowed in this table. When this number is reached, the oldest records will be removed to insert the new ones (first in, first out). Set it to 0 to disable this feature.
		 *  \param statusCode an integer pointer receiving the HTTP request status code.
		 *  \return the id of the created file. This id is invalid if an error occurs.
         */
		Id create(const char * name, unsigned long expireAfter = 0, unsigned long maxLength = 0, int * statusCode = 0);
		
	
	}
	
	
}



#endif /*_ETHING_TABLE_H_*/
