#include <endian.h>
#include "main.h"
#include "utility.h"
#include "games/hcrn/FrameBuffer.h"
#include "modules/sd.h"
#include "config/custom_board.h"

#include "adafruit/gfxfont.h"
#include "shim_timer.h"

#include <SDL2/SDL.h>

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

extern SDL_Window* window;
extern SDL_Renderer* renderer;

//Cursor coordinates
static int16_t m_cursor_x = 0;
static int16_t m_cursor_y = 0;
static area_t m_cursor_area = { 0, 0, 128, 128 };
static uint16_t m_color = COLOR_WHITE;
static bool m_wrap = true;
static volatile bool m_stop = false;

uint16_t frame[WIDTH * HEIGHT];
FrameBuffer canvas;

extern "C" {
	FrameBuffer *p_canvas()
	{
		return &canvas;
	}
}

void bigE16BufferToHost (uint16_t *buf, size_t bufferSize) {
	uint16_t native = 0xFF00;
	if(native != be16toh(native)) {
		// printf("Buffer wrong endian, correcting\n");
		for(size_t i = 0; i < bufferSize; i++) {
			buf[i] = be16toh(buf[i]);
		}
	} else {
		// printf("Buffer already correct format, skipping\n");
	}
}

FrameBuffer::FrameBuffer() {}
FrameBuffer::~FrameBuffer() {}

void FrameBuffer::clearScreen(uint16_t color) {
	for (int i=0; i<WIDTH*HEIGHT; ++i)
	{
		frame[i] = color;
	}

}

void FrameBuffer::drawPixel(int x, int y, uint16_t color) {
	frame[y*WIDTH+x] = color;
}

void FrameBuffer::drawHorizontalLine(int x1, int y, int x2, uint16_t color) {
	int s1 = min(x1, x2);
	int s2 = max(x1, x2);

	for (int i=s1; i<=s2; ++i)
	{
		frame[y*WIDTH+i]=color;
	}

}

void FrameBuffer::drawVerticalLine(int x, int y1, int y2, uint16_t color) {
	int s1 = min(y1, y2);
	int s2 = max(y1, y2);

	for (int i=s1; i<=s2; ++i)
	{
		frame[i*WIDTH+x]=color;
	}

}

void FrameBuffer::drawImage(int x, int y, int w, int h, const uint16_t *data)
{
	int idx = 0;

	for (int j = y; j < (y + h); ++j)
	{
		for (int i = x; i < (x + w); ++i)
		{
			frame[j * WIDTH + i] = data[idx++];
		}
	}

}

void FrameBuffer::drawImage(int x, int y, int w, int h, const uint16_t *data, uint16_t tansparent_color)
{
	int idx = 0;

	for (int j = y; j < (y + h); ++j)
	{
		for (int i=x; i < (x + w); ++i)
		{
			uint16_t c = data[idx++];

			if (c != tansparent_color)
			{
				frame[j*WIDTH+i] = c;
			}
		}
	}
}

void FrameBuffer::drawImage(int x, int y, int w, int h, const uint8_t *data)
{
	int idx = 0;

	for (int j = y; j < (y + h); ++j)
	{
		for (int i = x; i < (x + w); ++i)
		{
			uint8_t d1 = data[idx++];
			uint8_t d2 = data[idx++];

			frame[j * WIDTH + i] = ((uint16_t) d1 << 8) | d2;
		}
	}
}

void FrameBuffer::drawImage(int x, int y, int w, int h, const uint8_t *data, uint16_t tansparent_color)
{
	int idx = 0;

	for (int j = y; j < (y + h); ++j)
	{
		for (int i = x; i < (x + w); ++i)
		{
			uint8_t d1 = data[idx++];
			uint8_t d2 = data[idx++];
			uint16_t c = ((uint16_t) d1 << 8) | d2;

			if (c != tansparent_color)
			{
				frame[j * WIDTH + i] = c;
			}
		}
	}
}

void FrameBuffer::drawImage(int x, int y, int w, int h, const uint16_t *data, int fx, int fy, int pitch)
{
	for (int i = 0, idx = pitch * fy + fx; i < h; ++i, idx += pitch)
	{
		memcpy(&frame[(y + i) * WIDTH + x], &data[idx], sizeof(uint16_t) * w);
	}
}

void FrameBuffer::drawImage(int x, int y, int w, int h, const uint16_t *data, int fx, int fy, int pitch, uint16_t tansparent_color)
{
	for (int j = 0; j < h; ++j)
	{
		for (int i = 0; i < w; ++i)
		{
			uint16_t c = data[pitch * (fy + j) + i + fx];

			if (c != tansparent_color)
			{
				frame[(j + y) * WIDTH + i + x] = c;
			}
		}
	}
}

void FrameBuffer::drawImageFromFile(int x, int y, int w, int h, const char* filename, int fx, int fy, int pitch) {
	size_t bufferSize = w*h;
	uint16_t buf[bufferSize];
	FILE *fd = fopen(filename, "rb");
	fseek(fd, (pitch*fy+fx)*sizeof(uint16_t), SEEK_SET);

	for (int i=0; i<h; ++i)
	{
		fread(&buf[i*w], sizeof(uint16_t), w, fd);
		fseek(fd, (pitch-w)*sizeof(uint16_t), SEEK_CUR);
	}

	fclose(fd);
	bigE16BufferToHost(buf, bufferSize);
	drawImage(x, y, w, h, buf);
}

void FrameBuffer::drawImageFromFile(int x, int y, int w, int h, const char* filename, int fx, int fy, int pitch, uint16_t tansparent_color) {
	size_t bufferSize = w*h;
	uint16_t buf[bufferSize];

	FILE *fd = fopen(filename, "rb");
	fseek(fd, (pitch*fy+fx)*sizeof(uint16_t), SEEK_SET);

	for (int i=0; i<h; ++i)
	{
		fread(&buf[i*w], sizeof(uint16_t), w, fd);
		fseek(fd, (pitch-w)*sizeof(uint16_t), SEEK_CUR);
	}

	fclose(fd);

	bigE16BufferToHost(buf, bufferSize);
	drawImage(x, y, w, h, buf, tansparent_color);
}

void FrameBuffer::drawImageFromFile(int x, int y, int w, int h, const char *filename)
{
	size_t bufferSize = w * h;
	uint16_t buf[bufferSize];
	uint32_t offset = 0;
	m_stop = false;

	FILE *file;

	file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("Can't load file %s\n", filename);
		return;
	}

	int retval = fseek(file, 0, SEEK_END);

	if (retval != 0)
	{
		printf("Failed to get file size: (seek end) on file: %s\n", filename);
		fclose(file);
		return;
	}

	long tell_size = ftell(file);

	if (tell_size == -1)
	{
		printf("Failed to get file size: (ftell) on file: %s\n", filename);
		fclose(file);
		return;
	}

	size_t size = min((size_t)tell_size, sizeof(uint16_t) * w * h);
	uint16_t frames = MAX(tell_size / w / h / sizeof(uint16_t), 1);

	if (size == 0)
	{
		printf("Could not stat %s.\n", filename);
		return;
	}

	for (uint16_t i = 0; i < frames; i++)
	{
		retval = fseek(file, offset, SEEK_SET);

		if (retval != 0)
		{
			printf("Failed to seek the file %s\n", filename);
			fclose(file);
			return;
		}

		size_t read = fread(buf, sizeof(uint8_t), size, file);

		if (read != size)
		{
			printf("Failed to read file %s\n", filename);
			fclose(file);
			return;
		}

		bigE16BufferToHost(buf, bufferSize);
		canvas.drawImage(x, y, w, h, buf);

		uint8_t retVal = getButton(false);
		if (retVal != USER_BUTTON_NONE || m_stop)
		{
			break;
		}

		offset += size;
		nrf_delay_us(7500);
	}

	fclose(file);

	bigE16BufferToHost(buf, bufferSize);
	drawImage(x, y, w, h, buf);
	return;
}

void FrameBuffer::drawImageFromFile(int x, int y, int w, int h, const char *filename, void (*p_callback)(uint8_t frame, void *p_data), void *data)
{
	size_t bufferSize = w * h;
	uint16_t buf[bufferSize];
	uint32_t offset = 0;
	m_stop = false;

	FILE *file;

	file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("Can't load file %s\n", filename);
		return;
	}

	int retval = fseek(file, 0, SEEK_END);

	if (retval != 0)
	{
		printf("Failed to get file size: (seek end) on file: %s\n", filename);
		fclose(file);
		return;
	}

	long tell_size = ftell(file);

	if (tell_size == -1)
	{
		printf("Failed to get file size: (ftell) on file: %s\n", filename);
		fclose(file);
		return;
	}

	size_t size = min((size_t)tell_size, sizeof(uint16_t) * w * h);
	uint16_t frames = MAX(tell_size / w / h / sizeof(uint16_t), 1);

	if (size == 0)
	{
		printf("Could not stat %s.\n", filename);
		return;
	}

	for (uint16_t i = 0; i < frames; i++)
	{
		retval = fseek(file, offset, SEEK_SET);

		if (retval != 0)
		{
			printf("Failed to seek the file %s\n", filename);
			fclose(file);
			return;
		}

		size_t read = fread(buf, sizeof(uint8_t), size, file);

		if (read != size)
		{
			printf("Failed to read file %s\n", filename);
			fclose(file);
			return;
		}

		bigE16BufferToHost(buf, bufferSize);
		canvas.drawImage(x, y, w, h, buf);

		if (p_callback != NULL)
		{
			p_callback(i, data);
		}

		uint8_t retVal = getButton(false);
		if (retVal != USER_BUTTON_NONE || m_stop)
		{
			break;
		}

		offset += size;
		nrf_delay_us(7500);
	}

	fclose(file);
}

uint8_t FrameBuffer::drawLoopImageFromFile(int x, int y, int w, int h, const char *filename)
{
	size_t bufferSize = w * h;
	uint16_t buf[bufferSize];
	uint8_t retVal = USER_BUTTON_NONE;
	m_stop = false;

	FILE *file;

	file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("Can't load file %s\n", filename);
		return 0;
	}

	int retval = fseek(file, 0, SEEK_END);

	if (retval != 0)
	{
		printf("Failed to get file size: (seek end) on file: %s\n", filename);
		fclose(file);
		return 0;
	}

	long tell_size = ftell(file);

	if (tell_size == -1)
	{
		printf("Failed to get file size: (ftell) on file: %s\n", filename);
		fclose(file);
		return 0;
	}

	size_t size = min((size_t)tell_size, sizeof(uint16_t) * w * h);
	uint16_t frames = MAX(tell_size / w / h / sizeof(uint16_t), 1);

	if (size == 0)
	{
		printf("Could not stat %s.\n", filename);
		return 0;
	}

	retval = fseek(file, 0, SEEK_SET);

	if (retval != 0)
	{
		printf("Failed to get file size: (seek start) on file: %s\n", filename);
		fclose(file);
		return 0;
	}

	if (size == 0)
	{
		printf("Could not stat %s.\n", filename);
		return 0;
	}

	do
	{
		uint32_t offset = 0;

		for (uint16_t i = 0; i < frames; i++)
		{
			retval = fseek(file, offset, SEEK_SET);

			if (retval != 0)
			{
				printf("Failed to seek the file %s\n", filename);
				fclose(file);
				return 0;
			}

			size_t read = fread(buf, sizeof(uint8_t), size, file);

			if (read != size)
			{
				printf("Failed to read file %s\n", filename);
				fclose(file);
				return 0;
			}

			bigE16BufferToHost(buf, bufferSize);
			canvas.drawImage(x, y, w, h, buf);

			uint8_t retVal = getButton(false);
			if (retVal != USER_BUTTON_NONE || m_stop)
			{
				break;
			}

			offset += size;
			nrf_delay_us(7500);
		}

		//if we're looping give them a way out
		retVal = getButton(false);
		if (retVal != USER_BUTTON_NONE || m_stop)
		{
			break;
		}

	#ifdef DC801_DESKTOP
		if (application_quit != 0)
		{
			break;
		}
	#endif

		nrf_delay_ms(10);
	} while (m_stop == false);

	fclose(file);
	return retVal;
}

uint8_t FrameBuffer::drawLoopImageFromFile(int x, int y, int w, int h, const char *filename, void (*p_callback)(uint8_t frame, void *p_data), void *data)
{
	size_t bufferSize = w * h;
	uint16_t buf[bufferSize];
	uint8_t retVal = USER_BUTTON_NONE;
	m_stop = false;

	FILE *file;

	file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("Can't load file %s\n", filename);
		return 0;
	}

	int retval = fseek(file, 0, SEEK_END);

	if (retval != 0)
	{
		printf("Failed to get file size: (seek end) on file: %s\n", filename);
		fclose(file);
		return 0;
	}

	long tell_size = ftell(file);

	if (tell_size == -1)
	{
		printf("Failed to get file size: (ftell) on file: %s\n", filename);
		fclose(file);
		return 0;
	}

	size_t size = min((size_t)tell_size, sizeof(uint16_t) * w * h);
	uint16_t frames = MAX(tell_size / w / h / sizeof(uint16_t), 1);

	if (size == 0)
	{
		printf("Could not stat %s.\n", filename);
		return 0;
	}

	retval = fseek(file, 0, SEEK_SET);

	if (retval != 0)
	{
		printf("Failed to get file size: (seek start) on file: %s\n", filename);
		fclose(file);
		return 0;
	}

	if (size == 0)
	{
		printf("Could not stat %s.\n", filename);
		return 0;
	}

	do
	{
		uint32_t offset = 0;

		for (uint16_t i = 0; i < frames; i++)
		{
			retval = fseek(file, offset, SEEK_SET);

			if (retval != 0)
			{
				printf("Failed to seek the file %s\n", filename);
				fclose(file);
				return 0;
			}

			size_t read = fread(buf, sizeof(uint8_t), size, file);

			if (read != size)
			{
				printf("Failed to read file %s\n", filename);
				fclose(file);
				return 0;
			}

			bigE16BufferToHost(buf, bufferSize);
			canvas.drawImage(x, y, w, h, buf);

			if (p_callback != NULL)
			{
				p_callback(i, data);
			}

			uint8_t retVal = getButton(false);
			if (retVal != USER_BUTTON_NONE || m_stop)
			{
				break;
			}

			offset += size;
			nrf_delay_us(7500);
		}

		//if we're looping give them a way out
		retVal = getButton(false);
		if (retVal != USER_BUTTON_NONE || m_stop)
		{
			break;
		}

	#ifdef DC801_DESKTOP
		if (application_quit != 0)
		{
			break;
		}
	#endif

		nrf_delay_ms(10);
	} while (m_stop == false);

	fclose(file);
	return retVal;
}

void FrameBuffer::drawStop()
{
	m_stop = true;
}

void FrameBuffer::drawBitmapFromFile(int x, int y, int w, int h, const char *filename)
{
	SDL_Surface *image = SDL_LoadBMP(filename);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_Rect dstrect = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_RenderPresent(renderer);
}

void FrameBuffer::fillRect(int x, int y, int w, int h, uint16_t color)
{
	if ((x >= WIDTH) || (y >= HEIGHT))
	{
		printf("Rectangle off the screen\n");
		return;
	}

	// Clip to screen
	if ((x + w) > WIDTH)
	{
		w = WIDTH - x;
	}

	if ((y + h) > HEIGHT)
	{
		h = HEIGHT - y;
	}

	// X
	for (int i = x; i < (x + w); i++)
	{
		// Y
		for (int j = y; j < (y + h); j++)
		{
			int index = i + (WIDTH * j);
			frame[index] = color;
		}
	}
}

void FrameBuffer::drawRect(int x, int y, int w, int h, uint16_t color) {
	drawHorizontalLine(x, y, x + w, color);
	drawHorizontalLine(x, y + h, x + w, color);
	drawVerticalLine(x, y, y + h, color);
	drawVerticalLine(x + w, y, y + h, color);
}

void FrameBuffer::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

void FrameBuffer::drawLine(int x1, int y1, int x2, int y2, uint16_t color) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int y = y1;
	int p =2 * dy - dx;

	for(int x = x1; x < x2; ++x)
	{
		frame[x + y * WIDTH] = color;

		if(p>=0)
		{
			y++;
			p += 2 * dy - 2 * dx;
		}
		else
		{
			p += 2 * dy;
		}
	}
}

void FrameBuffer::fillCircle(int x, int y, int radius, uint16_t color){
	int rad2 = radius * radius;

	for (int j = (y - radius);j <= (y + radius); ++j)
	{
		int yd = fabs(y - j);
		int radx2 = rad2 - yd * yd;

		for (int i = (x - radius);i <= (x + radius); ++i)
		{
			int xd = fabs(x - i);
			int dist2 = xd * xd;

			if (dist2 <= radx2)
			{
				frame[i + j * WIDTH] = color;
			}
		}
	}
}

void FrameBuffer::mask(int px, int py, int rad1, int rad2, int rad3)
{
	int minx = px - rad3;
	int maxx = px + rad3;

	int miny = py - rad3;
	int maxy = py + rad3;

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			if ((x < minx) || (x > maxx) || (y < miny) || (y > maxy))
			{
				frame[x + y * WIDTH] = 0;
			}
			else
			{
				int dx = abs(x - px);
				int dy = abs(y - py);

				int dist = (dx * dx) + (dy * dy);

				if (dist > (rad3 * rad3))
				{
					frame[x + y * WIDTH] = 0;
				}
				else if (dist > (rad2 * rad2))
				{
					frame[x + y * WIDTH] = (frame[x + y * WIDTH] >> 2) & 0xF9E7;
				}
				else if (dist > (rad1 * rad1))
				{
					frame[x + y * WIDTH] = (frame[x + y * WIDTH] >> 1) & 0xFBEF;
				}
			}
		}
	}
}

static void __draw_char(int16_t x, int16_t y, unsigned char c, uint16_t color,
		uint16_t bg, GFXfont font) {

	// Character is assumed previously filtered by write() to eliminate
	// newlines, returns, non-printable characters, etc.  Calling drawChar()
	// directly with 'bad' characters of font may cause mayhem!

	c -= font.first;
	GFXglyph *glyph = &(font.glyph[c]);
	uint8_t *bitmap = font.bitmap;

	uint16_t bo = glyph->bitmapOffset;
	uint8_t w = glyph->width;
	uint8_t h = glyph->height;

	int8_t xo = glyph->xOffset;
	int8_t yo = glyph->yOffset;
	uint8_t xx, yy, bits = 0, bit = 0;

	// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
	// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
	// has typically been used with the 'classic' font to overwrite old
	// screen contents with new data.  This ONLY works because the
	// characters are a uniform size; it's not a sensible thing to do with
	// proportionally-spaced fonts with glyphs of varying sizes (and that
	// may overlap).  To replace previously-drawn text when using a custom
	// font, use the getTextBounds() function to determine the smallest
	// rectangle encompassing a string, erase the area with fillRect(),
	// then draw new text.  This WILL infortunately 'blink' the text, but
	// is unavoidable.  Drawing 'background' pixels will NOT fix this,
	// only creates a new set of problems.  Have an idea to work around
	// this (a canvas object type for MCUs that can afford the RAM and
	// displays supporting setAddrWindow() and pushC(olors()), but haven't
	// implemented this yet.

	int16_t yyy;
	for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++) {
			if (!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if (bits & 0x80) {
				yyy = y + yo + yy;
				if (yyy >= m_cursor_area.ys && yyy <= m_cursor_area.ye) {
					//util_gfx_set_pixel(x + xo + xx, y + yo + yy, color);
					canvas.drawPixel(	x + xo + xx, y + yo + yy, color);
				}
			}
			bits <<= 1;
		}
	}
}

void FrameBuffer::write_char(uint8_t c, GFXfont font) {
	//If newline, move down a row
	if (c == '\n') {
		m_cursor_x = m_cursor_area.xs;
		m_cursor_y += font.yAdvance;
	}
	//Otherwise, print the character (ignoring carriage return)
	else if (c != '\r') {
		uint8_t first = font.first;

		//Valid char?
		if ((c >= first) && (c <= font.last)) {
			uint8_t c2 = c - first;
			GFXglyph *glyph = &(font.glyph[c2]);

			uint8_t w = glyph->width;
			uint8_t h = glyph->height;

			if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
				int16_t xo = glyph->xOffset;

				if ((m_cursor_x + (xo + w)) >= m_cursor_area.xe && m_wrap) {
					// Drawing character would go off right edge; wrap to new line
					m_cursor_x = m_cursor_area.xs;
					m_cursor_y += font.yAdvance;
				}

				__draw_char(m_cursor_x, m_cursor_y, c, m_color, COLOR_BLACK, font);
			}
			m_cursor_x += glyph->xAdvance;
		}
	}
}

void FrameBuffer::printMessage(const char *text, GFXfont font, uint16_t color, int x, int y)
{
	m_color = color;
	m_cursor_x = x;
	m_cursor_y = y + (font.yAdvance / 2);

	for (uint16_t i = 0; i < strlen(text); i++)
	{
		write_char(text[i], font);
	}
}

void FrameBuffer::getCursorPosition(cursor_t *cursor)
{
	cursor->x = m_cursor_x;
	cursor->y = m_cursor_y;
}

void FrameBuffer::setTextArea(area_t *area)
{
	m_cursor_area = *area;
}

void FrameBuffer::getTextBounds(GFXfont font, const char *text, int16_t x, int16_t y, bounds_t *bounds)
{
	getTextBounds(font, text, x, y, NULL, bounds);
}

void FrameBuffer::getTextBounds(GFXfont font, const char *text, int16_t x, int16_t y, area_t *near, bounds_t *bounds)
{
	bounds->width = 0;
	bounds->height = 0;

	if (near != NULL)
	{
		m_cursor_area = *near;
	}

	GFXglyph glyph;

	uint8_t c;
	uint8_t first = font.first;
	uint8_t last = font.last;

	uint8_t gw, gh, xa;
	int8_t xo, yo;

	int16_t minx = WIDTH;
	int16_t miny = HEIGHT;

	int16_t maxx = -1;
	int16_t maxy = -1;

	int16_t gx1, gy1, gx2, gy2;
	int16_t ya = font.yAdvance;

	// Walk the string
	while ((c = *text++))
	{
		if ((c != '\n') && (c != '\r'))
		{
			if ((c >= first) && (c <= last))
			{
				c -= first;
				glyph = font.glyph[c];

				gw = glyph.width;
				gh = glyph.height;

				xa = glyph.xAdvance;
				xo = glyph.xOffset;
				yo = glyph.yOffset;

				if (m_wrap && ((x + ((int16_t)xo + gw)) >= WIDTH))
				{
					// Line wrap
					x = 0;
					y += ya;
				}

				gx1 = x + xo;
				gy1 = y + yo;
				gx2 = gx1 + gw - 1;
				gy2 = gy1 + gh - 1;

				if (gx1 < minx)
				{
					minx = gx1;
				}

				if (gy1 < miny)
				{
					miny = gy1;
				}

				if (gx2 > maxx)
				{
					maxx = gx2;
				}

				if (gy2 > maxy)
				{
					maxy = gy2;
				}

				x += xa;
			}
		}
		else if (c == '\n')
		{
			x = 0;
			y += ya;
		}
	}

	if (maxx >= minx)
	{
		bounds->width = maxx - minx + 1;
	}

	if (maxy >= miny)
	{
		bounds->height = maxy - miny + 1;
	}
}

uint8_t FrameBuffer::getFontHeight(GFXfont font)
{
	return font.yAdvance;
}

void FrameBuffer::blt()
{
	void* pixels;
	int pitch;
	SDL_Texture* tex = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGB565,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH,
		HEIGHT
	);
	SDL_LockTexture(tex, NULL, &pixels, &pitch);
	memcpy(pixels, frame, WIDTH*HEIGHT*sizeof(uint16_t));
	SDL_UnlockTexture(tex);
	SDL_RenderCopy(renderer, tex, NULL, NULL);
	SDL_DestroyTexture(tex);
	SDL_RenderPresent(renderer);
}