
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <ranges>
#include <iostream>
#include <algorithm>

template <typename T, typename U, typename V, typename W>
auto operator+(const std::pair<T, U> &l, const std::pair<V, W> &r)
{
    return std::pair<T, U>{ l.first + r.first, l.second + r.second };
}

template <typename T, typename U, typename V, typename W>
auto operator-(const std::pair<T, U> &l, const std::pair<V, W> &r)
{
    return std::pair<T, U>{ l.first - r.first, l.second - r.second };
}

template <typename T, typename U>
auto operator-(const std::pair<T, U> &l)
{
    return std::pair<T, U>{ -l.first, -l.second };
}

using xy_pos_t = std::pair<int, int>;

struct cell {
    bool has_wall{ false };
};

struct box {
    xy_pos_t pos1{ 0, 0 };
    xy_pos_t pos2{ 0, 0 };

    box(xy_pos_t pos) : pos1(pos), pos2(pos)
    {
    }

    box(xy_pos_t pos1, xy_pos_t pos2) : pos1(pos1), pos2(pos2)
    {
    }

    bool operator==(const box &) const = default;
};

struct robot {
    xy_pos_t pos{ 0, 0 };
};

using warehouse_row_t = std::vector<cell>;
using warehouse_map_t = std::vector<warehouse_row_t>;
using move_list_t = std::vector<xy_pos_t>;
using box_list_t = std::vector<box>;

template <bool part2_t = false>
std::tuple<warehouse_map_t, box_list_t, robot, move_list_t> read_file()
{
    std::ifstream infile("input.txt");
    warehouse_map_t csv_rows;
    robot rob;
    move_list_t moves;
    box_list_t boxes;
    int y{ 0 };

    for (std::string line; std::getline(infile, line);) {
        if (line.empty())
            break;
        std::istringstream ss(line);
        warehouse_row_t row;
        int x{ 0 };

        char value{ 0 };
        while (ss >> value) {
            switch (value) {
            case '#':
                row.push_back(cell{ .has_wall = true });
                if (part2_t)
                    row.push_back(cell{ .has_wall = true });
                break;
            case 'O':
                row.push_back(cell{});
                if (part2_t) {
                    row.push_back(cell{});
                    boxes.emplace_back(xy_pos_t{ 2 * x, y }, xy_pos_t{ 2 * x + 1, y });
                } else {
                    boxes.emplace_back(xy_pos_t{ x, y });
                }
                break;
            case '.':
                row.push_back(cell{});
                if (part2_t)
                    row.push_back(cell{});
                break;
            case '@':
                row.push_back(cell{});
                if (part2_t) {
                    row.push_back(cell{});
                    rob.pos = xy_pos_t{ 2 * x, y };
                } else {
                    rob.pos = xy_pos_t{ x, y };
                }
                break;
            default:
                throw std::runtime_error("Error parsing logic!");
            }
            x++;
        }

        csv_rows.push_back(std::move(row));
        y++;
    }

    for (std::string line; std::getline(infile, line);) {
        std::istringstream ss(line);
        char value{ 0 };
        while (ss >> value) {
            switch (value) {
            case '>':
                moves.emplace_back(1, 0);
                break;
            case 'v':
                moves.emplace_back(0, 1);
                break;
            case '<':
                moves.emplace_back(-1, 0);
                break;
            case '^':
                moves.emplace_back(0, -1);
                break;
            default:
                throw std::runtime_error("Error parsing logic!");
            }
        }
    }

    return { csv_rows, boxes, rob, moves };
}

bool peek_box_push(warehouse_map_t &map, box_list_t &boxes, box &b, xy_pos_t move)
{
    xy_pos_t pos1_push = b.pos1 + move;
    xy_pos_t pos2_push = b.pos2 + move;

    auto find_next_box = [pos1_push, pos2_push, &b](auto &a) {
        return a != b && (a.pos1 == pos1_push || a.pos2 == pos1_push || a.pos1 == pos2_push || a.pos2 == pos2_push);
    };

    //if either push into wall the push fails
    if (map.at(pos1_push.second).at(pos1_push.first).has_wall || map.at(pos2_push.second).at(pos2_push.first).has_wall)
        return false;

    //see if there is any boxes we push into
    auto itr = std::find_if(boxes.begin(), boxes.end(), find_next_box);
    if (itr == boxes.end())
        return true;
    else {
        bool push = peek_box_push(map, boxes, (*itr), move);
        //check if there is a second one
        auto itr2 = std::find_if(std::next(itr, 1), boxes.end(), find_next_box);

        if (itr2 != boxes.end()) {
            push &= peek_box_push(map, boxes, (*itr2), move);
        }

        return push;
    }
}

void do_box_push(warehouse_map_t &map, box_list_t &boxes, box &b, xy_pos_t move)
{
    xy_pos_t pos1_push = b.pos1 + move;
    xy_pos_t pos2_push = b.pos2 + move;

    auto find_next_box = [pos1_push, pos2_push, &b](auto &a) {
        return a != b && (a.pos1 == pos1_push || a.pos2 == pos1_push || a.pos1 == pos2_push || a.pos2 == pos2_push);
    };

    //see if there is any boxes we push into
    auto itr = std::find_if(boxes.begin(), boxes.end(), find_next_box);
    if (itr != boxes.end()) {
        do_box_push(map, boxes, (*itr), move);
        //check if there is a second one
        auto itr2 = std::find_if(std::next(itr, 1), boxes.end(), find_next_box);

        if (itr2 != boxes.end()) {
            do_box_push(map, boxes, (*itr2), move);
        }
    }

    b.pos1 = b.pos1 + move;
    b.pos2 = b.pos2 + move;
}

void execute_move(warehouse_map_t &map, box_list_t &boxes, robot &rob, xy_pos_t move)
{
    xy_pos_t look_ahead = rob.pos + move;

    //look ahead is a wall so we can do nothing and return
    if (map.at(look_ahead.second).at(look_ahead.first).has_wall)
        return;

    auto itr = std::find_if(boxes.begin(), boxes.end(),
                            [look_ahead](auto &a) { return a.pos1 == look_ahead || a.pos2 == look_ahead; });

    //no boxes so just move into empty space
    if (itr == boxes.end()) {
        rob.pos = look_ahead;
        return;
    }

    if (peek_box_push(map, boxes, (*itr), move)) {
        do_box_push(map, boxes, (*itr), move);
        rob.pos = look_ahead;
    }
}

void print_map(const warehouse_map_t &map, const box_list_t &boxes, const robot &rob, int iteration, xy_pos_t move)
{
    std::ofstream out("output.txt", std::ios_base::app);
    out << iteration << " " << move.first << " " << move.second << '\n';
    std::vector<std::string> print_output;
    print_output.reserve(map.size());

    for (const auto &row : map) {
        print_output.emplace_back();
        for (const auto &cell : row) {
            if (cell.has_wall)
                print_output.back().push_back('#');
            else
                print_output.back().push_back('.');
        }
    }

    for (const auto &box : boxes) {
        if (box.pos1 == box.pos2) {
            print_output.at(box.pos1.second).at(box.pos1.first) = 'O';
        } else {
            print_output.at(box.pos1.second).at(box.pos1.first) = '[';
            print_output.at(box.pos2.second).at(box.pos2.first) = ']';
        }
    }

    print_output.at(rob.pos.second).at(rob.pos.first) = '@';

    for (const auto &line : print_output) {
        out << line << '\n';
    }
}

long score_map(const box_list_t &boxes)
{
    long score{ 0 };
    for (const auto &box : boxes)
        score += box.pos1.second * 100 + box.pos1.first;

    return score;
}

int main()
{
    auto [map, boxes, rob, moves] = read_file();

    int iteration{ 0 };
    for (const auto &move : moves) {
        iteration++;
        execute_move(map, boxes, rob, move);
    }

    long score = score_map(boxes);
    std::cout << score << '\n';

    auto [map2, boxes2, rob2, moves2] = read_file<true>();
    int iteration2{ 0 };
    for (const auto &move2 : moves2) {
        iteration2++;
        execute_move(map2, boxes2, rob2, move2);
    }

    long score2 = score_map(boxes2);
    std::cout << score2 << '\n';
}
