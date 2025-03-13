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

class MathUtils {
public:
    static bool isSquare(int n);
    static int digitSum(int n);
    static bool isSpecial(int n);
    static int nextFibo(int n);
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
    virtual void setAttackScore();
    int attackScore = 0;
    void updateAttackScore(int score) { attackScore = score; }
private:
    virtual string typeToString() const = 0;
public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    virtual int getType() const = 0;
    friend class UnitList;
};

class Vehicle : public Unit
{
protected:
    int getType() const override { return vehicleType; }
    void setAttackScore() override;
private:
    VehicleType vehicleType;
    string typeToString() const override;
public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);
    int getAttackScore() override { return attackScore; }
    string str() const override { return "Vehicle[vehicleType=" + typeToString() + ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) + ",pos=" + pos.str() + "]"; }
};

class Infantry : public Unit
{
protected:
    int getType() const override { return infantryType; }
    void setAttackScore() override;
private:
    InfantryType infantryType;
    string typeToString() const override;
public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);
    int getAttackScore() override { return attackScore; }
    string str() const override { return "Infantry[infantryType=" + typeToString() + ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) + ",pos=" + pos.str() + "]"; }
};

class UnitList
{
private:
    void removeUnit(int type, bool isInfantry);
    struct Node
    {
        Unit *unit;
        Node *next;
        Node(Unit *unit, Node *next) : unit(unit), next(next) {}
    };
    Node *sentinal, *last;
    int capacity;
    int count_vehicle, count_infantry;
    string printList() const;
    vector<Unit *> findCombination(bool isInfantry, int targetScore);
    void updateWeights(double factor);
    void updateQuantities(double factor);
    void reinforce();
    int nextFibonacci(int n);
    void removeUnits(vector<Unit *> &unitList);
    void removeUnits(bool isInfatry); // remove all, 1 for infantry, 0 for vehicle
    void captureUnits(Army *enemy);
    void upToFibo();
    void resetScore();
    void removeUselessUnits();
public:
    UnitList();
    ~UnitList();
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(Unit *unit);
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists

    void updateUnit(Unit *unit);
    void updateUnit(VehicleType vehicleType, int weight);
    void updateUnit(InfantryType infantryType, int weight);
    string str() const;
    void setAttack(Node *p, double factor) { p->unit->updateAttackScore(round(p->unit->getAttackScore() * factor)); }
    int size() const { return count_vehicle + count_infantry; }

    friend class Army;
    friend class LiberationArmy;
    friend class ARVN;
};

class Army {
protected:
    int LF;
    int EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;
    void updateScore();
    void resetScore();
private:
    virtual vector<Unit *> getUnit(int target);
public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    ~Army() { delete unitList; }
    int getLF() const { return LF; }
    int getEXP() const { return EXP; }

    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    friend class UnitList;
};

class LiberationArmy : public Army {
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}
    // remember to update LF and EXP when insert new unit
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    friend class UnitList;
};

class ARVN : public Army {
private:
    vector<Unit *> getUnit(int target) override;
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    friend class UnitList;
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