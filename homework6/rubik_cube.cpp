#include "rubik_cube.hpp"

#include <ctime>

RubicCube::RubicCube()
    :isOut(false) {
    sides[0] = 0x00000000;
    sides[1] = 0x11111111;
    sides[2] = 0x22222222;
    sides[3] = 0x33333333;
    sides[4] = 0x44444444;
    sides[5] = 0x55555555;
}

void RubicCube::Rotation(int side) {
    uint32_t mask = 0x000000FF;
    uint32_t temp = (sides[side] & mask) << 24;
    sides[side] = (sides[side] >> 8) | temp;
}

void RubicCube::RotationEdges(int side[4], int offset_side[4]) {
    uint32_t mask = 0x00000000;
    uint32_t temp;

    for (size_t j = 0; j < 3; ++j) {
        mask |= 0xF0000000 >> (((j + offset_side[3]) % 8) * 4);
    }
    uint32_t prev_edge = sides[side[3]] & mask;
    size_t prev_side_offset = offset_side[3];

    for (size_t i = 0; i < 4; ++i) {
        mask = 0x00000000;
        for (size_t j = 0; j < 3; ++j) {
            mask |= 0xF0000000 >> (((j + offset_side[i]) % 8) * 4);
        }
        temp = sides[side[i]] & mask;
        sides[side[i]] = (sides[side[i]] & (~mask));
        prev_edge = MakeNewEdge(prev_edge, prev_side_offset, offset_side[i]);
        sides[side[i]] |= prev_edge;
        prev_edge = temp;
        prev_side_offset = offset_side[i];
    }
}

uint32_t RubicCube::MakeNewEdge(uint32_t edge, int offset_in, int offset_out) {
    if (offset_in == offset_out)
        return edge;

    else if (offset_in == 6)
        edge = ((edge & 0x000000FF) << (offset_in - offset_out) * 4) | 
            ((edge & 0xF0000000) >> (offset_out + 2) * 4);
    else if (offset_out == 6)
        edge = (((edge & (0xFF000000 >> offset_in * 4)) >> (offset_out - offset_in) * 4) | 
            (edge & (0x00F00000 >> offset_in * 4)) << (offset_in + 2) * 4);

    else if (abs(offset_in - offset_out) == 2) {
        offset_in > offset_out ?
            edge = edge << 8 :
            edge = edge >> 8;
    }
    else if (abs(offset_in - offset_out) == 4) {
        offset_in > offset_out ?
            edge = edge << 16 :
            edge = edge >> 16;
    }
    return edge;
}

void RubicCube::Up(bool reverse) {
    for (int i = 0; reverse ? i < 3 : i < 1; ++i) {
        Rotation(0);
        int index_side[4] = { 2, 1, 4, 3 };
        int offset_mask[4] = { 0, 0, 0, 0 };
        RotationEdges(index_side, offset_mask);
    }
    if (isOut)
        reverse ? std::cout << "U' " : std::cout << "U ";
}

void RubicCube::Down(bool reverse) {
    for (int i = 0; reverse ? i < 3 : i < 1; ++i) {
        Rotation(5);
        int index_side[4] = { 2, 3, 4, 1 };
        int offset_mask[4] = { 4, 4, 4, 4 };
        RotationEdges(index_side, offset_mask);
    }
    if (isOut)
        reverse ? std::cout << "D' " : std::cout << "D ";
}

void RubicCube::Front(bool reverse) {
    for (int i = 0; reverse ? i < 3 : i < 1; ++i) {
        Rotation(2);
        int index_side[4] = { 1, 0, 3, 5 };
        int offset_mask[4] = { 2, 4, 6, 0 };
        RotationEdges(index_side, offset_mask);
    }
    if (isOut)
        reverse ? std::cout << "F' " : std::cout << "F ";
}

void RubicCube::Back(bool reverse) {
    for (int i = 0; reverse ? i < 3 : i < 1; ++i) {
        Rotation(4);
        int index_side[4] = { 0, 1, 5, 3 };
        int offset_mask[4] = { 0, 6, 4, 2 };
        RotationEdges(index_side, offset_mask);
    }
    if (isOut)
        reverse ? std::cout << "B' " : std::cout << "B ";
}

void RubicCube::Left(bool reverse) {
    for (int i = 0; reverse ? i < 3 : i < 1; ++i) {
        Rotation(1);
        int index_side[4] = { 0, 2, 5, 4 };
        int offset_mask[4] = { 6, 6, 6, 2 };
        RotationEdges(index_side, offset_mask);
    }
    if (isOut)
        reverse ? std::cout << "L' " : std::cout << "L ";
}

void RubicCube::Right(bool reverse) {
    for (int i = 0; reverse ? i < 3 : i < 1; ++i) {
        Rotation(3);
        int index_side[4] = { 2, 0, 4, 5 };
        int offset_mask[4] = { 2, 2, 6, 2 };
        RotationEdges(index_side, offset_mask);
    }
    if (isOut)
        reverse ? std::cout << "R' " : std::cout << "R ";
}

int RubicCube::Color(int side, int square) {
    return (sides[side] << square * 4) >> 28;
}

bool RubicCube::CheckFlower(int side) {
    bool check = true;
    for (int j = 1; j <= 7; j += 2) {
        check &= Color(side, j) == 0;
    }
    return check;
}

void RubicCube::DownFlower(int cube) {
    while (Color(5, cube) == 0)
        Down();
}

void RubicCube::BigRotateFlower(int first_down, int second_down,
    void (RubicCube::* first_rotate)(bool),
    void (RubicCube::* second_rotate)(bool),
    bool first_reverse, bool second_reverse,
    bool isReverse)
{
    if (isReverse) {
        (this->*first_rotate)(first_reverse);
        (this->*first_rotate)(first_reverse);
    }
    DownFlower(first_down);
    (this->*first_rotate)(first_reverse);
    DownFlower(second_down);
    (this->*second_rotate)(second_reverse);
}

void RubicCube::SmallRotateFlower(int down, void (RubicCube::* rotate)(bool), bool isReverse) {
    DownFlower(down);
    (this->*rotate)(isReverse);
}

void RubicCube::Flower() {
    while (!CheckFlower(5)) {
        //up
        if (Color(0, 1) == 0)
            BigRotateFlower(5, 5, &RubicCube::Back, &RubicCube::Back);
        else if (Color(0, 3) == 0)
            BigRotateFlower(3, 3, &RubicCube::Right, &RubicCube::Right);
        else if (Color(0, 5) == 0)
            BigRotateFlower(1, 1, &RubicCube::Front, &RubicCube::Front);
        else if (Color(0, 7) == 0)
            BigRotateFlower(7, 7, &RubicCube::Left, &RubicCube::Left);

        //left
        if (Color(1, 1) == 0 || Color(1, 5) == 0)
            BigRotateFlower(7, 1, &RubicCube::Left, &RubicCube::Front, false, true, Color(1, 5) == 0);
        else if (Color(1, 3) == 0)
            SmallRotateFlower(1, &RubicCube::Front, true);
        else if (Color(1, 7) == 0)
            SmallRotateFlower(7, &RubicCube::Back);

        //front
        if (Color(2, 1) == 0 || Color(2, 5) == 0)
            BigRotateFlower(1, 3, &RubicCube::Front, &RubicCube::Right, false, true, Color(2, 5) == 0);
        else if (Color(2, 3) == 0)
            SmallRotateFlower(3, &RubicCube::Right, true);
        else if (Color(2, 7) == 0)
            SmallRotateFlower(7, &RubicCube::Left);

        //right
        if (Color(3, 1) == 0 || Color(3, 5) == 0)
            BigRotateFlower(3, 5, &RubicCube::Right, &RubicCube::Back, false, true, Color(3, 5) == 0);
        else if (Color(3, 3) == 0)
            SmallRotateFlower(5, &RubicCube::Back, true);
        else if (Color(3, 7) == 0)
            SmallRotateFlower(1, &RubicCube::Front);

        //back
        if (Color(4, 1) == 0 || Color(4, 5) == 0)
            BigRotateFlower(5, 7, &RubicCube::Back, &RubicCube::Left, false, true, Color(4, 5) == 0);
        else if (Color(4, 3) == 0)
            SmallRotateFlower(7, &RubicCube::Left, true);
        else if (Color(4, 7) == 0)
            SmallRotateFlower(3, &RubicCube::Right);
    }
}

void RubicCube::RotateCross(int color, int square, void (RubicCube::* rotate)(bool), bool reverse) {
    while (Color(color, 5) != color || Color(5, square) != 0)
        Down();
    (this->*rotate)(reverse);
    (this->*rotate)(reverse);
}

void RubicCube::Cross() {
    RotateCross(1, 7, &RubicCube::Left);
    RotateCross(2, 1, &RubicCube::Front);
    RotateCross(3, 3, &RubicCube::Right);
    RotateCross(4, 5, &RubicCube::Back);
}

bool RubicCube::CheckBangBang() {
    bool check = true;
    for (int j = 0; j <= 6; j += 2) {
        check &= Color(0, j) == 0 && ((j == 0 && Color(1, 0) == 1 && Color(4, 2) == 4) ||
            (j == 2 && Color(3, 2) == 3 && Color(4, 0) == 4) ||
            (j == 4 && Color(2, 2) == 2 && Color(3, 0) == 3) ||
            (j == 6 && Color(1, 2) == 1 && Color(2, 0) == 2));
    }
    return check;
}

void RubicCube::DownRotateBangBang(int square, void (RubicCube::*rotate)(bool), int amount_down) {
    for (int i = 0; i < amount_down; ++i)
        Down();
    while (Color(0, square) != 0) {
        (this->*rotate)(true);
        Down(true);
        (this->*rotate)(false);
        Down();
    }
}

void RubicCube::DownBangBang(int square, int size1, int size2, int down[4]) {
    int color1 = -1, color2 = -1;
    if (Color(5, square) == 0)
        color1 = Color(size1, 4), color2 = Color(size2, 6);
    else if (Color(size1, 4) == 0)
        color1 = Color(5, square), color2 = Color(size2, 6);
    else if (Color(size2, 6) == 0)
        color1 = Color(5, square), color2 = Color(size1, 4);

    if ((color1 == 1 && color2 == 2) || (color1 == 2 && color2 == 1))
        DownRotateBangBang(6, &RubicCube::Front, down[0]);
    else if ((color1 == 2 && color2 == 3) || (color1 == 3 && color2 == 2))
        DownRotateBangBang(4, &RubicCube::Right, down[1]);
    else if ((color1 == 3 && color2 == 4) || (color1 == 4 && color2 == 3))
        DownRotateBangBang(2, &RubicCube::Back, down[2]);
    else if ((color1 == 1 && color2 == 4) || (color1 == 4 && color2 == 1))
        DownRotateBangBang(0, &RubicCube::Left, down[3]);
}

void RubicCube::UpRotateBangBang(void (RubicCube::* rotate)(bool)) {
    (this->*rotate)(false);
    Down(); 
    (this->*rotate)(true);
}

void RubicCube::BangBang() {
    while (!CheckBangBang()) {
        //on down
        if (Color(5, 0) == 0 || Color(1, 4) == 0 || Color(2, 6) == 0) {
            int amount_down[4] = { 0, 1, 2, 3 };
            DownBangBang(0, 1, 2, amount_down);
        }
        if (Color(5, 2) == 0 || Color(2, 4) == 0 || Color(3, 6) == 0) {
            int amount_down[4] = { 3, 0, 1, 2 };
            DownBangBang(2, 2, 3, amount_down);
        }
        if (Color(5, 4) == 0 || Color(3, 4) == 0 || Color(4, 6) == 0) {
            int amount_down[4] = { 2, 3, 0, 1 };
            DownBangBang(4, 3, 4, amount_down);
        }
        if (Color(5, 6) == 0 || Color(4, 4) == 0 || Color(1, 6) == 0) {
            int amount_down[4] = { 1, 2, 3, 0 };
            DownBangBang(6, 4, 1, amount_down);
        }
        //on up
        if (!(Color(0, 0) == 0 && Color(1, 0) == 1 && Color(4, 2) == 4) &&
            (Color(0, 0) == 0 || Color(1, 0) == 0 || Color(4, 2) == 0)) {
            UpRotateBangBang(&RubicCube::Back);
        }
        if (!(Color(0, 2) == 0 && Color(3, 2) == 3 && Color(4, 0) == 4) &&
            (Color(0, 2) == 0 || Color(3, 2) == 0 || Color(4, 0) == 0)) {
            UpRotateBangBang(&RubicCube::Right);
        }
        if (!(Color(0, 4) == 0 && Color(2, 2) == 2 && Color(3, 0) == 3) &&
            (Color(0, 4) == 0 || Color(2, 2) == 0 || Color(3, 0) == 0)) {
            UpRotateBangBang(&RubicCube::Front);
        }
        if (!(Color(0, 6) == 0 && Color(1, 2) == 1 && Color(2, 0) == 2) &&
            (Color(0, 6) == 0 || Color(1, 2) == 0 || Color(2, 0) == 0)) {
            UpRotateBangBang(&RubicCube::Left);
        }
    }
}

bool RubicCube::CheckMiddleLayer() {
    return ((Color(1, 3) == 1 && Color(2, 7) == 2) &&
        (Color(2, 3) == 2 && Color(3, 7) == 3) &&
        (Color(3, 3) == 3 && Color(4, 7) == 4) &&
        (Color(4, 3) == 4 && Color(1, 7) == 1));
}

void RubicCube::RightRotateMiddleLayer(
    void (RubicCube::* first_rotate)(bool),
    void (RubicCube::* second_rotate)(bool),
    int amount_down)
{
    for (int i = 0; i < amount_down; ++i)
        Down();
    Down();
    (this->*first_rotate)(false);
    Down(true);
    (this->*first_rotate)(true);
    Down(true);
    (this->*second_rotate)(true);
    Down();
    (this->*second_rotate)(false);
}

void RubicCube::LeftRotateMiddleLayer(
    void (RubicCube::* first_rotate)(bool),
    void (RubicCube::* second_rotate)(bool),
    int amount_down)
{
    for (int i = 0; i < amount_down; ++i)
        Down();
    Down(true);
    (this->*first_rotate)(true);
    Down();
    (this->*first_rotate)(false);
    Down();
    (this->*second_rotate)(false);
    Down(true);
    (this->*second_rotate)(true);
}

void RubicCube::SideMiddleLayer(int side, int color, int down[4]) {
    if (Color(side, 5) == 2) {
        if (Color(5, color) == 1)
            RightRotateMiddleLayer(&RubicCube::Left, &RubicCube::Front, down[0]);
        else if (Color(5, color) == 3)
            LeftRotateMiddleLayer(&RubicCube::Right, &RubicCube::Front, down[0]);
    }
    else if (Color(side, 5) == 3) {
        if (Color(5, color) == 2)
            RightRotateMiddleLayer(&RubicCube::Front, &RubicCube::Right, down[1]);
        else if (Color(5, color) == 4)
            LeftRotateMiddleLayer(&RubicCube::Back, &RubicCube::Right, down[1]);
    }
    else if (Color(side, 5) == 4) {
        if (Color(5, color) == 3)
            RightRotateMiddleLayer(&RubicCube::Right, &RubicCube::Back, down[2]);
        else if (Color(5, color) == 1)
            LeftRotateMiddleLayer(&RubicCube::Left, &RubicCube::Back, down[2]);
    }
    else if (Color(side, 5) == 1) {
        if (Color(5, color) == 4)
            RightRotateMiddleLayer(&RubicCube::Back, &RubicCube::Left, down[3]);
        else if (Color(5, color) == 2)
            LeftRotateMiddleLayer(&RubicCube::Front, &RubicCube::Left, down[3]);
    }
}

void RubicCube::RotateMiddleLayer(
    void (RubicCube::* first_rotate)(bool),
    void (RubicCube::* second_rotate)(bool))
{
    (this->*first_rotate)(false);
    Down(true);
    (this->*first_rotate)(true);
    Down(true);
    (this->*second_rotate)(true);
    Down();
    (this->*second_rotate)(false);
}

void RubicCube::MiddleLayer() {
    while (!CheckMiddleLayer()) {
        //front
        int amount_down_front[4] = { 0, 1, 2, 3 };
        SideMiddleLayer(2, 1, amount_down_front);
        //right
        int amount_down_right[4] = { 3, 0, 1, 2 };
        SideMiddleLayer(3, 3, amount_down_right);
        //back
        int amount_down_back[4] = { 2, 3, 0, 1 };
        SideMiddleLayer(4, 5, amount_down_back);
        //left
        int amount_down_left[4] = { 1, 2, 3, 0 };
        SideMiddleLayer(1, 7, amount_down_left);

        if ((Color(1, 3) != 5 && Color(2, 7) != 5) &&
            !(Color(1, 3) == 1 && Color(2, 7) == 2)) {
            RotateMiddleLayer(&RubicCube::Left, &RubicCube::Front);
        }
        if ((Color(2, 3) != 5 && Color(3, 7) != 5) &&
            !(Color(2, 3) == 2 && Color(3, 7) == 3)) {
            RotateMiddleLayer(&RubicCube::Front, &RubicCube::Right);
        }
        if ((Color(3, 3) != 5 && Color(4, 7) != 5) &&
            !(Color(3, 3) == 3 && Color(4, 7) == 4)) {
            RotateMiddleLayer(&RubicCube::Right, &RubicCube::Back);
        }
        if ((Color(4, 3) != 5 && Color(1, 7) != 5) &&
            !(Color(4, 3) == 4 && Color(1, 7) == 1)) {
            RotateMiddleLayer(&RubicCube::Back, &RubicCube::Left);
        }
    }
}

void RubicCube::RotateLastCross(int amount_down) {
    for (int i = 0; i < amount_down; ++i)
        Down();
    Front();
    Left();
    Down();
    Left(true);
    Down(true);
    Front(true);
}

void RubicCube::LastCross() {
    if (Color(5, 1) == 5 &&
        Color(5, 3) == 5 &&
        Color(5, 5) == 5 &&
        Color(5, 7) == 5) {
        return;
    }
    if (Color(5, 1) != 5 &&
        Color(5, 3) != 5 &&
        Color(5, 5) != 5 &&
        Color(5, 7) != 5) {
        RotateLastCross();
    }

    if (Color(5, 1) == 5 && Color(5, 3) == 5)
        RotateLastCross(1);
    else if (Color(5, 3) == 5 && Color(5, 5) == 5)
        RotateLastCross();
    else if (Color(5, 5) == 5 && Color(5, 7) == 5)
        RotateLastCross(3);
    else if (Color(5, 1) == 5 && Color(5, 7) == 5)
        RotateLastCross(2);

    if (Color(5, 1) == 5 && Color(5, 5) == 5)
        RotateLastCross(1);
    else if (Color(5, 3) == 5 && Color(5, 7) == 5)
        RotateLastCross();
}

void  RubicCube::RotateCorrectLastCross(void (RubicCube::*rotate)(bool), int amount_down) {
    for (int i = 0; i < amount_down; ++i)
        Down();
    (this->*rotate)(false);
    Down();
    (this->*rotate)(true);
    Down();
    (this->*rotate)(false);
    Down();
    Down();
    (this->*rotate)(true);
    Down();
    for (int i = 0; (i < 4 - amount_down) && (amount_down != 0); ++i)
        Down();
}

void RubicCube::CorrectLastCross() {
    int count = 0, side = 1;
    while (count < 4) {
        while (Color(side, 5) != side)
            Down(false);
        count = 0;
        for (int i = 1; i <= 4; ++i)
            if (Color(i, 5) == i)
                count++;

        if (count == 2 && side == 1) {
            if (Color(4, 5) == 4)
                RotateCorrectLastCross(&RubicCube::Left);
            else if (Color(2, 5) == 2)
                RotateCorrectLastCross(&RubicCube::Front);
            else if (Color(3, 5) == 3) {
                RotateCorrectLastCross(&RubicCube::Back);
                side = 0;
            }
        }
        if (count == 2 && side == 2) {
            if (Color(1, 5) == 1)
                RotateCorrectLastCross(&RubicCube::Left, 3);
            else if (Color(2, 5) == 2)
                RotateCorrectLastCross(&RubicCube::Front, 3);
            else if (Color(3, 5) == 3) {
                RotateCorrectLastCross(&RubicCube::Back, 3);
                side = 0;
            }
        }
        if (count == 2 && side == 3) {
            if (Color(1, 5) == 1)
                RotateCorrectLastCross(&RubicCube::Left, 2);
            else if (Color(2, 5) == 2)
                RotateCorrectLastCross(&RubicCube::Front, 2);
            else if (Color(3, 5) == 3) {
                RotateCorrectLastCross(&RubicCube::Back, 2);
                side = 0;
            }
        }
        side += 1;
    }
}

void RubicCube::RotateLastLayer() {
    while (Color(5, 0) != 5 || Color(1, 4) != Color(1, 5) || Color(2, 6) != Color(2, 5)) {
        Left(true);
        Up();
        Left();
        Up(true);
    }
}

void RubicCube::LastLayer() {
    int count = 0, vertex = -1;
    while (count < 4) {
        count = 0;
        if ((Color(1, 4) == 1 || Color(2, 6) == 1 || Color(5, 0) == 1) &&
            (Color(1, 4) == 2 || Color(2, 6) == 2 || Color(5, 0) == 2)) {
            vertex = 0;
            count++;
        }
        if ((Color(2, 4) == 2 || Color(3, 6) == 2 || Color(5, 2) == 2) &&
            (Color(2, 4) == 3 || Color(3, 6) == 3 || Color(5, 2) == 3)) {
            vertex = 1;
            count++;
        }
        if ((Color(3, 4) == 3 || Color(4, 6) == 3 || Color(5, 4) == 3) &&
            (Color(3, 4) == 4 || Color(4, 6) == 4 || Color(5, 4) == 4)) {
            vertex = 2;
            count++;
        }
        if ((Color(4, 4) == 1 || Color(1, 6) == 1 || Color(5, 6) == 1) &&
            (Color(4, 4) == 4 || Color(1, 6) == 4 || Color(5, 6) == 4)) {
            vertex = 3;
            count++;
        }
        if (count == 1 || count == 0) {
            for (int i = 0; i < vertex; ++i) {
                Down(true);
            }
            Down();
            Left();
            Down(true);
            Right(true);
            Down();
            Left(true);
            Down(true);
            Right();
            for (int i = 0; i < vertex; ++i) {
                Down();
            }
        }
    }
    RotateLastLayer();
    Down();
    RotateLastLayer();
    Down();
    RotateLastLayer();
    Down();
    RotateLastLayer();

    while (Color(1, 4) != 1)
        Down();
    while (Color(1, 2) != 1)
        Up();
}

void RubicCube::AssembleRubicCube() {
    isOut = true;
    std::cout << "===================\n";
    Flower();
    Cross();
    BangBang();
    MiddleLayer();
    LastCross();
    CorrectLastCross();
    LastLayer();
    std::cout << "\n===================\n";
    isOut = false;
}

void RubicCube::RandomCombination() {
    isOut = true;
    std::cout << "===================\n";
    srand(time(0));
    int size = rand() % 51 + 50;
    int rotate;
    for (int i = 0; i < size; ++i) {
        rotate = rand() % 12;
        switch (rotate)
        {
        case 0:
            Up();
            break;
        case 1:
            Down();
            break;
        case 2:
            Front();
            break;
        case 3:
            Back();
            break;
        case 4:
            Left();
            break;
        case 5:
            Right();
            break;
        case 6:
            Up(true);
            break;
        case 7:
            Down(true);
            break;
        case 8:
            Front(true);
            break;
        case 9:
            Back(true);
            break;
        case 10:
            Left(true);
            break;
        case 11:
            Right(true);
            break;
        }
    }
    std::cout << "\n===================\n";
    isOut = false;
}

std::string DecToHex(int n, std::string str) {
    int a = n % 16;
    if (n / 16 != 0)
        str = DecToHex(n / 16, str);
    str += std::to_string(a);
    return str;
}

int RubicCube::ColorPrint(std::string side, int i) {
    if (static_cast<int>(side.size()) < i)
        return 0;
    else
        return side[side.size() - i] - '0';
}

void RubicCube::Print(bool isFile) {
    std::string side;
    int side_out[6][3][3];
    for (int i = 0; i < 6; ++i) {
        side = DecToHex(sides[i], "");
        side_out[i][0][0] = ColorPrint(side, 8);
        side_out[i][0][1] = ColorPrint(side, 7);
        side_out[i][0][2] = ColorPrint(side, 6);
        side_out[i][1][0] = ColorPrint(side, 1);
        side_out[i][1][1] = i;
        side_out[i][1][2] = ColorPrint(side, 5);
        side_out[i][2][0] = ColorPrint(side, 2);
        side_out[i][2][1] = ColorPrint(side, 3);
        side_out[i][2][2] = ColorPrint(side, 4);

    }

    std::string file;
    isFile ? file = "file.txt" : "del.txt";
    std::ofstream fout(file);
    for (int i = 0; i < 3; ++i) {
        !isFile ? std::cout << "      " : fout << "      " ;
        for (int j = 0; j < 3; ++j) {
            !isFile ? std::cout << side_out[0][i][j] << ' ' : fout << side_out[0][i][j] << ' ';
        }
        !isFile ? std::cout << "\n" : fout << "\n";
    }
    !isFile ? std::cout << "      -----\n" : fout << "      -----\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!isFile) {
                std::cout << side_out[1][i][j];
                j == 2 ? std::cout << '|' : std::cout << ' ';
            }
            else {
                fout << side_out[1][i][j];
                j == 2 ? fout << '|' : fout << ' ';
            }
        }
        for (int j = 0; j < 3; ++j) {
            if (!isFile) {
                std::cout << side_out[2][i][j];
                j == 2 ? std::cout << '|' : std::cout << ' ';
            }
            else {
                fout << side_out[2][i][j];
                j == 2 ? fout << '|' : fout << ' ';
            }
        }
        for (int j = 0; j < 3; ++j) {
            if (!isFile) {
                std::cout << side_out[3][i][j];
                j == 2 ? std::cout << '|' : std::cout << ' ';
            }
            else {
                fout << side_out[3][i][j];
                j == 2 ? fout << '|' : fout << ' ';
            }
        }
        for (int j = 0; j < 3; ++j) {
            !isFile ? std::cout << side_out[4][i][j] << ' ' : fout << side_out[4][i][j] << ' ';
        }
        !isFile ? std::cout << "\n" : fout << "\n";
    }
    !isFile ? std::cout << "      -----\n" : fout << "      -----\n";
    for (int i = 0; i < 3; ++i) {
        !isFile ? std::cout << "      " : fout << "      ";
        for (int j = 0; j < 3; ++j) {
            !isFile ? std::cout << side_out[5][i][j] << ' ' : fout << side_out[5][i][j] << ' ';

        }
        !isFile ? std::cout << "\n" : fout << "\n";
    }
    if (!isFile)
        remove("del.txt");
    fout.close();
}

void RemoveCharacterFromString(char character_for_remove, std::string& string) {
    auto it = std::remove_if(std::begin(string), std::end(string),
        [=](char c) { return c == character_for_remove; });
    string.erase(it, std::end(string));
}

uint32_t DecInHex(int side[3][3]) {
    uint32_t out = 0;
    int pow = 1;
    out += side[1][0] * pow;
    pow *= 16;
    out += side[2][0] * pow;
    pow *= 16;
    out += side[2][1] * pow;
    pow *= 16;
    out += side[2][2] * pow;
    pow *= 16;
    out += side[1][2] * pow;
    pow *= 16;
    out += side[0][2] * pow;
    pow *= 16;
    out += side[0][1] * pow;
    pow *= 16;
    out += side[0][0] * pow;
    pow *= 16;
    return out;
}

void RubicCube::ReadCube() {
    std::ifstream fin("file.txt");
    std::string line;

    int side_out[6][3][3];
    for (int i = 0; i < 3; ++i) {
        std::getline(fin, line);
        RemoveCharacterFromString(' ', line);
        for (int j = 0; j < 3; ++j)
            side_out[0][i][j] = line[j] - '0';
    }
    std::getline(fin, line);
    for (int i = 0; i < 3; ++i) {
        std::getline(fin, line);
        RemoveCharacterFromString(' ', line);
        for (int j = 0; j < 3; ++j)
            side_out[1][i][j] = line[j] - '0';
        for (int j = 0; j < 3; ++j)
            side_out[2][i][j] = line[j + 4] - '0';
        for (int j = 0; j < 3; ++j)
            side_out[3][i][j] = line[j + 8] - '0';
        for (int j = 0; j < 3; ++j)
            side_out[4][i][j] = line[j + 12] - '0';
    }
    std::getline(fin, line);
    for (int i = 0; i < 3; ++i) {
        std::getline(fin, line);
        RemoveCharacterFromString(' ', line);
        for (int j = 0; j < 3; ++j)
            side_out[5][i][j] = line[j] - '0';
    }
    for (int i = 0; i < 6; ++i) {
        sides[i] = DecInHex(side_out[i]);
    }
    fin.close();
}