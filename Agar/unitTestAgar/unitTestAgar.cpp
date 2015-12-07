// unitTestAgar.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include "main.h"

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(taille, creationFood)
{
	
	EXPECT_EQ(Food().getSize(), 5);
}