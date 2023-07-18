ИНСТРУКЦИЯ:
I. Как интегрировать  GTest в проект Visual Studio. 

1. Скачать сам  GTest по следующей ссылке:
https://github.com/SkillfactoryCoding/CPLUS-UnitTest-googletest

2.Открываем VisualStudio Command Prompt ⟶ переходим в папку с исходниками GTest ⟶ создаём папку build и запускаем оттуда Cmake.

cd // «путь к папке с GTest»

//создаём папку build
mkdir build 

//переходим в неё
cd build

//запускаем cmake родительской папки
//все файлы необходимые для сборки будут помещены в текущую папку build
cmake ..
Cmake выведет в консоль примерно следующие сообщения:

C:\Program Files (x86)\Microsoft Visual Studio\2019\Community>cd /D D:\CODE\googletest\build\

D:\CODE\googletest\build>cmake ..
-- Building for: Visual Studio 16 2019
-- Selecting Windows SDK version 10.0.16299.0 to target Windows 10.0.18363.
-- The C compiler identification is MSVC 19.29.30037.0
-- The CXX compiler identification is MSVC 19.29.30037.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30037/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30037/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Could NOT find Python (missing: Python_EXECUTABLE Interpreter)
-- Looking for pthread.h
-- Looking for pthread.h - not found
-- Found Threads: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: D:/CODE/googletest/build


3.После того как Cmake выполнит все операции, в папке build окажется solution VisualStudio, который можно использовать для сборки библиотеки.
Открываем проект в VisualStudio, настраиваем необходимый тип выпуска (debug/release) и разрядность сборки (x32/x64), нажимаем build (F7). В результате сборки в папке build/lib окажутся несколько библиотек.

4.Подключается скомпилированная GTest к проекту так же как и любая другая внешняя библиотека.
Открываем настройки проекта, переходим в закладку С\С++/General ⟶ находим строчку «Additional Include Directories» ⟶ добавляем туда путь до папки include из скачанного нами репозитория:

5.Затем переходим во вкладку Linker/All Options ⟶ находим пункт «Additional Library Directories» ⟶ указываем там путь до папки lib, где лежат скомпилированные библиотеки:

6.Потом находим пункт «Additional Dependencies» и указываем там имена скомпилированных библиотек с расширениями:

7. Готово!

Для проверки успешности подключения всех необходимых зависимостей можно скомпилировать самый простой пример:

//=============================================
#include <gtest/gtest.h>

//---------------------------------------------
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
 }

[==========] Running 0 tests from 0 test suites.
[==========] 0 tests from 0 test suites ran. (0 ms total)
[  PASSED  ] 0 tests.


II. Как связать проект visual studio с MySQL

 Сначала необходимо создатиь в MySQL базу данных, к котороый мы планируем 
 подключаться и использовать в программе.
   
Теперь база данных готова к работе. Осталось настроить Visual Studio для работы с сервером MySQL:
  1. Запустите среду разработки Microsoft Visual C++ и создайте новый пустой проект "Консольное приложение Windows».
  2. Далее нужно открыть страницу свойств (Property Pages) с помощью главного меню (Проект->Свойства).
  3. В окне страницы свойств сначала добавьте к параметру "Дополнительные каталоги включаемых файлов" (Свойства конфигурации->С/С++->Общие) строку "C:\Program Files\MySQL\MySQL Server 8.0\include".
  4. Затем — к параметру "Дополнительные каталоги библиотек" (Свойства конфигурации->Компоновщик->Общие) строку "C:\Program Files\MySQL\MySQL Server 8.0\lib".
  5. Далее к параметру "Дополнительные зависимости" (Свойства конфигурации->Компоновщик->Ввод) добавьте значения libmysql.lib:

   Теперь можно использовать клиент консольного чата для работы с сервером MySQL

II. Пример подключения к базе данных без использования тестов::

   #include <iostream>
#include "mysql.h"
 
using namespace std;
 
int main() {
	MYSQL mysql; // Дескриптор соединения c MySql
	mysql_init(&mysql);
 
	if (&mysql == NULL) 	{
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "root", "root", "testdb", 0, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		// Если соединение успешно установлено выводим фразу — "Success!"
		cout << "Success!" << endl;
	}
 
	// Закрываем соединение с базой данных
	mysql_close(&mysql);
 
	system("Pause");
	return 0;
}


IV. Пример Mock 

// определяем интерфейс объекта, который далее будем использовать в алгоритме
class PainterInterface
{
public:
   PainterInterface()
       {}
   
   // как и положено, не забудем виртуальный деструктор
   virtual ~PainterInterface()
       {}


   // несколько виртуальных методов, которыми будем пользоваться
   virtual void drawLine(double x1, double y1, double x2, double y2) = 0;
   virtual void drawRect(double xtl, double ytl, double xbr, double ybr) = 0;
   virtual void drawEllipse(double centerx, double centery, double ar, double br) = 0;

   virtual void saveState() = 0;
   virtual void restoreState()=0;

};


// алгоритм, который в дальнейшем можно исследовать
// при помощи мок-объектов
void drawSomething(PainterInterface* painter)
{
// сохраняем состояние painter’а
   painter->saveState();

// рисуем нужную фигуру — 
// окружность радиуса 5 вписанную в квадрат
   painter->drawLine(0,0,0,10);
   painter->drawLine(0,10,10,10);
   painter->drawLine(10,10,10,0);
   painter->drawLine(10,0,0,0);

   painter->drawEllipse(5,5,5,5);

// восстанавливаем состояние painter’а
// СПЕЦИАЛЬНО закомментировали restoreState для демонстрации
//   painter->restoreState();
}

// определяем класс мок-объекта на основе нужного нам интерфейса
class MockPainter : public PainterInterface {
public:
// пользуемся макросами GTest для определения мок-методов в объекте
 MOCK_METHOD(void, drawLine, (double x1, double y1, double x2, double y2), (override));
 MOCK_METHOD(void, drawRect, (double xtl, double ytl, double xbr, double ybr), (override));
 MOCK_METHOD(void, drawEllipse, (double centerx, double centery, double ar, double br), (override));
 MOCK_METHOD(void, saveState, (), (override));
 MOCK_METHOD(void, restoreState, (), (override));
};


// наш тест
TEST(PainterTest, test1)
{
   MockPainter mpainter;


// указываем «ожидания» по использованию — методы будут вызваны хотя бы N раз
   EXPECT_CALL(mpainter, saveState).Times(::testing::AtLeast(1));
   EXPECT_CALL(mpainter, restoreState).Times(::testing::AtLeast(1));

   EXPECT_CALL(mpainter, drawLine).Times(::testing::AtLeast(2));

//указываем «ожидания» по использованию — методы будут вызваны точно N раз
   EXPECT_CALL(mpainter, drawEllipse).Times(1);
   EXPECT_CALL(mpainter, drawRect).Times(0);


// запускаем алгоритм на выполнение
   drawSomething(&mpainter);
}

int main(int argc, char *argv[])
{
// инициализация и запуск как обычно
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}