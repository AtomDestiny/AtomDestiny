#pragma once

#include <exception>

namespace AtomDestiny
{
    class HashException final : public std::exception
    {
    public:
        explicit HashException(const char* message): m_message(message)
        {
        }

        virtual const char* what() const noexcept override
        {
            return m_message;
        }

    private:
        const char* m_message;
    };

} // namespace AtomDestiny