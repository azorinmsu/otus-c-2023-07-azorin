﻿**Задание**

Изучить предоставленный ассемблерный листинг, понять, над какой структурой данных в нём происходит работа и написать аналогичную программу на C, повторяющую вывод оригинальной программы.

**Сложность**

★★★★☆

**Цель задания**

Получить навык чтения ассемблерного кода.

**Критерии успеха**

1. Создано консольное приложение, выводящее при запуске тот же текст, что и предоставленная в листинге main.asm программа.
1. Приложение работает с той же структурой данных, что и оригинальная программа.
1. Код компилируется без warning’ов с ключами компилятора -Wall -Wextra -Wpedantic -std=c11.
1. **Bonus points** за указание того, каким типичным функциям высшего порядка соответствуют ассемблерные процедуры m и f.
1. **Bonus points** за оптимизацию процедур mи f в ассемблерном коде.
1. **Bonus points** за исправление утечки памяти под структуры данных в ассемблерном коде.
1. Далее успешность определяется code review.

**Обратная связь**

Cтудент коммитит все необходимое в свой github/gitlab репозиторий. Далее необходимо зайти в ЛК, найти занятие, ДЗ по которому выполнялось, нажать “Чат с преподавателем” и отправить ссылку. После этого ревью и общение на тему ДЗ будет происходить в рамках этого чата.

**Вспомогательные материалы**

1. [System V ABI](https://wiki.osdev.org/System_V_ABI#x86-64)
1. [NASM Intel x86 Assembly Language Cheat Sheet](https://www.bencode.net/blob/nasmcheatsheet.pdf)
