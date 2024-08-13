/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Character;
class Robot;
class Sherlock;
class Watson;
class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD,
    ITEM_PHARSE_2
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};
enum CharacterType
{
    SHERLOCK,
    WATSON
};

class MapElement
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    ElementType type;

public:
    MapElement(ElementType in_type)
    {
        type = in_type;
    }
    virtual ~MapElement() {}
    virtual ElementType getType() const
    {
        return type;
    }
};

class Path : public MapElement
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

public:
    Path() : MapElement(ElementType::PATH) {}
};

class Wall : public MapElement
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

public:
    Wall() : MapElement(ElementType::WALL) {}
};

class FakeWall : public MapElement
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int req_exp;

public:
    FakeWall(int in_req_exp) : MapElement(ElementType::FAKE_WALL)
    {
        req_exp = in_req_exp;
    }
    int getReqExp() { return req_exp; }
};

class Position
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0)
    {
        this->r = r;
        this->c = c;
    }

    Position(const string &str_pos)
    {
        size_t comma = str_pos.find(',');
        string r = str_pos.substr(1, comma - 1);
        string c = str_pos.substr(comma + 1, str_pos.length() - comma - 2);
        this->r = stoi(r);
        this->c = stoi(c);
    }

    int getRow() const
    {
        return r;
    }
    int getCol() const { return c; }
    void setRow(int r) { this->r = r; }
    void setCol(int c) { this->c = c; }

    string str() const
    {
        ostringstream oss;
        oss << "(" << r << "," << c << ")";
        return oss.str();
    }

    bool isEqual(int in_r, int in_c) const
    {
        return r == in_c && c == in_c;
    }
    bool isEqual(const Position &pos) const
    {
        return r == pos.r && c == pos.c;
    }
};

class Map
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls) : num_rows(num_rows), num_cols(num_cols)
    {
        map = new MapElement **[num_rows];
        for (int i = 0; i < num_rows; ++i)
        {
            map[i] = new MapElement *[num_cols];
        }
        for (int i = 0; i < num_rows; ++i)
        {
            for (int j = 0; j < num_cols; ++j)
            {
                map[i][j] = nullptr;
            }
        }
        for (int i = 0; i < num_walls; ++i)
        {
            int r = array_walls[i].getRow();
            int c = array_walls[i].getCol();
            map[r][c] = new Wall();
        }
        for (int i = 0; i < num_fake_walls; ++i)
        {
            int r = array_fake_walls[i].getRow();
            int c = array_fake_walls[i].getCol();
            map[r][c] = new FakeWall((r * 257 + c * 139 + 89) % 900 + 1);
        }
        for (int i = 0; i < num_rows; ++i)
        {
            for (int j = 0; j < num_cols; ++j)
            {
                if (map[i][j] == nullptr)
                {
                    map[i][j] = new Path();
                }
            }
        }
    }
    ~Map()
    {
        for (int i = 0; i < num_rows; ++i)
        {
            for (int j = 0; j < num_cols; ++j)
            {
                delete map[i][j];
            }
            delete[] map[i];
        }
        delete[] map;
    }

    int getRows()
    {
        return num_rows;
    }
    int getCols()
    {
        return num_cols;
    }

    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class MovingObject
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map *map;
    string name;
    ArrayMovingObject *arr_mv_objs;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "")
    {
        this->index = index;
        this->pos = pos;
        this->map = map;
        this->name = name;
        arr_mv_objs = nullptr;
    }

    string getName() { return name; }
    void setArrMvObjs(ArrayMovingObject *arr_mv_objs) { this->arr_mv_objs = arr_mv_objs; }
    ArrayMovingObject *ArrMvObjs() { return arr_mv_objs; }
    int getIndex() { return index; }
    void setIndex(int index) { this->index = index; }

    virtual ~MovingObject() {}
    Position getCurrentPosition() const { return pos; }

    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
};

class Character : public MovingObject
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    bool immune;
    Robot *robot;

public:
    Character(int index, const Position pos, Map *map, const string &name = "") : MovingObject(index, pos, map, name)
    {
        immune = false;
    }

    bool getImmunity() { return immune; }
    void setImmunity(bool immune) { this->immune = immune; }
    Robot *getRobot()
    {
        return robot;
    }
    void setRobot(Robot *robot)
    {
        this->robot = robot;
    }
};

class Sherlock : public Character
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int hp;
    int exp;
    int step;
    string moving_rule;
    SherlockBag *bag;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock")
    {
        this->moving_rule = moving_rule;
        step = 0;
        hp = init_hp;
        exp = init_exp;
        bag = nullptr;
    }

    int getHP()
    {
        return hp;
    }
    void setHP(int hp)
    {
        if (hp > 500)
        {
            this->hp = 500;
        }
        else if (hp < 0)
        {
            this->hp = 0;
        }
        else
        {
            this->hp = hp;
        }
    }
    int getEXP()
    {
        return exp;
    }
    void setEXP(int exp)
    {
        if (exp > 900)
        {
            this->exp = 900;
        }
        else if (exp < 0)
        {
            this->exp = 0;
        }
        else
        {
            this->exp = exp;
        }
    }
    SherlockBag *getBag()
    {
        return bag;
    }
    void setBag(SherlockBag *bag)
    {
        this->bag = bag;
    }
    string str() const
    {
        ostringstream oss;
        oss << "Sherlock[index=" << index << ";pos=" << pos.str() << ";moving_rule=" << moving_rule << "]";
        return oss.str();
    }
    Position getNextPosition();
    void move();
};

class Watson : public Character
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int hp;
    int exp;
    int step;
    string moving_rule;
    WatsonBag *bag;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson")
    {
        this->moving_rule = moving_rule;
        step = 0;
        hp = init_hp;
        exp = init_exp;
    }
    int getHP()
    {
        return hp;
    }
    void setHP(int hp)
    {
        if (hp > 500)
        {
            this->hp = 500;
        }
        else if (hp < 0)
        {
            this->hp = 0;
        }
        else
        {
            this->hp = hp;
        }
    }
    int getEXP()
    {
        return exp;
    }
    void setEXP(int exp)
    {
        if (exp > 900)
        {
            this->exp = 900;
        }
        else if (exp < 0)
        {
            this->exp = 0;
        }
        else
        {
            this->exp = exp;
        }
    }
    WatsonBag *getBag()
    {
        return bag;
    }
    void setBag(WatsonBag *bag)
    {
        this->bag = bag;
    }
    string str() const
    {
        ostringstream oss;
        oss << "Watson[index=" << index << ";pos=" << pos.str() << ";moving_rule=" << moving_rule << "]";
        return oss.str();
    }
    Position getNextPosition();
    void move();
};

class Criminal : public Character
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
    Watson *watson;
    int step;
    Position previous;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : Character(index, init_pos, map, "Criminal")
    {
        this->sherlock = sherlock;
        this->watson = watson;
        previous = Position::npos;
        step = 0;
    }
    Position getPrevious()
    {
        return previous;
    }
    string str() const
    {
        ostringstream oss;
        oss << "Criminal[index=" << index << ";pos=" << pos.str() << "]";
        return oss.str();
    }
    Position getNextPosition();
    void move();
    void isCreatedRobot(const Position &init_pos);
};

class ArrayMovingObject
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    MovingObject **arr_mv_objs;
    int capacity;
    int count;

public:
    ArrayMovingObject(int capacity)
    {
        arr_mv_objs = new MovingObject *[capacity];
        this->capacity = capacity;
        count = 0;
    }
    ~ArrayMovingObject()
    {
        for (int i = 3; i < count; i++)
        {
            delete arr_mv_objs[i];
        }
        delete[] arr_mv_objs;
    }
    bool isFull() const
    {
        return count == capacity;
    }
    MovingObject *get(int index) const
    {
        return arr_mv_objs[index];
    }
    int size() const { return count; }
    string str() const
    {
        ostringstream oss;
        oss << "ArrayMovingObject[count=" << count << ";capacity=" << capacity;
        for (int i = 0; i < count; i++)
        {
            oss << ";" << arr_mv_objs[i]->str();
        }
        oss << "]";
        return oss.str();
    }

    void remove(int index)
    {
        MovingObject *del = arr_mv_objs[index];
        for (int i = index; i < count - 1; i++)
        {
            arr_mv_objs[i] = arr_mv_objs[i + 1];
            arr_mv_objs[i]->setIndex(arr_mv_objs[i]->getIndex() - 1);
        }
        delete del;
        count--;
    }
    bool add(MovingObject *mv_obj)
    {
        if (isFull())
            return false;
        arr_mv_objs[count++] = mv_obj;
        mv_obj->setArrMvObjs(this);
        return true;
    }
};

class Configuration
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string &filepath)
    {
        ifstream file(filepath);
        string str;

        while (getline(file, str))
        {
            stringstream ss(str);
            string key, value;
            getline(ss, key, '=');
            getline(ss, value);

            if (key == "MAP_NUM_ROWS")
                map_num_rows = stoi(value);
            else if (key == "MAP_NUM_COLS")
                map_num_cols = stoi(value);
            else if (key == "MAX_NUM_MOVING_OBJECTS")
                max_num_moving_objects = stoi(value);
            else if (key == "ARRAY_WALLS")
                arr_walls = setWall(key, value);
            else if (key == "ARRAY_FAKE_WALLS")
                arr_fake_walls = setWall(key, value);
            else if (key == "SHERLOCK_MOVING_RULE")
                sherlock_moving_rule = value;
            else if (key == "SHERLOCK_INIT_POS")
                sherlock_init_pos = setPosition(value);
            else if (key == "SHERLOCK_INIT_HP")
                sherlock_init_hp = stoi(value);
            else if (key == "SHERLOCK_INIT_EXP")
                sherlock_init_exp = stoi(value);
            else if (key == "WATSON_MOVING_RULE")
                watson_moving_rule = value;
            else if (key == "WATSON_INIT_POS")
                watson_init_pos = setPosition(value);
            else if (key == "WATSON_INIT_HP")
                watson_init_hp = stoi(value);
            else if (key == "WATSON_INIT_EXP")
                watson_init_exp = stoi(value);
            else if (key == "CRIMINAL_INIT_POS")
                criminal_init_pos = setPosition(value);
            else if (key == "NUM_STEPS")
                num_steps = stoi(value);
        }
    }
    ~Configuration()
    {
        if (num_walls != 0)
            delete[] arr_walls;
        if (num_fake_walls != 0)
            delete[] arr_fake_walls;
    }
    string str() const
    {

        ostringstream oss;
        oss << "Configuration[\n";
        oss << "MAP_NUM_ROWS=" << map_num_rows << "\n";
        oss << "MAP_NUM_COLS=" << map_num_cols << "\n";
        oss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << "\n";
        oss << "NUM_WALLS=" << num_walls << "\n";
        oss << "ARRAY_WALLS=[";
        if (num_walls > 0)
        {
            oss << arr_walls[0].str();
            for (int i = 1; i < num_walls; i++)
            {
                oss << ";" << arr_walls[i].str();
            }
        }
        oss << "]\n";
        oss << "NUM_FAKE_WALLS=" << num_fake_walls << "\n";
        oss << "ARRAY_FAKE_WALLS=[";
        if (num_fake_walls > 0)
        {
            oss << arr_fake_walls[0].str();
            for (int i = 1; i < num_fake_walls; i++)
            {
                oss << arr_fake_walls[i].str();
            }
        }
        oss << "]\n";
        oss << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << "\n";
        oss << "SHERLOCK_INIT_POS=" << sherlock_init_pos.str() << "\n";
        oss << "SHERLOCK_INIT_HP=" << sherlock_init_hp << "\n";
        oss << "SHERLOCK_INIT_EXP=" << sherlock_init_exp << "\n";
        oss << "WATSON_MOVING_RULE=" << watson_moving_rule << "\n";
        oss << "WATSON_INIT_POS=" << watson_init_pos.str() << "\n";
        oss << "WATSON_INIT_HP=" << watson_init_hp << "\n";
        oss << "WATSON_INIT_EXP=" << watson_init_exp << "\n";
        oss << "CRIMINAL_INIT_POS=" << criminal_init_pos.str() << "\n";
        oss << "NUM_STEPS=" << num_steps << "\n";
        oss << "]";
        return oss.str();
    }
    Position *setWall(const string &key, const string &value)
    {
        stringstream ss(value.substr(1, value.size() - 2));
        string pos_str;

        int size = 0;
        while (getline(ss, pos_str, ';'))
        {
            size++;
        }

        if (key == "ARRAY_WALLS")
        {
            num_walls = size;
        }
        else
        {
            num_fake_walls = size;
        }
        if (size == 0)
            return nullptr;

        Position *array = new Position[size];

        ss.clear();
        ss.str(value.substr(1, value.size() - 2));

        size = 0;
        while (getline(ss, pos_str, ';'))
        {
            array[size++] = setPosition(pos_str);
        }
        return array;
    }
    Position setPosition(const string &value)
    {
        stringstream ss(value.substr(1, value.size() - 2));
        int r, c;
        char comma;
        ss >> r >> comma >> c;
        return Position(r, c);
    }
};

class BaseItem
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    ItemType type;

public:
    BaseItem *next;
    BaseItem(ItemType type)
    {
        this->type = type;
        next = nullptr;
    }

    ItemType getType()
    {
        return type;
    }

    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
};

class MagicBook : public BaseItem
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

public:
    MagicBook() : BaseItem(ItemType::MAGIC_BOOK) {}

    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class EnergyDrink : public BaseItem
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

public:
    EnergyDrink() : BaseItem(ItemType::ENERGY_DRINK) {}

    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class FirstAid : public BaseItem
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

public:
    FirstAid() : BaseItem(ItemType::FIRST_AID) {}

    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class ExcemptionCard : public BaseItem
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

public:
    ExcemptionCard() : BaseItem(ItemType::EXCEMPTION_CARD) {}

    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class PassingCard : public BaseItem
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    string challenge;

public:
    PassingCard(string challenge) : BaseItem(ItemType::PASSING_CARD)
    {
        this->challenge = challenge;
    }

    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class Robot : public MovingObject
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    RobotType type;
    BaseItem *item;

public:
    Robot(int index, const Position &init_pos, Map *map) : MovingObject(index, init_pos, map, "Robot")
    {
        int i = init_pos.getRow();
        int j = init_pos.getCol();
        int p = i * j;
        int s = recursiveSum(p);

        if (s >= 0 && s <= 1)
        {
            item = new MagicBook();
        }
        else if (s >= 2 && s <= 3)
        {
            item = new EnergyDrink();
        }
        else if (s >= 4 && s <= 5)
        {
            item = new FirstAid();
        }
        else if (s >= 6 && s <= 7)
        {
            item = new ExcemptionCard();
        }
        else if (s >= 8 && s <= 9)
        {
            int t = (i * 11 + j) % 4;
            if (t == 0)
            {
                item = new PassingCard("RobotS");
            }
            else if (t == 1)
            {
                item = new PassingCard("RobotC");
            }
            else if (t == 2)
            {
                item = new PassingCard("RobotSW");
            }
            else if (t == 3)
            {
                item = new PassingCard("all");
            }
        }
    }

    ~Robot() {}

    int recursiveSum(int num)
    {
        if (num < 10)
        {
            return num;
        }

        int sum = 0;
        while (num)
        {
            sum += num % 10;
            num = num / 10;
        }

        return recursiveSum(sum);
    }

    BaseItem *getItem() { return item; }

    string str() const
    {
        string type_str = "";
        switch (type)
        {
        case RobotType::C:
            type_str = "C";
            break;
        case RobotType::S:
            type_str = "S";
            break;
        case RobotType::W:
            type_str = "W";
            break;
        default:
            type_str = "SW";
            break;
        }

        std::ostringstream oss;
        oss << "Robot[pos=" << pos.str() << ";type=" << type_str << ";dist=";
        if (type_str != "C")
        {
            oss << getDistance();
        }
        oss << "]";
        return oss.str();
    }
    RobotType getType() const
    {
        return type;
    }

    virtual int getDistance() const = 0;
    void move();
};

class RobotC : public Robot
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    Criminal *criminal;

public:
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map)
    {
        type = RobotType::C;
        this->criminal = criminal;
    }

    Position getNextPosition()
    {
        return criminal->getPrevious();
    }
    int getDistance() const
    {
        return 0;
    }
};

class RobotS : public Robot
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    Criminal *criminal;
    Sherlock *sherlock;

public:
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock) : Robot(index, init_pos, map)
    {
        type = RobotType::S;
        this->criminal = criminal;
        this->sherlock = sherlock;
    }

    Position getNextPosition();
    int getDistance() const;
};

class RobotW : public Robot
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    Criminal *criminal;
    Watson *watson;

public:
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson) : Robot(index, init_pos, map)
    {
        type = RobotType::W;
        this->criminal = criminal;
        this->watson = watson;
    }
    Position getNextPosition();
    int getDistance() const;
};

class RobotSW : public Robot
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    Criminal *criminal;
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson) : Robot(index, init_pos, map)
    {
        type = RobotType::SW;
        this->sherlock = sherlock;
        this->criminal = criminal;
        this->watson = watson;
    }
    Position getNextPosition();
    int getDistance() const;
};

class BaseBag
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

protected:
    BaseItem *head;
    Character *obj;
    int count;

public:
    BaseBag(Character *obj)
    {
        this->obj = obj;
        head = nullptr;
        count = 0;
    }
    virtual bool insert(BaseItem *item) { return false; }
    virtual string str() const
    {
        ostringstream oss;
        oss << "Bag[count=" << count;
        BaseItem *item = head;
        while (item)
        {
            string type;
            switch (item->getType())
            {
            case ItemType::MAGIC_BOOK:
                type = "MagicBook";
                break;
            case ItemType::ENERGY_DRINK:
                type = "EnergyDrink";
                break;
            case ItemType::FIRST_AID:
                type = "FirstAid";
                break;
            case ItemType::EXCEMPTION_CARD:
                type = "ExcemptionCard";
                break;
            case ItemType::PASSING_CARD:
                type = "PassingCard";
                break;
            default:
                break;
            }
            oss << ";" << type;
            item = item->next;
        }
        oss << "]";
        return oss.str();
    }
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);

    void swapItem(int left, int right)
    {
        if (left == right)
            return;

        BaseItem *prevLeft = nullptr, *currLeft = head;
        int indexLeft = 0;
        BaseItem *prevRight = nullptr, *currRight = head;
        int indexRight = 0;

        while (currLeft && indexLeft != left)
        {
            prevLeft = currLeft;
            currLeft = currLeft->next;
            indexLeft++;
        }

        while (currRight && indexRight != right)
        {
            prevRight = currRight;
            currRight = currRight->next;
            indexRight++;
        }

        if (currLeft == nullptr || currRight == nullptr)
            return;

        if (prevLeft != nullptr)
        {
            prevLeft->next = currRight;
        }
        else
            head = currRight;

        if (prevRight != nullptr)
        {
            prevRight->next = currLeft;
        }
        else
            head = currLeft;

        BaseItem *item = currRight->next;
        currRight->next = currLeft->next;
        currLeft->next = item;
    }

    bool include(ItemType itemType)
    {
        if (!head)
            return false;

        BaseItem *item = head;
        while (item)
        {
            if (item->getType() == itemType)
                return true;
            item = item->next;
        }

        return false;
    }
};

class SherlockBag : public BaseBag
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int capacity;

public:
    SherlockBag(Character *sherlock) : BaseBag(sherlock)
    {
        capacity = 13;
    }

    bool insert(BaseItem *item);
};

class WatsonBag : public BaseBag
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int capacity;

public:
    WatsonBag(Character *watson) : BaseBag(watson)
    {
        capacity = 15;
    }

    bool insert(BaseItem *item);
};

class StudyPinkProgram
{
private:
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;
    SherlockBag *sherlockBag;
    WatsonBag *watsonBag;

public:
    static bool end;
    StudyPinkProgram(const string &config_file_path)
    {
        config = new Configuration(config_file_path);
        map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
        sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
        watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
        criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

        arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
        arr_mv_objs->add(criminal);
        arr_mv_objs->add(sherlock);
        arr_mv_objs->add(watson);
        criminal->setArrMvObjs(arr_mv_objs);
        sherlock->setArrMvObjs(arr_mv_objs);
        watson->setArrMvObjs(arr_mv_objs);

        sherlockBag = new SherlockBag(sherlock);
        watsonBag = new WatsonBag(watson);
        sherlock->setBag(sherlockBag);
        watson->setBag(watsonBag);
    }

    ~StudyPinkProgram()
    {
        delete config;
        delete map;
        delete sherlockBag;
        delete watsonBag;
        delete arr_mv_objs;
        delete sherlock;
        delete criminal;
        delete watson;
    }

    bool isStop() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            return true;
        }

        return sherlock->getHP() == 0 || watson->getHP() == 0 || end;
    }

    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose)
    {
        for (int istep = 0; istep < config->num_steps; ++istep)
        {
            for (int i = 0; i < arr_mv_objs->size(); ++i)
            {
                arr_mv_objs->get(i)->move();
                if (isStop())
                {
                    printStep(istep);
                    break;
                }
                if (verbose)
                {
                    printStep(istep);
                }
            }
        }
        printResult();
    }
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
