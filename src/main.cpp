#include <iostream>
#include "libnm/version.h"

int main( int argc, char* argv[] )
{
	std::cout << "SkeletonCpp program with version " << libnm::version::GitDescribe << "\n";
	return 0;
}
