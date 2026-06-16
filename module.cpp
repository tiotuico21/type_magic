#include <type_traits>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <cxxabi.h>
#include <fstream>
#include <sstream>
#include "preamble.h"
#include <unistd.h>
#include <fcntl.h>  
#include "sanity_check.cpp"

#include "config.h"

#include "container/mod.h"
#include "context/mod.h"
#include "core/mod.h"

#include "postamble.h"

// Traits
struct isA
{
};
struct isB
{
};

struct isPrimeFinder
{
};

struct isLogger
{
};

/*
template <typename CONTEXT>
struct A
{
    int x;
    double &get_y()
    {
        return via<isB>(this).y;
    }
};

using aModule = context::SimpleModule<
    Meta<A>,
    context::RequirementSet<>,
    context::ImplementationSet<isA>>;
*/

template <typename CONTEXT>
struct PrimeFinder
{

    std::vector<int> findPrimes(int min, int max)
    {
        std::vector<int> primes;
        for (int i = min; i < max; i++)
        {
            bool is_prime = true;
            for (int j = 2; j < i; j++)
            {
                if (i % j == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
            {
                primes.push_back(i);
            }
        }
        return primes;
    }
};

using primeFinderModule = context::SimpleModule<
    Meta<PrimeFinder>,
    context::RequirementSet<isLogger>,
    context::ImplementationSet<isPrimeFinder>>;

template <typename CONTEXT>
struct Logger
{
    int returnFile;
    Logger(std::string filePath) 
    {
        returnFile = open(filePath.c_str(), O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
        if (returnFile == -1) {
            std::cout << "could not open file: " << filePath << std::endl;
        }
    }
    Logger(Logger &&other) : returnFile(std::move(other.returnFile)) {}
    template <typename T>
    Logger &operator<<(T message)
    {
        std::stringstream ss;
        ss << message;
        toLog(ss.str());
        return (*this);
    }
    void toLog(std::string message)
    {

        int bytes = write(returnFile, message.c_str(), message.size());
        if (bytes < 0){
            std::cout << "write error" << '\n';
            close(returnFile);
            return;
        }
        close(returnFile);
    }
};

using LoggerModule = context::SimpleModule<
    Meta<Logger>,
    context::RequirementSet<>,
    context::ImplementationSet<isLogger>>;
/*
template <typename CONTEXT>
struct B
{
    double y;
    int &get_x()
    {
        return via<isA>(this).x;
    }
};

using bModule = context::SimpleModule<
    Meta<B>,
    context::RequirementSet<>,
    context::ImplementationSet<isB>>;

typedef context::ModuleBundle<
    aModule,
    bModule>
    rootModule;
*/

typedef context::ModuleBundle<
    primeFinderModule,
    LoggerModule>
    practiceRootModule; // is this log?????

template <typename CTX>
void run()
{

    if constexpr (CTX::Info::SATISFIED)
    {

        CTX ctx(
            As<isPrimeFinder, CTX>{},
            As<isLogger, CTX>{"./log.txt"});

        std::vector<int> primes = as<isPrimeFinder>(ctx).findPrimes(1, 100);
        std::stringstream stringPrimes;
        for (int prime : primes)
        {
            stringPrimes << prime << " ";
        }
        std::string returnValue = stringPrimes.str();
        as<isLogger>(ctx).toLog(returnValue); 
      

    }
    else
    {
        CTX ctx;
        std::cout << as<context::ContextInfo>(ctx).error_string();
    }
}

int main()
{

    typedef typename context::CreateContextType<
        practiceRootModule,
        container::TypeSet<isPrimeFinder>,
        context::EagerSolve>::type Ctx;

    run<Ctx>();

    return 0;
}
