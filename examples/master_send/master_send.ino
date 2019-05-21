//  Данный скетч демонстрирует работу Trema-модуля bluetooth в режиме ведущего (master)
//  Модуль передаёт данные другому Trema-модулю bluetooth, который принимает данные (скетч slave_read)
//  Библиотека написана http://iarduino.ru только для Trema-модулей bluetooth и не будет работать с другими модулями

#include <SoftwareSerial.h>                // Подключаем библиотеку SoftwareSerial для общения с модулем по программной шине UART
#include <iarduino_Bluetooth_HC05.h>       // Подключаем библиотеку iarduino_Bluetooth_HC05 для работы с модулем
SoftwareSerial softSerial(2,3);            // Создаём объект softSerial указывая выводы RX, TX (можно указывать любые выводы Arduino UNO)
                                           // - назначенный вывод RX Arduino (в данном примере вывод 2) подключается к выводу TX модуля
                                           // - назначенный вывод TX Arduino (в данном примере вывод 3) подключается к выводу RX модуля
iarduino_Bluetooth_HC05 hc05(4);           // Создаём объект hc05 указывая любой вывод Arduino. Назначенный вывод подключается к выводу K модуля
int myArray[3];                            // Объявляем массив который будем передавать, можно создавать массивы или переменные любых типов, в т.ч. и char, но не более 54 байт
bool i;                                    // Объявляем переменную для вывода результатов работы функций

void setup(){
//  Инициируем передачу данных по аппаратной шине UART для вывода результата в монитор
    Serial.begin(9600);
//  Инициируем работу с модулем hc05, указывая объект UART через который осуществляется связь
    i=hc05.begin(softSerial);
    Serial.print("begin: "); if(i){Serial.println("Ok");} else {Serial.println("Error");}
//  Создаем ведущую роль модулю, указывая имя и pin-код ведомого к которому требуется подключиться (в примере имя = "MyName", pin-код = "4567")
    i=hc05.createMaster("MyName", "4567");
    Serial.print("create master: "); if(i){Serial.println("Ok");} else {Serial.println("Error");}
}                                        

//  Инициализация модуля hc05.begin(); и создание ведущей роли hc05.createMaster(); может занять несколько секунд, пока ведущий не подключится к ведомому
//  В этом примере модуль подключается через программный UART используя библиотеку SoftwareSerial, а при инициализации работы с модулем hc05.begin() указывается объект softSerial
//  Но модуль можно подключать и к аппаратному UART, тогда при инициализации работы с модулем hc05.begin() нужно указать Serial или (для ArduinoMega) Serial1, Serial2, Serial3.

void loop (){
//  Заполняем массив myArray какими либо данными
    myArray[0]=123;
    myArray[1]=456;
    myArray[2]=789;
//  Передаём данные массива myArray через Trema-модуль bluetooth с проверкой доставки
    i=hc05.send(myArray);
    Serial.print("send: "); if(i){Serial.println("Ok");} else {Serial.println("Error");}
//  Ждём 1 секунду
    delay(1000);
}

//  Результат выполнения функций можно и не отслеживать:
//  hc05.begin(softSerial);                // Выполнение функции без возврата результата в переменную i
//  hc05.createMaster("MyName", "4567"));  // Выполнение функции без возврата результата в переменную i
//  hc05.send(myArray);                    // Передача данных без проверки доставки

//  Trema-модуль в режиме ведущего (master) может не только отправлять, но и принимать данные функцией read().