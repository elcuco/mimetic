#include <iostream>
#include <iomanip>
#include "cutee.h"
using namespace std;
using namespace cutee;

// static vars initialization
CuteeTest* TestList::list[MAX_TEST_COUNT];
int TestList::list_idx = 0;


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      smime_test_runtest_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	Runner r(argc, argv);
	r.run();
}
