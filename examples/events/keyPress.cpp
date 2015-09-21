#include <die-tk.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
using namespace tk;

/*
 * TODO this example shows the following bugs which need to be fixed:
 * - WinApi: maximizing window has some rendering glitches when handled by .onResize()
 * - WinApi: maximizing window won't have its size constrained when handled by .onResize()
 */

enum class Status { noChange, newChar, deleteChar, newLine, };

class TextLog {
	vector<string> lines;
public:
	TextLog();
	Status processChar(char ch);
	vector<string> const & allLines();
};

class LinesView {
	TextLog & log;
	Window & window;
	int lineHeight;
	RGBColor textColor,backgroundColor;
public:
	LinesView(TextLog & log, Window & window);
	void drawCharLastLine(char key);
	void removeCharLastLine(char key);
	void verifyScroll();
	void updateLinesInRect(Rect rect);
private:
	struct Line {
		int y;
		string line;
	};
	Line lastLine() const;
	int linesFit() const;
	void drawLinesFrom(size_t startLine, size_t endLine, int startY);
};

// the main function acts as a controller
int main()
{
	Window window(WindowParams("testex").dims(320,200));

	bool open = true;
	window.onClose([&open]() -> bool { open = false; return true; });

	TextLog log;
	LinesView linesView(log,window);

	// key press = write stuff on the screen, like it were a log
	window.onKeypress([&](char key) {
		auto status = log.processChar(key);
		switch(status) {
		case Status::newChar:
			linesView.drawCharLastLine(key);
			break;
		case Status::deleteChar:
			linesView.removeCharLastLine(key);
			break;
		case Status::newLine:
			linesView.verifyScroll();
			break;
		}
		return '\0';
	});

	// resize = redraw lines if the height is bigger, scroll if the height is smaller - limit to 800,600
	auto lastHeight = window.dims().height;
	window.onResize([&](WDims newDims) {
		newDims = newDims.fitInto(WDims(800,600));
		static int x = 0;
		cout << ++x << newDims << endl;
		if( newDims.height != lastHeight ) {
			lastHeight = newDims.height;
			linesView.verifyScroll();
		}
		return newDims;
	});

	// repaint = redraw lines according to the intersecting rectangle
	window.onPaint([&](Canvas & canvas, Rect rect) {
		linesView.updateLinesInRect(rect);
	});

	// horizontal bounds are not being checked for simplicity and it is left as an exercise for the reader :)

/*
	window.onKeyDown();
	window.onKeyUp();

	window.onMouseDown();
	window.onMouseUp();

	window.onMouseEnter();
	window.onMouseOver();
	window.onMouseLeave();
*/

	Application app;
	do {
		app.waitForMessages();
		app.processMessages();
	} while( open );
}

// -------------

TextLog::TextLog():
	lines(1)
{}

Status TextLog::processChar(char ch)
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

vector<string> const & TextLog::allLines() { return lines; }

// -------------

LinesView::LinesView(TextLog & log, Window & window):
	log(log),
	window(window),
	lineHeight(window.canvas().measureText("XZ|"_ns).height + 2),
	textColor(0,200,0),
	backgroundColor(0,0,0)
{
}

int LinesView::linesFit() const
{
	auto totalHeight = window.dims().height;
	return std::max(1,totalHeight / lineHeight);
}

auto LinesView::lastLine() const -> Line
{
	auto & lines = log.allLines();
	Line result;
	result.line = lines.back();
	result.y = (std::min(int(lines.size()),linesFit())-1) * lineHeight;
	return result;
}

void LinesView::drawCharLastLine(char key)
{
	auto & canvas = window.canvas();
	auto line = lastLine();
	auto lineDims = canvas.measureText(line.line);

	NativeString keyNS(key);
	auto charDims = canvas.measureText(keyNS);

	Point p(lineDims.width - charDims.width, line.y);
	canvas.drawText(p,keyNS,textColor,backgroundColor);
}

void LinesView::removeCharLastLine(char key)
{
	auto & canvas = window.canvas();
	auto line = lastLine();
	auto lineDims = canvas.measureText(line.line);

	auto charDims = canvas.measureText(key);

	Point p(lineDims.width,line.y);
	canvas.fillRect(Rect::closed(p,charDims.setHeight(lineHeight)),backgroundColor);
}

void LinesView::verifyScroll()
{
	auto lf = linesFit();
	auto & lines = log.allLines();
	if( lines.size() < lf ) return;

	drawLinesFrom(lines.size() - lf,lines.size(),0);
}

void LinesView::drawLinesFrom(size_t startLine, size_t endLine, int startY)
{
	auto & lines = log.allLines();
	auto & canvas = window.canvas();
	Point p(0,startY);
	for( auto l = startLine; l < endLine; ++l, p.y+=lineHeight ) {
		canvas.drawText(p,lines[l],textColor,backgroundColor);
		auto lineDims = canvas.measureText(lines[l]);
		canvas.fillRect(Rect::closed(p.addX(lineDims.width),WDims(window.width()-lineDims.width,lineHeight)),backgroundColor);
	}
}

void LinesView::updateLinesInRect(Rect rect)
{
	auto lf = linesFit();
	auto & lines = log.allLines();
	auto startLine = lines.size() <= lf ? 0 : lines.size() - lf;

	drawLinesFrom(rect.top / lineHeight + startLine,std::min(lines.size(),rect.bottom / lineHeight + startLine),rect.top / lineHeight * lineHeight);
}
