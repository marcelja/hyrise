import random
import string
import time
from threading import Thread

def random_string(length):
    return ''.join(random.choices(string.ascii_uppercase + string.ascii_lowercase + string.digits, k = length))


def generate_distinct_strings(index, num_distinct_strings, distinct_strings):
    string_len = pow(2, index) - 1
    distinct_strings[index - 1] = [random_string(string_len) for x in range(num_distinct_strings)]


def write_to_file(filename, distinct_strings, rows, columns):
    counter = 0
    with open(filename, 'w') as file:
        file.write('|'.join(['string{}'.format(x) for x in range(columns)]) + '\n')
        file.write('|'.join(['string' for x in range(columns)]) + '\n')

        for i in range(rows):
            lst = []
            for j in range(columns):
                lst.append(distinct_strings[j][counter % len(distinct_strings[j])])
            file.write('|'.join(lst) + '\n')
            counter += 1


def main():
    columns = 8
    rows = 200
    distinctness = 0.01

    threads = []
    distinct_strings = [None] * columns

    for i in range(1, columns + 1):
        thread = Thread(target=generate_distinct_strings, args=(i, int(rows * distinctness), distinct_strings))
        thread.start()
        threads.append(thread)

    [t.join() for t in threads]

    write_to_file('string_table.tbl', distinct_strings, rows, columns)


if __name__ == '__main__':
    main()
