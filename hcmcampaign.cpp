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

int nextFibo(int n) {
    int a = 0, b = 1;
    while (b < n) {
        int temp = b;
        b = a + b;
        a = temp;
    }
    return b;
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
Unit::Unit(int quantity, int weight, const Position pos) : quantity(quantity), weight(weight), pos(pos) { setAttackScore(); }
Position Unit::getCurrentPosition() const { return pos; }

//====================== Vehicle implementations ======================
Vehicle::Vehicle (int quantity , int weight , const Position pos , VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {}

void Vehicle::setAttackScore() {
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

void Infantry::setAttackScore() {
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

// Private methods
void UnitList::removeUnit(int type, bool isInfantry) {
    Node *p = sentinal;
    while (p->next != nullptr) {
        if (isInfantry && dynamic_cast<Infantry*>(p->next->unit) && p->next->unit->getType() == type) {
            Node *temp = p->next;
            p->next = p->next->next;
            delete temp;
            count_infantry--;
            return;
        }
        else if (!isInfantry && dynamic_cast<Vehicle*>(p->next->unit) && p->next->unit->getType() == type) {
            Node *temp = p->next;
            p->next = p->next->next;
            delete temp;
            count_vehicle--;
            return;
        }
        p = p->next;
    }
}

string UnitList::printList() const {
    string list = "";
    Node *p = sentinal->next;
    list += p->unit->str();
    p = p->next;
    while (p) {
        list += "," + p->unit->str();
        p = p->next;
    }
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

    if (dynamic_cast<Vehicle*>(unit)) {
        count_vehicle++;
        last->next = new Node(unit, nullptr);
        last = last->next;
    }
    else if (dynamic_cast<Infantry*>(unit)) {
        Node *temp = new Node(unit, sentinal->next);
        sentinal->next = temp;
        if (last == sentinal) last = temp;
        count_infantry++;
    }
    return true;
}

bool UnitList::isContain(Unit* unit) {
    if (dynamic_cast<Vehicle*>(unit)) {
        return isContain(static_cast<VehicleType>(unit->getType()));
    }
    else if (dynamic_cast<Infantry*>(unit)) {
        return isContain(static_cast<InfantryType>(unit->getType()));
    }
    return false;
}

bool UnitList::isContain(VehicleType vehicleType) {
    Node *p = sentinal->next;
    while (p != nullptr) {
        if (p->unit->getType() == vehicleType) return true;
        p = p->next;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType) {
    Node *p = sentinal->next;
    while (p != nullptr) {
        if (p->unit->getType() == infantryType) return true;
        p = p->next;
    }
    return false;
}

void UnitList::updateWeights(double factor) {
    Node *p = sentinal->next;
    while (p != nullptr) {
        p->unit->weight = round(p->unit->weight * factor);
        p = p->next;
    }
}

void UnitList::updateQuantities(double factor) {
    Node *p = sentinal->next;
    while (p != nullptr) {
        p->unit->quantity = round(p->unit->quantity * factor);
        p = p->next;
    }
}

void UnitList::removeUnits(vector<Unit *> &unitList) {
    for (int i = 0; i < unitList.size(); i++) {
        removeUnit(unitList[i]->getType(), dynamic_cast<Infantry*>(unitList[i]));
    }
}

void UnitList::removeUnits(bool isInfantry) {
    Node *p = sentinal->next;
    if (isInfantry) {
        while (p != nullptr) {
            if (dynamic_cast<Vehicle*>(p->unit)) return;
            Node *temp = p;
            p = p->next;
            delete temp;
            count_infantry--;
        }
    }
    else {
        while (p != nullptr) {
            if (dynamic_cast<Infantry*>(p->unit)) return;
            Node *temp = p;
            p = p->next;
            delete temp;
            count_vehicle--;
        }
    }
}

void UnitList::updateUnit(Unit *unit) {
    if (dynamic_cast<Vehicle*>(unit)) {
        updateUnit(static_cast<VehicleType>(unit->getType()), unit->weight);
    }
    else if (dynamic_cast<Infantry*>(unit)) {
        updateUnit(static_cast<InfantryType>(unit->getType()), unit->weight);
    }
}

void UnitList::updateUnit(VehicleType vehicleType, int weight) {
    Node *p = sentinal->next;
    while (p != nullptr) {
        if (dynamic_cast<Vehicle*>(p->unit) && p->unit->getType() == vehicleType) {
            p->unit->quantity += weight;
            return;
        }
        p = p->next;
    }
}

void UnitList::updateUnit(InfantryType infantryType, int weight) {
    Node *p = sentinal->next;
    while (p != nullptr) {
        if (dynamic_cast<Infantry*>(p->unit) && p->unit->getType() == infantryType) {
            p->unit->quantity += weight;
            return;
        }
        p = p->next;
    }
}

void UnitList::captureUnits(Army *enemy) {
    Node *p = enemy->unitList->sentinal->next;
    while (p != nullptr) {
        if (this->size() == capacity) return;

        if (isContain(p->unit)) updateUnit(p->unit);
        else insert(p->unit);
        Node *temp = p;
        p = p->next;
        delete temp;
    }
}

void UnitList::upToFibo() {
    for (Node *p = sentinal->next; p; p = p->next) {
        p->unit->quantity = MathUtils::nextFibo(p->unit->quantity);
    }
}

void UnitList::resetScore() {
    for (Node *p = sentinal->next; p; p = p->next) {
        p->unit->setAttackScore();
    }
}

void UnitList::removeUselessUnits() {
    for (Node *p = sentinal->next; p; p = p->next) {
        if (p->unit->getAttackScore() <= 1) {
            Node *temp = p;
            p = p->next;
            delete temp;
        }
    }
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

void Army::updateScore() {
    LF = EXP = 0;
    UnitList::Node *p = unitList->sentinal->next;
    while (p != nullptr) {
        if (dynamic_cast<Infantry*>(p->unit))
            EXP += p->unit->getAttackScore();
        else
            LF += p->unit->getAttackScore();
        p = p->next;
    }
}

void Army::resetScore() {
    unitList->resetScore();
    updateScore();
}

//====================== LiberationArmy implementations ======================
void LiberationArmy::fight(Army *enemy, bool defense) {
    if (!defense) {
        LF = round((double) LF * 1.5);
        EXP = round((double) EXP * 1.5);
        int enemyLF = enemy->getLF();
        int enemyEXP = enemy->getEXP();

        // Modify attackScore
        for (UnitList::Node *p = unitList->sentinal->next; p != nullptr; p = p->next) {
            unitList->setAttack(p, 1.5);
        }
        
        // Find optimal combination
        vector<Unit*> infantryCombination = unitList->findCombination(true, enemyEXP);
        vector<Unit*> vehicleCombination = unitList->findCombination(false, enemyLF);
        bool hasVehicleCombo = !vehicleCombination.empty();
        bool hasInfantryCombo = !infantryCombination.empty();
        
        bool hadBattle = false;
        if (hasInfantryCombo && hasVehicleCombo) {
            hadBattle = true;
            unitList->removeUnits(infantryCombination);
            unitList->removeUnits(vehicleCombination);
            unitList->captureUnits(enemy);
        } 
        else if (hasInfantryCombo && LF > enemyLF) {
            hadBattle = true;
            unitList->removeUnits(infantryCombination);
            unitList->removeUnits(false); // remove all Vehicle
            unitList->captureUnits(enemy);
        } 
        else if (hasVehicleCombo && EXP > enemyEXP) {
            hadBattle = true;
            unitList->removeUnits(vehicleCombination);
            unitList->removeUnits(true); // remove all Infantry
            unitList->captureUnits(enemy);
        } 
        else {
            unitList->updateWeights(0.9);
            updateScore();
        }
        
        updateScore();
    }
    else {
        LF = round((double) LF * 1.3);
        EXP = round((double) EXP * 1.3);
        int enemyLF = enemy->getLF();
        int enemyEXP = enemy->getEXP();
        bool endFight = false;

        while (!endFight) {
            for (UnitList::Node *p = unitList->sentinal->next; p != nullptr; p = p->next) 
                unitList->setAttack(p, 1.3);

            if (LF >= enemyLF && EXP >= enemyEXP) {
                endFight = true;
                break;
            }
            else if (LF >= enemyLF || EXP >= enemyEXP) {
                unitList->updateQuantities(0.9);
                updateScore();
                endFight = true;
            } else {
                unitList->upToFibo();
                updateScore();
            }
        }
    }
    resetScore();
}

string LiberationArmy::str() const {
    return "LiberationArmy[name=" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + ",unitList=" + unitList->str() + "]";
}

//====================== ARVN implementations ======================
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}

void ARVN::fight(Army *enemy, bool defense) {
    if (!defense) {
        unitList->updateQuantities(0.8);
        unitList->removeUselessUnits();
        updateScore();
    }
    else {
        LiberationArmy* liberationArmy = dynamic_cast<LiberationArmy*>(enemy);
        if (liberationArmy) {
            UnitList* originalList = unitList;
            int originalSize = originalList->size();

            liberationArmy->fight(this, false);

            if (unitList->size() < originalSize) {
                unitList->updateWeights(0.8);
                updateScore();
            }
        }
    }
}


string ARVN::str() const {
    return "ARVN[name=" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + ",unitList=" + unitList->str() + "]";
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