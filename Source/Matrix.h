
#ifndef MATRIX_H
#define MATRIX_H

class RGBmatrixPanel {
    static const int maxH = 16;
    static const int maxW = 64;
public:
    RGBmatrixPanel() {
    }
    
#define PROGMEM
#define pgm_read_word *
    
#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif
    
    void setCursor(int16_t x, int16_t y) {
        cursorX = x;
        cursorY = y;
    }
    
    void fillScreen(uint16_t c) {
        for (int x = 0; x < maxW; x++) {
            for (int y = 0; y < maxH; y++) {
                writePixel(x, y, 0);
            }
        }
    }
    
    void writePixel(int16_t x, int16_t y, uint16_t color) {
        if (x >= 0 && y >= 0 && x < maxW && y < maxH) {
            buffer[x][y] = color;
        } else {
            //jassert(false);
        }
    }
    
    void drawPixel(int16_t x, int16_t y, uint16_t color) {
        writePixel(x, y, color);
    }
    
    void writeFastHLine(int16_t x, int16_t y, int16_t size, uint16_t color) {
        for (int16_t i = x; i < x + size; i++) {
            writePixel(i, y, color);
        }
    }
    
    void writeFastVLine(int16_t x, int16_t y, int16_t size, uint16_t color) {
        for (int16_t j = y; j < y + size; j++) {
            writePixel(x, j, color);
        }
    }
    
    void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
        for (int16_t j = y; j < y + height; j++) {
            writeFastHLine(x, j, width, color);
        }
    }
    
    void drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
        writeFastHLine(x, y, width, color);
        writeFastVLine(x, y, height, color);
        writeFastHLine(x, y + height - 1, width, color);
        writeFastVLine(x + width - 1, y, height, color);
    }
    
    uint16_t Color333(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0x7) << 13) | ((r & 0x6) << 10) | ((g & 0x7) << 8) |
                 ((g & 0x7) << 5) | ((b & 0x7) << 2) | ((b & 0x6) >> 1);
    }
    
    void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                 uint16_t color) {
        
      int16_t steep = abs(y1 - y0) > abs(x1 - x0);
      if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
      }

      if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
      }

      int16_t dx, dy;
      dx = x1 - x0;
      dy = abs(y1 - y0);

      int16_t err = dx / 2;
      int16_t ystep;

      if (y0 < y1) {
        ystep = 1;
      } else {
        ystep = -1;
      }

      for (; x0 <= x1; x0++) {
        if (steep) {
          writePixel(y0, x0, color);
        } else {
          writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
          y0 += ystep;
          err += dx;
        }
      }
    }
    
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                uint16_t color) {
      // Update in subclasses if desired!
      if (x0 == x1) {
        if (y0 > y1)
          _swap_int16_t(y0, y1);
        writeFastVLine(x0, y0, y1 - y0 + 1, color);
      } else if (y0 == y1) {
        if (x0 > x1)
          _swap_int16_t(x0, x1);
          writeFastHLine(x0, y0, x1 - x0 + 1, color);
      } else {
        startWrite();
        writeLine(x0, y0, x1, y1, color);
        endWrite();
      }
    }
    
    void drawCircle(int16_t x0, int16_t y0, int16_t r,
                                  uint16_t color) {

      int16_t f = 1 - r;
      int16_t ddF_x = 1;
      int16_t ddF_y = -2 * r;
      int16_t x = 0;
      int16_t y = r;

      startWrite();
      writePixel(x0, y0 + r, color);
      writePixel(x0, y0 - r, color);
      writePixel(x0 + r, y0, color);
      writePixel(x0 - r, y0, color);

      while (x < y) {
        if (f >= 0) {
          y--;
          ddF_y += 2;
          f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        writePixel(x0 + x, y0 + y, color);
        writePixel(x0 - x, y0 + y, color);
        writePixel(x0 + x, y0 - y, color);
        writePixel(x0 - x, y0 - y, color);
        writePixel(x0 + y, y0 + x, color);
        writePixel(x0 - y, y0 + x, color);
        writePixel(x0 + y, y0 - x, color);
        writePixel(x0 - y, y0 - x, color);
      }
      endWrite();
    }
    
    uint16_t ColorHSV(long hue, uint8_t sat, uint8_t val,
                                      bool gflag) {

      uint8_t r, g, b, lo;
      uint16_t s1, v1;

      // Hue
      hue %= 1536; // -1535 to +1535
      if (hue < 0)
        hue += 1536;      //     0 to +1535
      lo = hue & 255;     // Low byte  = primary/secondary color mix
      switch (hue >> 8) { // High byte = sextant of colorwheel
      case 0:
        r = 255;
        g = lo;
        b = 0;
        break; // R to Y
      case 1:
        r = 255 - lo;
        g = 255;
        b = 0;
        break; // Y to G
      case 2:
        r = 0;
        g = 255;
        b = lo;
        break; // G to C
      case 3:
        r = 0;
        g = 255 - lo;
        b = 255;
        break; // C to B
      case 4:
        r = lo;
        g = 0;
        b = 255;
        break; // B to M
      default:
        r = 255;
        g = 0;
        b = 255 - lo;
        break; // M to R
      }

      // Saturation: add 1 so range is 1 to 256, allowig a quick shift operation
      // on the result rather than a costly divide, while the type upgrade to int
      // avoids repeated type conversions in both directions.
      s1 = sat + 1;
      r = 255 - (((255 - r) * s1) >> 8);
      g = 255 - (((255 - g) * s1) >> 8);
      b = 255 - (((255 - b) * s1) >> 8);

      // Value (brightness) & 16-bit color reduction: similar to above, add 1
      // to allow shifts, and upgrade to int makes other conversions implicit.
      v1 = val + 1;
      if (gflag) { // Gamma-corrected color?
        
      } else {              // linear (uncorrected) color
        r = (r * v1) >> 12; // 4-bit results
        g = (g * v1) >> 12;
        b = (b * v1) >> 12;
      }
      return (r << 12) | ((r & 0x8) << 8) | // 4/4/4 -> 5/6/5
             (g << 7) | ((g & 0xC) << 3) | (b << 1) | (b >> 3);
    }
    
    void startWrite() {
    };
    
    void endWrite() {
    };
    
    void setTextColor(uint16_t color) {
        textColor = color;
    }
    void setTextSize(uint16_t size) {
        textSize = size;
    }
    void setTextWrap(bool wrap) {
    }
    
    void print(const char* toPrint) {
        int size = maxH/2;
        if (textSize == 2) {
            size = maxH-2;
        }
        fillRect(cursorX, cursorY, size , size, textColor);
    }
    
    void print( char toPrint) {
        print(&toPrint);
    }
    
    uint16_t buffer[maxW][maxH];
    uint16_t cursorX = 0;
    uint16_t cursorY = 0;
    uint16_t textColor;
    uint16_t textSize = 1;
    
    
};

static RGBmatrixPanel matrix;


#endif
