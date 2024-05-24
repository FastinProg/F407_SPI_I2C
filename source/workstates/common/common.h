#ifndef SOURCE_COMMON_H_
#define SOURCE_COMMON_H_

#include <stdint.h>


#define kHz							000L
#define MHz							000000L

#define	OFF							0
#define	ON							1

// ********************************* СТРУКТУРЫ ********************************

// MixedValue
typedef union
{
	int32_t Integer;
	uint16_t Short[2];
	uint8_t Byte[4];
	float Float;
} MixedValue;

// Структура  CAN сообщения
typedef struct
{
	uint32_t ID;
	union
	{
		uint8_t data[8];
		struct
		{
			uint32_t data32[2];
		};
	};

	uint8_t 
		Ext : 4,
		Rtr	: 4;
	uint8_t DLC;
} CanMsg;


// Дата и время
typedef struct
{
    uint8_t Year;
    uint8_t Months;
    uint8_t Days;
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
}  TIME_DATE_STR;

// Кольцевой буфер
typedef struct
{
	uint8_t* data;
	uint8_t* p;			// Указатель на данные для чтения/записи
	uint16_t size;		// Размер данных буфера в байтах
	uint16_t len;		// Размер заполненных данных

	uint16_t tail;		// Индекс в массиве, откуда можно будет считать следующий байт
	uint16_t head;		// Индекс в массиве, куда будет записан следующий байт

	uint16_t maxLoad;	// Максимальная загрузка буфера

	// Только для отладки
	uint32_t TotalRead;
} cbuf;

// Буфер общего назначения
typedef struct
{
	uint8_t* data;			// Указатель на данные буфера
	uint16_t size;			// Размер данных буфера в байтах
	uint16_t len;			// Размер заполненных данных
	uint16_t maxLoad;		// Максимальная загрузка буфера
} gbuf;

// ********************************* МАКРОСЫ **********************************

#if !defined(__XC)
#define min(a,b) 				(a>b ? b:a)
#define max(a,b) 				(a>b ? a:b)
#endif

#define IS_BIT_SET(reg, bit)	(reg & (1L << bit))
#define SET_BIT(reg, bit)		(reg |= (1L << bit))
#define CLR_BIT(reg, bit)		(reg &= ~(1L << bit))
//#define GET_BIT(reg,bit)		(reg & (1L << bit)? 1 : 0)

#define SET_BITS(reg, bit)		(reg |= bit)
#define CLR_BITS(reg, bit)		(reg &= ~bit)
#define GET_BITS(reg, bit)		(reg & bit)

#define SWAP16(v)				((v >> 8) | ((v << 8) & 0xFF00))
#define LOBYTE(w)				((uint8_t)((w) & 0xff))

// Извлечение компонентов цвета RGB
#define GetColorA(rgb)			(LOBYTE((rgb) >> 24))
#define GetColorR(rgb)			(LOBYTE((rgb) >> 16))
#define GetColorG(rgb)			(LOBYTE((rgb) >> 8))
#define GetColorB(rgb)			(LOBYTE(rgb))


// Возвращает количество элементов в указанном массиве
#define ARRAY_LEN(a)			(sizeof(a) / sizeof(a[0]))


// Аппаратно-зависимые функции. Должны быть реализованы для каждого контроллера в HW_Func.c
uint32_t Get_usTime();					// Счетчик микросекунд
uint32_t Get_msTime();					// Счетчик милисекунд
uint32_t Get_usTimeFrom(uint32_t time);
uint32_t Get_msTimeFrom(uint32_t time);


#endif /* SOURCE_COMMON_H_ */




