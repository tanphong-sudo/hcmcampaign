/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Position;
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

namespace MathUtils {
    static bool isSquare(int n);
    static int digitSum(int n);
    static bool isSpecial(int n);
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0) : r(r), c(c) {}
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const { return r; }
    int getCol() const { return c; }
    void setRow(int r) { this->r = r; }
    void setCol(int c) { this->c = c; }
    string str() const { return "(" + to_string(r) + "," + to_string(c) + ")"; }
};

class BattleField
{
private:
    int n_rows, n_cols;
    vector<vector<Position *>> terrain;
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;
private:
    virtual string typeToString() const = 0;
public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    virtual int getType() const = 0;
};

class Vehicle : public Unit
{
private:
    enum VehicleType vehicleType;
    string typeToString() const override;
public:
    Vehicle(int quantity, int weight, const Position pos, enum VehicleType vehicleType);
    int getAttackScore() override;
    string str() const override { return "Vehicle[" + typeToString() + "=<" + to_string(quantity) + "," + to_string(weight) + pos.str() + ">]"; }
    int getType() const override { return vehicleType; }
};

class Infantry : public Unit
{
private:
    enum InfantryType infantryType;
    string typeToString() const override;
public:
    Infantry(int quantity, int weight, const Position pos, enum InfantryType infantryType);
    int getAttackScore() override;
    string str() const override { return "Infantry[" + typeToString() + "=<" + to_string(quantity) + "," + to_string(weight) + pos.str() + ">]"; }
    int getType() const override { return infantryType; }
};

class UnitList
{
protected:
    int getEXP() const;
    int getLF() const;
    void removeUnit(enum VehicleType vehicleType);
    void removeUnit(enum InfantryType infantryType);
private:
    struct Node
    {
        Unit *unit;
        Node *next;
        Node(Unit *unit, Node *next) : unit(unit), next(next) {}
    };
    int capacity;
    int size;
    int count_vehicle, count_infantry;
    Node *sentinal, *last;
    string printList() const;
public:
    UnitList(Unit **unitArray, int size);
    ~UnitList();
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(enum VehicleType vehicleType);   // return true if it exists
    bool isContain(enum InfantryType infantryType); // return true if it exists
    string str() const;

    friend class Army;
};

class Army {
protected:
    int LF;
    int EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;
private:
    void updateScore();
    virtual vector<Unit *> getUnit(int target);
public:
    vector<Unit *> vehicleList;
    vector<Unit *> infantryList;
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual ~Army();

    int getLF() const;
    int getEXP() const;

    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
};

class LiberationArmy : public Army {
private:
    bool win(int enemy, vector<Unit *> &unitList);
    vector<Unit *> getUnit(int target) override;
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
};

class ARVN : public Army {
private:
    vector<Unit *> getUnit(int target) override;
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
};
/*
class TerrainElement
{
public:
    TerrainElement();
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *ARVN;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
};
*/
#endif