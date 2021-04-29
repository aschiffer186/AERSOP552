#ifndef EXCPETIONS_H
#define EXCEPTIONS_H 1

#include <exception>
#include <string>

namespace final_project
{
    namespace exceptions 
    {
    //Thrown when the compiler cannot find the specified file.
    struct file_not_found_exception : public std::exception
    {
        public:
            file_not_found_exception(const std::string& message);

            const char* what() const throw();

            ~file_not_found_exception() throw()
            {
                
            }
        private:
            std::string _M_message;
    };

    //Throw when trying to parse an invalid regular expression.
    struct invalid_regex_exception : public std::exception 
    {
        public: 
            invalid_regex_exception(const std::string& message);

            const char* what() const throw();

            ~invalid_regex_exception() throw()
            {

            }
        private:
            std::string _M_message;
    };
    }
} // namespace final_project::exceptions


#endif //EXCEPTIONS_H