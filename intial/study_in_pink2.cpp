#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

const Position Position::npos(-1, -1);

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

MapElement::MapElement(ElementType in_type)
{
    this->type = in_type;
}

MapElement::~MapElement()
{
}

ElementType MapElement::getType() const
{
    return this->type;
}

Path::Path() : MapElement(PATH) {}

Wall::Wall() : MapElement(WALL) {}

FakeWall::FakeWall(int req_exp) : MapElement(FAKE_WALL)
{
    this->req_exp = req_exp;
}

int FakeWall::getReqExp() const
{
    return this->req_exp;
}

Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
    : num_rows(num_rows), num_cols(num_cols)
{
    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++)
    {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++)
        {
            map[i][j] = new Path();
        }
    }

    for (int i = 0; i < num_walls; i++)
    {
        int x = array_walls[i].getRow();
        int y = array_walls[i].getCol();

        delete map[x][y];
        map[x][y] = new Wall();
    }

    for (int i = 0; i < num_fake_walls; i++)
    {
        int x = array_fake_walls[i].getRow();
        int y = array_fake_walls[i].getCol();

        //(r * 257 + c * 139 + 89)%900 + 1
        int req_exp = (x * 257 + y * 139 + 89) % 900 + 1;

        delete map[x][y];
        map[x][y] = new FakeWall(req_exp);
    }
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    if (pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols)
    {
        return false;
    }

    if (map[pos.getRow()][pos.getCol()]->getType() == WALL)
    {
        return false;
    }

    if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL)
    {
        FakeWall *fake_wall = dynamic_cast<FakeWall *>(map[pos.getRow()][pos.getCol()]);

        if (mv_obj->getName() == "Watson")
        {
            Watson *watson = dynamic_cast<Watson *>(mv_obj);

            return watson->getExp() > fake_wall->getReqExp();
        }

        return true;
    }

    return true;
}

Map::~Map()
{
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
}

Position::Position(const string &str_pos)
{
    // str_pos = "(1,15)"
    int pos1 = str_pos.find(",");
    int pos2 = str_pos.find(")");
    this->r = stoi(str_pos.substr(1, pos1 - 1));
    this->c = stoi(str_pos.substr(pos1 + 1, pos2 - pos1 - 1));
}

int Position::getRow() const
{
    return this->r;
}

int Position::getCol() const
{
    return this->c;
}

void Position::setRow(int r)
{
    this->r = r;
}

void Position::setCol(int c)
{
    this->c = c;
}

string Position::str() const
{

    return "(" + to_string(this->r) + "," + to_string(this->c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const
{

    return this->r == in_r && this->c == in_c;
}

bool Position::isEqual(const Position &pos) const
{
    return this->r == pos.r && this->c == pos.c;
}

Position Position::incrementUp() const
{
    return Position(this->r - 1, this->c);
}

Position Position::incrementDown() const
{
    return Position(this->r + 1, this->c);
}

Position Position::incrementLeft() const
{
    return Position(this->r, this->c - 1);
}

Position Position::incrementRight() const
{
    return Position(this->r, this->c + 1);
}

MovingObject::MovingObject()
{
}

MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
}

MovingObject::~MovingObject()
{
}

Position MovingObject::getCurrentPosition() const
{
    return this->pos;
}

string MovingObject::getName() const
{
    return this->name;
}

int MovingObject::getIndex() const
{
    return this->index;
}

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
{

    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "Sherlock";

    this->moving_rule = moving_rule;
    this->moving_rule_index = 0;
    this->hp = min(max(init_hp, 0), 500);
    this->exp = min(max(init_exp, 0), 900);
}

Position Sherlock::getNextPosition()
{
    Position next_pos = this->pos;

    char move = this->moving_rule[this->moving_rule_index];
    moving_rule_index = (moving_rule_index + 1) % moving_rule.size();

    if (move == 'U')
    {
        next_pos = next_pos.incrementUp();
    }
    else if (move == 'D')
    {
        next_pos = next_pos.incrementDown();
    }
    else if (move == 'L')
    {
        next_pos = next_pos.incrementLeft();
    }
    else if (move == 'R')
    {
        next_pos = next_pos.incrementRight();
    }

    if (map->isValid(next_pos, this))
    {
        return next_pos;
    }
    else
    {
        return Position::npos;
    }
}

void Sherlock::move()
{
    if (hp == 0) // Sherlock is dead
    {
        return;
    }

    if (exp == 0) // Sherlock cannot move
    {
        return;
    }

    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))
    {
        return;
    }

    this->pos = next_pos;
}

string Sherlock::str() const
{

    // Sherlock[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Sherlock[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
}

int Sherlock::getExp() const
{
    return this->exp;
}

int Sherlock::getHp() const
{
    return this->hp;
}

void Sherlock::setExp(int exp)
{
    this->exp = min(max(exp, 0), 900);
}

void Sherlock::setHp(int hp)
{
    this->hp = min(max(hp, 0), 500);
}

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
{
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "Watson";

    this->moving_rule = moving_rule;
    this->moving_rule_index = 0;
    this->hp = min(max(init_hp, 0), 500);
    this->exp = min(max(init_exp, 0), 900);
}

Position Watson::getNextPosition()
{
    Position next_pos = this->pos;

    char move = this->moving_rule[this->moving_rule_index];
    moving_rule_index = (moving_rule_index + 1) % moving_rule.size();

    if (move == 'U')
    {
        next_pos = next_pos.incrementUp();
    }
    else if (move == 'D')
    {
        next_pos = next_pos.incrementDown();
    }
    else if (move == 'L')
    {
        next_pos = next_pos.incrementLeft();
    }
    else if (move == 'R')
    {
        next_pos = next_pos.incrementRight();
    }

    if (map->isValid(next_pos, this))
    {
        return next_pos;
    }
    else
    {
        return Position::npos;
    }
}

void Watson::move()
{
    if (hp == 0) // Watson is dead
    {
        return;
    }

    if (exp == 0) // Watson cannot move
    {
        return;
    }

    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))
    {
        return;
    }

    this->pos = next_pos;
}

string Watson::str() const
{
    // Watson[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Watson[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
}

int Watson::getExp() const
{
    return this->exp;
}

int Watson::getHp() const
{
    return this->hp;
}

void Watson::setExp(int exp)
{
    this->exp = min(max(exp, 0), 900);
}

void Watson::setHp(int hp)
{
    this->hp = min(max(hp, 0), 500);
}

Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
{
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "Criminal";

    this->num_moves = 0;
    this->lastMove = Position::npos;
    this->sherlock = sherlock;
    this->watson = watson;
}

Position Criminal::getNextPosition()
{
    // Calculate 4 direction sum of distances to Sherlock and Watson, priority from ULDR
    Position sherlock_pos = sherlock->getCurrentPosition();
    Position watson_pos = watson->getCurrentPosition();

    Position UP_POS = this->pos.incrementUp();
    Position LEFT_POS = this->pos.incrementLeft();
    Position DOWN_POS = this->pos.incrementDown();
    Position RIGHT_POS = this->pos.incrementRight();

    int max_dist = INT_MIN;
    Position nextPos = Position::npos;

    // UP
    if (map->isValid(UP_POS, this))
    {
        int dist = abs(UP_POS.getRow() - sherlock_pos.getRow()) + abs(UP_POS.getCol() - sherlock_pos.getCol()) + abs(UP_POS.getRow() - watson_pos.getRow()) + abs(UP_POS.getCol() - watson_pos.getCol());
        if (dist > max_dist)
        {
            max_dist = dist;
            nextPos = UP_POS;
        }
    }

    // LEFT
    if (map->isValid(LEFT_POS, this))
    {
        int dist = abs(LEFT_POS.getRow() - sherlock_pos.getRow()) + abs(LEFT_POS.getCol() - sherlock_pos.getCol()) + abs(LEFT_POS.getRow() - watson_pos.getRow()) + abs(LEFT_POS.getCol() - watson_pos.getCol());
        if (dist > max_dist)
        {
            max_dist = dist;
            nextPos = LEFT_POS;
        }
    }

    // DOWN
    if (map->isValid(DOWN_POS, this))
    {
        int dist = abs(DOWN_POS.getRow() - sherlock_pos.getRow()) + abs(DOWN_POS.getCol() - sherlock_pos.getCol()) + abs(DOWN_POS.getRow() - watson_pos.getRow()) + abs(DOWN_POS.getCol() - watson_pos.getCol());
        if (dist > max_dist)
        {
            max_dist = dist;
            nextPos = DOWN_POS;
        }
    }

    // RIGHT
    if (map->isValid(RIGHT_POS, this))
    {
        int dist = abs(RIGHT_POS.getRow() - sherlock_pos.getRow()) + abs(RIGHT_POS.getCol() - sherlock_pos.getCol()) + abs(RIGHT_POS.getRow() - watson_pos.getRow()) + abs(RIGHT_POS.getCol() - watson_pos.getCol());
        if (dist > max_dist)
        {
            max_dist = dist;
            nextPos = RIGHT_POS;
        }
    }

    return nextPos;
}

void Criminal::move()
{
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))
    {
        return;
    }

    this->lastMove = this->pos;
    this->pos = next_pos;
    this->num_moves++;
}

string Criminal::str() const
{
    // Criminal[index=<index>;pos=<pos>]
    return "Criminal[index=" + to_string(this->index) + ";pos=" + this->pos.str() + "]";
}

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    this->arr_mv_objs = new MovingObject *[capacity];
}

ArrayMovingObject::~ArrayMovingObject()
{
    // Start from 3 because 0, 1, 2 are Sherlock, Watson, Criminal are already deleted
    for (int i = 3; i < this->count; i++)
    {
        delete this->arr_mv_objs[i];
    }
    delete[] this->arr_mv_objs;
}

bool ArrayMovingObject::isFull() const
{
    return this->count == this->capacity;
}

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (isFull())
    {
        return false;
    }

    this->arr_mv_objs[this->count++] = mv_obj;
    return true;
}

MovingObject *ArrayMovingObject::get(int index) const
{
    if (index < 0 || index >= this->count)
    {
        return nullptr;
    }

    return this->arr_mv_objs[index];
}

int ArrayMovingObject::size() const
{
    return this->count;
}

string ArrayMovingObject::str() const
{
    string res = "ArrayMovingObject[count=" + to_string(this->count) + ";capacity=" + to_string(this->capacity);

    for (int i = 0; i < this->count; i++)
    {
        res += ";" + this->arr_mv_objs[i]->str();
    }

    return res;
}

Configuration::Configuration(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.find("MAP_NUM_ROWS") != string::npos)
        {
            map_num_rows = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("MAP_NUM_COLS") != string::npos)
        {
            map_num_cols = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("MAX_NUM_MOVING_OBJECTS") != string::npos)
        {
            max_num_moving_objects = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("SHERLOCK_MOVING_RULE") != string::npos)
        {
            sherlock_moving_rule = line.substr(line.find("=") + 1);
        }
        else if (line.find("WATSON_MOVING_RULE") != string::npos)
        {
            watson_moving_rule = line.substr(line.find("=") + 1);
        }
        else if (line.find("SHERLOCK_INIT_POS") != string::npos)
        {
            sherlock_init_pos = Position(line.substr(line.find("=") + 1));
        }
        else if (line.find("WATSON_INIT_POS") != string::npos)
        {
            watson_init_pos = Position(line.substr(line.find("=") + 1));
        }
        else if (line.find("CRIMINAL_INIT_POS") != string::npos)
        {
            criminal_init_pos = Position(line.substr(line.find("=") + 1));
        }
        else if (line.find("SHERLOCK_INIT_HP") != string::npos)
        {
            sherlock_init_hp = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("SHERLOCK_INIT_EXP") != string::npos)
        {
            sherlock_init_exp = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("WATSON_INIT_HP") != string::npos)
        {
            watson_init_hp = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("WATSON_INIT_EXP") != string::npos)
        {
            watson_init_exp = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("NUM_STEPS") != string::npos)
        {
            num_steps = stoi(line.substr(line.find("=") + 1));
        }
        else if (line.find("ARRAY_WALLS") != string::npos)
        {
            num_walls = countChar(line, '(');

            if (num_walls == 0)
            {
                continue;
            }

            arr_walls = new Position[num_walls];

            for (int i = line.find("("), j = 0; i != string::npos; i = line.find("("), j++)
            {
                int pos1 = line.find("(");
                int pos2 = line.find(")");
                arr_walls[j] = Position(line.substr(pos1, pos2 - pos1 + 1));
                line = line.substr(pos2 + 1);
            }
        }
        else if (line.find("ARRAY_FAKE_WALLS") != string::npos)
        {
            num_fake_walls = countChar(line, '(');

            if (num_fake_walls == 0)
            {
                continue;
            }

            arr_fake_walls = new Position[num_fake_walls];

            for (int i = line.find("("), j = 0; i != string::npos; i = line.find("("), j++)
            {
                int pos1 = line.find("(");
                int pos2 = line.find(")");
                arr_fake_walls[j] = Position(line.substr(pos1, pos2 - pos1 + 1));
                line = line.substr(pos2 + 1);
            }
        }
    }
}

Configuration::~Configuration()
{
    delete[] arr_walls;
    delete[] arr_fake_walls;
}

string Configuration::str() const
{
    string res = "Configuration[\n";

    res += "MAP_NUM_ROWS=" + to_string(this->map_num_rows) + "\n";
    res += "MAP_NUM_COLS=" + to_string(this->map_num_cols) + "\n";
    res += "MAX_NUM_MOVING_OBJECTS=" + to_string(this->max_num_moving_objects) + "\n";
    res += "NUM_WALLS=" + to_string(this->num_walls) + "\n";

    res += "ARRAY_WALLS=[";
    for (int i = 0; i < this->num_walls; i++)
    {
        res += arr_walls[i].str();

        if (i != this->num_walls - 1)
        {
            res += ";";
        }
    }
    res += "]\n";

    res += "NUM_FAKE_WALLS=" + to_string(this->num_fake_walls) + "\n";
    res += "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < this->num_fake_walls; i++)
    {
        res += arr_fake_walls[i].str();

        if (i != this->num_fake_walls - 1)
        {
            res += ";";
        }
    }
    res += "]\n";

    res += "SHERLOCK_MOVING_RULE=" + this->sherlock_moving_rule + "\n";
    res += "SHERLOCK_INIT_POS=" + this->sherlock_init_pos.str() + "\n";
    res += "SHERLOCK_INIT_HP=" + to_string(this->sherlock_init_hp) + "\n";
    res += "SHERLOCK_INIT_EXP=" + to_string(this->sherlock_init_exp) + "\n";
    res += "WATSON_MOVING_RULE=" + this->watson_moving_rule + "\n";
    res += "WATSON_INIT_POS=" + this->watson_init_pos.str() + "\n";
    res += "WATSON_INIT_HP=" + to_string(this->watson_init_hp) + "\n";
    res += "WATSON_INIT_EXP=" + to_string(this->watson_init_exp) + "\n";
    res += "CRIMINAL_INIT_POS=" + this->criminal_init_pos.str() + "\n";
    res += "NUM_STEPS=" + to_string(this->num_steps) + "\n]";

    return res;
}

int Configuration::countChar(string s, char c)
{
    int count = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == c)
        {
            count++;
        }
    }

    return count;
}

Robot::Robot()
{
}

Robot::~Robot()
{
}

RobotType Robot::getType() const
{
    return this->type;
}

BaseItem *Robot::getItem() const
{
    return this->item;
}

void Robot::setItem(BaseItem *item)
{
    this->item = item;
}

string Robot::getTypeString() const
{
    switch (this->type)
    {
    case C:
        return "C";
    case S:
        return "S";
    case W:
        return "W";
    case SW:
        return "SW";
    }

    return "INVALID";
}

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal)
{
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "RobotC";
    this->type = C;
    this->item = nullptr;

    this->criminal = criminal;
}

Position RobotC::getNextPosition()
{
    return criminal->getLastMove();
}

void RobotC::move()
{
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))
    {
        return;
    }

    this->pos = next_pos;
}

string RobotC::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=" + this->getTypeString() + ";dist=]";
}

int RobotC::getDistance(Sherlock *sherlock) const
{
    return abs(this->pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(this->pos.getCol() - sherlock->getCurrentPosition().getCol());
}

int RobotC::getDistance(Watson *watson) const
{
    return abs(this->pos.getRow() - watson->getCurrentPosition().getRow()) + abs(this->pos.getCol() - watson->getCurrentPosition().getCol());
}

RobotS::RobotS(int index, const Position &init_pos, Map *map, Sherlock *sherlock)
{
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "RobotS";
    this->type = S;
    this->item = nullptr;

    this->sherlock = sherlock;
}

Position RobotS::getNextPosition()
{
    Position UP_POS = this->pos.incrementUp();
    Position LEFT_POS = this->pos.incrementLeft();
    Position DOWN_POS = this->pos.incrementDown();
    Position RIGHT_POS = this->pos.incrementRight();

    int min_dist = INT_MAX;
    Position next_pos = Position::npos;

    if (map->isValid(UP_POS, this))
    {
        int dist = abs(UP_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(UP_POS.getCol() - sherlock->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = UP_POS;
        }
    }

    if (map->isValid(LEFT_POS, this))
    {
        int dist = abs(LEFT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(LEFT_POS.getCol() - sherlock->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = LEFT_POS;
        }
    }

    if (map->isValid(DOWN_POS, this))
    {
        int dist = abs(DOWN_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(DOWN_POS.getCol() - sherlock->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = DOWN_POS;
        }
    }

    if (map->isValid(RIGHT_POS, this))
    {
        int dist = abs(RIGHT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(RIGHT_POS.getCol() - sherlock->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = RIGHT_POS;
        }
    }

    return next_pos;
}

void RobotS::move()
{
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))
    {
        return;
    }

    this->pos = next_pos;
}

string RobotS::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=" + this->getTypeString() + ";dist=" + to_string(getDistance()) + "]";
}

RobotS::~RobotS()
{
}

int RobotS::getDistance() const
{
    return abs(this->pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(this->pos.getCol() - sherlock->getCurrentPosition().getCol());
}

RobotW::RobotW(int index, const Position &init_pos, Map *map, Watson *watson)
{
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "RobotW";
    this->type = W;
    this->item = nullptr;

    this->watson = watson;
}

Position RobotW::getNextPosition()
{
    // Same as RobotS
    Position UP_POS = this->pos.incrementUp();
    Position LEFT_POS = this->pos.incrementLeft();
    Position DOWN_POS = this->pos.incrementDown();
    Position RIGHT_POS = this->pos.incrementRight();

    int min_dist = INT_MAX;
    Position next_pos = Position::npos;

    if (map->isValid(UP_POS, this))
    {
        int dist = abs(UP_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(UP_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = UP_POS;
        }
    }

    if (map->isValid(LEFT_POS, this))
    {
        int dist = abs(LEFT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(LEFT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = LEFT_POS;
        }
    }

    if (map->isValid(DOWN_POS, this))
    {
        int dist = abs(DOWN_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(DOWN_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = DOWN_POS;
        }
    }

    if (map->isValid(RIGHT_POS, this))
    {
        int dist = abs(RIGHT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(RIGHT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = RIGHT_POS;
        }
    }

    return next_pos;
}

void RobotW::move()
{
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))
    {
        return;
    }

    this->pos = next_pos;
}

string RobotW::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=" + this->getTypeString() + ";dist=" + to_string(getDistance()) + "]";
}

int RobotW::getDistance() const
{
    return abs(this->pos.getRow() - watson->getCurrentPosition().getRow()) + abs(this->pos.getCol() - watson->getCurrentPosition().getCol());
}

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
{
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "RobotSW";
    this->type = SW;
    this->item = nullptr;

    this->sherlock = sherlock;
    this->watson = watson;
}

Position RobotSW::getNextPosition()
{
    // Move to the position that 2 units away from original position
    // There will be 8 possible positions
    Position UP_POS = this->pos.incrementUp().incrementUp();
    Position LEFT_POS = this->pos.incrementLeft().incrementLeft();
    Position DOWN_POS = this->pos.incrementDown().incrementDown();
    Position RIGHT_POS = this->pos.incrementRight().incrementRight();

    Position UP_LEFT_POS = this->pos.incrementUp().incrementLeft();
    Position UP_RIGHT_POS = this->pos.incrementUp().incrementRight();
    Position DOWN_LEFT_POS = this->pos.incrementDown().incrementLeft();
    Position DOWN_RIGHT_POS = this->pos.incrementDown().incrementRight();

    // Priority in clockwise direction and minimum sum distance to Sherlock and Watson, starting from UP_POS
    int min_dist = INT_MAX;
    Position next_pos = Position::npos;

    if (map->isValid(UP_POS, this))
    {
        int dist = abs(UP_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(UP_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(UP_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(UP_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = UP_POS;
        }
    }

    if (map->isValid(UP_RIGHT_POS, this))
    {
        int dist = abs(UP_RIGHT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(UP_RIGHT_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(UP_RIGHT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(UP_RIGHT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = UP_RIGHT_POS;
        }
    }

    if (map->isValid(RIGHT_POS, this))
    {
        int dist = abs(RIGHT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(RIGHT_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(RIGHT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(RIGHT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = RIGHT_POS;
        }
    }

    if (map->isValid(DOWN_RIGHT_POS, this))
    {
        int dist = abs(DOWN_RIGHT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(DOWN_RIGHT_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(DOWN_RIGHT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(DOWN_RIGHT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = DOWN_RIGHT_POS;
        }
    }

    if (map->isValid(DOWN_POS, this))
    {
        int dist = abs(DOWN_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(DOWN_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(DOWN_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(DOWN_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = DOWN_POS;
        }
    }

    if (map->isValid(DOWN_LEFT_POS, this))
    {
        int dist = abs(DOWN_LEFT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(DOWN_LEFT_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(DOWN_LEFT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(DOWN_LEFT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = DOWN_LEFT_POS;
        }
    }

    if (map->isValid(LEFT_POS, this))
    {
        int dist = abs(LEFT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(LEFT_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(LEFT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(LEFT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = LEFT_POS;
        }
    }

    if (map->isValid(UP_LEFT_POS, this))
    {
        int dist = abs(UP_LEFT_POS.getRow() - sherlock->getCurrentPosition().getRow()) + abs(UP_LEFT_POS.getCol() - sherlock->getCurrentPosition().getCol()) + abs(UP_LEFT_POS.getRow() - watson->getCurrentPosition().getRow()) + abs(UP_LEFT_POS.getCol() - watson->getCurrentPosition().getCol());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = UP_LEFT_POS;
        }
    }

    return next_pos;
}

void RobotSW::move()
{
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos))
    {
        return;
    }

    this->pos = next_pos;
}

string RobotSW::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=" + this->getTypeString() + ";dist=" + to_string(getDistance()) + "]";
}

int RobotSW::getDistance() const
{
    // Sum of Manhattan distance from current position to Sherlock and Watson
    return abs(this->pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(this->pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(this->pos.getRow() - watson->getCurrentPosition().getRow()) + abs(this->pos.getCol() - watson->getCurrentPosition().getCol());
}

BaseItem::BaseItem()
{
}

BaseItem::BaseItem(ItemType type)
{
    this->type = type;
}

BaseItem::~BaseItem()
{
}

ItemType BaseItem::getType() const
{
    return this->type;
}

MagicBook::MagicBook()
    : BaseItem()
{
    this->type = MAGIC_BOOK;
}

bool MagicBook::canUse(Character *obj, Robot *robot)
{
    // Can be used only when robot == nullptr
    if (robot != nullptr || obj == nullptr)
    {
        return false;
    }

    string name = obj->getName();

    if (name == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        return sherlock->getExp() <= 350;
    }
    else if (name == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        return watson->getExp() <= 350;
    }
    else
    {
        return false;
    }
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        sherlock->setExp(sherlock->getExp() * 1.25);
    }
    else if (obj->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        watson->setExp(watson->getExp() * 1.25);
    }
    else
    {
        return;
    }
}

EnergyDrink::EnergyDrink()
    : BaseItem()
{
    this->type = ENERGY_DRINK;
}

bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    // Can be used only when robot == nullptr
    if (robot != nullptr || obj == nullptr)
    {
        return false;
    }

    string name = obj->getName();

    if (name == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        return sherlock->getHp() <= 100;
    }
    else if (name == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        return watson->getHp() <= 100;
    }
    else
    {
        return false;
    }
}

void EnergyDrink::use(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        sherlock->setHp(sherlock->getHp() * 1.2);
    }
    else if (obj->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        watson->setHp(watson->getHp() * 1.2);
    }
    else
    {
        return;
    }
}

FirstAid::FirstAid()
    : BaseItem()
{
    this->type = FIRST_AID;
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{
    // Can be used only when robot == nullptr
    if (robot != nullptr || obj == nullptr)
    {
        return false;
    }

    string name = obj->getName();

    if (name == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        return sherlock->getHp() <= 100 || sherlock->getExp() <= 350;
    }
    else if (name == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        return watson->getHp() <= 100 || watson->getExp() <= 350;
    }
    else
    {
        return false;
    }
}

void FirstAid::use(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        sherlock->setHp(sherlock->getHp() * 1.5);
    }
    else if (obj->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        watson->setHp(watson->getHp() * 1.5);
    }
    else
    {
        return;
    }
}

ExemptionCard::ExemptionCard()
    : BaseItem()
{
    this->type = EXCEMPTION_CARD;
}

bool ExemptionCard::canUse(Character *obj, Robot *robot)
{
    if (robot == nullptr || obj == nullptr)
    {
        return false;
    }

    if (obj->getName() != "Sherlock")
    {
        return false;
    }

    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    return sherlock->getHp() % 2 != 0;
}

void ExemptionCard::use(Character *obj, Robot *robot)
{
}

PassingCard::PassingCard(string challenge)
    : BaseItem()
{
    this->type = PASSING_CARD;

    this->challenge = challenge;
}

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (robot == nullptr || obj == nullptr)
    {
        return false;
    }

    if (obj->getName() != "Watson")
    {
        return false;
    }

    Watson *watson = dynamic_cast<Watson *>(obj);
    return watson->getHp() % 2 == 0;
}

void PassingCard::use(Character *obj, Robot *robot)
{
}

BaseBag::BaseBag(int capacity, Character *owner)
{
    this->capacity = capacity;
    this->size = 0;
    this->head = nullptr;
    this->owner = owner;
}

BaseBag::~BaseBag()
{
    BagNode *cur = head;
    while (cur != nullptr)
    {
        BagNode *temp = cur;
        cur = cur->next;
        delete temp;
    }
}

bool BaseBag::insert(BaseItem *item)
{
    return insertToHead(item);
}

BaseItem *BaseBag::get()
{
    return getFirstUsable(nullptr);
}

BaseItem *BaseBag::get(ItemType itemType)
{
    return getFirstUsable(nullptr, itemType);
}

string BaseBag::str() const
{
    string res = "Bag[count=" + to_string(size) + ";";

    for (BagNode *cur = head; cur != nullptr; cur = cur->next)
    {
        switch (cur->item->getType())
        {
        case MAGIC_BOOK:
            res += "MagicBook";
            break;
        case ENERGY_DRINK:
            res += "EnergyDrink";
            break;
        case FIRST_AID:
            res += "FirstAid";
            break;
        case EXCEMPTION_CARD:
            res += "ExemptionCard";
            break;
        case PASSING_CARD:
            res += "PassingCard";
            break;
        }

        if (cur->next != nullptr)
        {
            res += ",";
        }
    }

    res += "]";
    return res;
}

bool BaseBag::insertToHead(BaseItem *item)
{
    if (isFull())
    {
        return false;
    }

    BagNode *newNode = new BagNode(item, head);
    head = newNode;
    size++;
    return true;
}

BaseItem *BaseBag::getFirstUsable(Robot *robot)
{
    BagNode *cur = head;
    while (cur != nullptr)
    {
        if (cur->item->canUse(owner, robot))
        {
            swapWithHead(cur);
            return removeHead();
        }
        cur = cur->next;
    }

    return nullptr;
}

BaseItem *BaseBag::getFirstUsable(Robot *robot, ItemType itemType)
{
    BagNode *cur = head;
    while (cur != nullptr)
    {
        if (cur->item->getType() == itemType && cur->item->canUse(owner, robot))
        {
            swapWithHead(cur);
            return removeHead();
        }
        cur = cur->next;
    }

    return nullptr;
}

void BaseBag::swapWithHead(BagNode *node)
{
    if (node == nullptr || size == 0 || node == head)
    {
        return;
    }

    // Swap item only
    BaseItem *temp = head->item;
    head->item = node->item;
    node->item = temp;
}

BaseItem *BaseBag::removeHead()
{
    if (size == 0)
    {
        return nullptr;
    }

    BagNode *temp = head;
    head = head->next;
    BaseItem *item = temp->item;
    delete temp;
    size--;

    return item;
}

SherlockBag::SherlockBag(Sherlock *sherlock)
    : BaseBag(13, sherlock) {}

BaseItem *SherlockBag::getForRobot(Robot *robot)
{
    return getFirstUsable(robot);
}

BaseItem *SherlockBag::getForRobot(Robot *robot, ItemType itemType)
{
    return getFirstUsable(robot, itemType);
}

WatsonBag::WatsonBag(Watson *watson)
    : BaseBag(15, watson) {}

BaseItem *WatsonBag::getForRobot(Robot *robot)
{
    return getFirstUsable(robot);
}

BaseItem *WatsonBag::getForRobot(Robot *robot, ItemType itemType)
{
    return getFirstUsable(robot, itemType);
}

StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    config = new Configuration(config_file_path);

    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);

    // Criminal -> Sherlock -> Watson
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);

    sherlockBag = new SherlockBag(sherlock);
    watsonBag = new WatsonBag(watson);

    stopped = false;
}

bool StudyPinkProgram::isStop() const
{
    if (sherlock->getHp() == 0 || watson->getHp() == 0)
    {
        return true;
    }

    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
    {
        return true;
    }

    if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
    {
        return true;
    }

    return stopped;
}

void StudyPinkProgram::printResult() const
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

void StudyPinkProgram::printStep(int si) const
{
    cout << "Step: " << setw(4) << setfill('0') << si
         << "--"
         << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
}

void StudyPinkProgram::run(bool verbose)
{
    // Note: This is a sample code. You can change the implementation as you like.
    // TODO
    if (isStop())
    {
        printResult();
        return;
    }

    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            arr_mv_objs->get(i)->move();

            generalProcessing(arr_mv_objs->get(i));

            if (isStop())
            {
                printStep(istep);
                break;
            }
            if (verbose)
            {
                printStep(istep);
            }

            // Robot creation, future development
            if (arr_mv_objs->get(i)->getName() == "Criminal")
            {
                if (criminal->getNumMoves() >= 3 && criminal->getNumMoves() % 3 == 0 && !arr_mv_objs->isFull())
                {
                    Position pos = criminal->getLastMove();

                    Robot *robot = createRobot(pos.getRow(), pos.getCol());

                    arr_mv_objs->add(robot);
                }
            }
        }
    }
    printResult();
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete config;
    delete sherlock;
    delete watson;
    delete criminal;
    delete map;
    delete arr_mv_objs;
}

void StudyPinkProgram::generalProcessing(MovingObject *mv_obj)
{
    // This function processes the general logic of the game
    // After characters move, we need to check if they meet each other
    // If they meet, we need to process the meeting
    // This function is called after the character moves
    string obj_name = mv_obj->getName();

    if (obj_name == "Sherlock")
    {
        // If sherlock meets criminal
        // If sherlock meets robot
        // If sherlock meets watson

        if (isSherlockMeetCriminal())
        {
            stopped = true;
        }
        else if (sherlock->getCurrentPosition().isEqual(watson->getCurrentPosition()))
        {
            processSherlockMeetWatson();
        }
        else if (isSherlockMeetRobotC() != -1)
        {
            processSherlockMeetRobotC(isSherlockMeetRobotC());
        }
        else if (isSherlockMeetRobotS() != -1)
        {
            processSherlockMeetRobotS(isSherlockMeetRobotS());
        }
        else if (isSherlockMeetRobotW() != -1)
        {
            processSherlockMeetRobotW(isSherlockMeetRobotW());
        }
        else if (isSherlockMeetRobotSW() != -1)
        {
            processSherlockMeetRobotSW(isSherlockMeetRobotSW());
        }
    }
    else if (obj_name == "Watson")
    {
        // If watson meets criminal
        // If watson meets robot
        // If watson meets sherlock

        if (isWatsonMeetCriminal())
        {
            stopped = true;
        }
        else if (isSherlockMeetWatson())
        {
            processSherlockMeetWatson();
        }
        else if (isWatsonMeetRobotC() != -1)
        {
            processWatsonMeetRobotC(isWatsonMeetRobotC());
        }
        else if (isWatsonMeetRobotS() != -1)
        {
            processWatsonMeetRobotS(isWatsonMeetRobotS());
        }
        else if (isWatsonMeetRobotW() != -1)
        {
            processWatsonMeetRobotW(isWatsonMeetRobotW());
        }
        else if (isWatsonMeetRobotSW() != -1)
        {
            processWatsonMeetRobotSW(isWatsonMeetRobotSW());
        }
    }
    else if (obj_name == "Criminal")
    {
        // If criminal meets sherlock
        // If criminal meets watson
        if (isSherlockMeetCriminal())
        {
            stopped = true;
        }
        else if (isWatsonMeetCriminal())
        {
            stopped = true;
        }
    }
    else if (obj_name == "RobotC")
    {
        // If robot meets sherlock  -> process same with sherlock meets robot
        // If robot meets watson    -> process same with watson meets robot
        // If robot meets criminal  -> skip

        if (isSherlockMeetRobotC() != -1)
        {
            processSherlockMeetRobotC(isSherlockMeetRobotC());
        }
        else if (isWatsonMeetRobotC() != -1)
        {
            processWatsonMeetRobotC(isWatsonMeetRobotC());
        }
        else
        {
            return;
        }
    }
    else if (obj_name == "RobotS")
    {
        if (isSherlockMeetRobotS() != -1)
        {
            processSherlockMeetRobotS(isSherlockMeetRobotS());
        }
        else if (isWatsonMeetRobotS() != -1)
        {
            processWatsonMeetRobotS(isWatsonMeetRobotS());
        }
        else
        {
            return;
        }
    }
    else if (obj_name == "RobotW")
    {
        if (isSherlockMeetRobotW() != -1)
        {
            processSherlockMeetRobotW(isSherlockMeetRobotW());
        }
        else if (isWatsonMeetRobotW() != -1)
        {
            processWatsonMeetRobotW(isWatsonMeetRobotW());
        }
        else
        {
            return;
        }
    }
    else if (obj_name == "RobotSW")
    {
        if (isSherlockMeetRobotSW() != -1)
        {
            processSherlockMeetRobotSW(isSherlockMeetRobotSW());
        }
        else if (isWatsonMeetRobotSW() != -1)
        {
            processWatsonMeetRobotSW(isWatsonMeetRobotSW());
        }
        else
        {
            return;
        }
    }
}

int StudyPinkProgram::isSherlockMeetRobotC()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotC" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

int StudyPinkProgram::isSherlockMeetRobotS()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotS" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

int StudyPinkProgram::isSherlockMeetRobotW()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotW" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

int StudyPinkProgram::isSherlockMeetRobotSW()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotSW" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

int StudyPinkProgram::isWatsonMeetRobotC()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotC" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

int StudyPinkProgram::isWatsonMeetRobotS()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotS" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

int StudyPinkProgram::isWatsonMeetRobotW()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotW" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

int StudyPinkProgram::isWatsonMeetRobotSW()
{
    if (arr_mv_objs->size() < 4)
    {
        return -1;
    }

    for (int i = 3; i < arr_mv_objs->size(); i++)
    {
        if (arr_mv_objs->get(i)->getName() == "RobotSW" && arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition()))
        {
            return i;
        }
    }

    return -1;
}

bool StudyPinkProgram::isSherlockMeetCriminal()
{
    return sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition());
}

bool StudyPinkProgram::isWatsonMeetCriminal()
{
    return watson->getCurrentPosition().isEqual(criminal->getCurrentPosition());
}

bool StudyPinkProgram::isSherlockMeetWatson()
{
    return sherlock->getCurrentPosition().isEqual(watson->getCurrentPosition());
}

void StudyPinkProgram::processSherlockMeetRobotC(int indexRobot)
{
    RobotC *robot = dynamic_cast<RobotC *>(arr_mv_objs->get(indexRobot));

    bool canCatch = sherlock->getExp() > 500;

    if (canCatch)
    {
        stopped = true;
    }
    else
    {
        addToSherlockBag(robot->getItem());
    }

    processSherlockAfterMeetRobot(robot);
}

void StudyPinkProgram::processSherlockMeetRobotS(int indexRobot)
{
    RobotS *robot = dynamic_cast<RobotS *>(arr_mv_objs->get(indexRobot));

    bool isWin = sherlock->getExp() > 400;

    if (isWin)
    {
        addToSherlockBag(robot->getItem());
    }
    else
    {
        BaseItem *exemptionCard = sherlockBag->getForRobot(robot, EXCEMPTION_CARD);

        if (exemptionCard && exemptionCard->getType() == EXCEMPTION_CARD)
        {
            exemptionCard->use(sherlock, robot);
            delete exemptionCard;
        }
        else
        {
            sherlock->setExp(sherlock->getExp() * 0.9);
        }
    }

    processSherlockAfterMeetRobot(robot);
}

void StudyPinkProgram::processSherlockMeetRobotW(int indexRobot)
{
    RobotW *robot = dynamic_cast<RobotW *>(arr_mv_objs->get(indexRobot));

    bool isWin = true;

    if (isWin)
    {
        addToSherlockBag(robot->getItem());
    }
    else
    {
        // always win
    }

    processSherlockAfterMeetRobot(robot);
}

void StudyPinkProgram::processSherlockMeetRobotSW(int indexRobot)
{
    RobotSW *robot = dynamic_cast<RobotSW *>(arr_mv_objs->get(indexRobot));

    bool isWin = sherlock->getExp() > 300 && sherlock->getHp() > 335;

    if (isWin)
    {
        addToSherlockBag(robot->getItem());
    }
    else
    {
        BaseItem *exemptionCard = sherlockBag->getForRobot(robot, EXCEMPTION_CARD);

        if (exemptionCard && exemptionCard->getType() == EXCEMPTION_CARD)
        {
            exemptionCard->use(sherlock, robot);
            delete exemptionCard;
        }
        else
        {
            sherlock->setExp(sherlock->getExp() * 0.85);
            sherlock->setHp(sherlock->getHp() * 0.85);
        }
    }
}

void StudyPinkProgram::processWatsonMeetRobotC(int indexRobot)
{
    RobotC *robot = dynamic_cast<RobotC *>(arr_mv_objs->get(indexRobot));
    BaseItem *passingCard = watsonBag->getForRobot(robot, PASSING_CARD);

    bool isWin = true; // always win

    if (isWin || passingCard)
    {
        if (passingCard && passingCard->getType() == PASSING_CARD)
        {
            passingCard->use(watson, robot);
            delete passingCard;
        }

        addToWatsonBag(robot->getItem());
    }
    else
    {
        // always win
    }

    processWatsonAfterMeetRobot(robot);
}

void StudyPinkProgram::processWatsonMeetRobotS(int indexRobot)
{
    // do nothing
}

void StudyPinkProgram::processWatsonMeetRobotW(int indexRobot)
{
    RobotW *robot = dynamic_cast<RobotW *>(arr_mv_objs->get(indexRobot));
    BaseItem *passingCard = watsonBag->getForRobot(robot, PASSING_CARD);

    bool isWin = watson->getHp() > 350 || passingCard != nullptr;

    if (isWin || passingCard)
    {
        if (passingCard && passingCard->getType() == PASSING_CARD)
        {
            passingCard->use(watson, robot);
            delete passingCard;
        }

        addToWatsonBag(robot->getItem());
    }
    else
    {
        watson->setHp(watson->getHp() * 0.95);
    }

    processWatsonAfterMeetRobot(robot);
}

void StudyPinkProgram::processWatsonMeetRobotSW(int indexRobot)
{
    RobotSW *robot = dynamic_cast<RobotSW *>(arr_mv_objs->get(indexRobot));
    BaseItem *passingCard = watsonBag->getForRobot(robot, PASSING_CARD);

    bool isWin = watson->getHp() > 165 && watson->getExp() > 600;

    if (isWin || passingCard)
    {
        if (passingCard && passingCard->getType() == PASSING_CARD)
        {
            passingCard->use(watson, robot);
            delete passingCard;
        }

        addToWatsonBag(robot->getItem());
    }
    else
    {
        watson->setHp(watson->getHp() * 0.85);
        watson->setExp(watson->getExp() * 0.85);
    }

    processWatsonAfterMeetRobot(robot);
}

void StudyPinkProgram::processSherlockSearchItem()
{
}

void StudyPinkProgram::processWatsonSearchItem()
{
}

void StudyPinkProgram::addToSherlockBag(BaseItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    if (sherlockBag->isFull())
    {
        delete item;
        return;
    }

    sherlockBag->insert(item);
}

void StudyPinkProgram::addToWatsonBag(BaseItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    if (watsonBag->isFull())
    {
        delete item;
        return;
    }

    watsonBag->insert(item);
}

Robot *StudyPinkProgram::createRobot(int i, int j)
{
    if (criminal->getNumMoves() % 3 != 0 || criminal->getNumMoves() < 3)
    {
        return nullptr;
    }

    if (criminal->getNumMoves() == 3)
    {
        // RobotC
        RobotC *robotC = new RobotC(arr_mv_objs->size(), Position(i, j), map, criminal);
        BaseItem *robotItem = createRobotItem(i, j);

        robotC->setItem(robotItem);

        return robotC;
    }
    else
    {
        // calculate distance:
        // 1. closer to Sherlock -> RobotS
        // 2. closer to Watson -> RobotW
        // 3. closer to both -> RobotSW

        int distSherlock = abs(i - sherlock->getCurrentPosition().getRow()) + abs(j - sherlock->getCurrentPosition().getCol());
        int distWatson = abs(i - watson->getCurrentPosition().getRow()) + abs(j - watson->getCurrentPosition().getCol());

        if (distSherlock < distWatson)
        {
            RobotS *robotS = new RobotS(arr_mv_objs->size(), Position(i, j), map, sherlock);
            BaseItem *robotItem = createRobotItem(i, j);

            robotS->setItem(robotItem);

            return robotS;
        }
        else if (distSherlock > distWatson)
        {
            RobotW *robotW = new RobotW(arr_mv_objs->size(), Position(i, j), map, watson);
            BaseItem *robotItem = createRobotItem(i, j);

            robotW->setItem(robotItem);

            return robotW;
        }
        else
        {
            RobotSW *robotSW = new RobotSW(arr_mv_objs->size(), Position(i, j), map, sherlock, watson);
            BaseItem *robotItem = createRobotItem(i, j);

            robotSW->setItem(robotItem);

            return robotSW;
        }
    }
}

BaseItem *StudyPinkProgram::createRobotItem(int i, int j)
{
    int p = i * j;

    while (p / 10)
    {
        int sum = 0;

        while (p)
        {
            sum += p % 10;
            p /= 10;
        }

        p = sum;
    }

    if (p == 0 || p == 1)
    {
        return new MagicBook();
    }
    else if (p == 2 || p == 3)
    {
        return new EnergyDrink();
    }
    else if (p == 4 || p == 5)
    {
        return new FirstAid();
    }
    else if (p == 6 || p == 7)
    {
        return new ExemptionCard();
    }
    else
    {
        int t = (i * 11 + j) % 4;
        string challenge;

        switch (t)
        {
        case 0:
            challenge = "RobotS";
            break;
        case 1:
            challenge = "RobotC";
            break;
        case 2:
            challenge = "RobotSW";
            break;
        case 3:
            challenge = "all";
            break;
        }

        return new PassingCard(challenge);
    }
}

void StudyPinkProgram::processSherlockMeetWatson()
{
    // Sherlock gives Watson all PassingCard
    // Watson gives Sherlock all ExemptionCard
    // Priority: Sherlock gives first -> Watson gives second

    while (!sherlockBag->isEmpty())
    {
        BaseItem *passing_card = sherlockBag->get(PASSING_CARD);

        if (passing_card == nullptr)
        {
            break;
        }

        PassingCard *passingCard = dynamic_cast<PassingCard *>(passing_card);

        watsonBag->insert(passingCard);
    }

    while (!watsonBag->isEmpty())
    {
        BaseItem *exemption_card = watsonBag->get(EXCEMPTION_CARD);

        if (exemption_card == nullptr)
        {
            break;
        }

        ExemptionCard *exemptionCard = dynamic_cast<ExemptionCard *>(exemption_card);

        sherlockBag->insert(exemptionCard);
    }
}

ItemType StudyPinkProgram::processSherlockBeforeMeetRobot(Robot *robot)
{
    // True means Sherlock has an EXCEMPTION_CARD to use
    // False otherwise
    BaseItem *item = sherlockBag->getForRobot(robot, EXCEMPTION_CARD);

    if (item != nullptr)
    {
        item->use(sherlock, robot);
        delete item;
        return EXCEMPTION_CARD;
    }

    return MAGIC_BOOK;
}

ItemType StudyPinkProgram::processWatsonBeforeMeetRobot(Robot *robot)
{
    // True means Watson has an PASSING_CARD to use
    // False otherwise
    BaseItem *item = watsonBag->getForRobot(robot, PASSING_CARD);

    if (item != nullptr)
    {
        item->use(watson, robot);
        delete item;
        return PASSING_CARD;
    }

    return MAGIC_BOOK;
}

ItemType StudyPinkProgram::processSherlockAfterMeetRobot(Robot *)
{
    // True means Sherlock has a MAGIC_BOOK | ENERGY_DRINK | FIRST_AID to use
    // False otherwise
    BaseItem *item = sherlockBag->get();
    ItemType itemType = item->getType();

    if (item != nullptr)
    {
        item->use(sherlock, nullptr);
        delete item;
        return itemType;
    }

    return PASSING_CARD;
}

ItemType StudyPinkProgram::processWatsonAfterMeetRobot(Robot *)
{
    // True means Watson has a MAGIC_BOOK | ENERGY_DRINK | FIRST_AID to use
    // False otherwise
    BaseItem *item = watsonBag->get();
    ItemType itemType = item->getType();

    if (item != nullptr)
    {
        item->use(watson, nullptr);
        delete item;
        return itemType;
    }

    return PASSING_CARD;
}

void StudyPinkProgram::processDefeatedRobot(int index)
{
    // A defeated robot will be removed from the array of moving objects
    arr_mv_objs->remove(index);
}

Character::Character()
{
}

Character::~Character()
{
}

Criminal::~Criminal()
{
}

int Criminal::getNumMoves() const
{
    return this->num_moves;
}

Position Criminal::getLastMove() const
{
    return this->lastMove;
}

bool ArrayMovingObject::remove(int index)
{
    if (index < 0 || index >= size())
    {
        return false;
    }

    delete arr_mv_objs[index];

    for (int i = index; i < size() - 1; i++)
    {
        arr_mv_objs[i] = arr_mv_objs[i + 1];
    }

    arr_mv_objs[size() - 1] = nullptr;
    count--;

    return true;
}
