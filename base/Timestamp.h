#ifndef RABBIT_TIMESTAMP_H
#define RABBIT_TIMESTAMP_H

#include <sys/types.h>
#include <string>

namespace Rabbit{
namespace base{

class Timestamp
{
public:
    Timestamp(double microSeconds = 0.0);
    ~Timestamp();
    bool valid();
    int64_t microSecondsSinceEpoch();
    std::string toString() const;

    static Timestamp now();
    static Timestamp nowAfter(double seconds);
    static double nowMicroSeconds();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t _microSecondsSinceEpoch;
};
bool operator <(Timestamp l, Timestamp r);
bool operator ==(Timestamp l, Timestamp r);

}
}
#endif
