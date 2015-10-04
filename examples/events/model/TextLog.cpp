#include "TextLog.h"

TextLog::TextLog():
	lines(1)
{}

auto TextLog::processChar(char ch) -> Status
{
	switch(ch) {
	case '\r':
		lines.resize(lines.size()+1);
		return Status::newLine;
	case '\b':
		if( lines.back().empty() ) return Status::noChange;
		lines.back().pop_back();
		return Status::deleteChar;
	default:
		lines.back().push_back(ch);
		return Status::newChar;
	}
}

std::vector<std::string> const & TextLog::allLines() { return lines; }
