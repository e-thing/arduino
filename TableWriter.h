#ifndef _ETHING_TABLEWRITER_H_
#define _ETHING_TABLEWRITER_H_


/*!
 * \file TableWriter.h
 * \brief Store data in a table
 * \author Adrien Mezerette
 * \version 0.1
 */

#include "Request.h"
#include "Id.h"



namespace EThing {
	
	/*!
     *  \class TableWriter
     *
     *  Use this class to store data into a Table
     */
	class TableWriter : public Request
	{
		
		public:
			
			TableWriter() : Request(POST,"table/","application/json") {}
			
			/*!
			 *  Open an existing table for writing.
			 *
			 *  \param id id of the table to open
			 *  \return true on success.
			 */
			bool open(const Id& id);
			
			
			/*!
			 *  Store a string into a field.
			 *
			 *  \param field the field name.
			 *  \param data an array to send as a series of bytes 
			 *  \param size the length of the buffer
			 *  \return the number of byte written.
			 */
			inline bool store(const char * field, const char * data, size_t size){
				return write_(field,data,size,true);
			}
			/*!
			 *  Store a nul-terminated string into a field.
			 *
			 *  \param field the field name.
			 *  \param data a nul-terminated array to send as a series of bytes 
			 *  \return the number of byte written.
			 */
			inline bool store(const char * field, const char * data){
				return write_(field,data,strlen(data),true);
			}
			/*!
			 *  Store an integer into a field.
			 *
			 *  \param field the field name.
			 *  \param data the value
			 *  \return the number of byte written.
			 */
			inline bool store(const char * field, int data){
				return store(field,(long)data);
			};
			/*!
			 *  Store a long integer into a field.
			 *
			 *  \param field the field name.
			 *  \param data the value
			 *  \return the number of byte written.
			 */
			bool store(const char * field, long data);
			/*!
			 *  Store a unsigned integer into a field.
			 *
			 *  \param field the field name.
			 *  \param data the value
			 *  \return the number of byte written.
			 */
			inline bool store(const char * field, unsigned int data){
				return store(field,(unsigned long)data);
			};
			/*!
			 *  Store a unsigned long integer into a field.
			 *
			 *  \param field the field name.
			 *  \param data the value
			 *  \return the number of byte written.
			 */
			bool store(const char * field, unsigned long data);
			/*!
			 *  Store a double into a field.
			 *
			 *  \param field the field name.
			 *  \param data the value
			 *  \param precision the number of decimal to print
			 *  \return the number of byte written.
			 */
			bool store(const char * field, double data, unsigned char precision = 3);
			
			/*!
			 *  Close the table by closing the HTTP request and wait for the response.
			 *
			 *  \return true if no error occurs.
			 */
			bool close();
			
		private:
			int index_;
			bool write_(const char * field, const char * data, size_t size, bool quoted);
	
	};
	
	
}



#endif /*_ETHING_TABLEWRITER_H_*/
