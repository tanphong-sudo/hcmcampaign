#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

//====================== MathUtils implementations ======================
    bool MathUtils::isSquare(int n) {
        int root = sqrt(n);
        return root * root == n;
    }

    int MathUtils::digitSum(int n) {
        int sum = 0;
        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }
        return sum;
    }

    bool MathUtils::isSpecial(int n) {
        // Check if n can be represented as sum of powers of 3, 5, 7
        int temp = n;
        int bases[] = {3, 5, 7};
        for (int base : bases) {
            temp = n;
            vector<int> powers;

            for (int p = 1; p <= temp; p *= base) {
                powers.push_back(p);
            }

            for (int i = powers.size() - 1; i >= 0 && temp > 0; i--) {
                if (powers[i] <= temp) 
                    temp -= powers[i];
            }

            if (temp == 0) return true;
        }
        return false;
    }

//====================== Position implementations ======================
Position::Position(const string &str_pos) {
    r = stoi(str_pos.substr(1, str_pos.find(',') - 1));
    c = stoi(str_pos.substr(str_pos.find(',') + 1, str_pos.size() - 2));
}
//====================== BattleField implementations ======================
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                           vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                           vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
    : n_rows(n_rows), n_cols(n_cols) {
    terrain.push_back(arrayForest);
    terrain.push_back(arrayRiver);
    terrain.push_back(arrayFortification);
    terrain.push_back(arrayUrban);
    terrain.push_back(arraySpecialZone);
}

BattleField::~BattleField() {
    terrain.clear();
}

//====================== Unit implementations ======================
Unit::~Unit() {}
Unit::Unit(int quantity, int weight, const Position pos) : quantity(quantity), weight(weight), pos(pos) { updateAttackScore(); }
Position Unit::getCurrentPosition() const { return pos; }

//====================== Vehicle implementations ======================
Vehicle::Vehicle (int quantity , int weight , const Position pos , VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {}

void Vehicle::updateAttackScore() {
    attackScore = (quantity * weight + 304 * vehicleType) / 30;
}

string Vehicle::typeToString() const {
    switch(vehicleType) {
        case TRUCK: return "TRUCK";
        case MORTAR: return "MORTAR";
        case ANTIAIRCRAFT: return "ANTIAIRCRAFT";
        case ARMOREDCAR: return "ARMOREDCAR";
        case APC: return "APC";
        case ARTILLERY: return "ARTILLERY";
        case TANK: return "TANK";
    }
    return "";
}

//====================== Infantry implementations ======================
Infantry::Infantry (int quantity , int weight , const Position pos , InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}

void Infantry::updateAttackScore() {
    int score = quantity * weight + 56 * infantryType;
    if (infantryType == SPECIALFORCES && MathUtils::isSquare(weight))
        score += 75;

    int n = MathUtils::digitSum(score);
    n = MathUtils::digitSum(n + 4);
    if (n > 7 || n < 3) {
        if (n > 7) {
            quantity += round((double) quantity * 0.2);
        }
        else
            quantity -= round((double) quantity * 0.1);

        score = quantity * weight + 56 * infantryType;
        if (infantryType == SPECIALFORCES && MathUtils::isSquare(weight))
            score += 75;
    }
    attackScore = score;
}

string Infantry::typeToString() const {
    switch(infantryType) {
        case SNIPER: return "SNIPER";
        case ANTIAIRCRAFTSQUAD: return "ANTIAIRCRAFTSQUAD";
        case MORTARSQUAD: return "MORTARSQUAD";
        case ENGINEER: return "ENGINEER";
        case SPECIALFORCES: return "SPECIALFORCES";
        case REGULARINFANTRY: return "REGULARINFANTRY";
    }
    return "";
}
//====================== UnitList implementations ======================

// Protected methods
void UnitList::removeUnit(InfantryType infantryType) {
    Node *p = sentinal;
    while (p->next->unit->getType() != infantryType)
        p = p->next;
    Node *temp = p->next;
    p->next = p->next->next;
    delete temp;
    count_infantry--;
}

void UnitList::removeUnit(VehicleType vehicleType) {
    Node *p = sentinal;
    while (p->next->unit->getType() != vehicleType)
        p = p->next;
    Node *temp = p->next;
    count_vehicle--;
    p->next = p->next->next;
    delete temp;
}

// Private methods
string UnitList::printList() const {
    string list = "";
    Node *p = sentinal->next;
    while (p)
        list += p->unit->str() + ",";
    return list;
}

// Public methods
UnitList::UnitList() {
    sentinal = new Node(nullptr, nullptr);
    last = sentinal;
    count_infantry = count_vehicle = 0;
}

UnitList::~UnitList() {
    Node *current = sentinal;
    while (current != nullptr) {
        Node *next = current->next;
        delete current;
        current = next;
    }
}

bool UnitList::insert(Unit *unit) {
    Node *p = sentinal;
    if (count_infantry + count_vehicle == capacity) return false;
    if (dynamic_cast<Vehicle*>(unit)) {
        count_vehicle++;
        last->next = new Node(unit, nullptr);
    }
    else if (dynamic_cast<Infantry*>(unit)) {
        sentinal->next = new Node(unit, sentinal->next);
        count_infantry++;
    }
    return true;
}

bool UnitList::isContain(VehicleType vehicleType) {
    Node *p = sentinal;
    while (p != nullptr) {
        if (p->unit->getType() == vehicleType) return true;
        p = p->next;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType) {
    Node *p = sentinal;
    while (p != nullptr) {
        if (p->unit->getType() == infantryType) return true;
        p = p->next;
    }
    return false;
}

string UnitList::str() const {
    return "UnitList[count_vehicle=" + to_string(count_vehicle) + ";count_infantry=" + to_string(count_infantry) + ";" + printList() + ']';
}

//====================== Army implementations ======================
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField) {
    LF = EXP = 0;
    unitList = new UnitList();

    for (int i = 0; i < size; i++) {
        unitList->insert(unitArray[i]);
        if (dynamic_cast<Infantry*>(unitArray[i]))
            EXP += unitArray[i]->getAttackScore();
        else
            LF += unitArray[i]->getAttackScore();
    }
    if (MathUtils::isSpecial(LF + EXP))
        unitList->capacity = 12;
    else    
        unitList->capacity = 8;

}

//====================== LiberationArmy implementations ======================
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}

bool LiberationArmy::win(int enemy, vector<Unit *> &unitList) {
    int sum = 0;
    for (auto unit : unitList) {
        sum += unit->getAttackScore();
    }
    return sum > enemy;
}

void LiberationArmy::fight(Army *enemy, bool defense) {
    if (!defense) {
        LF += round((double) LF * 0.5);
        EXP += round((double) EXP * 0.5);
        int enemyLF = enemy->getLF();
        int enemyEXP = enemy->getEXP();
    }
    else {

    }
}

vector<Unit *> LiberationArmy::getUnit(int target) {
    return {};
}

string LiberationArmy::str() const {
    return "LiberationArmy[" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + "]";
}

//====================== ARVN implementations ======================
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}

void ARVN::fight(Army *enemy, bool defense) {
    if (!defense) {

    }
    else {

    }
}

vector<Unit *> ARVN::getUnit(int target) {
    return {};
}

string ARVN::str() const {
    return "ARVN[" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + "]";
}
/*
//====================== TerrainElement implementations ======================
TerrainElement::TerrainElement() {}

TerrainElement::~TerrainElement() {}

//====================== Road implementations ======================
Road::Road() {}

Road::~Road() {}

void Road::getEffect(Army *army) { return; }

//====================== Mountain implementations ======================
Mountain::Mountain() {}

Mountain::~Mountain() {}

void Mountain::getEffect(Army *army) {

} 

//====================== HCMCampaign implementations ======================
HCMCampaign::HCMCampaign(const string &config_file_path) {
    // Implementation not provided
}

void HCMCampaign::run() {
    // Implementation not provided
}

string HCMCampaign::printResult() {
    // Implementation not provided
    return "";
}

*/
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////