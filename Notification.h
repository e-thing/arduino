#ifndef _ETHING_NOTIFICATION_H_
#define _ETHING_NOTIFICATION_H_

/*!
 * \file Notification.h
 * \brief Send nofification
 * \author Adrien Mezerette
 * \version 0.1
 */
 



namespace EThing {
	
	
	/*!
     *  Send a notification (email) to the user.
     *
     *  \param subject the subject of the email
	 *  \param message the message of the email
	 *  \param statusCode an integer pointer receiving the HTTP request status code.
     */
	bool notify(const char * subject, const char * message, int * statusCode = 0);
	
}



#endif /*_ETHING_NOTIFICATION_H_*/
