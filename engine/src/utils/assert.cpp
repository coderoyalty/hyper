#include "utils/assert.hpp"
#include <windows.h>
#include "utils/logger.hpp"
#include <stdio.h>
#include <cstdint>

using namespace hyp;

void report_assert(const char* expr, const char* message, const char* filename, uint32_t line) {
	log_output(hyp::LOG_TYPE::FATAL, "assert: %s\n", expr);

	HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	if (message != nullptr && message[0] != '\0')
	{
		SetConsoleTextAttribute(consoleOutput, 11);
		printf("\t\tmessage: '%s'\n", message);
	}

	SetConsoleTextAttribute(consoleOutput, 14);

	printf("\t\tfile: %s\n", filename);

	SetConsoleTextAttribute(consoleOutput, 12);

	printf("\t\tline: %d\n", line);

	SetConsoleTextAttribute(consoleOutput, 7);
}