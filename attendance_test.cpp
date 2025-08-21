#include "gmock/gmock.h"
#include "attendance.cpp"
#include <iostream>

TEST(Attendance, checkWholeResult) {
	std::streambuf* original_cout_buffer = std::cout.rdbuf();
	std::ostringstream captured_output;
	std::cout.rdbuf(captured_output.rdbuf());
	
	AttendenceManager am;

	std::ifstream file("expected_output.txt");
	std::string expected_output;
	std::string line;
	bool fileOpen = false;

	if (fileOpen = file.is_open()) {
		while (getline(file, line))
			expected_output += line + "\n";
		file.close();
	}
	
	ASSERT_TRUE(fileOpen);

	am.input();

	std::cout.rdbuf(original_cout_buffer);
	EXPECT_EQ(captured_output.str(), expected_output);
}

int main(void) {
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}