ОЦЕНКА 3
Создали файл sort.c

![alt text](kartinki/image.png) 

Вставили код функции любой сортировки

![alt text](image.png)

Вот так сейчас выглядит git status

![alt text](image-1.png)

Добавили файл в область add

![alt text](image-2.png)

Смотрим как сейчас выглядит git status

![alt text](image-3.png)

Закоммитим файл в репозиторий

![alt text](image-4.png)

Смотрим, как сейчас выглядит git status

![alt text](image-7.png)

Добавляем комментарий с любым текстом в этот файл

 ![alt text](image-6.png)

Смотрим, как выглядит git status

![alt text](image-8.png)

Добавляем add изменение файла

![alt text](image-9.png)

Смотрим, как выглядит git status 
![alt text](image-10.png)

Добавляем еще один комментарий

![alt text](image-11.png)

Делаем коммит

![alt text](image-12.png)

Смотрим, как выглядит git status

![alt text](image-13.png) 

Смотрим, как выглядит журнал log

![alt text](image-14.png)

Добавляем в add

![alt text](image-15.png)

Коммитим

![alt text](image-16.png)

Запушим на git push

![alt text](image-17.png)

Используем git branch mybranch, чтобы создать новую ветку с именем mybranch

![alt text](image-18.png).

Используем git branch, чтобы увидеть новую созданную ветку

![alt text](image-19.png)

Используем git switch mybranch, чтобы переключиться на новую ветку

![alt text](image-20.png)

Основное видимое изменение в выводе git status - это смена имени ветки и временное отсутствие информации о синхронизации с удаленным репозиторием для новой ветки

![alt text](image-21.png)

Убеждаемся, что мы находимся на своей ветке

![alt text](image-22.png)

Создаем файл file1.txt и своим именем

![alt text](image-23.png)

Добавляем файл

![alt text](image-24.png)

Коммитем изменение

![alt text](image-25.png) 

Используйте git log --oneline --graph, чтобы увидеть, что ваша ветка указывает на новый коммит

![alt text](image-26.png)
Вернемся к ветке с именем master

![alt text](image-27.png)

Пропал коммит a54b0fa с file1.txt, так как он есть только в ветке mybranch

![alt text](image-28.png)

Создаем файл и коммитем его

![alt text](image-29.png)

Используем git log --oneline --graph --all, чтобы увидеть, что наша ветка указывает на новый коммит, и что теперь у двух веток разные коммиты

![alt text](image-30.png)

Переключаемся на вашу ветку mybranch

![alt text](image-31.png)

Проверяем наличие file2.txt

![alt text](image-32.png)

file2.txt пропал, так как каждый файл существует только в своей ветке.

Используем git diff mybranch master, чтобы увидеть разницу между двумя ветками.

![alt text](image-33.png)


ОЦЕНКА 4

Переключились на ветку mybranch

![alt text](image-34.png)

Перезаписали содержимое в sort.c добавили функцию main()

![alt text](image-35.png)

git diff отслеживает изменения

![alt text](image-36.png)

git diff --staged показывает, какие изменения будут сохранены в следующем коммите, а он у нас пустой

![alt text](image-37.png)

Добавила  в staged файл sort.c

![alt text](image-38.png)

git diff ничего не говорит, потому что в данный момент содержит одинаковую версию файла sort.c

![alt text](image-39.png)

git diff --staged показывает, какие изменения уже подготовлены для следующего коммита.

![alt text](image-40.png)

Удалила число 0 в массиве в sort.c

![alt text](image-41.png)

git diff  показывает изменения, он показал, что у мен исчезло число 0

![alt text](image-42.png)

git diff –-staged показывает нам, что подготовил к коммиту замену быстрой сортировки и добавил main(), но он не учитывает последнее изменение с удалением одного числа

![alt text](image-43.png)

У нас в файле есть 3 состояния: 

1)Директория-то, что сейчас редактирую

2)Промежуточная зона-то, что подготовила к отправке

3)Репозиторий-то, что было в последнем коммите

Что я сделала:

git diff – удалила число 0 из массива

git diff –staged- заменила алгоритм сортировки и добавила функцию main

Запускаю git status и замечаю, что sort.c присутствует дважды в выводе

![alt text](image-45.png)

Запускаю git restore –staged sort.c, чтобы отменить индексацию изменения

![alt text](image-46.png)

Мы перенесли обратно из stage в VSCode

![alt text](image-47.png)

Индексирую изменение (add) и делаю коммит

![alt text](image-48.png)

Вот так выглядит журнал

![alt text](image-49.png)

Добавляю в sort.c в main() printf(“hello git\n”);

![alt text](image-50.png)

 Содержимое sort.c будет включать добавленную строку printf в main()

git status говорит, что файл sort.c изменен в моей рабочей директории, но эти изменения не подготовлены для следующего коммита

![alt text](image-51.png)

Запустила git restore sort.c

![alt text](image-52.png)

 Содержимое sort.c вернулось к состоянию последнего коммита (без printf)
git status говорит, что все синхронизировано с репозиторием git

![alt text](image-53.png)

Создала файл greeting.txt, проиндексировала его и закоммитила с сообщением “Add file greeting.txt”

![alt text](image-55.png)

Добавила  в этот файл слово hello, индексирую и коммичу с текстом "Add content to greeting.txt"

![alt text](image-56.png)

Создаю ветку с именем feature/uppercase

![alt text](image-57.png)

Переключилась на эту ветку

![alt text](image-58.png)

git status говорит, что я нохожусь в ветке freature/uppercase

![alt text](image-59.png)

Отредактировала  greeting.txt, чтобы он содержал приветствие в верхнем регистре (HELLO)

![alt text](image-60.png)

Добавила файл greeting.txt и закоммитела

![alt text](image-61.png)

git brabch показывает на какой ветки мы сейчас находимся

![alt text](image-62.png)

Он вывел все коммиты(визуальная структура ветвления, последовательность коммитов, расположение веток)

![alt text](image-63.png)

Переключилась на главную ветку

![alt text](image-64.png)

Использовала cat, чтобы увидеть содержимое файла greetings.txt

![alt text](image-65.png)

Сравниваю ветки

![alt text](image-66.png)

![alt text](image-67.png)

Объединила ветки

![alt text](image-68.png)

Использую cat, чтобы увидеть содержимое файла greetings.txt

![alt text](image-69.png)

Удалила ветку с заглавными буквами (feature/uppercase)

![alt text](image-70.png)

Смержила ветку mybranch в master (git merge), но так как мы до этого уже так делали, то нам выдает сообщение, что мы ранее сделали слияние

![alt text](image-71.png)

git log --oneline --graph –all выводит, что находится HEAD на ветке mybranch

![alt text](image-72.png)

Запушисала изменения ветки master на удаленный репо

![alt text](image-73.png)


ОЦЕНКА 5
Создала ветку branch1

![alt text](image-74.png)

Переключилась на нее

![alt text](image-75.png)

Выполнила команду  echo "This is a relevant fact" > file.txt

![alt text](image-76.png)

Закоммитила  это изменение

![alt text](image-77.png)

Переключилась на главную ветку

![alt text](image-87.png)

Выполнила команду

![alt text](image-88.png)

Закоммитила именения в master

![alt text](image-89.png
)
Вот что вывел git log --oneline --graph –all

![alt text](image-90.png)

Использовала команду git merge чтобы смержить ветку branch1 в master (получла конфликт это норм)

![alt text](image-91.png)

Находимся в ветке master, есть неразрешенные конфликты слияния, файл file.txt имеет конфликт - изменен в обеих ветках, git предлагает исправить конфликты и выполнить git commit

![alt text](image-92.png)

Показывает истории коммитов

![alt text](image-93.png)

Запушила изменения

![alt text](image-94.png)

Проиндексировала файл и закоммитила

![alt text](image-95.png)

Создаю новую ветку Mergesort-Impl и переключаюсь на нее

![alt text](image-96.png)

Содержимое файла mergesort.py заменила на код из righty.py и закоммитила

![alt text](image-97.png)

Перешла на ветку master

![alt text](image-98.png)

Поменяла все содержимое mergesort.py на lefty.py и закоммитила

![alt text](image-99.png)

Показывает историю коммитов

![alt text](image-100.png)

git branch показывает, что я нахожусь на ветке master

![alt text](image-101.png)

Смержила  Mergesort-Impl в master

![alt text](image-102.png)

После исправления всех merge конфликтов запушила в master изменения

![alt text](image-103.png)


