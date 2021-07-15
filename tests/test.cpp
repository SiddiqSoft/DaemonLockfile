#include "gtest/gtest.h"
#include "../src/DaemonLockfile.hpp"


TEST(Helpers, Test_isLocked_true)
{
	siddiqsoft::DaemonLockfile dl("basic_tests");

	// First time, we should be the only ones owning this file!
	EXPECT_TRUE(dl.isLocked);

	// Next, we will attempt to simulate another attempt at obtaining this lock file..
	siddiqsoft::DaemonLockfile duplicate_dl("basic_tests");
	// We should succeed! The owning process is us!
	EXPECT_TRUE(duplicate_dl.isLocked);
}
