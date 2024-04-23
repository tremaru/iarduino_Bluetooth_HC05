//   Данный скетч демонстрирует работу Trema-модуля bluetooth HC-05 в режиме ведущего (master).
//   Модуль передаёт данные другому Trema-модулю bluetooth, который принимает данные (скетч slave_read).
//   Библиотека написана http://iarduino.ru только для Trema-модулей bluetooth HC-05 и не будет работать с другими модулями.
//   ==========================================================================================
     #define pinK 7                          // Любой вывод Arduino который подключается к выводу K модуля (вывод управления режимом команд).

     #include <iarduino_Bluetooth_HC05.h>    // Подключаем библиотеку iarduino_Bluetooth_HC05 для работы с модулем.
     iarduino_Bluetooth_HC05 hc05(pinK);     // Создаём объект hc05 указывая вывод Arduino подключённый к выводу K модуля.
     int myArray[3];                         // Объявляем массив в который будем передавать, можно создавать массивы или переменные любых типов, в т.ч. и char, но не более 54 байт.
     int i;                                  // Объявляем переменную для вывода результатов работы функций.

void setup(){
//   Инициируем передачу данных по аппаратной шине UART для вывода результата в монитор.
     Serial.begin(9600);
//   Инициируем работу с модулем hc05, указывая объект UART через который осуществляется связь.
     i=hc05.begin(Serial1);                  // Инициализация модуля может занять несколько секунд.
     Serial.print("begin: "); if(i){Serial.println("Ok");} else {Serial.println("Error");}
//   Создаем ведущую роль модулю, указывая имя и pin-код ведомого к которому требуется подключиться (в примере имя = "MyName", pin-код = "4567").
     i=hc05.createMaster("MyName", "4567");  // Функция создаёт роль ведущего (Master) и ждёт сопряжение с ведомым. Если соединение не установлено в течении 1 мин, возвращает false.
     Serial.print("create master: "); if(i){Serial.println("Ok");} else {Serial.println("Error");}
}                                        

void loop (){
//   Заполняем массив myArray какими либо данными:
     myArray[0]=123;
     myArray[1]=456;
     myArray[2]=789;
//   Передаём данные массива myArray через Trema-модуль bluetooth с проверкой доставки:
     i=hc05.send(myArray);
     Serial.print("send: "); if(i){Serial.println("Ok");} else {Serial.println("Error");}
//   Ждём 1 секунду:
     delay(1000);
}

//   Результат выполнения функций можно и не отслеживать:
//   hc05.begin(softSerial);                 // Выполнение функции без возврата результата в переменную i.
//   hc05.createMaster("MyName", "4567"));   // Выполнение функции без возврата результата в переменную i.
//   hc05.send(myArray);                     // Передача данных без проверки доставки.
//   Trema-модуль в режиме ведущего (master) может не только отправлять, но и принимать данные функцией read().
//   Наличие принятых данных можно проверить функцией available(), как в скетче slave_read.
