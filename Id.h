#ifndef _ETHING_ID_H_
#define _ETHING_ID_H_


#include <Printable.h>
#include <Print.h>
#include "config.h"

 
namespace EThing {

	
	/*! \class Id
     *
     *  This class store an id.
     */
	class Id : public Printable
	{
	
		public:
			/*!
             *  Default empty constructor.
             */
			Id();
			/*!
             *  Construct an Id object from a string representing the id.
             *
             *  \param id 7 character hexadecimal string
             */
			Id(const char * id);
			/*!
             *  Test if the id stored by this object is valid.
             *
			 *  \return true if the id is valid, else false.
             */
			inline bool isValid() const {
				return data_[0] != '\0';
			}
			/*!
             *  Get the null terminated id string.
             *
			 *  \return the pointer of the id string.
             */
			inline const char * data() const {
				return data_;
			}
			
			/*!
             *  Boolean operator overloading.
             *
			 *  \return true if the id is valid, else false.
             */
			inline operator bool() const {
				return isValid();
			}
			/*!
             *  Print the id into a printer
             *
             *  \param printer the printer
			 *  \return the number of bytes written. Always 7.
             */
			virtual inline size_t printTo(Print& printer) const {
				printer.print(data_);
				return ETHING_ID_LENGTH;
			}
			
		private:
			char data_[ETHING_ID_LENGTH+1]; // 7 + '\0'
			
	};
	

}


#endif /*_ETHING_ID_H_*/

