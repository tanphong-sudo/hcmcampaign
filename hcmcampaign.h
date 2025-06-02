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
    static int safeCeil(double value);
    static double calculateDistance(const Position& pos1, const Position& pos2);
    static string trim(const string &str);
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0) : r(r), c(c) {}
    Position(const string &str_pos); 
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
    int getNumRows() const { return n_rows; }
    int getNumCols() const { return n_cols; }

    TerrainElement* getElement(int row, int col);

    void getForestEffect(Army *army);
    void getRiverEffect(Army *army);
    void getFortificationEffect(Army *army);
    void getUrbanEffect(Army *army);
    void getSpecialZoneEffect(Army *army);

    string str() const {
        return "BattleField[n_rows=" + to_string(n_rows) + ",n_cols=" + to_string(n_cols) + "]";
    }
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;
    int attackScore = -1;
private:
    virtual string typeToString() const = 0;
public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    void updateAttackScore(int score) { attackScore = score; }
    int getAttackScore();
    virtual void setAttackScore() = 0;
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
    string str() const override { return "Vehicle[vehicleType=" + typeToString() + ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) + ",position=" + pos.str() + "]"; }
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
    string str() const override { return "Infantry[infantryType=" + typeToString() + ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) + ",position=" + pos.str() + "]"; }
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
    void removeUnits(vector<Unit *> &unitList);
    void removeUnits(bool isInfatry); 
    void captureUnits(Army *enemy);
    void upToFibo();
    void resetScore();
    bool isSameType(Unit* unit1, Unit* unit2);
public:
    UnitList(int S);
    ~UnitList() {}
    bool insert(Unit *unit);             
    bool isContain(Unit *unit);
    bool isContain(VehicleType vehicleType); 
    bool isContain(InfantryType infantryType); 

    void updateUnit(Unit *unit);
    void updateUnit(VehicleType vehicleType, int weight);
    void updateUnit(InfantryType infantryType, int weight);
    void removeUselessUnits(int attackScore);
    void removeARVN();
    string str() const;
    void setAttack(Node *p, double factor) { p->unit->updateAttackScore(MathUtils::safeCeil(p->unit->getAttackScore() * factor)); }
    int size() const { return count_vehicle + count_infantry; }
    vector<Unit *> getInfantry();
    vector<Unit *> getVehicle();

    friend class Army;
    friend class LiberationArmy;
    friend class ARVN;
};

class Army {
protected:
    int LF;
    int EXP;
    string name;
    BattleField *battleField;
    UnitList *unitList;
    
public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual ~Army() {}
    int getLF() const { return LF; }
    int getEXP() const { return EXP; }
    void setLF(int LF) { this->LF = LF; }
    void setEXP(int EXP) { this->EXP = EXP; }
    string getName() const { return name; }
    void modifyScore();
    void updateScore();
    UnitList* getUnitList() const { return unitList; }
    void resetScore();

    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    friend class UnitList;
};

class LiberationArmy : public Army {
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}
    ~LiberationArmy() override { delete unitList; }
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    friend class UnitList;
};

class ARVN : public Army {
private:
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    ~ARVN() override { delete unitList; }
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    friend class UnitList;
};

class TerrainElement {
protected:
    Position pos; 
    double calculateDistance(const Position& pos1, const Position& pos2);
    int safeCeil(double value) {
        double diff = abs(value - round(value));
        return (diff < 1e-9) ? round(value) : ceil(value);
    }
public:
    TerrainElement(Position);
    virtual ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class Road : public TerrainElement {
public:
    Road(Position pos) : TerrainElement(pos) {}
    void getEffect(Army *army);
};

class Mountain : public TerrainElement {
public:
    Mountain(Position pos) : TerrainElement(pos) {}
    void getEffect(Army *army);
};

class River : public TerrainElement {
public:
    River(Position pos) : TerrainElement(pos) {}
    void getEffect(Army *army);
};

class Urban : public TerrainElement {
public:
    Urban(Position pos) : TerrainElement(pos) {}
    void getEffect(Army *army);
};

class Fortification : public TerrainElement {
public:
    Fortification(Position pos) : TerrainElement(pos) {}
    void getEffect(Army *army);
};

class SpecialZone : public TerrainElement {
public:
    SpecialZone(Position pos) : TerrainElement(pos) {}
    void getEffect(Army *army);
};

class Configuration {
    private:
        int num_rows;
        int num_cols;
        vector<Position*> arrayForest;
        vector<Position*> arrayRiver;
        vector<Position*> arrayFortification;
        vector<Position*> arrayUrban;
        vector<Position*> arraySpecialZone;
        Unit** liberationUnits = nullptr;
        Unit** ARVNUnits = nullptr;
        
        int eventCode;
        int liberationSize = 0;
        int ARVNSize = 0;
        void parseTerrainArray(const string &line, vector<Position*> &array);
        void parseUnitList(const string &line, Unit** &liberationUnits, Unit** &ARVNUnits);
        Unit* createUnit(const string &unitStr);
    public:
        Configuration(const string & filepath);
        ~Configuration();
        string str() const;
        
        int getNumRows() const { return num_rows; }
        int getNumCols() const { return num_cols; }
        const vector<Position*>& getForestPositions() const { return arrayForest; }
        const vector<Position*>& getRiverPositions() const { return arrayRiver; }
        const vector<Position*>& getFortificationPositions() const { return arrayFortification; }
        const vector<Position*>& getUrbanPositions() const { return arrayUrban; }
        const vector<Position*>& getSpecialZonePositions() const { return arraySpecialZone; }
        int getEventCode() const { return eventCode; }
        Unit** getLiberationUnits() { return liberationUnits; }
        Unit** getARVNUnits() { return ARVNUnits; }
        int getLiberationSize() const { return liberationSize; }
        int getARVNSize() const { return ARVNSize; }
    };

class HCMCampaign {
private:
    Configuration* config;
    BattleField* battleField;
    LiberationArmy* liberationArmy;
    ARVN* arvnArmy;
    void getTerrainEffect();
    void removeUselessUnits();
public:
    HCMCampaign(const string & config_file_path);
    ~HCMCampaign();
    void run();
    string printResult();
};
#endif