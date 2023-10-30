//	Библиотека для работы с Bluetooth Trema-модулем HC05: http://iarduino.ru/shop/Expansion-payments/bluetooth-hc-05-trema-modul.html
//  Версия: 1.1.0
//  Последнюю версию библиотеки Вы можете скачать по ссылке: http://iarduino.ru/file/301.html
//  Подробное описание функции бибилиотеки доступно по ссылке: http://wiki.iarduino.ru/page/trema-modul-bluetooth-hc-05/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_Bluetooth_HC05_h																					//
#define iarduino_Bluetooth_HC05_h																					//
																													//
#ifdef    SoftwareSerial_h																							//	Если в скетче подключена библиотека  SoftwareSerial,
#include "SoftwareSerial.h"																							//	то разрешаем работать  с библиотекой SoftwareSerial.
#endif																												//
																													//
#if defined(ARDUINO) && (ARDUINO >= 100)																			//
#include <Arduino.h>																								//
#else																												//
#include <WProgram.h>																								//
#endif																												//
																													//
class iarduino_Bluetooth_HC05{																						//
																													//
	public:			iarduino_Bluetooth_HC05	(uint8_t);																//	Объявляем конструктор класса						(аргумент функции: № вывода к которому подключён вход K модуля)
		bool		begin					(HardwareSerial &j){flgType=1; objSerial=&j; return funSerialBegin();}	//	Определяем функцию инициализации модуля				(аргумент функции: объект для работы с аппаратным UART)
		#ifdef SoftwareSerial_h																						//
		bool		begin					(SoftwareSerial &j){flgType=0; objSerial=&j; return funSerialBegin();}	//	Определяем функцию инициализации модуля				(аргумент функции: объект для работы с программным UART)
		#endif																										//
		char *		runAT					(const char*, uint32_t=500, bool=true, bool=false, bool=false);			//	Объявляем функцию для выполнения AT-команд			(аргумент функции: строка с АТ-командой, таймаут в миллисекундах, флаг разрешающий досрочный выход при успешном ответе модуля, флаг отправки команды без «1» на выводе pinKey, флаг дополнительной обработки ответа команд INQ или RNAME)
		char *		runAT					(String,      uint32_t=500, bool=true, bool=false, bool=false);			//	Объявляем функцию для выполнения AT-команд			(аргумент функции: строка с АТ-командой, таймаут в миллисекундах, флаг разрешающий досрочный выход при успешном ответе модуля, флаг отправки команды без «1» на выводе pinKey, флаг дополнительной обработки ответа команд INQ или RNAME)
		int8_t		flgResult=0;																					//	Флаг ответа на команду функции runAT()				(-1: Error(*), 0: Неизвестно, 1: OK)
		bool		createSlave				(const char*, const char*);												//	Объявляем функцию для создания ведомого устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		createSlave				(String,      const char*);												//	Объявляем функцию для создания ведомого устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		createSlave				(const char*, String     );												//	Объявляем функцию для создания ведомого устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		createSlave				(String,      String     );												//	Объявляем функцию для создания ведомого устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		createMaster			(const char*, const char*);												//	Объявляем функцию для создания ведущего устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		createMaster			(String,      const char*);												//	Объявляем функцию для создания ведущего устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		createMaster			(const char*, String     );												//	Объявляем функцию для создания ведущего устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		createMaster			(String,      String     );												//	Объявляем функцию для создания ведущего устройства	(аргумент функции: имя ведомого устройства, пин код ведомого устройства)
		bool		checkConnect			(void);																	//	Объявляем функцию для проверки соединения
		void		end						(void);																	//	Объявляем функцию для разрыва соединения
		uint8_t		find					(uint8_t,     bool=false );												//	Объявляем функцию для обнаружения ведомых устройств	(аргумент функции: максимальное время поиска в секундах * 1.28, флаг поиска по интенсивности сигнала)
		char		findName[5][32];																				//	Объявляем массив строк с именами (или адресами) найденных устройств
		char		findAddr[5][15];																				//	Объявляем массив строк с адресами найденных устройств
		bool		available				(void*, void*);															//	Объявляем функцию для проверки наличия данных		(аргумент функции: вернуть кол-во принятых элементов массива, вернуть кол-во принятых байт массива)
		bool		available				(void* i     ){       int j; return available( i,&j);}					//	Объявляем функцию для проверки наличия данных		(аргумент функции: вернуть кол-во принятых элементов массива)
		bool		available				(void        ){int i; int j; return available(&i,&j);}					//	Объявляем функцию для проверки наличия данных		(без аргументов)
		template	<typename myType, size_t b> bool send (myType(&a)[b]){return funSend( a,b,sizeof(a));}			//	Определяем функцию с шаблоном для передачи массивов	(a: ссылка на массив, b: количество элементов в массиве, sizeof(a): размер всего массива)
		template	<typename myType>			bool send (myType(a)    ){return funSend(&a,0,sizeof(a));}			//	Определяем функцию с шаблоном для передачи перем-ых	(a: переменная,       b: 0,                              sizeof(a): размер переменной)
		template	<typename myType, size_t b>	bool read (myType(&a)[b]){return funRead( a,  sizeof(a));}			//	Определяем функцию с шаблоном для приёма массивов	(a: ссылка на массив, b: количество элементов в массиве, sizeof(a): размер всего массива)
		template	<typename myType>			bool read (myType(&a)   ){return funRead(&a,  sizeof(a));}			//	Определяем функцию с шаблоном для приёма переменных	(a: переменная,       b: 0,                              sizeof(a): размер переменной)

	private:
		char *		runATd	/* ATdouble */	(const char*, uint32_t=500, bool=true, bool=false, bool=false);			//	Объявляем функцию для двойного выполнения AT-команд	(аргумент функции: строка с АТ-командой, таймаут в миллисекундах, флаг разрешающий досрочный выход при успешном ответе модуля, флаг отправки команды без «1» на выводе pinKey, флаг дополнительной обработки ответа команд INQ или RNAME)
		char *		runATd	/* ATdouble */	(String,      uint32_t=500, bool=true, bool=false, bool=false);			//	Объявляем функцию для двойного выполнения AT-команд	(аргумент функции: строка с АТ-командой, таймаут в миллисекундах, флаг разрешающий досрочный выход при успешном ответе модуля, флаг отправки команды без «1» на выводе pinKey, флаг дополнительной обработки ответа команд INQ или RNAME)
		uint8_t		pinKey;																							//	Объявляем переменную для хранения № вывода K
		void *		objSerial;																						//	Объявляем указатель на объект работы с UART			(Serial, Serial1, ..., SoftwareSerial)
		char		strOut[64];																						//	Объявляем строку для вывода информации
		uint8_t		lenOut;																							//	Объявляем переменную длины строки вывода информации
		uint8_t		findDeviceSum;																					//	Объявляем переменную для хранения количества обнаруженных устройств
		bool		flgType;																						//	Объявляем флаг указывающий на тип соединения		(0-SoftwareSerial, 1-HardwareSerial)
		bool		flgBegin=false;																					//	Определяем флаг успешной инициализации				(1-OK, 0-Error)
		bool		flgMaster=false;																				//	Определяем флаг роли мастера						(1-Master, 0-Slave)
		bool		flgAvailable=false;																				//	Определяем флаг успешного приёма данных				(1-OK, 0-Error)
		bool		funSerialBegin			(void);																	//	Объявляем функцию инициализации модуля				(без аргументов)
		void		funSetLED				(void);																	//	Объявляем функцию управления светодиодами			(без аргументов)
		uint8_t		funFindArrName			(const char*);															//	Объявляем функцию поиска имени  в массиве findName	(аргумент функции: искомое имя)
		bool		funSend					(const void*, uint8_t, uint8_t);										//	Объявляем функцию передачи данных					(аргумент функции: указатель на массив или переменную, количество элементов в массиве или 0 для переменных, размер массива или переменной)
		bool		funRead					(void*, uint8_t);														//	Объявляем функцию получения данных					(аргумент функции: указатель на массив или переменную, размер массива или переменной на который указывает указатель)
		uint16_t	funcCRC16				(uint8_t);																//	Объявляем функцию получения CRC из строки strOut	(аргумент функции: количество учитываемых симолов строки)
		
};

#endif