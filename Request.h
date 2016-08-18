#ifndef _ETHING_REQUEST_H_
#define _ETHING_REQUEST_H_


/*!
 * \file Request.h
 * \brief Request manager
 * \author Adrien Mezerette
 * \version 0.1
 */
 
 

#include <Client.h>


namespace EThing {
	
	/**
     * \enum Method
     *
     * HTTP request's method
     */
	enum Method {GET,POST,PUT,PATCH,DELETE};
	
	
	/*!
     *  \class Request
     *
     *  Use this class to send HTTP request to your eThing server
	 *
	 *  Example :
	 *  
	 *    // GET request
	 *    EThing::Request get_request(EThing::GET,"resources");
	 *    if(get_request.send()){
	 *      // print the status code
	 *      Serial.println(get_request.getStatusCode());
	 *      // read the response
	 *      while(get_request){
	 *        if(get_request.available())
	 *          Serial.write(get_request.read());
	 *        else
	 *          delay(5); // temporise
	 *      }
	 *      get_request.close();
	 *    }
	 *
	 *    // POST request
	 *    EThing::Request post_request(EThing::PUT,"file/F5hyt45");
	 *    if(post_request.connect()){
	 *      post_request.print("foobar");
	 *      post_request.println(45);
	 *      post_request.send();
	 *      // print the status code
	 *      Serial.println(post_request.getStatusCode());
	 *      // read the response
	 *      while(post_request){
	 *        if(post_request.available())
	 *          Serial.write(post_request.read());
	 *        else
	 *          delay(5); // temporise
	 *      }
	 *      post_request.close();
	 *    }
	 *
     */
	class Request : public Stream
	{
		
		public:
			
			/*!
             *  Instanciate a new Request.
             *
             *  \param method the method of the request
			 *  \param url the url. Relative to the root HTTP API path.
			 *  \param contentType the content-type for this request. Only available for POST, PUT or PATCH requests.
             */
			Request(Method method, const char * url = NULL, const char * contentType = NULL) :
				method_(method), url_(url), contentType_(contentType)
			{}
			
			/*!
             *  Set a new method to this request instance.
             *
             *  \param method the method of the request.
             */
			void setMethod(Method method) {
				method_ = method;
			}
			
			/*!
             *  Set a new URL to this request instance.
             *
             *  \param url the URL of the request.
             */
			void setUrl(const char * url) {
				url_ = url;
			}
			
			/*!
             *  Set a content-type header for this request. Only available for POST, PUT or PATCH requests.
			 *  Set to NULL to remove the content-type header.
             *
             *  \param contentType the content-type value (e.g. "application/json" ).
             */
			void setContentType(const char * contentType) {
				contentType_ = contentType;
			}
			
			/*!
             *  Connect to the server and send the headers.
             *
             *  \param query some data to append to the url, usually a query string.
             */
			bool connect(const char * query = NULL);
			
			/*!
             *  Append a character to the body. Only available for POST, PUT or PATCH requests.
             *
             *  \param character the character to send.
			 *  \return the number of bytes sent. 0 if an error occurs.
             */
			size_t write(uint8_t character);
			
			/*!
             *  Append a buffer to the body. Only available for POST, PUT or PATCH requests.
             *
             *  \param buf the buffer to send.
			 *  \param size the size of the buffer.
			 *  \return the number of bytes sent. 0 if an error occurs.
             */
			size_t write(const uint8_t *buf, size_t size);
			
			
			/*!
             *  Sends the request to the server. For POST, PUT or PATCH requests, you may pass some data to write in the request's body. For GET and DELETE request, the data will be appended to the url (if not already connected).
             *
             *  \param data the data to send along with the request.
             *  \param contentType the content-type value (e.g. "application/json" ).
			 */
			bool send(const char * data = NULL, const char * contentType = NULL);
			
			
			/*!
			 *  Get the number of bytes (characters) available for reading.
			 *
			 *  \return the number of bytes available.
			 */
			inline int available(){
				return client_->available();
			}
			/*!
			 *  reads one character from the answer.
			 *
			 *  \return the next byte (or character) of the answer (or -1 if no data is available).
			 */
			inline int read(){
				return client_->read();
			}
			/*!
			 *  reads a buffer from the answer.
			 *
			 *  \param buf the buffer where to store the incoming data.
			 *  \param size the size of the buffer.
			 *  \return the next byte (or character) of the answer (or -1 if no data is available).
			 */
			inline int read(uint8_t *buf, size_t size){
				return client_->read(buf, size);
			}
			/*!
			 *  Read a byte from the answer without advancing to the next one.
			 *
			 *  \return The next byte (or character) of the answer (or -1 if no data is available).
			 */
			inline int peek(){
				return client_->peek();
			}
			/*!
			 *  Reads all the available incoming data.
			 */
			inline void flush(){
				return client_->flush();
			}
			
			/*!
			 *  Boolean operator overloading.
			 *
			 *  \return true if the connexion is still open.
			 */
			inline operator bool(){
				return (bool)client_->connected();
			}
			
			
			/*!
			 *  Close the connexion.
			 */
			inline void close(){
				return client_->stop();
			}
			
			/*!
			 *  Return the status code of the last HTTP request.
			 *
			 *  \return the status code of the last HTTP request or 0 if no request was made.
			 */
			inline int getStatusCode() const {
				return statusCode_;
			};
			
		private:
			Client * client_;
			int statusCode_;
			const char * url_;
			const char * contentType_;
			Method method_;
	};
	
	
}



#endif /*_ETHING_REQUEST_H_*/
