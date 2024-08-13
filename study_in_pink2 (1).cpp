#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

const Position Position::npos(-1, -1);
bool StudyPinkProgram::end = false;

int dist(const Position &A, const Position &B)
{
    return abs(A.getRow() - B.getRow()) + abs(A.getCol() - B.getCol());
}

void robotSherlock(Sherlock *sherlock, Robot *robot, int i)
{
    sherlock->setRobot(robot);

    BaseItem *item;
    item = sherlock->getBag()->get(ItemType::EXCEMPTION_CARD);
    if (item)
    {
        item->use(sherlock, sherlock->getRobot());
        delete item;
    }

    switch (robot->getType())
    {
    case S:
        if (sherlock->getEXP() > 400)
        {
            sherlock->getBag()->insert(robot->getItem());
            sherlock->ArrMvObjs()->remove(i);
        }
        else
        {
            if (sherlock->getImmunity())
                sherlock->setEXP(ceil(sherlock->getEXP() * 0.9));
        }
        break;

    case W:
        sherlock->getBag()->insert(robot->getItem());
        sherlock->ArrMvObjs()->remove(i);
        break;

    case SW:
        if (sherlock->getEXP() > 300 && sherlock->getHP() > 335)
        {
            sherlock->getBag()->insert(robot->getItem());
            sherlock->ArrMvObjs()->remove(i);
        }
        else
        {
            if (sherlock->getImmunity())
            {
                sherlock->setHP(ceil(sherlock->getHP() * 0.85));
                sherlock->setEXP(ceil(sherlock->getEXP() * 0.85));
            }
        }
        break;
    default:
        if (sherlock->getEXP() > 500)
        {
            StudyPinkProgram::end = true;
            BaseItem *item = robot->getItem();
            delete item;
            sherlock->ArrMvObjs()->remove(i);
        }
        else
        {
            sherlock->getBag()->insert(robot->getItem());
            sherlock->ArrMvObjs()->remove(i);
        }
        break;
    }

    item = sherlock->getBag()->get(ItemType::ITEM_PHARSE_2);
    if (item)
    {
        item->use(sherlock, sherlock->getRobot());
        delete item;
    }

    sherlock->setImmunity(false);
    sherlock->setRobot(nullptr);
}

void robotWatson(Watson *watson, Robot *robot, int i)
{
    watson->setRobot(robot);

    BaseItem *item;
    item = watson->getBag()->get(ItemType::PASSING_CARD);
    if (item)
    {
        item->use(watson, watson->getRobot());
        delete item;
        watson->getBag()->insert(robot->getItem());
        watson->ArrMvObjs()->remove(i);
    }
    else
    {
        switch (robot->getType())
        {
        case S:
            break;

        case W:
            if (watson->getHP() > 350)
            {
                watson->getBag()->insert(robot->getItem());
                watson->ArrMvObjs()->remove(i);
            }
            else
            {
                watson->setHP(ceil(watson->getHP() * 0.95));
            }
            break;

        case SW:
            if (watson->getEXP() > 600 && watson->getHP() > 165)
            {
                watson->getBag()->insert(robot->getItem());
                watson->ArrMvObjs()->remove(i);
            }
            else
            {
                watson->setHP(ceil(watson->getHP() * 0.85));
                watson->setEXP(ceil(watson->getEXP() * 0.85));
            }
            break;
        default:
            watson->getBag()->insert(robot->getItem());
            watson->ArrMvObjs()->remove(i);
            break;
        }
    }

    item = watson->getBag()->get(ItemType::ITEM_PHARSE_2);
    if (item)
    {
        item->use(watson, watson->getRobot());
        delete item;
    }

    watson->setRobot(nullptr);
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    if (pos.isEqual(Position::npos))
        return false;

    MapElement *element = map[pos.getRow()][pos.getCol()];

    if (element->getType() == ElementType::WALL)
    {
        return false;
    }

    if (mv_obj->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(mv_obj);
        if (element->getType() == ElementType::FAKE_WALL)
        {
            FakeWall *fakeWall = dynamic_cast<FakeWall *>(element);
            if (watson->getEXP() <= fakeWall->getReqExp())
            {
                return false;
            }
        }
    }

    return true;
}

Position Sherlock::getNextPosition()
{
    Position next = pos;

    switch (moving_rule[step])
    {
    case 'U':
        if (pos.getRow() - 1 >= 0)
        {
            next.setRow(next.getRow() - 1);
        }
        break;
    case 'D':
        if (pos.getRow() + 1 < map->getRows())
        {
            next.setRow(next.getRow() + 1);
        }
        break;
    case 'L':
        if (pos.getCol() - 1 >= 0)
        {
            next.setCol(next.getCol() - 1);
        }
        break;
    case 'R':
        if (pos.getCol() + 1 < map->getCols())
        {
            next.setCol(next.getCol() + 1);
        }
        break;
    default:
        break;
    }
    if (step == moving_rule.length() - 1)
    {
        step = 0;
    }
    else
        step++;

    return next;
}

void Sherlock::move()
{
    Position nextPosition = getNextPosition();

    if (map->isValid(nextPosition, this))
    {
        pos = nextPosition;

        if (!arr_mv_objs)
            return;

        for (int i = 0; i < arr_mv_objs->size(); i++)
        {
            if (index != i && pos.isEqual(arr_mv_objs->get(i)->getCurrentPosition()))
            {

                Robot *robot = dynamic_cast<Robot *>(arr_mv_objs->get(i));
                Watson *watson = dynamic_cast<Watson *>(arr_mv_objs->get(i));

                if (robot)
                {
                    robotSherlock(this, robot, i);
                }

                if (watson)
                {
                    BaseItem *item;
                    while (bag->include(ItemType::EXCEMPTION_CARD) && watson->getBag()->include(ItemType::PASSING_CARD))
                    {
                        item = bag->get(ItemType::PASSING_CARD);
                        watson->getBag()->insert(item);
                        item = watson->getBag()->get(ItemType::EXCEMPTION_CARD);
                        bag->insert(item);
                    }
                }
            }
        }
    }
}

Position Watson::getNextPosition()
{
    Position next = pos;
    switch (moving_rule[step])
    {
    case 'U':
        if (pos.getRow() - 1 >= 0)
        {
            next.setRow(next.getRow() - 1);
        }
        break;

    case 'D':
        if (pos.getRow() + 1 < map->getRows())
        {
            next.setRow(next.getRow() + 1);
        }
        break;

    case 'L':
        if (pos.getCol() - 1 >= 0)
        {
            next.setCol(next.getCol() - 1);
        }
        break;
    case 'R':
        if (pos.getCol() + 1 < map->getCols())
        {
            next.setCol(next.getCol() + 1);
        }
        break;
    default:
        break;
    }

    if (step == moving_rule.length() - 1)
    {
        step = 0;
    }
    else
        step++;

    return next;
}

void Watson::move()
{
    Position next = getNextPosition();

    if (map->isValid(next, this))
    {
        pos = next;
        if (!arr_mv_objs)
            return;

        for (int i = 0; i < arr_mv_objs->size(); i++)
        {
            if (index != i && pos.isEqual(arr_mv_objs->get(i)->getCurrentPosition()))
            {
                Robot *robot = dynamic_cast<Robot *>(arr_mv_objs->get(i));
                Sherlock *sherlock = dynamic_cast<Sherlock *>(arr_mv_objs->get(i));
                BaseItem *item;

                if (robot)
                {
                robotWatson(this, robot, i);
                }

                if (sherlock)
                {
                    BaseItem *item;
                    while (bag->include(ItemType::EXCEMPTION_CARD) && sherlock->getBag()->include(ItemType::PASSING_CARD))
                    {
                        item = sherlock->getBag()->get(ItemType::PASSING_CARD);
                        bag->insert(item);
                        item = bag->get(ItemType::EXCEMPTION_CARD);
                        sherlock->getBag()->insert(item);
                    }
                }
            }
        }
    }
}

Position Criminal::getNextPosition()
{
    int r = pos.getRow();
    int c = pos.getCol();
    int pos_r = map->getRows();
    int pos_c = map->getCols();

    Position next = Position::npos;
    int max_distance = 0;

    Position north = Position(r - 1, c);
    Position east = Position(r, c + 1);
    Position south = Position(r + 1, c);
    Position west = Position(r, c - 1);

    Position sherlock_pos = sherlock->getCurrentPosition();
    Position watson_pos = watson->getCurrentPosition();
    if (r - 1 >= 0 && max_distance < dist(north, sherlock_pos) + dist(north, watson_pos))
    {
        max_distance = dist(north, sherlock_pos) + dist(north, watson_pos);
        next = north;
    }
    if (c - 1 >= 0 && max_distance < dist(west, sherlock_pos) + dist(west, watson_pos))
    {
        max_distance = dist(west, sherlock_pos) + dist(west, watson_pos);
        next = west;
    }
    if (r + 1 < pos_r && max_distance < dist(south, sherlock_pos) + dist(south, watson_pos))
    {

        max_distance = dist(south, sherlock_pos) + dist(south, watson_pos);
        next = south;
    }
    if (c + 1 < pos_c && max_distance < dist(east, sherlock_pos) + dist(east, watson_pos))
    {
        max_distance = dist(east, sherlock_pos) + dist(east, watson_pos);
        next = east;
    }

    return next;
}

void Criminal::move()
{
    Position next = getNextPosition();
    if (map->isValid(next, this))
    {
        previous = pos;
        pos = next;

        if (pos.isEqual(sherlock->getCurrentPosition()) || pos.isEqual(watson->getCurrentPosition()))
        {
            StudyPinkProgram::end = true;
        }

        step++;
        if (step == 3)
        {
            isCreatedRobot(previous);
            step = 0;
        }
    }
}

void Criminal::isCreatedRobot(const Position &init_pos)
{
    if (arr_mv_objs->isFull())
        return;

    if (arr_mv_objs->size() == 3)
    {
        RobotC *robotC = new RobotC(3, init_pos, map, this);
        arr_mv_objs->add(robotC);
        return;
    }

    int sherlock_distance = dist(pos, sherlock->getCurrentPosition());
    int watson_distance = dist(pos, watson->getCurrentPosition());

    if (sherlock_distance < watson_distance)
    {
        RobotS *robotS = new RobotS(arr_mv_objs->size(), init_pos, map, this, sherlock);
        arr_mv_objs->add(robotS);
    }
    else if (sherlock_distance > watson_distance)
    {
        RobotW *robotW = new RobotW(arr_mv_objs->size(), init_pos, map, this, watson);
        arr_mv_objs->add(robotW);
    }
    else
    {
        RobotSW *robotSW = new RobotSW(arr_mv_objs->size(), init_pos, map, this, sherlock, watson);
        arr_mv_objs->add(robotSW);
    }
}

void Robot::move()
{
    Position next = getNextPosition();
    if (map->isValid(next, this))
    {
        pos = next;
        if (!arr_mv_objs)
            return;

        for (int i = 0; i < arr_mv_objs->size(); i++)
        {
            if (index != i && pos.isEqual(arr_mv_objs->get(i)->getCurrentPosition()))
            {
                Watson *watson = dynamic_cast<Watson *>(arr_mv_objs->get(i));
                Sherlock *sherlock = dynamic_cast<Sherlock *>(arr_mv_objs->get(i));
                if (watson)
                {
                    robotWatson(watson, this, index);
                }
                if (sherlock)
                {
                    robotSherlock(sherlock, this, index);
                }
            }
        }
    }
}

Position RobotS::getNextPosition()
{
    int row = pos.getRow();
    int col = pos.getCol();
    int pos_r = map->getRows();
    int pos_c = map->getCols();

    Position next = Position::npos;
    int minDist = INT_MAX;

    Position north = Position(row - 1, col);
    Position east = Position(row, col + 1);
    Position south = Position(row + 1, col);
    Position west = Position(row, col - 1);

    Position sherlock_pos = sherlock->getCurrentPosition();

    if (row - 1 >= 0 && minDist > dist(north, sherlock_pos))
    {
        minDist = dist(north, sherlock_pos);
        next = north;
    }
    if (col + 1 < pos_c && minDist > dist(east, sherlock_pos))
    {
        minDist = dist(east, sherlock_pos);
        next = east;
    }
    if (row + 1 < pos_r && minDist > dist(south, sherlock_pos))
    {
        minDist = dist(south, sherlock_pos);
        next = south;
    }
    if (col - 1 >= 0 && minDist > dist(west, sherlock_pos))
    {
        minDist = dist(west, sherlock_pos);
        next = west;
    }

    return next;
}

int RobotS::getDistance() const
{
    return dist(pos, sherlock->getCurrentPosition());
}

Position RobotW::getNextPosition()
{

    int row = pos.getRow();
    int col = pos.getCol();
    int pos_r = map->getRows();
    int pos_c = map->getCols();

    Position next = Position::npos;
    int minDist = INT_MAX;

    Position north = Position(row - 1, col);
    Position east = Position(row, col + 1);
    Position south = Position(row + 1, col);
    Position west = Position(row, col - 1);

    Position watson_pos = watson->getCurrentPosition();

    if (row - 1 >= 0 && minDist > dist(north, watson_pos))
    {
        minDist = dist(north, watson_pos);
        next = north;
    }
    if (col + 1 < map->getCols() && minDist > dist(east, watson_pos))
    {
        minDist = dist(east, watson_pos);
        next = east;
    }
    if (row + 1 < map->getRows() && minDist > dist(south, watson_pos))
    {
        minDist = dist(south, watson_pos);
        next = south;
    }
    if (col - 1 >= 0 && minDist > dist(west, watson_pos))
    {
        minDist = dist(west, watson_pos);
        next = west;
    }

    return next;
}

int RobotW::getDistance() const
{
    return dist(pos, watson->getCurrentPosition());
}

Position RobotSW::getNextPosition()
{
    int row = pos.getRow();
    int col = pos.getCol();
    int pos_r = map->getRows();
    int pos_c = map->getCols();

    Position next = Position::npos;
    int minDist = INT_MAX;

    Position watson_pos = watson->getCurrentPosition();
    Position sherlock_pos = sherlock->getCurrentPosition();

    Position position_1 = Position(row - 2, col);
    Position position_2 = Position(row - 1, col + 1);
    Position position_3 = Position(row, col + 2);
    Position position_4 = Position(row + 1, col + 1);
    Position position_5 = Position(row + 2, col);
    Position position_6 = Position(row + 1, col - 1);
    Position position_7 = Position(row, col - 2);
    Position position_8 = Position(row - 1, col - 1);

    if (row - 2 >= 0 && minDist > (dist(position_1, watson_pos) + dist(position_1, sherlock_pos)))
    {
        minDist = (dist(position_1, watson_pos) + dist(position_1, sherlock_pos));
        next = position_1;
    }
    if (row - 1 >= 0 && col + 1 < pos_c && minDist > (dist(position_2, watson_pos) + dist(position_2, sherlock_pos)))
    {
        minDist = (dist(position_2, watson_pos) + dist(position_2, sherlock_pos));
        next = position_2;
    }
    if (col + 2 < pos_c && minDist > (dist(position_3, watson_pos) + dist(position_3, sherlock_pos)))
    {
        minDist = (dist(position_3, watson_pos) + dist(position_3, sherlock_pos));
        next = position_3;
    }
    if (row + 1 < pos_r && col + 1 < pos_c && minDist > (dist(position_4, watson_pos) + dist(position_4, sherlock_pos)))
    {
        minDist = (dist(position_4, watson_pos) + dist(position_4, sherlock_pos));
        next = position_4;
    }
    if (row + 2 < pos_r && minDist > (dist(position_5, watson_pos) + dist(position_5, sherlock_pos)))
    {
        minDist = (dist(position_5, watson_pos) + dist(position_5, sherlock_pos));
        next = position_5;
    }
    if (row + 1 < pos_r && col - 1 >= 0 && minDist > (dist(position_6, watson_pos) + dist(position_6, sherlock_pos)))
    {
        minDist = (dist(position_6, watson_pos) + dist(position_6, sherlock_pos));
        next = position_6;
    }
    if (col - 2 >= 0 && minDist > (dist(position_7, watson_pos) + dist(position_7, sherlock_pos)))
    {
        minDist = (dist(position_7, watson_pos) + dist(position_7, sherlock_pos));
        next = position_7;
    }
    if (row - 1 >= 0 && col - 1 >= 0 && minDist > (dist(position_8, watson_pos) + dist(position_8, sherlock_pos)))
    {
        minDist = (dist(position_8, watson_pos) + dist(position_8, sherlock_pos));
        next = position_8;
    }

    return next;
}

int RobotSW::getDistance() const
{
    return dist(pos, sherlock->getCurrentPosition()) + dist(pos, watson->getCurrentPosition());
}

bool MagicBook::canUse(Character *obj, Robot *robot)
{
    if (!robot)
        return false;
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    Watson *watson = dynamic_cast<Watson *>(obj);
    return (sherlock && sherlock->getEXP() <= 350) || (watson && watson->getEXP() <= 350);
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        sherlock->setEXP(ceil(sherlock->getEXP() + sherlock->getEXP() * 0.25));
    }

    if (obj->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        watson->setEXP(ceil(watson->getEXP() + watson->getEXP() * 0.25));
    }
}

bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    if (!robot)
        return false;
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    Watson *watson = dynamic_cast<Watson *>(obj);
    return (sherlock && sherlock->getHP() <= 100) || (watson && watson->getHP() <= 100);
}

void EnergyDrink::use(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        sherlock->setHP(ceil(sherlock->getHP() + sherlock->getHP() * 0.2));
    }
    if (obj->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        watson->setHP(ceil(watson->getHP() + watson->getHP() * 0.2));
    }
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{
    if (!robot)
        return false;
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    Watson *watson = dynamic_cast<Watson *>(obj);
    return (sherlock && (sherlock->getHP() <= 100 || sherlock->getEXP() <= 350)) || (watson && (watson->getHP() <= 100 || watson->getEXP() <= 350));
}

void FirstAid::use(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        sherlock->setHP(ceil(sherlock->getHP() + sherlock->getHP() * 0.5));
    }
    if (obj->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(obj);
        watson->setHP(ceil(watson->getHP() + watson->getHP() * 0.5));
    }
}

/* ==================================================================== */

bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    if (!robot)
        return false;
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    return (sherlock && sherlock->getHP() % 2);
}

void ExcemptionCard::use(Character *obj, Robot *robot)
{
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    sherlock->setImmunity(true);
}

/* ==================================================================== */

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (!robot)
        return false;
    Watson *watson = dynamic_cast<Watson *>(obj);
    return (watson && watson->getHP() % 2 == 0);
}

void PassingCard::use(Character *obj, Robot *robot)
{
    if (challenge == "RobotS" && robot->getType() == S)
        return;
    if (challenge == "RobotC" && robot->getType() == C)
        return;
    if (challenge == "RobotSW" && robot->getType() == SW)
        return;
    if (challenge == "all")
        return;
    Watson *watson = dynamic_cast<Watson *>(obj);
    watson->setEXP(watson->getEXP() - 50);
}

BaseItem *BaseBag::get()
{
    if (!head)
        return nullptr;
    BaseItem *item = head;
    int index = 0;
    while (item)
    {
        if (item->canUse(obj, obj->getRobot()))
        {
            swapItem(0, index);
            head = head->next;
            return item;
        }
        index++;
        item = item->next;
    }
    return nullptr;
}

BaseItem *BaseBag::get(ItemType itemType)
{
    if (!head)
        return nullptr;

    BaseItem *item = head;
    int index = 0;
    while (item)
    {
        bool itemPharse2 = itemType == ItemType::ITEM_PHARSE_2 && (item->getType() == ItemType::MAGIC_BOOK || item->getType() == ItemType::ENERGY_DRINK || item->getType() == ItemType::FIRST_AID);
        if (item->canUse(obj, obj->getRobot()) && (item->getType() == itemType || itemPharse2))
        {
            swapItem(0, index);
            head = head->next;
            return item;
        }
        item = item->next;
        index++;
    }
    return nullptr;
}

bool SherlockBag::insert(BaseItem *item)
{
    if (count == capacity)
        return false;
    if (!head)
    {
        head = item;
        count++;
        return true;
    }

    item->next = head;
    head = item;
    count++;
    return false;
}

bool WatsonBag::insert(BaseItem *item)
{
    if (count == capacity)
        return false;
    if (!head)
    {
        head = item;
        count++;
        return true;
    }

    item->next = head;
    head = item;
    count++;
    return true;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////