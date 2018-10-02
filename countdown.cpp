/**\
**
**  Copyright cmdLP
**
**  >> LICENSE.md
**
**  digital clock/countdown, it counts down on the last day of the year
**
\**/

/// remove for different colors:
#define FIXED_COLOR

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <array>
#include <ctime>

    #define WINVER 0x0501
#include <windows.h>

using digit_font = std::array<uint8_t, 12>;

    template<
        char _0,
        char _1,
        char _2,
        char _3,
        char _4,
        char _5,
        char _6,
        char _7
    >
struct parse_line {
    constexpr static uint8_t value = (
        ((uint8_t)_0 & 1)      |
        ((uint8_t)_1 & 1) << 1 |
        ((uint8_t)_2 & 1) << 2 |
        ((uint8_t)_3 & 1) << 3 |
        ((uint8_t)_4 & 1) << 4 |
        ((uint8_t)_5 & 1) << 5 |
        ((uint8_t)_6 & 1) << 6 |
        ((uint8_t)_7 & 1) << 7
    );
};


    template<typename t_Char, t_Char... chars>
constexpr auto operator""_line() {
    return parse_line<chars...>::value;
};
    template<typename t_Char, t_Char... chars>
constexpr auto operator""_buffer() {
    return std::array<char, 10> { chars... };
};

/// 0 1 2 3 4 5 6 7 8 9 :
constexpr std::array<digit_font, 11> digits = {{
    {{
        "  ###   "_line,
        " #####  "_line,
        "### ### "_line,
        "##   ## "_line,
        "##   ## "_line,
        "##   ## "_line,
        "##   ## "_line,
        "##   ## "_line,
        "##   ## "_line,
        "### ### "_line,
        " #####  "_line,
        "  ###   "_line,
    }},
    {{
        "     ## "_line,
        "    ### "_line,
        "   #### "_line,
        "     ## "_line,
        "     ## "_line,
        "     ## "_line,
        "     ## "_line,
        "     ## "_line,
        "     ## "_line,
        "     ## "_line,
        "     ## "_line,
        "     ## "_line,
    }},
    {{
        "  ###   "_line,
        " #####  "_line,
        "### ### "_line,
        "##   ## "_line,
        "    ### "_line,
        "   ###  "_line,
        "  ###   "_line,
        " ###    "_line,
        "###     "_line,
        "##      "_line,
        "####### "_line,
        "####### "_line,
    }},
    {{
        "  ###   "_line,
        " #####  "_line,
        "### ### "_line,
        "     ## "_line,
        "    ### "_line,
        "  ####  "_line,
        "  ####  "_line,
        "    ### "_line,
        "     ## "_line,
        "### ### "_line,
        " #####  "_line,
        "  ###   "_line,
    }},
    {{
        "##  ##  "_line,
        "##  ##  "_line,
        "##  ##  "_line,
        "##  ##  "_line,
        "##  ##  "_line,
        "####### "_line,
        "####### "_line,
        "    ##  "_line,
        "    ##  "_line,
        "    ##  "_line,
        "    ##  "_line,
        "    ##  "_line,
    }},
    {{
        " ###### "_line,
        " ###### "_line,
        " ##  ## "_line,
        "###     "_line,
        "##      "_line,
        "#####   "_line,
        "######  "_line,
        "    ### "_line,
        "     ## "_line,
        "##  ### "_line,
        " #####  "_line,
        "  ###   "_line,
    }},
    {{
        "  ###   "_line,
        " #####  "_line,
        "### ### "_line,
        "##   ## "_line,
        "##      "_line,
        "#####   "_line,
        "######  "_line,
        "##  ### "_line,
        "##   ## "_line,
        "### ### "_line,
        " #####  "_line,
        "  ###   "_line,
    }},
    {{
        "####### "_line,
        "####### "_line,
        "##  ### "_line,
        "    ##  "_line,
        "    ##  "_line,
        "   ###  "_line,
        "   ##   "_line,
        "   ##   "_line,
        "  ###   "_line,
        "  ##    "_line,
        "  ##    "_line,
        "  ##    "_line,
    }},
    {{
        "  ###   "_line,
        " #####  "_line,
        "### ### "_line,
        "##   ## "_line,
        "####### "_line,
        "####### "_line,
        "##   ## "_line,
        "##   ## "_line,
        "##   ## "_line,
        "### ### "_line,
        " #####  "_line,
        "  ###   "_line,
    }},
    {{
        "  ###   "_line,
        " #####  "_line,
        "### ### "_line,
        "##   ## "_line,
        "###  ## "_line,
        " ### ## "_line,
        "  ##### "_line,
        "   ###  "_line,
        "  ###   "_line,
        " ###    "_line,
        "###     "_line,
        "##      "_line,
    }},
    {{
        "        "_line,
        "        "_line,
        "   ##   "_line,
        "   ##   "_line,
        "   ##   "_line,
        "        "_line,
        "        "_line,
        "   ##   "_line,
        "   ##   "_line,
        "   ##   "_line,
        "        "_line,
        "        "_line,
    }}
}};




auto get_random_char() -> char {
    int i = std::rand() & 0xff;
    if((i &~ 0x80) < 0x20) { i+= 0x20; };
    if(i == 0x7f) i = ' ';

    return (char)i;
};

auto get_char() -> char { return 0xDB; };

std::array<std::array<char, 80>, 24> lines;
std::array<int, 16> matrx;

bool rand_bool_prob(double prob) {
    return rand() < int(RAND_MAX * prob);
};

auto render_lines(std::array<char, 10> chars) {

    for(auto& line: lines) {
        for(auto& cell: line) {
            if((unsigned char)cell > (unsigned char)' ')
                cell = std::max<unsigned char>(cell - (rand() & 0x1f), ' ');
        };
    };


    for(int& c: matrx) {
        c+=80;
        if(c >= 80 * 24) {
            c = rand() % 80;
        } else {
            lines[c / 80][c % 80] = get_random_char();
        };
    };

    for(int line = 0; line < 12; ++line) {
        if(rand_bool_prob(0.95))
        for(int cell = 0; cell < 80; ++cell) {
            char current_char = chars[cell >> 3];
            if(current_char &~ ' ') {
                if((digits[current_char & 0xf][line] >> (cell & 7)) & 1) {
                    if(rand_bool_prob(0.95)) lines[line + 6][cell] = get_char();
                };
            };
        };
    };

    constexpr int i = 24;

    std::cout.write((char*)lines.data(), 80*i);
};

int main() {

    system("chcp 850");
    system("mode con: cols=80 lines=25");

    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    bool countdown = timeinfo->tm_mon == 11 && timeinfo->tm_mday == 31;

    std::array<char, 10> buffer;

    #ifdef FIXED_COLOR
        system("color 0A");
    #else
        snprintf(buffer.data(), 9, "color 0%x", 15 ^ timeinfo->tm_wday);
        system(buffer.data());
    #endif

    for(int& c: matrx) {
        c = rand() % (80 * 24);
    };


    int i = 0, r = 0;
    int hour, min, sec;

    for(;;) {

        time(&rawtime);
        timeinfo = localtime(&rawtime);


        hour = timeinfo->tm_hour;
        min = timeinfo->tm_min;
        sec = timeinfo->tm_sec;

        if(countdown) {
            hour = 23 - hour;
            min = 59 - min;
            sec = 59 - sec;
        };

        snprintf(buffer.data(), 10, " %02i %02i %02i", hour, min, sec);

        if(!i) {
            i = rand() & 31;
            r = 1+(rand()&1)+3 * (rand() % 3);
        } else if((rand() & 7) == 0) {
            --i;
        };
        if(sec & 4) {
            char c = i % 12;
            c = c ? ('0'-1) + c : 0;
            buffer[  r] = c;
        };

        if(sec&1) buffer[6] = ':';
        if(sec&2) buffer[3] = ':';

        render_lines(buffer);

        std::cout << std::flush;

        Sleep(35);

    };
};
