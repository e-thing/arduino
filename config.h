#ifndef _ETHING_CONFIG_H_
#define _ETHING_CONFIG_H_


/*!
 * \file Config.h
 * \brief eThing configuration
 * \author Adrien Mezerette
 * \version 0.1
 */
 
 


 /*!
 * The port number where the eThing server is listening.
 */
#define ETHING_PORT 80


 /*!
 * The path of the API. Must end with a slash '/'.
 */
#define ETHING_PATH  "/ething/api/"


 /*!
 * Timeout for the HTTP requests (in seconds).
 */
#define ETHING_HTTP_TIMEOUT 10000


 /*!
 * The boundary for multipart content type request.
 */
#define ETHING_MULTIPART_BOUNDARY "gc0p4Jq0M2Yt"


 /*!
 * The length of the ID of the resource.
 */
#define ETHING_ID_LENGTH 7


 /*!
 * Address offset to store ID in EEPROM memory.
 */
#define ETHING_EEPROM_ADDRESS_START 0


 /*!
 * The length of the temporary buffer used for URI encoding a query string.
 */
#define ETHING_QUERY_ENCODED_MAX_SIZE 64



#endif /*_ETHING_CONFIG_H_*/
