#include "sqlite-error.h"
#include <string.h>

SQLite::Error::Error() :
	msg(NULL)
{
}

SQLite::Error::Error(const SQLite::Error & e) :
	msg(e.msg ? strdup(e.msg) : NULL)
{
}

SQLite::Error::Error(const char * e) :
	msg(e ? strdup(e) : NULL)
{
}

SQLite::Error::~Error() throw()
{
	free(msg);
}

const char *
SQLite::Error::what() const throw()
{
	return msg ? msg : "No message";
}

