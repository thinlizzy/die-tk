#ifndef TEXTLOG_H_
#define TEXTLOG_H_

#include <string>
#include <vector>

class TextLog {
	std::vector<std::string> lines;
public:
	enum class Status { noChange, newChar, deleteChar, newLine, };

	TextLog();
	Status processChar(char ch);
	std::vector<std::string> const & allLines();
};

#endif /* TEXTLOG_H_ */
