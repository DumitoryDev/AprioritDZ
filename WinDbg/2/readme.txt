1. Открыл проект, разобрался с кодом.
2. Понял, что функция из библиотеки должна возвращать число 0. Ей передается значение счетчика цикла от 0 до 65535.
3. Поставил точку останова на 32 строке, указал условия и действия для обработки. 
неправильное значение из функции возвращается при следующих условиях:

main(int, char * *) i = 2, result = 2,but the result should be 0
main(int, char * *) i = 666, result = 2,but the result should be 0
main(int, char * *) i = 999, result = 4,but the result should be 0
main(int, char * *) i = 64535, result = 8,but the result should be 0

