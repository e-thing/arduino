#ifndef _ETHING_H_
#define _ETHING_H_


/*!
 * \file EThing.h
 * \brief EThing Arduino SDK main file
 * \author Adrien Mezerette
 * \version 0.1
 */
 
 
 /*! \mainpage eThing Arduino SDK
 *
 * see <a href="../">Install EThing library</a>
 *
 */

#include "config.h"
#include "Id.h"

class Client;


/*! \namespace EThing
 * 
 * Namespace regrouping all the EThing classes and functions
 */
 
namespace EThing {

	
	
	/**
     * \enum Type
     *
     * Resource's type 
     */
	enum Type { FILE_RESOURCE, TABLE_RESOURCE, APP_RESOURCE, DEVICE_RESOURCE };
	
	
	/*!
     *  Initialize the EThing library. Must be executed before any other EThing function calls.
     *
	 *  \param apiHost the hostname or IP address where the eThing server is hosted. Must not contains the protocol. (ie: 192.168.1.056)
     *  \param apiKey the API key of the device
	 *  \param client a client object inherited of the Client abstract class.
     */
	void init(const char * apiHost, const char * apiKey, Client * client);
	
	
	/*!
     *  Find the first resource that match the query given in argument.
	 *  If not found, it returns an invalid Id object.
	 *  Be careful, if the request failed (ie: could not reach the host ...) it also returns an invalid Id, though the resource could exist !
	 *  That is why it is very important to check that the statusCode is equal to 200.
     *
	 *  \param query the query string for searching resources
	 *  \param statusCode an integer pointer receiving the HTTP request status code.
     */
	Id findOne(const char * query, int * statusCode = 0);
	
	

}

#include "utils.h"
#include "Request.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "TableWriter.h"
#include "File.h"
#include "Table.h"
#include "Notification.h"


#endif /*_ETHING_H_*/

