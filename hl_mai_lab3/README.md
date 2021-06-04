# Лабораторная работа №3 #
Студентка: Никитина Анна

Группа: М8О-103М-20
## Установка необходимых компонентов ##
[Описана здесь](https://github.com/DVDemon/hl_mai_lab_01/blob/master/README.md)

## Сборка проекта ##
```bash
$ cmake --build CMakeFiles --target hl_mai_lab_01 -- -j 3 
```
## Запуск необходимых конейнеров ##
```bash
$ docker-compose up -d
```
## Создание базы данных во всех шардах ##

```bash
$ mysql -utest -ppzjqUkMnc7vfNHET -h 127.0.0.1 -P6033 --comments
mysql> source infrastructure/db_prepare/init.sql;
mysql> exit; 
```
## Заполнение шардов сгенерированными данными ##
### Разделение сгенерированных данных по шардам согласно внутреней логике ##
В итоге получим файл infrastructure/db_prepare/sharded_gen_data100k.sql
```bash
$ cmake --build CMakeFiles --target gen_data -- -j 3 && ./CMakeFiles/gen_data
```
### Загрузка сгенерированных данные по шардам ###
```bash
$ mysql -utest -ppzjqUkMnc7vfNHET -h 127.0.0.1 -P6033 --comments
mysql> source infrastructure/db_prepare/sharded_gen_data100k.sql;
mysql> exit
```
## Запуск приложения ##
```bash
$ sudo sh ./start.sh
```
## Отключение конейнеров ##
```bash
$ docker-compose stop
```

## Модульное тестирование ##
### Запуск отдельное окружение для тестирования и инициализация шардов ###
```bash
$ docker-compose -f docker-compose-test.yaml up -d
$ mysql -utest -ppzjqUkMnc7vfNHET -h 127.0.0.1 -6043 --comments
mysql> source infrastructure/db_prepare/init.sql;
mysql> exit; 
```
### Компиляция и запуск тестов ###
```bash
$ cmake --build CMakeFiles --target gtests -- -j 3 && ./CMakeFiles/gtests 
```
### Выключение тестового окружения ###
```bash
$ docker-compose -f docker-compose-test.yaml stop
```

## Нагрузочное тестирование ##
Нагрузочное тестирование осуществлялось с помощью утилиты wrk. Комплексное нагрузочное тестирование не входило в задание лабораторной, тем не менее очевидно что при тестировании нагрузки на получение одного конкретного значения, оно попадет в кэш apache ignite и результат тестирования буде практически аналогичным что и в лабораторной работе №2

```bash
$ wrk -t $Threads -c100 -d30s http://localhost:8080/person\?login\=166-06-8645
```

Кластер Apache Ignite из двух узлов и непосредственно приложение с БД были запущены на одной физической машине.  

В зависимости от числа потоков количество ответов в секуду и задержка менялись следующим образом:

Threads | Req/sec | Latency(ms)
--- | --- | ---
1 | 2358.97 | 11.92
2 | 3259.86 | 4.69
6 | 2968.89 | 5.34
10 | 2764.17 | 5.76

## Точки входа ##
Добавление пользователя
```bash
$ curl -d "login=anikkz&last_name=Nikitina&first_name=Anna&age=24" -X POST http://localhost:8080/person
```
Получение всех пользователей
```bash
curl -X GET http://localhost:8080/person
```
Получение пользователей по маске
```bash
curl -X GET http://localhost:8080/person\?first_name\=A\&last_name\=Ni
```
Получение конкретного пользователя
```bash
curl -X GET http://localhost:8080/person\?login\=anikkz
```
