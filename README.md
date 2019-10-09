[![](https://iarduino.ru/img/logo.svg)](https://iarduino.ru)[![](https://wiki.iarduino.ru/img/git-shop.svg?3)](https://iarduino.ru) [![](https://wiki.iarduino.ru/img/git-wiki.svg?2)](https://wiki.iarduino.ru) [![](https://wiki.iarduino.ru/img/git-lesson.svg?2)](https://lesson.iarduino.ru)[![](https://wiki.iarduino.ru/img/git-forum.svg?2)](http://forum.trema.ru)

# iarduino\_Bluetooth\_HC05

**This is a library for Arduino IDE. It allows to control [Bluetooth HC-05](https://iarduino.ru/shop/Expansion-payments/bluetooth-hc-05-trema-modul.html), UART modules made by iArduino.ru**

**Данная библиотека для Arduino IDE позволяет управлять модулями UART [Bluethooth HC-05](https://iarduino.ru/shop/Expansion-payments/bluetooth-hc-05-trema-modul.html) от [iArduino.ru](https://iarduino.ru)**

> Подробнее про установку библиотеки читайте в нашей [инструкции](https://wiki.iarduino.ru/page/Installing_libraries/).

> Подробнее про подключение к [Arduino UNO](https://iarduino.ru/shop/boards/arduino-uno-r3.html)/[Piranha UNO](https://iarduino.ru/shop/boards/piranha-uno-r3.html) читайте на нашей [wiki](https://wiki.iarduino.ru/page/trema-modul-bluetooth-hc-05/#h3_3)


| Модель | Ссылка на магазин |
|---|---|
| Bluetooth HC-05 <img src="https://wiki.iarduino.ru/img/resources/794/794.svg" width="100px"></img>| https://iarduino.ru/shop/Expansion-payments/bluetooth-hc-05-trema-modul.html |


## Назначение функций:

**Подробное описание работы с библиотекой и примеры смотрите на [нашем сайте]()**

**Подключение библиотеки**

```C++
#include <iarduino_Bluetooth_HC05.h> // Подключаем библиотеку для работы с модулем
iarduino_Bluetooth_HC05 ОБЪЕКТ( ВЫВОД_K ); // Создаём объект указывая вывод Arduino подключённый к выводу K модуля
```

**Инициализация работы** 

```C++
ОБЪЕКТ.begin( [ ОБЪЕКТ_UART ] ); // Инициализация работы с модулем по шине UART.
```

**Создание ведомого** 

```C++
ОБЪЕКТ.createSlave( ИМЯ , PIN-КОД ); // Создание ведомого (Slave) ожидающего подключение.
```

**Создание ведущего** 

```C++
ОБЪЕКТ.createMaster( ИМЯ , PIN-КОД ); // Создание ведущего (Master) с подключением к ведомому.
```

**Поиск устройств** 

```C++
ОБЪЕКТ.find( ВРЕМЯ , [ ТИП_ПОИСКА ] ); // Поиск устройств Bluetooth в радиусе действия.
```

**Разрыв подключения** 

```C++
ОБЪЕКТ.end(); // Разрыв подключения к внешнему Bluetooth устройству.
```

**Проверка подключения** 

```C++
ОБЪЕКТ.checkConnect(); // Проверка подключения к внешнему Bluetooth устройству.
```

**Отправка данных** 

```C++
ОБЪЕКТ.send( ДАННЫЕ ); // Отправка данных внешнему Bluetooth использующему эту библиотеку.
```

**Чтение данных** 

```C++
ОБЪЕКТ.read( ПЕРЕМЕННАЯ ); // Чтение данных от внешнего Bluetooth использующего эту библиотеку.
```

**Проверка наличия данных** 

```C++
ОБЪЕКТ.available( [ &ЭЛЕМЕНТЫ [ ,& БАЙТЫ ] ); // Проверка наличия принятых данных.
```

**Выполнение AT-команд**

```C++
ОБЪЕКТ.runAT( КОМАНДА [ , ВРЕМЯ [ , ВЫХОД ]] ); // Выполнение AT-команд и вывод результата.
```

**Результат AT-команды**

```C++
ОБЪЕКТ.flgResult // (int8_t) содержит результат AT-команды (-1: Error(*), 0: Неизвестно, 1: OK).
```

**Адреса найденных устройств**

```C++
Массив findAddr // (char*) содержит пять строк с адресами найденных Bluetooth устройств.
```

**Массив найденных устройств**

```C++
ОБЪЕКТ.findName // (char*) содержит пять строк с именами найденных Bluetooth устройств. Если имя устройства не получено, то вместо имени будет указан его адрес.
```

**Примечание:**

Данная библиотека работает только с Trema Bluetooth модулями HC-05.

Если Вы используете Trema Bluetooth модуль HC-05 для соединения со сторонними Bluetooth устройствами (не использующими данную библиотеку), то можно вызывать любые функции, кроме функций передачи и приёма данных: available(), read() и send() данной библиотеки. Вместо них используйте функции available(), read() и print(), println(), или write() класса Serial, или объекта UART указанного при инициализации.

Дело в том что указанные функции данной библиотеки дополнительно передают / принимают 4 байта (информация о типе данных, типе переменной или размере массива и CRC16 для проверки целостности принятых данных). Также функция available(), при наличии успешно принятых данных, отправляет передатчику сигнал подтверждения приёма, благодаря чему функция send() возвращает true или false.
