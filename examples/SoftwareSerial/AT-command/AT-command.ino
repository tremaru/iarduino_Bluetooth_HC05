//   Данный скетч демонстрирует работу Trema-модуля bluetooth HC-05.
//   Модуль выолняет AT-команды пользователя. Со списком AT-команд Вы можете ознакомиться на сайте https://wiki.iarduino.ru/page/at-komandy-bluetooth-hc-05/
//   Библиотека написана http://iarduino.ru только для Trema-модулей bluetooth HC-05 и не будет работать с другими модулями.
//   ==========================================================================================
     #define pinK  7                         // Любой вывод Arduino который подключается к выводу K модуля (вывод управления режимом команд).
     #define pinRX 8                         // Вывод Arduino RX подключаемый к выводу TX Trema-модуля bluetooth HC-05.
     #define pinTX 9                         // Вывод Arduino TX подключаемый к выводу RX Trema-модуля bluetooth HC-05.

     #include <SoftwareSerial.h>             // Подключаем библиотеку SoftwareSerial, до подключения библиотеки iarduino_Bluetooth_HC05.
     #include <iarduino_Bluetooth_HC05.h>    // Подключаем библиотеку iarduino_Bluetooth_HC05 для работы с модулем.
     SoftwareSerial softSerial(pinRX,pinTX); // Создаём объект softSerial указывая выводы RX, TX (можно указывать любые выводы Arduino UNO)
     iarduino_Bluetooth_HC05 hc05(pinK);     // Создаём объект hc05 указывая вывод Arduino подключённый к выводу K модуля.
     bool i;                                 // Объявляем переменную для вывода результатов работы функций.

void setup(){
//   Инициируем передачу данных по аппаратной шине UART для вывода результата в монитор.
     Serial.begin(9600);
//   Инициируем работу с модулем hc05, указывая объект UART через который осуществляется связь.
     i=hc05.begin(softSerial);               // Инициализация модуля может занять несколько секунд.
     Serial.print("begin: "); if(i){Serial.println("Ok");} else {Serial.println("Error");}
}                                        

void loop (){
//   Выполняем AT-команды используя функцию hc05.runAT().
//   Функция hc05.runAT() возвращает строку ответа модуля.
     Serial.println("------------------------");
     Serial.println(hc05.runAT("AT+NAME?\r\n"));                  // Получаем текущее имя модуля.
     Serial.println(hc05.runAT("AT+ADDR?\r\n"));                  // Получаем адрес модуля.
     Serial.println(hc05.runAT("AT+PSWD?\r\n"));                  // Получаем PIN-код модуля.
     Serial.println(hc05.runAT("AT+PSWD=1234\r\n"));              // Устанавливаем PIN-код модуля = 1234.
     Serial.println(hc05.runAT("AT+PSWD?\r\n"));                  // Получаем PIN-код модуля.
     Serial.println(hc05.runAT("AT+STATE?\r\n"));                 // Получаем состояние модуля.
//   Выполнять AT-команды можно не выводя ответ модуля, а лишь отслежывать результат их выполнения.
     hc05.runAT("AT+PSWD=6789\r\n");                              // Устанавливаем PIN-код модуля = 6789.
     if(hc05.flgResult < 0){Serial.println("Set PIN - Error"  );} // Если hc05.flgResult <0  значит в ответе модуля была строка с Error:(x).
     if(hc05.flgResult > 0){Serial.println("Set PIN - OK"     );} // Если hc05.flgResult >0  значит в ответе модуля была строка с OK\r\n.
     if(hc05.flgResult ==0){Serial.println("Set PIN - Unknown");} // Если hc05.flgResult ==0 значит в ответе модуля не было ни Error ни OK.
//   Ждём 5 секунд (можно и не ждать, если Вы быстро читаете d:ь)
     delay(5000);
}

//   Функция hc05.runAT(команда); может принимать и дополнительные параметры:
//   hc05.runAT(команда, [ таймаут [, флаг ]] );
//   - таймаут (по умолчанию 500мс) - количество миллисекунд в течении которых функция ждёт ответ от модуля.
//   - флаг    (по умолчанию true ) - установка данного флага разрешает досрочный выход (до истечения таймаута) при наличии успешного ответа от модуля.
