#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "mysql.h"
using namespace std;
// подключаем библеотеку для тестирования, подробнее об её
// установке в файле Readme
// Так же подключаем  mysql.h для возможности подклбчения к базам данных
// предполагается что уже имеется база данных  testdb

class DBConnection { // класс подключения к базе данных
public:
    void openDB(const char* Login, const char* Password);
    void closeDB();
    void execQueryDB();

};

void  DBConnection::openDB(const char* Login, const char* Password) { // Функция для инициализации соединения с бахой данных (насколько я понял подразумевается именно это)
    MYSQL mysql; // Дескриптор соединения c MySql
    mysql_init(&mysql);
   

    if (&mysql == NULL) {
        // Если дескриптор не получен — выводим сообщение об ошибке
        cout << "Error: can't create MySQL-descriptor" << endl;
    }
    
    // Подключаемся к серверу
    //if (!mysql_real_connect(&mysql, "localhost", "root", "root", "testdb", 0, NULL, 0)) {
    if (!mysql_real_connect(&mysql, "localhost",  Login, Password, "testdb", 0, NULL, 0)) {
        // Если нет возможности установить соединение с БД выводим сообщение об ошибке
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
    }
    else {
        // Если соединение успешно установлено выводим фразу — "Success!"
        cout << "Success!" << endl;
    }

}

void  DBConnection::closeDB() { // Функция завершения работы с базой данных
    // Закрываем соединение с базой данных
    MYSQL mysql;  // добавил сюда для того чтобы  не было проблем с инициализацией
    mysql_init(&mysql);
    mysql_close(&mysql);
    system("Pause");
}

void  DBConnection::execQueryDB() { // Функция запроса в бае данных
    MYSQL mysql;  // добавил сюда для того чтобы  не было проблем с инициализацией
    mysql_init(&mysql);
    MYSQL_RES* res;
    MYSQL_ROW row;
    int i = 0;
    mysql_query(&mysql, "SELECT * FROM test"); //Делаем запрос к таблице

    //Выводим все что есть в базе через цикл
    if (res = mysql_store_result(&mysql)) {
        while (row = mysql_fetch_row(res)) {
            for (i = 0; i < mysql_num_fields(res); i++) {
                cout << row[i] << "  ";
            }
            cout << endl;
        }
    }
    else
        cout << "Ошибка MySql номер " << mysql_error(&mysql);
}

class ClassThatUsesDB : DBConnection { // интерфейс
public:
    ClassThatUsesDB(){};
    virtual  ~ClassThatUsesDB() {};
    
   virtual void openConnection(const char* Login, const char* Password);
   virtual void useConnection();
   virtual void closeonnection();

};

 void  ClassThatUsesDB::openConnection( const char* Login, const char* Password) { // Фукнция попытки подключения к базе
     cout << "Connection attempt " << endl;
     const char* Login = "root";
     const char* password = "root2";
     openDB(Login, Password);
}

void  ClassThatUsesDB::useConnection() { // Функция использования базы данных
    execQueryDB();
}

void  ClassThatUsesDB::closeonnection() { // Функция закрытия подключения к базе данных
    closeDB();
}

class MockTesting : public ClassThatUsesDB, public DBConnection {
public:
    // Столкнулся здесь с очень странными ошибками, не понимаю  в чем дело
    MOCK_METHOD(void openConnection(const char* Login, const char* Password), (override));
    MOCK_METHOD(void useConnection, (), (override));
    MOCK_METHOD(void, closeonnection, (), (override));
   
};

// наш тест
TEST(PainterTest, test1)
{
    MockTesting mDBconnector;

    // Аналогично, не понятно почему в образце работы GMock все работает  а тут нет.
    // вроде синтаксис такой же.В целом  в модуле  очень неинтуитивный пример теста приведен
    EXPECT_CALL(mDBconnector, openConnection).Times(::testing::AtLeast(1));
    EXPECT_CALL(mDBconnector, useConnection).Times(::testing::AtLeast(1));
    EXPECT_CALL(mDBconnector, closeonnection).Times(::testing::AtLeast(2));
    
}

int main()
{
    ::testing::InitGoogleTest(); // Запускаем само тестирование
    return RUN_ALL_TESTS();
}

