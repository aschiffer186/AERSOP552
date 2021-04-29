#include "exception/exceptions.hh"

namespace final_project
{
    namespace exceptions 
    {
    file_not_found_exception::file_not_found_exception(const std::string& message) 
        : _M_message(message)
    {

    }

    const char* file_not_found_exception::what() const throw()
    {
        return _M_message.c_str();
    }

    invalid_regex_exception::invalid_regex_exception(const std::string& message)
        : _M_message(message)
    {

    }

    const char* invalid_regex_exception::what() const throw() 
    {
        return _M_message.c_str();
    }
    }
} // namespace final_project::exceptions
