#ifndef _ETHING_DATASET_H_
#define _ETHING_DATASET_H_


#include <stdint.h>
#include <Print.h>
#include <Stream.h>

class Object
{
	
public:
	
	enum type_t { INVALID=0, STRING, INT, FLOAT, BOOL };
	
	Object();
	~Object();
	
	Object(const Object &obj); // copy constructor
	
	Object & operator = (const Object &obj);
	
	
	bool add(const char * key, const char * value, uint8_t length);
	bool add(const char * key, const char * value);
	bool add(const char * key, float value);
	bool add(const char * key, bool value);
	bool add(const char * key, long value);
	inline bool add(const char * key, int value) {
		return add(key, (long)value);
	}
	
	const char* getString(const char * key, char * buf, size_t buflen) const;
	bool getBool(const char * key) const;
	long getInt(const char * key) const;
	float getFloat(const char * key) const;
	
	inline int length() const{
		return length_;
	}
	
	void clear();
	
	
	/* iterative way */
	inline bool next() {
		context_ = next(context_);
		return bufPtr_ > context_;
	}
	
	inline void rewind() {
		context_ = buf_;
	}
	
	inline const char * getKey() const {
		return (char *)context_;
	}
	type_t getType() const;
	const void* getRawData() const;
	inline bool getDataAsBool() const {
		return getDataAsBool(context_);
	}
	inline long getDataAsInt() const {
		return getDataAsInt(context_);
	}
	inline float getDataAsFloat() const {
		return getDataAsFloat(context_);
	}
	inline char * getDataAsString(char * buf, size_t buflen) const {
		return getDataAsString(context_, buf, buflen);
	}
	
	
	
	
	static bool parse(Stream& stream, Object * object);
	void serialize(Print& printer) const;
	
private:

	uint8_t * context_;
	uint8_t * buf_;
	int capacity_;
	int length_;
	uint8_t * bufPtr_;
	
	struct info_t {
		char * key;
		type_t type;
		void * data;
	};
	
	info_t get(uint8_t * pt) const;
	uint8_t * next(uint8_t * pt) const;
	
	bool getDataAsBool(uint8_t * pt) const;
	long getDataAsInt(uint8_t * pt) const;
	float getDataAsFloat(uint8_t * pt) const;
	char * getDataAsString(uint8_t * pt, char * buf, size_t buflen) const;
	
	uint8_t * findKey(const char * key) const;
	
	bool add(const char * key, uint8_t keyLength, type_t type, const void * value, uint8_t valueLength = 0);
	
	bool reserve(unsigned int size);
};



#endif /*_ETHING_DATASET_H_*/
