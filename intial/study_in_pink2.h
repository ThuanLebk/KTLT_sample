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

class Sherlock;
class Watson;
class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class Character;
class Robot;
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

class MapElement
{
protected:
    ElementType type;

public:
    MapElement(ElementType in_type);

    virtual ~MapElement();

    virtual ElementType getType() const;
};

class Path : public MapElement
{
public:
    Path();
};

class Wall : public MapElement
{
public:
    Wall();
};

class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    FakeWall(int req_exp);

    int getReqExp() const;
};

class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);

    ~Map();

    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class Position
{
private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0);

    Position(const string &str_pos);

    int getRow() const;

    int getCol() const;

    void setRow(int r);

    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;

    bool isEqual(const Position &pos) const;

    Position incrementUp() const;

    Position incrementDown() const;

    Position incrementLeft() const;

    Position incrementRight() const;
};

class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject();
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();

    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;

    Position getCurrentPosition() const;

public:
    int getIndex() const;
    string getName() const;
};

class Character : public MovingObject
{
public:
    Character();
    virtual ~Character();
};

class Sherlock : public Character
{
private:
    string moving_rule;
    int hp;
    int exp;
    int moving_rule_index;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);

    ~Sherlock() {}

    Position getNextPosition() override;

    void move() override;

    string str() const override;

    int getExp() const;

    int getHp() const;

    void setExp(int exp);

    void setHp(int hp);
};

class Watson : public Character
{
private:
    string moving_rule;
    int hp;
    int exp;
    int moving_rule_index;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);

    ~Watson() {}

    Position getNextPosition() override;

    void move() override;

    string str() const override;
    
    int getExp() const;

    int getHp() const;

    void setExp(int exp);

    void setHp(int hp);
};

class Criminal : public Character
{
private:
    int num_moves;
    Position lastMove;
    Sherlock *sherlock;
    Watson *watson;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);

    Position getNextPosition() override;

    void move() override;

    string str() const override;

    ~Criminal();

    int getNumMoves() const;

    Position getLastMove() const;
};

class ArrayMovingObject
{
private:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject();
    
    bool isFull() const;

    bool add(MovingObject *mv_obj);

    bool remove(int index);

    MovingObject *get(int index) const;

    int size() const;

    string str() const;
};

class Configuration
{
    friend class StudyPinkProgram;

private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    int num_fake_walls;

    Position *arr_walls;
    Position *arr_fake_walls;

    string sherlock_moving_rule;
    string watson_moving_rule;

    Position sherlock_init_pos;
    Position watson_init_pos;
    Position criminal_init_pos;

    int sherlock_init_hp;
    int sherlock_init_exp;
    int watson_init_hp;
    int watson_init_exp;

    int num_steps;

public:
    Configuration(const string &filepath);

    ~Configuration();

    string str() const;

private:
    int countChar(string s, char c);
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject
{
protected:
    RobotType type;
    BaseItem *item;

public:
    Robot();

    virtual ~Robot();

    RobotType getType() const;

    BaseItem *getItem() const;
    
    void setItem(BaseItem *item);

    virtual string getTypeString() const;
};

class RobotC : public Robot
{
private:
    Criminal *criminal;

public:
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);

    Position getNextPosition();

    void move();

    string str() const;

    ~RobotC() {}

    int getDistance(Sherlock *sherlock) const;

    int getDistance(Watson *watson) const;
};

class RobotS : public Robot
{
private:
    Sherlock *sherlock;

public:
    RobotS(int index, const Position &init_pos, Map *map, Sherlock *sherlock);

    Position getNextPosition();

    void move();

    string str() const;

    ~RobotS();

    int getDistance() const;
};

class RobotW : public Robot
{
private:
    Watson *watson;

public:
    RobotW(int index, const Position &init_pos, Map *map, Watson *watson);

    Position getNextPosition();

    void move();

    string str() const;

    ~RobotW() {}

    int getDistance() const;
};

class RobotSW : public Robot
{
private:
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);

    Position getNextPosition();

    void move();

    string str() const;

    ~RobotSW() {}

    int getDistance() const;
};

class BaseItem
{
protected:
    ItemType type;

public:
    BaseItem();
    BaseItem(ItemType type);
    virtual ~BaseItem();
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;

    ItemType getType() const;
};

class MagicBook : public BaseItem
{
public:
    MagicBook();

    bool canUse(Character *obj, Robot *robot) override;

    void use(Character *obj, Robot *robot) override;
};

class EnergyDrink : public BaseItem
{
public:
    EnergyDrink();

    bool canUse(Character *obj, Robot *robot) override;

    void use(Character *obj, Robot *robot) override;
};

class FirstAid : public BaseItem
{
public:
    FirstAid();

    bool canUse(Character *obj, Robot *robot) override;

    void use(Character *obj, Robot *robot) override;
};

class ExemptionCard : public BaseItem
{
public:
    ExemptionCard();

    bool canUse(Character *obj, Robot *robot) override;

    void use(Character *obj, Robot *robot) override;
};

class PassingCard : public BaseItem
{
private:
    string challenge;

public:
    PassingCard(string challenge);

    bool canUse(Character *obj, Robot *robot) override;

    void use(Character *obj, Robot *robot) override;
};

class BaseBag
{
private:
    struct BagNode
    {
        BaseItem *item;
        BagNode *next;

        BagNode()
        {
            this->item = nullptr;
            this->next = nullptr;
        }

        BagNode(BaseItem *item)
        {
            this->item = item;
            this->next = nullptr;
        }

        BagNode(BaseItem *item, BagNode *next)
        {
            this->item = item;
            this->next = next;
        }
    };

    BagNode *head;
    int size;
    int capacity;

    Character *owner;

public:
    BaseBag(int capacity, Character *owner);

    ~BaseBag();

    virtual bool insert(BaseItem *item);

    virtual BaseItem *get(); 

    virtual BaseItem *get(ItemType itemType);

    virtual string str() const;

    virtual int getSize() const { return size; }

    virtual int getCapacity() const { return capacity; }

    virtual bool isFull() const { return size == capacity; }

    virtual bool isEmpty() const { return size == 0; }

public:
    virtual BaseItem* getForRobot(Robot *robot) = 0;

    virtual BaseItem* getForRobot(Robot *robot, ItemType itemType) = 0;

protected:
    bool insertToHead(BaseItem *item);

    BaseItem *getFirstUsable(Robot*);

    BaseItem *getFirstUsable(Robot*, ItemType itemType);

    void swapWithHead(BagNode *node);

private: 
    BaseItem *removeHead();
};

class SherlockBag : public BaseBag
{
public:
    SherlockBag(Sherlock *sherlock);

    BaseItem* getForRobot(Robot *robot) override;

    BaseItem* getForRobot(Robot *robot, ItemType itemType) override;
};

class WatsonBag : public BaseBag
{
public:
    WatsonBag(Watson *watson);

    BaseItem* getForRobot(Robot *robot) override;

    BaseItem* getForRobot(Robot *robot, ItemType itemType) override;
};

class StudyPinkProgram
{
private:
    // Sample attributes
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;

    SherlockBag *sherlockBag;
    WatsonBag *watsonBag;

    bool stopped;

public:
    StudyPinkProgram(const string &config_file_path);

    bool isStop() const;

    void printResult() const;

    void printStep(int si) const;

    void run(bool verbose);

    ~StudyPinkProgram();

private:
    // Fighting methods
    void generalProcessing(MovingObject*);

    // Check meeting
    int isSherlockMeetRobotC();
    int isSherlockMeetRobotS();
    int isSherlockMeetRobotW();
    int isSherlockMeetRobotSW();

    int isWatsonMeetRobotC();
    int isWatsonMeetRobotS();
    int isWatsonMeetRobotW();
    int isWatsonMeetRobotSW();

    bool isSherlockMeetCriminal();
    bool isWatsonMeetCriminal();
    bool isSherlockMeetWatson();

    // Process fighting
    void processSherlockMeetRobotC(int indexRobot);
    void processSherlockMeetRobotS(int indexRobot);
    void processSherlockMeetRobotW(int indexRobot);
    void processSherlockMeetRobotSW(int indexRobot);

    void processWatsonMeetRobotC(int indexRobot);
    void processWatsonMeetRobotS(int indexRobot);
    void processWatsonMeetRobotW(int indexRobot);
    void processWatsonMeetRobotSW(int indexRobot);

    // Bag actions
    void processSherlockSearchItem();
    void processWatsonSearchItem();
    void addToSherlockBag(BaseItem *item);
    void addToWatsonBag(BaseItem *item);

    // Create new robot
    Robot* createRobot(int i, int j);
    BaseItem* createRobotItem(int i, int j);

    // Process Sherlock and Watson meeting
    void processSherlockMeetWatson();

    // Process Sherlock and Watson before meeting with Robot, return item type that Character used
    ItemType processSherlockBeforeMeetRobot(Robot*);
    ItemType processWatsonBeforeMeetRobot(Robot*);
    
    // Process Sherlock and Watson after meeting with Robot, return item type that Character used
    ItemType processSherlockAfterMeetRobot(Robot*);
    ItemType processWatsonAfterMeetRobot(Robot*);

    // Process Robot killed
    void processDefeatedRobot(int index);
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
