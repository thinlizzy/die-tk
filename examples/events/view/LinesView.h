#ifndef LINESVIEW_H_
#define LINESVIEW_H_

#include <string>
#include <die-tk.h>
#include "../model/TextLog.h"

class LinesView {
	TextLog & log;
	tk::Window & window;
	int lineHeight;
	tk::RGBColor textColor,backgroundColor;
public:
	LinesView(TextLog & log, tk::Window & window);
	void drawCharLastLine(char key);
	void removeCharLastLine(char key);
	void verifyScroll();
	void updateLinesInRect(tk::Rect rect);
private:
	struct Line {
		int y;
		std::string line;
	};
	Line lastLine() const;
	size_t linesFit() const;
	void drawLinesFrom(size_t startLine, size_t endLine, int startY);
};

#endif /* LINESVIEW_H_ */
