/*
	!БЕТА ВЕРСИЯ!
	
	GyverOLED - лёгкая и быстрая библиотека для олед дисплея
	- Выбор буфера
		- Без буфера
		- Буфер на стороне МК
		- Буфер на стороне дисплея (только для SSH1106)		
	- Вывод текста
		- Поддержка русского языка
		- Два размера букв
		- Возможность писать чёрным-по-белому и белым-по-чёрному
	- Использование библиотеки microWire позволяет максимально облегчить код
	- Установка яркости
	- Включение/выключение дисплея из скетча
	- Изменение ориентации дисплея
	- Графика
		- Точки
		- Линии
		- Прямоугольники (контур или заливка)
		- Прямоугольники со скруглёнными углами (контур или заливка)
		- Окружности (контур или заливка)
	
	===== ДЕФАЙНЫ =====
	OLED_NO_BUFFER			- не использовать буфер вообще (активно по умолчанию)
	OLED_HARD_BUFFER_64 	- буфер на стороне дисплея (только для SSH1106)
	OLED_SOFT_BUFFER_32		- буфер на стороне МК (для 128x32)
	OLED_SOFT_BUFFER_64		- буфер на стороне МК (для 128x64)
	USE_MICRO_WIRE			- использовать библиотеку microWire.h (сильно облегчает код)
*/

#pragma once

#if defined(USE_MICRO_WIRE)
#include <microWire.h>	// лёгкая библиотека Wire (для atmega328)
#elif defined(USE_DEFAULT_WIRE)
#include <Wire.h>		// обычная Wire
#else
#include <microWire.h>	// лёгкая библиотека Wire (для atmega328)
#endif

#include <Arduino.h>
#include "charMap.h"

#define OLED128x32 0
#define OLED128x64 1

#define OLED_HEIGHT_32          0x02  // oled 128x32
#define OLED_HEIGHT_64          0x12  // oled 128x64
#define OLED_64          		0x3F
#define OLED_32          		0x1F

#define OLED_COMMAND_MODE       0x00
#define OLED_ONE_COMMAND_MODE   0x80
#define OLED_DATA_MODE          0x40
#define OLED_ONE_DATA_MODE      0xC0

#define OLED_DISPLAY_OFF		0xAE
#define OLED_DISPLAY_ON			0xAF

static const uint8_t _oled_init[] PROGMEM = {
	0xAE,	// oled off
	0xD5,	// CLOCK_DIV_RATIO
	0x80,
	0x8D,	// Charge pump
	0x14,
	0x20,	// Memory mode
	0x01,	// Vertical OLED_ADDRESSing
	0xA1,	// Flip horizontally
	0xC8,	// Flip vertically
	0x81,	// Set contrast
	0xCF,	// brighter
	0xDB,	// Set vcom detect
	0x40,	// brighter
	0xAF,	// oled on
};

#ifdef OLED_HARD_BUFFER_64
#define HARD_BUFFER
#endif

#ifdef OLED_SOFT_BUFFER_32
#define SOFT_BUFFER
#define OLED_BUF_SIZE 128*32/8
byte _oled_buffer[OLED_BUF_SIZE];
#endif

#ifdef OLED_SOFT_BUFFER_64
#define SOFT_BUFFER
#define OLED_BUF_SIZE 128*64/8
byte _oled_buffer[OLED_BUF_SIZE];
#endif

#ifndef SOFT_BUFFER
#define OLED_NO_BUFFER
#endif

#ifdef OLED_NO_BUFFER
#pragma message "Using OLED without buffer!"
#endif


class GyverOLED {
public:
	GyverOLED(byte address = 0x3C);			// создаём с указанием адреса
	
	// СЕРВИС
	void init(uint8_t oledType, int freq = 400);	// инициализация. Можно передать частоту шины (до 900 кГц). Дефолт 400
	void clear();							// очистить
	void setContrast(byte value);			// яркость
	void setPower(boolean mode);			// вкл/выкл (энергосбережение)
	void flipH();							// отразить по горизонтали
	void flipV();							// отразить по вертикали
	void update();							// обновить дисплей из буфера (если буфер включен)
	
	// ПЕЧАТЬ
	void home();							// отправить курсор в 0,0
	void setCursor(int column, int line);	// поставить курсор для символа (20 по горизонтали, 4/8 по вертикали)
	void scale1X();							// масштаб шрифта х1
	void scale2X();							// масштаб шрифта х2
	void inverse(boolean inv);					// инвертировать текст (чёрным по белому)
	
	void printChar(char c);					// вывести букву	
	void println();							// перевод строки
	
	// методы для печати
	void print(char c);	
	void print(char data[]);
	void print(String data);
	void print(uint32_t data);
	void print(int32_t data);
	void print(uint16_t data);
	void print(int16_t data);
	void print(uint8_t data);
	void print(int8_t data);
	void print(double data);
	void print(double data, byte decimals);
	
	void println(char c);
	void println(char data[]);
	void println(String data);
	void println(uint32_t data);
	void println(int32_t data);
	void println(uint16_t data);
	void println(int16_t data);
	void println(uint8_t data);
	void println(int8_t data);	
	void println(double data);
	void println(double data, byte decimals);
	
	boolean isEnd();						// возвращает true, если дисплей "кончился" - при побуквенном выводе
	
	// ГРАФИКА
	void setPosition(int x, int y);				// установка курсора на "столбик" x: 0-128, y: 0-4 (128*32) / 0-8 (128*64)
	void sendData(byte data);					// шлёт байт в указанный в setPosition "столбик"
	void dot(int x, int y, bool fill = true);	// рисует точку
	void line(int x0, int y0, int x1, int y1);	// рисует линию
	void rect(int x0, int y0, int x1, int y1, bool fill = false);		// прямоугольник (лев. верхн, прав. нижн)
	void roundRect(int x0, int y0, int x1, int y1, bool fill = false);	// прямоугольник скруглённый (лев. верхн, прав. нижн)
	void circle(int x, int y, int radius, bool fill = false);			// окружность
	void drawBitmap(int x, int y, const uint8_t *frame, int sizeX, int sizeY);	// битмап
	
	// ДРУГОЕ
	void sendCommand(byte command);			// отправить команду по I2C
	
private:
	void setWindow(byte x0, byte y0, byte x1, byte y1);
	void createBuf();
	void sendBuf();
	void _printBytes(uint32_t data);
	void _printBytes(uint32_t data, byte decimals);
	byte OLED_ADDRESS = 0x3C;
	uint8_t _getFont(uint8_t font, uint8_t row);
	int _stretch(int x);
	boolean _type, _invState = 0;
	byte _scale = 1;
	byte _x = 0, _y = 0;
	byte _lastChar;
	byte _numRows = 8;
	
	byte bufsizeX, bufsizeY;
	byte bufX, bufY;
	byte *buf_ptr;
	bool buf_flag = false;
};

// ====================== РЕАЛИЗАЦИЯ =======================
GyverOLED::GyverOLED(byte address){
	OLED_ADDRESS = address;
}

// ======================= INIT =======================
void GyverOLED::init(uint8_t oledType, int freq) {
	Wire.begin();
	Wire.setClock(freq);
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_COMMAND_MODE);
	for (uint8_t i = 0; i < 14; i++) Wire.write(pgm_read_byte(&_oled_init[i]));
	
	Wire.write(0xDA);
	if (oledType) Wire.write(OLED_HEIGHT_64);
	else Wire.write(OLED_HEIGHT_32);

	Wire.write(0xA8);
	if (oledType) Wire.write(OLED_64);
	else Wire.write(OLED_32);
	
	Wire.endTransmission();
	
	_type = oledType;
	_numRows = (_type) ? 8 : 4;
	GyverOLED::setCursor(0, 0);
}

// ======================= CLEAR =======================
void GyverOLED::clear() {
#ifdef SOFT_BUFFER
	memset(_oled_buffer, 0, OLED_BUF_SIZE);		
#else
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_COMMAND_MODE);
	Wire.write(0x21);
	Wire.write(0);
	Wire.write(127);
	
	Wire.write(0x22);
	Wire.write(0);
	Wire.write(_numRows-1);
	
	Wire.endTransmission();	
	
#if defined(USE_MICRO_WIRE)
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_DATA_MODE);
	int maxPacks = (_type) ? 1024 : 512;
	for (int i = 0; i < maxPacks; i++)
	Wire.write(0);
	Wire.endTransmission();
#else
	int maxPacks = (_type) ? 32 : 16;
	for (int i = 0; i < maxPacks; i++) {
		Wire.beginTransmission(OLED_ADDRESS);
		Wire.write(OLED_DATA_MODE);
		for (int j = 0; j < 32; j++) {
			Wire.write(0);
		}
		Wire.endTransmission();
	}
#endif
	
#endif
}

// ======================= CONTRAST =======================
void GyverOLED::setContrast(byte value) {
	sendCommand(0x81);
	sendCommand(value);
}

void GyverOLED::setPower(boolean mode) {
	sendCommand(mode ? OLED_DISPLAY_ON : OLED_DISPLAY_OFF);
}

// ======================= CURSOR =======================
void GyverOLED::setCursor(int column, int line) {
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_COMMAND_MODE);
	// Set column address range
	Wire.write(0x21); Wire.write(column * 6); Wire.write(column * 6 + _scale * 6 - 1);
	// Set page address range
	Wire.write(0x22); Wire.write(line); Wire.write(line + _scale - 1);
	Wire.endTransmission();
	_x = column;
	_y = line;
}

void GyverOLED::home() {
	GyverOLED::setCursor(0, 0);
}

// ======================= PRINT =======================
uint8_t GyverOLED::_getFont(uint8_t font, uint8_t row) {
	font = font - '0' + 16;   // перевод код символа из таблицы ASCII в номер согласно нумерации массива
	if (font <= 90) return pgm_read_byte(&(charMap[font][row]));		// для английских букв и символов
	else if (font >= 112 && font <= 159) {							// и пизд*ц для русских
		return pgm_read_byte(&(charMap[font - 17][row]));
	} else if (font >= 96 && font <= 111) {
		return pgm_read_byte(&(charMap[font + 47][row]));
	}
}

void GyverOLED::printChar(char c) {	
#if !defined(SOFT_BUFFER) || defined(HARD_BUFFER)
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_DATA_MODE);
#endif
	for (uint8_t col = 0; col < 6; col++) {
		int bits;
		bits = (col != 5) ? GyverOLED::_getFont((char)c, col) : 0;
		if (_invState) bits = ~bits;
		
#ifdef SOFT_BUFFER		
		int index = _y + (_x*6 + col * _scale) * _numRows;
		if (_scale == 1) {		
			_oled_buffer[index] |= bits;
			bits = _oled_buffer[index];
		} else {
			int newBits = _stretch(bits);
			_oled_buffer[index] |= newBits;
			_oled_buffer[index+1] |= newBits >> 8;
			_oled_buffer[index+_numRows] |= newBits;
			_oled_buffer[index+_numRows+1] |= newBits >> 8;
		}
#endif

#if !defined(SOFT_BUFFER) || defined(HARD_BUFFER)
		// тут нужно для ХАРД буфера прочитать байт и дописать к нему bits
		
		if (_scale == 1) Wire.write(bits);
		else {
			bits = _stretch(bits);
			Wire.write(bits);
			Wire.write(bits >> 8);
			Wire.write(bits);
			Wire.write(bits >> 8);
		}
#endif
	}
#if !defined(SOFT_BUFFER) || defined(HARD_BUFFER)
	Wire.endTransmission();
#endif
}

void GyverOLED::_printBytes(uint32_t data) {
	int8_t bytes[10];
	byte amount;
	for (byte i = 0; i < 10; i++) {
		bytes[i] = data % 10;
		data /= 10;
		if (data == 0) {
			amount = i;
			break;
		}
	}	
	for (int8_t i = amount; i >= 0; i--) {
		GyverOLED::print((char)(bytes[i] + '0'));
	}
}

void GyverOLED::print(uint32_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(uint32_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(uint16_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(uint16_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(uint8_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(uint8_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}

void GyverOLED::print(int32_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(int32_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(int16_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(int16_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(int8_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(int8_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}

void GyverOLED::print(double data, byte decimals) {
	if (data < 0) {
		print('-');
		data = -data;
	}
	uint32_t integer = data;
	_printBytes((uint32_t) integer);
	print('.');
	data -= integer;
	for (byte i = 0; i < decimals; i++) {	
		data *= 10.0;
		_printBytes((uint32_t) data);
		data -= (byte)data;
	}
}

void GyverOLED::print(double data) {
	GyverOLED::print(data, 2);
}

void GyverOLED::println(double data, byte decimals) {
	print(data, decimals);
	println();
}

void GyverOLED::println(double data) {
	print(data, 2);
	println();
}

void GyverOLED::println() {
	_y += _scale;
	_x = 0;
	if (_y > _numRows-1) _y = _numRows-1;
	setCursor(_x, _y);
}

void GyverOLED::print(char c) {	
	if ((byte)c == 10) GyverOLED::println();
	else {
		if ((byte)c <= 127) {printChar(c); _x += _scale;}		// если лат. буквы и символы
		else {
			if (_lastChar == 209 && (byte)c == 145) printChar((char)181);			// е вместо ё
			else if (_lastChar == 208 && (byte)c == 129) printChar((char)149);		// Е вместо Ё
			else if (_lastChar == 226 && (byte)c == 128) ;							// тире вместо длинного тире
			else if (_lastChar == 128 && (byte)c == 147) {printChar((char)45);_x += _scale;}		// тире вместо длинного тире			
			else if ((byte)c <= 191) {printChar(c); _x += _scale;}
		}
		_lastChar = c;		
		
		if (_x >= 20) {
			_x = 0;
			_y += _scale;			
		}
		setCursor(_x, _y);
	}	
}

void GyverOLED::println(char c) {
	printChar(c);
	_y += _scale;
	if (_y > _numRows-1) _y = _numRows-1;
	setCursor(_x, _y);
}

void GyverOLED::print(char data[]) {
	byte i = 0;
	byte lastPref;
	while (data[i] != '\0') {
		print(data[i]);
		i++;
	}
}

void GyverOLED::println(char data[]) {
	GyverOLED::print(data);
	GyverOLED::println();
}

void GyverOLED::print(String data) {
	byte stringSize = data.length();
	for (byte i = 0; i < stringSize; i++) {
		print((char)data[i]);
	}
}

void GyverOLED::println(String data) {
	GyverOLED::print(data);
	GyverOLED::println();
}

// ======================= FONT =======================
void GyverOLED::scale1X() {
	_scale = 1;
	setCursor(_x, _y);
}
void GyverOLED::scale2X() {
	_scale = 2;
	setCursor(_x, _y);
}

void GyverOLED::inverse(boolean inv) {
	_invState = inv;
}

void GyverOLED::flipH() {
	GyverOLED::sendCommand(0xA0);
}

void GyverOLED::flipV() {
	GyverOLED::sendCommand(0xC0);
}

// ======================= GRAPHICS =======================
void GyverOLED::setPosition(int x, int y) {
	setWindow(x, 127, y, _numRows-1);
}

void GyverOLED::dot(int x, int y, bool fill) {
	if (x > 127 || y > ((_type) ? 63 : 31) ||
			x < 0 || y < 0) return;
#if defined(SOFT_BUFFER)
	// записали в софт буфер
	int index = y / 8 + x * _numRows;
	bitWrite(_oled_buffer[index], y % 8, fill);

	/*
	// отправить на дисплей сразу
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_COMMAND_MODE);
	Wire.write(0x21); Wire.write(x); Wire.write(127);
	Wire.write(0x22); Wire.write(y / _numRows); Wire.write(_numRows-1);
	Wire.endTransmission();

	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_DATA_MODE);
	Wire.write(_oled_buffer[index]);
	Wire.endTransmission();
*/
	
#elif defined(HARD_BUFFER)
	// хард буфер - прочитали, дописали, отправили
	

#else	
	// без буфера - отправили и всё
	if (!buf_flag) {
		setWindow(x, 127, y / 8, _numRows-1);

		Wire.beginTransmission(OLED_ADDRESS);
		Wire.write(OLED_DATA_MODE);
		Wire.write(1 << (y % 8));	// задвигаем 1 на высоту y (для этого столбец-байта)
		Wire.endTransmission();
	} else {
		// локальный мини буфер
		x -= bufX;
		y -= bufY;
		int index = y / 8 + x * bufsizeY;
		bitWrite(buf_ptr[index], y % 8, fill);
	}
	
#endif
}

void GyverOLED::update() {
#if defined(SOFT_BUFFER)
	setWindow(0, 127, 0, (_type) ? 7 : 3);
	
#if defined(USE_MICRO_WIRE)
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_DATA_MODE);
	int maxPacks = (_type) ? 1024 : 512;	
	for (int i = 0; i < maxPacks; i++)
	Wire.write(_oled_buffer[i]);
	Wire.endTransmission();
#else
	int maxPacks = (_type) ? 32 : 16;
	int counter = 0;
	for (int i = 0; i < maxPacks; i++) {
		Wire.beginTransmission(OLED_ADDRESS);
		Wire.write(OLED_DATA_MODE);
		for (int j = 0; j < 32; j++) {
			Wire.write(_oled_buffer[counter]);
			counter++;
		}
		Wire.endTransmission();
	}
#endif
	
#endif
}


// динамический буфер
void GyverOLED::createBuf() {
#ifdef OLED_NO_BUFFER
	buf_flag = true;
	int bufSize = bufsizeX * bufsizeY;
	buf_ptr = malloc(bufSize);
	memset(buf_ptr, 0, bufSize);
#endif
}

void GyverOLED::sendBuf() {
#ifdef OLED_NO_BUFFER
	setWindow(bufX, 127, bufY/8, bufY/8 + bufsizeY - 1);
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_DATA_MODE);
	for (int i = 0; i < bufsizeX * (bufsizeY+1); i++) Wire.write(buf_ptr[i]);
	Wire.endTransmission();
	buf_flag = false;
	free(buf_ptr);
#endif
}

// линия
void GyverOLED::line(int x0, int y0, int x1, int y1) {
	if (x0 == x1) {
//#if defined(SOFT_BUFFER)
	for (int i = min(y0, y1); i < max(y0, y1); i++) dot(x0, i);
/*#else
	setPosition(x0, y0 / 8 - 1);
	byte thisByte = 0;
	if (y0 / 8 != y1 / 8) {		// в разных блоках
		for (byte i = 0; i < y0 % 8; i++) bitSet(thisByte, i);
		sendData(255);
		thisByte = 0;
		for (byte i = 0; i < y0 % 8; i++) bitSet(thisByte, i);
		//sendData(~thisByte);
	}
	
#endif*/
	} else if (y0 == y1) {
		for (int i = min(x0, x1); i < max(x0, x1); i++) dot(i, y0);
	} else {
		int sx, sy, e2, err;
		int dx = abs(x1 - x0);
		int dy = abs(y1 - y0);
		if (x0 < x1) sx = 1; else sx = -1;
		if (y0 < y1) sy = 1; else sy = -1;
		err = dx - dy;
		for (;;) {
			dot(x0, y0);
			if (x0==x1 && y0==y1) return;
			e2 = err<<1;
			if (e2 > -dy) { err = err - dy; x0 = x0 + sx; }
			if (e2 < dx) { err = err + dx; y0 = y0 + sy; }
		}
	}	
}


// прямоугольник
void GyverOLED::rect(int x0, int y0, int x1, int y1, bool fill) {
	int tmp;
	if (x0>x1) {
		tmp=x1;
		x0=x1;
		x1=tmp;
	}
	if (y0>y1) {
		tmp=y0;
		y0=y1;
		y1=tmp;
	}
#ifdef OLED_NO_BUFFER
	bufX = x0;
	bufsizeX = x1 - x0;
	bufY = y0;
	bufsizeY = y1 / 8 - y0 / 8 + 1;
	createBuf();
#endif
	
	if (fill) {		
		for (byte i = y0; i < y1; i++) line(x0, i, x1, i);		
	} else {
		line(x0, y0, x1, y0);
		line(x1, y0, x1, y1);
		line(x1, y1, x0, y1);
		line(x0, y1, x0, y0);
	}

#ifdef OLED_NO_BUFFER
	sendBuf();
#endif	
}

// скруглённый прямоугольник
void GyverOLED::roundRect(int x0, int y0, int x1, int y1, bool fill) {
	int tmp;
	if (x0>x1) {
		tmp=x1;
		x0=x1;
		x1=tmp;
	}
	if (y0>y1) {
		tmp=y0;
		y0=y1;
		y1=tmp;
	}

#ifdef OLED_NO_BUFFER
	bufX = x0;
	bufsizeX = x1 - x0;
	bufY = y0;
	bufsizeY = y1 / 8 - y0 / 8 + 1;
	createBuf();
#endif

	line(x0+2, y0, x1-1, y0);
	line(x1, y0+2, x1, y1-1);
	line(x1-1, y1, x0+2, y1);
	line(x0, y1-1, x0, y0+2);
	
	if (fill) {
		for (byte i = y0+1; i < y1; i++) line(x0+1, i, x1, i);
	} else {
		dot(x0+1, y0+1);
		dot(x1-1, y0+1);
		dot(x1-1, y1-1);
		dot(x0+1, y1-1);
	}
#ifdef OLED_NO_BUFFER
	sendBuf();
#endif	
}

// круг
void GyverOLED::circle(int x, int y, int radius, bool fill) {
#ifdef OLED_NO_BUFFER
	bufX = x - radius;
	bufsizeX = radius + radius + 1;
	bufY = y - radius;
	bufsizeY = bufsizeX / 8 + 1;
	createBuf();
#endif
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
	
	dot(x, y + radius);
	dot(x, y - radius);
	dot(x + radius, y);
	dot(x - radius, y);
	if (fill) line(x - radius, y, x + radius, y);

	while(x1 < y1) {
		if(f >= 0) {
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		if (fill) {
			line(x + x1 + 1, y + y1, x - x1, y + y1);
			line(x + x1 + 1, y - y1, x - x1, y - y1);
			line(x + y1 + 1, y + x1, x - y1, y + x1);
			line(x + y1 + 1, y - x1, x - y1, y - x1);
		} else {
			dot(x + x1, y + y1);
			dot(x - x1, y + y1);
			dot(x + x1, y - y1);
			dot(x - x1, y - y1);			
			dot(x + y1, y + x1);
			dot(x - y1, y + x1);
			dot(x + y1, y - x1);
			dot(x - y1, y - x1);
		}		
	}
#ifdef OLED_NO_BUFFER
	sendBuf();
#endif
}

// битмап
void GyverOLED::drawBitmap(int x, int y, const uint8_t *frame, int sizeX, int sizeY) {
#ifdef OLED_NO_BUFFER
	sendCommand(0x20);
	sendCommand(0x00);
	setWindow(x, x+sizeX-1, y/8, y/8+sizeY/8-1);
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_DATA_MODE);
	for (int i = 0; i < (sizeX * sizeY/8); i++) Wire.write(pgm_read_word(&(frame[i])));
	Wire.endTransmission();
	sendCommand(0x20);
	sendCommand(0x01);
#else
	int index = y / 8 + x * _numRows;
	byte col = 0;
	byte row = 0;
	for (int i = 0; i < (sizeX * sizeY/8); i++) {
		_oled_buffer[index + _numRows * col + row] = pgm_read_word(&(frame[i]));
		if (++col >= sizeX) {
			col = 0;
			row++;
		}
	}
#endif
}

// ======================= SERVICE =======================
void GyverOLED::setWindow(byte x0, byte x1, byte y0, byte y1) {
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_COMMAND_MODE);
	Wire.write(0x21);
	Wire.write(x0);
	Wire.write(x1);	
	Wire.write(0x22);
	Wire.write(y0);
	Wire.write(y1);	
	Wire.endTransmission();
}

boolean GyverOLED::isEnd() {
	if (_y > _numRows-1) return true;
	else return false;
}

void GyverOLED::sendCommand(byte command) {
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_ONE_COMMAND_MODE);
	Wire.write(command);
	Wire.endTransmission();
}

void GyverOLED::sendData(byte data) {
	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_ONE_DATA_MODE);
	Wire.write(data);
	Wire.endTransmission();
}

int GyverOLED::_stretch (int x) {
	x = (x & 0xF0) << 4 | (x & 0x0F);
	x = (x << 2 | x) & 0x3333;
	x = (x << 1 | x) & 0x5555;
	return x | x << 1;
}