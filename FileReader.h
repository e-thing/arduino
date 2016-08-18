#ifndef _ETHING_FILEREADER_H_
#define _ETHING_FILEREADER_H_


/*!
 * \file FileReader.h
 * \brief Read data from a file
 * \author Adrien Mezerette
 * \version 0.1
 */
 
 
#include "Request.h"
#include "Id.h"


namespace EThing {
	
	/*!
     *  \class FileReader
     *
     *  Use this class to read a File
     */
	class FileReader : public Request
	{
		
		public:
			
			FileReader() : Request(GET,"file/") {}
			
			
			/*!
			 *  Open an existing file for reading.
			 *
			 *  \param id id of the file to open
			 *  \return true on success.
			 */
			bool open(const Id& id);
			
			
			/*!
			 *  Get the number of bytes (characters) available for reading.
			 *
			 *  \return the number of bytes available.
			 */
			using Request::available;
			/*!
			 *  reads one character from the file.
			 *
			 *  \return the next byte (or character) of the file (or -1 if no data is available).
			 */
			using Request::read;
			/*!
			 *  Read a byte from the file without advancing to the next one.
			 *
			 *  \return The next byte (or character) of the file (or -1 if no data is available).
			 */
			using Request::peek;
			/*!
			 *  Reads all the available incoming data.
			 */
			using Request::flush;
			/*!
			 *  Close the file by closing the HTTP request.
			 */
			using Request::close;
			/*!
			 *  Return the status code of the last HTTP request.
			 *
			 *  \return the status code of the last HTTP request or 0 if no request was made.
			 */
			using Request::getStatusCode;
			
		protected: // make the stream read-only
			using Request::setUrl;
			using Request::setContentType;
			using Request::connect;
			using Request::write;
			using Request::send;
			
	
	};
	
	
}



#endif /*_ETHING_FILEREADER_H_*/
