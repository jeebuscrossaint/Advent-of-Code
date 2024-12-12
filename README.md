# Advent of Codee

My advent of codes throughout the years

Languages:

2023: c++

2024: wanted to do cuda but cant so im on c++ (again) might redo in a different lang

2025: ???

____

## CLI

In the `cli` folder you can find the source code to a C++ written CLI utility that I use to speed up my times for advent of code.

Compilation:

It depends on 2 libraries, `curl`, and `gumbo`. Both gumbo and curl should be available in your package manager.
Curl is of course the famous command line tool and library for transferring data with URLs.
Gumbo is a HTML5 parsing library, which is used to parse the problem descriptions from the advent of code website.

```bash
git clone https://github.com/jeebuscrossaint/advent-of-code.git
cd advent-of-code/cli
make
# If you want to install it to your system
sudo make install # only works on unix systems
```

It can so far,

Fetch the input for a given day
View the problem for a given day
Submit the answer for a given day

Default behaviour:

- If no day/year provided, uses current date during December
- Outside December, defaults to day 1 of current year
- Cookie is stored in ~/.cache/aocli/cookie
- Inputs are cached in ~/.cached/aocli/inputs
- Problem descriptions are cached in ~/.cached/aocli/problems
- Answers are cached in ~/.cached/aocli/answers

Argument ranges:

- Day: 1-25
- Year: 2015-present
- Part: 1 or 2
- Answer: Any string (use quotes if it contains spaces)

```C++

void help() {
    std::cout << "Usage: aocli <command> [options] [arguments]\n\n";

    std::cout << "Commands:\n";
    std::cout << "  fetch         Fetch puzzle input\n";
    std::cout << "                aocli fetch [day] [year]\n";
    std::cout << "                aocli fetch -f [day] [year]      (force refresh)\n\n";

    std::cout << "  view          View puzzle description\n";
    std::cout << "                aocli view [day] [year]\n";
    std::cout << "                aocli view -f [day] [year]       (force refresh)\n\n";

    std::cout << "  submit        Submit puzzle answer\n";
    std::cout << "                aocli submit <part> <answer> [day] [year]\n";
    std::cout << "                part: 1 or 2\n";
    std::cout << "                answer: your solution\n\n";

    std::cout << "  update-cookie Update session cookie\n";
    std::cout << "                aocli update-cookie\n\n";

    std::cout << "  cookie-status Check cookie validity\n";
    std::cout << "                aocli cookie-status\n\n";

    std::cout << "Options:\n";
    std::cout << "  -f, --refresh Force refresh cached content\n\n";

    std::cout << "Arguments:\n";
    std::cout << "  day           Puzzle day (1-25)\n";
    std::cout << "  year          Puzzle year (2015-present)\n";
    std::cout << "                If not provided, defaults to current day/year\n";
    std::cout << "                during December, or day 1 otherwise\n\n";

    std::cout << "Examples:\n";
    std::cout << "  aocli fetch                    Fetch today's input\n";
    std::cout << "  aocli fetch 1 2023             Fetch day 1, 2023 input\n";
    std::cout << "  aocli view -f 5 2022           View day 5, 2022 puzzle (force refresh)\n";
    std::cout << "  aocli submit 1 \"123\" 3 2023    Submit 123 as part 1 answer for day 3, 2023\n";
    std::cout << "  aocli update-cookie            Update session cookie\n";
}
```

____
