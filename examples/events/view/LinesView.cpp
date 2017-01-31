#include <algorithm>
#include "LinesView.h"
using namespace std;
using namespace tk;

LinesView::LinesView(TextLog & log, Window & window):
	log(log),
	window(window),
	lineHeight(window.canvas().measureText("XZ|"_ns).height + 2),
	textColor(0,200,0),
	backgroundColor(0,0,0)
{
}

size_t LinesView::linesFit() const
{
	auto totalHeight = window.dims().height;
	return std::max(1,totalHeight / lineHeight);
}

auto LinesView::lastLine() const -> Line
{
	auto & lines = log.allLines();
	Line result;
	result.line = lines.back();
	result.y = (std::min(lines.size(),linesFit())-1) * lineHeight;
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
