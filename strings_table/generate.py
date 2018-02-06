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
    with open(filename, 'w') as file:
        file.write('|'.join(['string{}'.format(x) for x in range(columns)]) + '\n')
        file.write('|'.join(['string({})'.format(pow(2, x + 1) - 1) for x in range(columns)]) + '\n')

        counter = 0
        for i in range(rows):
            if (counter % (rows / 100) == 0):
                print('    {}%'.format(int(counter * 100 / rows)), end='\r')
            lst = []
            for j in range(columns):
                lst.append(distinct_strings[j][counter % len(distinct_strings[j])])
            file.write('|'.join(lst) + '\n')
            counter += 1
        print('   100%')


def main():
    columns = 8
    rows = 1000000
    distinctness = 0.0001

    threads = []
    distinct_strings = [None] * columns

    print('Generating distinct strings.')
    for i in range(1, columns + 1):
        thread = Thread(target=generate_distinct_strings, args=(i, int(rows * distinctness), distinct_strings))
        thread.start()
        threads.append(thread)

    [t.join() for t in threads]

    print('Writing to file.')
    write_to_file('string_table.tbl', distinct_strings, rows, columns)


if __name__ == '__main__':
    main()
