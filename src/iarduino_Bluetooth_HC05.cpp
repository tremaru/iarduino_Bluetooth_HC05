#include "iarduino_Bluetooth_HC05.h"

		iarduino_Bluetooth_HC05::iarduino_Bluetooth_HC05(uint8_t i){pinKey=i;}
//		ФУНКЦИЯ ВЫПОЛНЕНИЯ AT-КОМАНД:
char *	iarduino_Bluetooth_HC05::runAT(const char* i, uint32_t j, bool k, bool n, bool f){							//	Аргументы функции:	i - строка с АТ-командой,
//			Проверяем наличие и результат инициализации:															//						j - таймаут в миллисекундах,
			if(!flgBegin){return "Error(1F)\r\n";}																	//						k - флаг разрешающий досрочный выход при успешном ответе модуля,
//			Сбрасываем флаг успешного приёма данных:																//						n - флаг отправки команды без «1» на выводе pinKey)
			flgAvailable=false;																						//						f - флаг дополнительной обработки ответа команд INQ или RNAME
//			Определяем время выхода из функции:
			uint32_t timeOut = millis() + j;
//			Определяем необходимость дополнительной обработки:
			uint8_t t=0;
			if(f){
				if(!strncasecmp(i,"AT+INQ\r\n",8)){t=1;}															//	Если поступила команда "AT+INQ\r\n" c флагом f, значит результат должен сохраняться в массив findName
				if(!strncasecmp(i,"AT+RNAME? ",9)){t=2;}															//	Если поступила команда "AT+RNAME? " c флагом f, значит результат должен состоять из одной строки
			}	if(!strncasecmp(i,"AT+ROLE="  ,8)){t=3;}															//	Если поступила команда "AT+ROLE=...", значит библиотека или пользователь пытается изменить роль модуля
//			Сбрасываем флаг наличия результата в ответе:
			flgResult=0;
//			Устанавливаем «1» или «0» на выводе pinKey (зависит от флага n):
			digitalWrite(pinKey, !n);
//			Чистим буфер UART:
			if(flgType)	{while((*(HardwareSerial*)objSerial).available()>0){(*(HardwareSerial*)objSerial).read();}}
			else		{while((*(SOFTwareSerial*)objSerial).available()>0){(*(SOFTwareSerial*)objSerial).read();}}
//			Передаём AT-команду:
			if(flgType)	{(*(HardwareSerial*)objSerial).write(i);}
			else		{(*(SOFTwareSerial*)objSerial).write(i);}
//			Чистим строку strOut и её длину lenOut:
			memset(strOut,0,64);
			lenOut=0;
//			Читаем ответ модуля в строку strOut:
			while(millis()<timeOut && lenOut<64){
//				Читаем очередной символ:
				if(flgType)	{if((*(HardwareSerial*)objSerial).available()>0){strOut[lenOut]=(*(HardwareSerial*)objSerial).read(); lenOut++;}}
				else		{if((*(SOFTwareSerial*)objSerial).available()>0){strOut[lenOut]=(*(SOFTwareSerial*)objSerial).read(); lenOut++;}}
//				Перехватываем прочитанные данные для их дополнительной обработки:
				if(t==1){																							//	Если параметр "t" равен 1 значит данные нужно сохранить в массив findName
					if(lenOut>1){																					//	Если в ответе больше 1 символа
						if(findDeviceSum<5){																		//	Если найдено не более 5 строк с адресами устройств
							if(!strncasecmp(&strOut[lenOut-2],"\r\n",2)){											//	Если последние символы ответа = "\r\n"
								if(!strncasecmp(strOut,"+INQ:",5)){													//	Если первые символы ответа новой строки = "+INQ:"
									for(uint8_t n=5; n<lenOut; n++){												//	Проходим по всем символам адреса устройства
										if(strOut[n]==','){strOut[n]=0; lenOut=n; n=255;}							//	Если встретился символ запятая, значит символы адреса закончились (дальше идут символы типа и уровня приёма), устанавливаем символ конца строки
									}	strncpy( findName[findDeviceSum], &strOut[5], (lenOut-5) );					//	Копируем строку начиная с 5 символа в очередной элемент массива findName
										findDeviceSum++;															//	Увеличиваем количество найденных устройств
										flgResult=1;																//	Устанавливаем флаг наличия результата в ответе
									if(funFindArrName(findName[findDeviceSum-1])<findDeviceSum){					//	Если найденный адрес был ранее найден (повторное обнаружение того же модуля)
										findDeviceSum--;															//	Уменьшаем количество найденных устройств
										memset(findName[findDeviceSum],0,32);										//	Чистим ошибочно заполненный элемент массива findName
									}
								}		lenOut=0;																	//	Обнуляем общее количество принятых символов
										memset(strOut,0,64);														//	Чистим строку strOut
							}
						}
					}
				}else if(t==2){																						//	Если параметр "t" равен 2 значит данные должны состоять из одной строки
					if(!strncasecmp(&strOut[lenOut-2],"\r\n",2)){													//	Если последние символы ответа = "\r\n"
						if(!strncasecmp(strOut,"+RNAME:",7)){														//	Если первые символы ответа новой строки = "+RNAME:"
							timeOut=millis();																		//	Выходим из цикла чтения ответа
							lenOut-=2;																				//	Возвращаем длину строки на два символа назад (убираем "\r\n")
							flgResult=1;																			//	Устанавливаем флаг наличия результата в ответе
						}else{
							lenOut=0;																				//	Обнуляем общее количество принятых символов
							memset(strOut,0,64);																	//	Чистим строку strOut
						}
					}
				}
//				Выходим из цикла чтения:
				if(k){																								//	Если установлен флаг "k" разрешающий выйти из функции до таймаута при наличии положительного ответа модуля
					if(lenOut>3){																					//	Если в ответе более 3 символов
						if(!strncasecmp(&strOut[lenOut-4],"OK\r\n" ,4)){flgResult= 1; timeOut=millis();}			//	Если последние символы ответа = "OK\r\n",  то устанавливаем флаг наличия результата в ответе в состояние +1 и выходим из цикла чтения ответа
						if(!strncasecmp(&strOut[lenOut-7],"Error:(",7)){flgResult=-1;}								//	Если последние символы ответа = "Error:(", то устанавливаем флаг наличия результата в ответе в состояние -1
					}
				}
			}
//			Добавляем символ \0 в строку strOut, сразу после последнего символа ответа модуля
			strOut[lenOut]=0;
//			Устанавливаем «0» на выводе pinKey:
			digitalWrite(pinKey,LOW);
//			Возвращаем ответ модуля
			return strOut;
}

//		ФУНКЦИЯ СОЗДАНИЯ ВЕДОМОГО УСТРОЙСТВА:
bool	iarduino_Bluetooth_HC05::createSlave(const char* i, const char* j){											//	Аргументы функции:	i - имя     ведомого устройства
//			Проверяем наличие и результат инициализации:															//						j - пин код ведомого устройства
			if(!flgBegin){return false;}
//			Сбрасываем флаг успешного приёма данных:
			flgAvailable=false;
//			Переводим модуль в режим ведомого:
			end();
			runATd((String)F("AT+NAME=")+i+F("\r\n"));																//	Отправляем команду модулю - запомнить имя
			runATd((String)F("AT+PSWD=")+j+F("\r\n"));																//	Отправляем команду модулю - запомнить pin-код
			runATd(F("AT+ROLE=0\r\n"));																				//	Отправляем команду модулю - установить роль ведомого
			runAT (F("AT+RESET\r\n"),500,1,1); delay(1000);															//	Отправляем команду модулю - перезагрузиться (таймаут выполнения 500мс, при успехе разрешаем выйти раньше, команда подаётся без «1» на выводе pinKey)
			runATd(F("AT\r\n"));																					//	Отправляем команду модулю - тест
//			Устанавливаем уровни для светодиодов:
			flgMaster=false;																						//	Сбрасываем флаг flgMaster
			funSetLED(); delay(100); funSetLED();																	//	Функция включает светодиоды в соответствии со значением флага flgMaster.
			return true;
}

//		ФУНКЦИЯ СОЗДАНИЯ ВЕДУЩЕГО УСТРОЙСТВА С ПОДКЛЮЧЕНИЕМ К ВЕДОМОМУ:
bool	iarduino_Bluetooth_HC05::createMaster(const char* i, const char* j){										//	Аргументы функции:	i - имя     ведомого устройства к которому требуется подключиться
//			Проверяем наличие и результат инициализации:															//						j - пин код ведомого устройства к которому требуется подключиться
			if(!flgBegin){return false;}
//			Сбрасываем флаг успешного приёма данных:
			flgAvailable=false;
//			Находим устройство с именем i в зоне действия (переведя модуль в режим мастера)
			int f=5, n=0;																							//	f - количество попыток поиска всех устройств, n - результат обнаружения устройства с именем i среди всех найденных findName
			do{find(10); f--; n=funFindArrName(i);}while(!n && f);													//	Ищем все ведомые устройства в зоне действия в течение 10 секунд и пытаемся найти среди них устройство с именем i, а с каждым проходом уменьшаем значение f
			if(!n){return false;}																					//	Выходим из функции если устройство не найдено за 5 попыток
//			Подключаемся к ведомому устройству (Имя: i или findName[n-1], Адрес: findAddr[n-1], Пин: j)
			runATd((String)F("AT+PSWD=")+j+F("\r\n"));																//	Отправляем команду модулю - запимнить pin-код ведомого устройства
			runATd((String)F("AT+PAIR=")+findAddr[n-1]+F(",10\r\n"));												//	Отправляем команду модулю - образовать пару с ведомым устройством, указав его адрес findAddr[n-1] и таймаут 10 сек
			runATd((String)F("AT+BIND=")+findAddr[n-1]+F("\r\n"));													//	Отправляем команду модулю - установить фиксированный адрес для подключения findAddr[n-1]
			runATd(F("AT+CMODE=0\r\n"));																			//	Отправляем команду модулю - подключаться только к фиксированному адресу
			runAT (F("AT+RESET\r\n"),500,1,1);  delay(1000);														//	Отправляем команду модулю - перезагрузиться (таймаут выполнения 500мс, при успехе разрешаем выйти раньше, команда подаётся без «1» на выводе pinKey)
			runATd(F("AT\r\n"));																					//	Отправляем команду модулю - тест
			runATd((String)F("AT+LINC=")+findAddr[n-1]+F("\r\n")); delay(15000);									//	Отправляем команду модулю - подключиться к ведомому устройству с адресом findAddr[n-1]
//			Проверяем соединение
			if(checkConnect()){return true;}	delay(15000);														//	Если соединение установлено то возвращаем true иначе ждём 15 секунд
			if(checkConnect()){return true;}	delay(20000);														//	Если соединение установлено то возвращаем true иначе ждём 20 секунд
			runAT (F("AT+RESET\r\n"),500,1,1);	delay(5000);														//	Отправляем команду модулю - перезагрузиться (таймаут выполнения 500мс, при успехе разрешаем выйти раньше, команда подаётся без «1» на выводе pinKey)
			runATd(F("AT\r\n"));																					//	Отправляем команду модулю - тест
			if(checkConnect()){return true;}	delay(25000);														//	Если соединение установлено то возвращаем true иначе ждём 25 секунд
			if(checkConnect()){return true;}																		//	Если соединение установлено то возвращаем true
//			Устанавливаем уровни для светодиодов:
			flgMaster=true;																							//	Устанавливаем флаг flgMaster
			funSetLED(); delay(100); funSetLED();																	//	Функция включает светодиоды в соответствии со значением флага flgMaster.
			return false;
}

//		ФУНКЦИЯ ПРОВЕРКИ СОЕДИНЕНИЯ:
bool	iarduino_Bluetooth_HC05::checkConnect(void){																//	Аргументы функции:	отсутствуют
//			Проверяем наличие и результат инициализации:															//
			if(!flgBegin){return 0;}
//			Сбрасываем флаг успешного приёма данных:
			flgAvailable=false;
//			Устанавливаем уровни для светодиодов:
			funSetLED();																							//	Функция включает светодиоды в соответствии со значением флага flgMaster.
//			Проверяем состояние модуля:
			runATd(F("AT+STATE?\r\n"));																				//	Отправляем команду модулю - вернуть состояние.
			if(strncasecmp(strOut,"+STATE:CONNECTED",16)){return false;}else{return true;}							//	Если состояние CONNECTED то возвращаем true иначе false
}

//		ФУНКЦИЯ ОБНАРУЖЕНИЯ ВЕДОМЫХ УСТРОЙСТВ:
uint8_t	iarduino_Bluetooth_HC05::find(uint8_t i, bool j){															//	Аргументы функции:	i - максимальное время поиска в секундах * 1.28
			if(i>48){i=48;}																							//						j - флаг поиска по интенсивности сигнала
//			Проверяем наличие и результат инициализации:
			if(!flgBegin){return 0;}
//			Сбрасываем флаг успешного приёма данных:
			flgAvailable=false;
//			Чистим массивы findName, findAddr и количество найденных устройств findDeviceSum:
			findDeviceSum=0;
			memset(findName,0,(32*5));
			memset(findAddr,0,(15*5));
//			Переводим модуль в режим ведущего:
			end();
			runATd(F("AT+ROLE=1\r\n"));																				//	Отправляем команду модулю - установить роль ведущего
			runAT (F("AT+RESET\r\n"),500,1,1); delay(1000);															//	Отправляем команду модулю - перезагрузиться (таймаут выполнения 500мс, при успехе разрешаем выйти раньше, команда подаётся без «1» на выводе pinKey)
			runATd(F("AT\r\n"));																					//	Отправляем команду модулю - тест
//			Ищем адреса ведомых устройств в зоне действия и сохраняем их в массив findName:
			runATd(F("AT+INQC\r\n"));																				//	Отправляем команду модулю - завершить поиск устройств
			runATd((String)F("AT+INQM=")+uint8_t(j)+F(",5,")+i+F("\r\n"));											//	Отправляем команду модулю - установить режим поиска: сортировать по j, не более 5 устройств, не дольше i секунд
			runAT (F("AT+INQ\r\n"),((uint32_t)i*1300),0,0,1);														//	Отправляем команду модулю - начать поиск устройств (таймаут выполнения i*1300мс, запрещаем выйти раньше, запрещаем отправку без «1» на выводе pinKey, разрешаем сохранить результат в массив findName)
			runATd(F("AT+INQC\r\n"));																				//	Отправляем команду модулю - завершить поиск устройств
//			Ищем имена ведомых устройств по их адресу и заменяем адрес на имя в массиве findName:
			if(findDeviceSum){																						//	Если в массиве findName есть найденные адреса
				for(uint8_t l=0; l<findDeviceSum; l++){																//	Проходим по всем найденным адресам
//					Получаем строку с очередным адресом устройства в findAddr в формате NAP,UAP,LAP:
					for(uint8_t n=0; n<15; n++){																	//	Проходим по всем символам адреса устройства (который пока находится в массиве findName)
						findAddr[l][n]=findName[l][n];																//	Копируем символы из элемента массива findName в элемент массива findAddr
						if(findAddr[l][n]==':'){findAddr[l][n]=',';}												//	Заменяем символ двоеточие на запятую, это разделитель адреса NAP:UAP:LAP
					}	findAddr[l][14]=0;																			//	Ставим символ конца строки, если он не встретился
//					Получаем имя очередного устройства по его адресу в strOut
					runAT(((String)F("AT+RNAME? ")+findAddr[l]+F("\r\n")),20000,0,0,1);								//	Отправляем команду модулю - вернуть имя устройства по его адресу (таймаут выполнения 20000мс, запрещаем выйти раньше, запрещаем отправку без «1» на выводе pinKey, разрешаем сохранить результат в одну строку и выйти раньше при наличии имени в ответе)
					if(flgResult){																					//	Если имя прочитано
						if(lenOut<39){																				//	Если имя не длиннее чем 32 символа + 7 символов "+RNAME:"
							strcpy(findName[l],&strOut[7]);															//	Копируем имя на место адреса в массив findName, иначе в массиве так и останется адрес
						}
						runATd(F("AT\r\n"));																		//	Отправляем команду модулю - тест. Без этой команды модуль не всегда отправляет имя устройства при l>0.
					}
				}
			}
//			Устанавливаем уровни для светодиодов:
			funSetLED(); delay(100); funSetLED();																	//	Функция включает светодиоды в соответствии со значением флага flgMaster.
//			Возвращаем результат:
			return findDeviceSum;																					//	Возвращаем количество найденных устройств
}

//		ФУНКЦИЯ РАЗРЫВА СОЕДИНЕНИЯ:
void	iarduino_Bluetooth_HC05::end(void){
//			Проверяем наличие и результат инициализации:
			if(!flgBegin){return;}
//			Сбрасываем флаг успешного приёма данных:
			flgAvailable=false;
//			Читаем роль модуля в переменную i:
			char i = runATd(F("AT+ROLE?\r\n"))[6];
//			Удаляем настройки и спаренные устройства:
			runATd(F("AT+DISC\r\n"));																				//	Отправляем команду модулю - отключиться
			runATd(F("AT+ORGL\r\n"));																				//	Отправляем команду модулю - сброс настроек (CLASS=0; IAC=9E8B33; ROLE=0; CMODE=0; UART=38400,0,0; PSWD=1234; NAME=hc01.com HC-05)
			runATd(F("AT+RMAAD\r\n"));																				//	Отправляем команду модулю - очистить список авторизованных устройств
			runATd(F("AT+BIND=0,0,0\r\n"));																			//	Отправляем команду модулю - установить фиксированный адрес для подключения NAP=0, UAP=0, LAP=0
			runATd(F("AT+CMODE=0\r\n"));																			//	Отправляем команду модулю - подключаться только к фиксированному адресу
			runATd(F("AT+NAME=iArduino\r\n"));																		//	Отправляем команду модулю - запомнить имя
			runATd(F("AT+PSWD=12345678\r\n"));																		//	Отправляем команду модулю - запомнить pin-код
			runATd((String)F("AT+ROLE=")+i+F("\r\n"));																//	Отправляем команду модулю - установить ранее прочитанную роль
//			Перезагружаем модуль:
			runAT (F("AT+RESET\r\n"),500,1,1); delay(1000);															//	Отправляем команду модулю - перезагрузиться (таймаут выполнения 500мс, при успехе разрешаем выйти раньше, команда подаётся без «1» на выводе pinKey)
			runATd(F("AT\r\n"));																					//	Отправляем команду модулю - тест
//			Устанавливаем уровни для светодиодов:
			funSetLED(); delay(100); funSetLED();																	//	Функция включает светодиоды в соответствии со значением флага flgMaster.
}

//		ФУНКЦИЯ ПЕРЕДАЧИ ДАННЫХ
bool	iarduino_Bluetooth_HC05::funSend(const void* i, uint8_t j, uint8_t k){										//	Аргументы функции:	i - указатель на массив или переменную
//			Проверяем наличие и результат инициализации:															//						j - количество элементов в массиве или 0 для переменных
			if(!flgBegin){return false;}																			//						k - размер массива или переменной
//			Сбрасываем флаг успешного приёма данных:
			flgAvailable=false;
//			Преобразуем тип указателя на однобайтный беззнаковый (uint8_t*)
			const uint8_t* l = reinterpret_cast<const uint8_t*>(i);
//			Создаём строку для передачи
			strOut[0]=j;
			strOut[1]=k;
			for(int n=0; n<k; n++){strOut[n+2]=l[n];}
//			Получаем циклически избыточный код из строки strOut
			uint16_t c=funcCRC16(k+2);
//			Добавляем полученный код в конец строки strOut
			strOut[k+2]=highByte(c);
			strOut[k+3]=lowByte(c);
//			Чистим буфер UART:
			if(flgType)	{while((*(HardwareSerial*)objSerial).available()>0){(*(HardwareSerial*)objSerial).read();}}
			else		{while((*(SOFTwareSerial*)objSerial).available()>0){(*(SOFTwareSerial*)objSerial).read();}}
//			Передаём строку:
			for(int n=0; n<k+4; n++){
				if(flgType)	{(*(HardwareSerial*)objSerial).write(strOut[n]);}
				else		{(*(SOFTwareSerial*)objSerial).write(strOut[n]);}
			}
//			Запрашиваем подтверждение приёма:
			runAT("ACK\r\n",1000,1,1);
			if(flgResult < 1){return false;}else{return true;}
}

//		ФУНКЦИЯ ПОЛУЧЕНИЯ ДАННЫХ
bool	iarduino_Bluetooth_HC05::funRead(void* i, uint8_t j){														//	Аргументы функции:	i - указатель на массив или переменную
//			Проверяем наличие и результат инициализации:															//						j - размер массива или переменной
			if(!flgBegin){return false;}
//			Проверяем флаг успешного приёма данных:
			if(!flgAvailable){return false;}
//			Сбрасываем флаг успешного приёма данных:
			flgAvailable=false;
//			Преобразуем тип указателя на однобайтный беззнаковый (uint8_t*):
			uint8_t* l = reinterpret_cast<uint8_t*>(i);
//			Кпируем данные в указатель:
			memcpy(l,&strOut[2],min(j,uint8_t(strOut[1])));
			return true;
}

//		ФУНКЦИЯ СОЗДАНИЯ ЦИКЛИЧЕСКИ ИЗБЫТОЧНОГО КОДА ИЗ ДАННЫХ В СТРОКЕ strOut
uint16_t iarduino_Bluetooth_HC05::funcCRC16(uint8_t i){																//	Аргументы функции:	i - размер передаваемых данных
		uint16_t c=0xFFFF;																							//	предустанавливаем CRC в значение 0xFFFF
		for(uint8_t a=0; a<i; a++){ c^=strOut[a];																	//	проходим по строке strOut и выполняем операцию XOR между CRC и очередным байтом массива
		for(uint8_t b=0; b<8; b++){ if(c & 0x01){c>>=1; c^=0xA001;}else{c>>=1;}										//	проходим по битам байта       и сдвигаем значение CRC на 1 байт вправо, если младший (сдвинутый) байт был равен 1, то выполняем операцию XOR между CRC и полиномом 0xA001
		}} return c;
}

//		ФУНКЦИЯ ПРОВЕРКИ НАЛИЧИЯ ПРИНЯТЫХ ДАННЫХ
bool	iarduino_Bluetooth_HC05::available(void* i,void* j){														//	Аргументы функции:	i - указатель на переменную в которую требуется вернуть количество элементов в принятом массиве или 0 если приянята переменная
			flgAvailable=false;																						//						j - указатель на переменную в которую требуется вернуть размер принятого массива или принятой переменной
//			Проверяем наличие и результат инициализации:
			if(!flgBegin){return false;}
//			Чистим строку strOut и её длину lenOut:
			memset(strOut,0,64);
			lenOut=0;
			bool f=1;
			while(f && lenOut<64){
//				Читаем данные побайтно в строку strOut, сохраняя количество прочтённых байт в переменную lenOut:
						f=0;
						if(flgType)	{if((*(HardwareSerial*)objSerial).available()>0){f=1; strOut[lenOut]=(*(HardwareSerial*)objSerial).read(); lenOut++;}}
						else		{if((*(SOFTwareSerial*)objSerial).available()>0){f=1; strOut[lenOut]=(*(SOFTwareSerial*)objSerial).read(); lenOut++;}}
				if(!f){ delay(20);																					//	Если данные закончились, не радуемся))), ждём 20 мс и проверяем еще раз
						if(flgType)	{if((*(HardwareSerial*)objSerial).available()>0){f=1; strOut[lenOut]=(*(HardwareSerial*)objSerial).read(); lenOut++;}}
						else		{if((*(SOFTwareSerial*)objSerial).available()>0){f=1; strOut[lenOut]=(*(SOFTwareSerial*)objSerial).read(); lenOut++;}}
				}
			}																										//	Строка strOut должна состоять из следующих полученных байтов: [1 байт: кол-во элементов массива][1 байт: кол-во байт массива][n байт: байты массива][2 байта: CRC][5 байт: запрос подтверждения "ACK\r\n"]
//			Проверяем размер полученных данных
			if(lenOut<5)					{return false;}															//	Если получено меньше 5 байт, значит это шум
			if(lenOut<uint8_t(strOut[1])+9)	{return false;}															//	Если получено меньше байт чем значение во втором байте (кол-во байт в массиве) + 9 байт (2 первых, 2 CRC и 5 "ACK\r\n"), значит это не данные
//			Получаем циклически избыточный код из строки strOut
			uint16_t c=funcCRC16(uint8_t(strOut[1])+2);																//	Участвуют только первые байты: 2 первых байта strOut[0] и strOut[1] и все байты массива, количество которых указано в strOut[1]
//			Сравниваем циклически избыточный код с пришедшими байтами CRC:
			if(uint8_t(strOut[uint8_t(strOut[1])+2])!=highByte(c)){return false;}									//	Если не совпал старший байт CRC, значит данные пришли с ошибкой
			if(uint8_t(strOut[uint8_t(strOut[1])+3])!=lowByte (c)){return false;}									//	Если не совпал младший байт CRC, значит данные пришли с ошибкой
//			Возвращаем количество принятых элементов массива i и количество байт массива j
			*(int*)i=int(strOut[0]);
			*(int*)j=int(strOut[1]);
//			Передаём ответ передатчику об успешном приёме данных:
			if(flgType)	{(*(HardwareSerial*)objSerial).write("OK\r\n");}
			else		{(*(SOFTwareSerial*)objSerial).write("OK\r\n");}
//			Устанавливаем флаг успешного приёма данных и возвращаем true
			flgAvailable=true;
			return true;
}
//		ФУНКЦИЯ УПРАВЛЕНИЯ СВЕТОДИОДАМИ SLAVE И MASTER:
void	iarduino_Bluetooth_HC05::funSetLED(void){																	//	Аргументы функции:	отсутствуют. Установка происходит по значению переменной flgMaster
//			Проверяем наличие и результат инициализации:
			if(!flgBegin)	{return;}
//			Устанавливаем уровни для светодиодов:
			if(flgMaster)	{runATd(F("AT+PIO=2,0\r\n")); runATd(F("AT+PIO=3,1\r\n"));}
			else			{runATd(F("AT+PIO=2,1\r\n")); runATd(F("AT+PIO=3,0\r\n"));}
}
//		ФУНКЦИЯ ПОИСКА ИМЕНИ В МАССИВЕ findName
uint8_t	iarduino_Bluetooth_HC05::funFindArrName	(const char* name)									{for(uint8_t i=0; i<findDeviceSum; i++){if(!strcasecmp(findName[i], name)){return i+1;}} return 0;	}
//		ФУНКЦИИ ДУБЛЁРЫ:
char *	iarduino_Bluetooth_HC05::runAT			(String i, uint32_t j, bool k, bool n, bool f)		{char _i[32]; _i[31]=0; strncpy( _i, i.c_str(), i.length()+1 ); return runAT(_i, j, k, n, f);		}
char *	iarduino_Bluetooth_HC05::runATd			(String i, uint32_t j, bool k, bool n, bool f)		{char _i[32]; _i[31]=0; strncpy( _i, i.c_str(), i.length()+1 ); return runATd(_i, j, k, n, f);		}
char *	iarduino_Bluetooth_HC05::runATd			(const char* i, uint32_t j, bool k, bool n, bool f)	{runAT(i,j,k,n,f); if(flgResult < 1){runAT(i,j,k,n,f);} 		return strOut;						}

bool	iarduino_Bluetooth_HC05::createSlave	(String i, const char* j)							{char _i[16]; _i[15]=0; strncpy( _i, i.c_str(), i.length()+1 ); return createSlave(_i, j);			}
bool	iarduino_Bluetooth_HC05::createSlave	(const char* i, String j)							{char _j[16]; _j[15]=0; strncpy( _j, j.c_str(), j.length()+1 ); return createSlave( i,_j);			}
bool	iarduino_Bluetooth_HC05::createSlave	(String i, String j)								{char _i[16]; _i[15]=0; strncpy( _i, i.c_str(), i.length()+1 );
																									 char _j[16]; _j[15]=0; strncpy( _j, j.c_str(), j.length()+1 ); return createSlave(_i,_j);			}
bool	iarduino_Bluetooth_HC05::createMaster	(String i, const char* j)							{char _i[16]; _i[15]=0; strncpy( _i, i.c_str(), i.length()+1 ); return createMaster(_i, j);			}
bool	iarduino_Bluetooth_HC05::createMaster	(const char* i, String j)							{char _j[16]; _j[15]=0; strncpy( _j, j.c_str(), j.length()+1 ); return createMaster( i,_j);			}
bool	iarduino_Bluetooth_HC05::createMaster	(String i, String j)								{char _i[16]; _i[15]=0; strncpy( _i, i.c_str(), i.length()+1 );
																									 char _j[16]; _j[15]=0; strncpy( _j, j.c_str(), j.length()+1 ); return createMaster(_i,_j);			}