#ifndef OFFICE_HPP
#define OFFICE_HPP

#include "app.hpp"

class Office final: public App
{
    private:
        std::vector<std::string> extensions;

    public:
        Office();
        Office(const char *serialnum, const std::string& name,
               const std::string& os, Manufacturer *manf,
               const int price, const std::vector<std::string>& ext);
        Office(const Office& of);
        ~Office();

        const std::vector<std::string> get_exts() const {return extensions;}
        void set_exts(const std::vector<std::string>& extensions);
        void print(std::ostream& stream) const override;
};

#endif /* OFFICE_HPP */
