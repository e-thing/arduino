#ifndef _ETHING_FILEWRITER_H_
#define _ETHING_FILEWRITER_H_


/*!
 * \file FileWriter.h
 * \brief Write into a file
 * \author Adrien Mezerette
 * \version 0.1
 */

#include "Request.h"
#include "Id.h"


namespace EThing {
	
	/*!
     *  \class FileWriter
     *
     *  Use this class to write data into a File
     */
	class FileWriter : public Request
	{
		
		public:
			
			FileWriter() : Request(PUT,"file/") {}
			
			/*!
			 *  Open an existing file for writing.
			 *
			 *  \param id id of the file to open
			 *  \return true on success.
			 */
			bool open(const Id& id);
			/*!
			 *  Create a new file and open it for writing.
			 *
			 *  \param name the name of the file
			 *  \param expireAfter the amount of time (in seconds) after which this resource will be removed
			 *  \return true on success.
			 */
			bool createAndOpen(const char * name, unsigned long expireAfter = 0);
			/*!
			 *  Close the file by closing the HTTP request and wait for the response.
			 *
			 *  \return true if no error occurs.
			 */
			bool close();
			
	
	};
	
	
}



#endif /*_ETHING_FILEWRITER_H_*/
