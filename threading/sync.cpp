#include "sync.hpp"

namespace thread_sync
{

std::mutex timer::timers_mtx;
std::vector<timer *> timer::timers;

}
