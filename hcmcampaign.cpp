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
    if (n == 0) return 0;
    while (n > 9) {
        int sum = 0;
        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }
        n = sum;
    }
    return n;
}

bool MathUtils::isSpecial(int n) {
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

int MathUtils::nextFibo(int n) {
    int a = 0, b = 1;
    while (b <= n) {
        int temp = b;
        b = a + b;
        a = temp;
    }
    return b;
}

int MathUtils::safeCeil(double value) {
    int a = round(value);
    if (abs(value - a) < 1e-7) {
        return a;
    }
    return ceil(value);
}

double MathUtils::calculateDistance(const Position& pos1, const Position& pos2) {
    int rowDiff = pos1.getRow() - pos2.getRow();
    int colDiff = pos1.getCol() - pos2.getCol();
    return (sqrt(double(rowDiff * rowDiff + colDiff * colDiff)));
}

string MathUtils::trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}
//====================== Position implementations ======================
Position::Position(const string &str_pos) {
    string clean = str_pos.substr(1, str_pos.size() - 2);
    clean = MathUtils::trim(clean);
    size_t comma = clean.find(',');
    long long tempR = (int)stoll(MathUtils::trim(clean.substr(0, comma)));
    long long tempC = (int)stoll(MathUtils::trim(clean.substr(comma + 1)));
    if (tempR <= INT_MAX) r = (int)tempR;
    else r = INT_MAX; 
    if (tempC <= INT_MAX) c = (int)tempC;
    else c = INT_MAX;
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

void BattleField::getForestEffect(Army *army) {
    for (auto &f : terrain[0]) {
        TerrainElement *forest = new Mountain(*f);
        forest->getEffect(army);
        delete forest; 
    }
}

void BattleField::getRiverEffect(Army *army) {
    for (auto &r : terrain[1]) {
        TerrainElement *river = new River(*r);
        river->getEffect(army);
        delete river;
    }
}

void BattleField::getFortificationEffect(Army *army) {
    for (auto &f : terrain[2]) {
        TerrainElement *fortification = new Fortification(*f);
        fortification->getEffect(army);
        delete fortification;
    }
}

void BattleField::getUrbanEffect(Army *army) {
    for (auto &u : terrain[3]) {
        TerrainElement *urban = new Urban(*u);
        urban->getEffect(army);
        delete urban;
    }
}

void BattleField::getSpecialZoneEffect(Army *army) {
    for (auto &s : terrain[4]) {
        TerrainElement *specialZone = new SpecialZone(*s);
        specialZone->getEffect(army);
        delete specialZone;
    }
}

TerrainElement* BattleField::getElement(int row, int col) {
    if (row < 0 || row >= n_rows || col < 0 || col >= n_cols) {
        return nullptr; 
    }
    for (auto &forest : terrain[0]) {
        if (forest->getRow() == row && forest->getCol() == col) {
            return new Mountain(*forest);
        }
    }
    for (auto &river : terrain[1]) {
        if (river->getRow() == row && river->getCol() == col) {
            return new River(*river);
        }
    }
    for (auto &fortification : terrain[2]) {
        if (fortification->getRow() == row && fortification->getCol() == col) {
            return new Fortification(*fortification);
        }
    }
    for (auto &urban : terrain[3]) {
        if (urban->getRow() == row && urban->getCol() == col) {
            return new Urban(*urban);
        }
    }
    for (auto &specialZone : terrain[4]) {
        if (specialZone->getRow() == row && specialZone->getCol() == col) {
            return new SpecialZone(*specialZone);
        }
    }
    return new Road(Position(row, col)); 
}

//====================== Unit implementations ======================
Unit::~Unit() {}
Unit::Unit(int quantity, int weight, const Position pos) : quantity(quantity), weight(weight), pos(pos) {
    if (this->quantity < 1) this->quantity = 1; 
    if (this->weight < 1) this->weight = 1;    
}
Position Unit::getCurrentPosition() const { return pos; }

int Unit::getAttackScore() {
    if (attackScore == -1) {
        setAttackScore(); 
    }
    return attackScore;
}

string Unit::str() const {
    return "Unit[quantity=" + to_string(quantity) + ",weight=" + to_string(weight) + ",pos=" + pos.str() + "]";
}

int Unit::getType() const {
    return -1; 
}

//====================== Vehicle implementations ======================
Vehicle::Vehicle (int quantity , int weight , const Position pos , VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {
    setAttackScore();
    }

void Vehicle::setAttackScore() {
    attackScore = MathUtils::safeCeil((double)(quantity * weight + 304 * vehicleType) / 30);
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
    : Unit(quantity, weight, pos), infantryType(infantryType) {
    //setAttackScore();
    }

void Infantry::setAttackScore() {
    int score = quantity * weight + 56 * infantryType;
    if (infantryType == SPECIALFORCES && MathUtils::isSquare(weight))
        score += 75;

    int n = MathUtils::digitSum(score);
    n = MathUtils::digitSum(n + 4);
    if (n > 7 || n < 3) {
        if (n > 7) {
            quantity += MathUtils::safeCeil((double) quantity * 0.2);
        }
        else
            quantity = MathUtils::safeCeil(quantity * 0.9);

        if (quantity < 1) quantity = 1;
    }
    score = quantity * weight + 56 * infantryType;
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
void UnitList::removeUnit(int type, bool isInfantry) {
    Node *p = sentinal;
    while (p->next != nullptr) {
        if (p->next->unit != nullptr) {
            if (isInfantry && dynamic_cast<Infantry*>(p->next->unit) && p->next->unit->getType() == type) {
                Node *temp = p->next;
                count_infantry--;
                p->next = temp->next;
                if (temp == last) last = p;
                delete temp; 
                
                return;
            }
            else if (!isInfantry && dynamic_cast<Vehicle*>(p->next->unit) && p->next->unit->getType() == type) {
                Node *temp = p->next;
                count_vehicle--;
                p->next = temp->next;
                if (temp == last) last = p;
                delete temp; 
               
                return;
            }
        }
        p = p->next;
    }
}

string UnitList::printList() const {
    string list = "";
    Node *p = sentinal->next;
    if (p == nullptr) return list; 
    list = ";";
    if (p->unit) list += p->unit->str();
    p = p->next;
    while (p) {
        if (p->unit) list += "," + p->unit->str(); 
        p = p->next;
    }
    return list;
}

UnitList::UnitList(int S) : capacity(S), count_vehicle(0), count_infantry(0) {
    sentinal = new Node(nullptr, nullptr);
    last = sentinal;
}

bool UnitList::isSameType(Unit* unit1, Unit* unit2) {
    if (!unit1 || !unit2) return false;

    Infantry* infantry1 = dynamic_cast<Infantry*>(unit1);
    Infantry* infantry2 = dynamic_cast<Infantry*>(unit2);
    if (infantry1 && infantry2) {
        return unit1->getType() == unit2->getType();
    }

    Vehicle* vehicle1 = dynamic_cast<Vehicle*>(unit1);
    Vehicle* vehicle2 = dynamic_cast<Vehicle*>(unit2);
    if (vehicle1 && vehicle2) {
        return unit1->getType() == unit2->getType();
    }

    return false;
}

bool UnitList::insert(Unit *unit) {
    if (isContain(unit)) {
        Node *p = sentinal;
        while (p->next != nullptr) {
            if (isSameType(p->next->unit, unit)) {
                p->next->unit->quantity += unit->quantity;
                p->next->unit->weight = max(p->next->unit->weight, unit->weight);
                p->next->unit->setAttackScore();
                return false;
            }
            p = p->next;
        }
    }
    if (this->count_infantry + this->count_vehicle >= capacity || !unit) {
        return false;
    }
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
        p->unit->weight = MathUtils::safeCeil(p->unit->weight * factor);
        p->unit->setAttackScore();
        p = p->next;
    }
}

void UnitList::updateQuantities(double factor) {
    Node *p = sentinal->next;
    while (p != nullptr) {
        p->unit->quantity = MathUtils::safeCeil(p->unit->quantity * factor);
        p->unit->setAttackScore();
        p = p->next;
    }
}

void UnitList::removeUnits(vector<Unit*>& unitList) {
    for (size_t i = 0; i < unitList.size(); i++) {
        if (unitList[i] != nullptr) {
            bool isInfantry = dynamic_cast<Infantry*>(unitList[i]) != nullptr;
            removeUnit(unitList[i]->getType(), isInfantry);
        }
    }
}

void UnitList::removeUnits(bool isInfantry) {
    Node *p = sentinal;
    while (p->next != nullptr) {
        if (p->next->unit != nullptr) {
            if (isInfantry && dynamic_cast<Infantry*>(p->next->unit)) {
                Node *temp = p->next;
                count_infantry--;
                p->next = temp->next;
                if (temp == last) last = p;
                delete temp; 
                
            }
            else if (!isInfantry && dynamic_cast<Vehicle*>(p->next->unit)) {
                Node *temp = p->next;
                count_vehicle--;
                p->next = temp->next;
                if (temp == last) last = p;
                delete temp;
                
            }
            else 
                p = p->next; 
        }
        else 
            p = p->next;
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
    vector <Unit*> unitList;
    if (!enemy || !enemy->unitList) return;
    Node *p = enemy->unitList->sentinal;
    while (p->next != nullptr) {
        if (this->size() == capacity) break;
        if (p->next->unit != nullptr) {
            unitList.push_back(p->next->unit);
            Node *temp = p->next;
            p->next = temp->next;
            if (temp == enemy->unitList->last) enemy->unitList->last = p;
            if (dynamic_cast<Infantry*>(temp->unit)) enemy->unitList->count_infantry--;
            else if (dynamic_cast<Vehicle*>(temp->unit)) enemy->unitList->count_vehicle--;
            delete temp; 
        }
        else {
            p = p->next;
        }
    }
    for (int i = unitList.size() - 1; i >= 0; i--) {
        if (unitList[i] != nullptr) 
            insert(unitList[i]);
    }
}

void UnitList::upToFibo() {
    for (Node *p = sentinal->next; p; p = p->next) {
        p->unit->quantity = MathUtils::nextFibo(p->unit->quantity);
        p->unit->setAttackScore();
    }
}

void UnitList::resetScore() {
    for (Node *p = sentinal->next; p; p = p->next) {
        p->unit->setAttackScore();
    }
}

void UnitList::removeUselessUnits(int attackScore) {
    Node *prev = sentinal;
    Node *p = sentinal->next;
    while (p) {
        if (p->unit->getAttackScore() <= attackScore) {
            if (dynamic_cast<Infantry*>(p->unit)) {
                count_infantry--;
            } else if (dynamic_cast<Vehicle*>(p->unit)) {
                count_vehicle--;
            }
            if (p == last) {
                last = prev;
            }

            prev->next = p->next;
            delete p;
            p = prev->next;
        } else {
            prev = p;
            p = p->next;
        }
    }
}

vector<Unit*> UnitList::findCombination(bool isInfantry, int targetScore) {
    vector<Unit*> result;
    Node *p = sentinal->next;
    vector<Unit*> unitList;
    if (isInfantry) {
        while (p != nullptr) {
            if (dynamic_cast<Infantry*>(p->unit)) {
                unitList.push_back(p->unit);
            }
            p = p->next;
        }
    }
    else {
        while (p != nullptr) {
            if (dynamic_cast<Vehicle*>(p->unit)) {
                unitList.push_back(p->unit);
            }
            p = p->next;
        }
    }

    if (targetScore == 0) {
        int minScore = INT_MAX;
        for (auto &unit : unitList) {
            if (unit->getAttackScore() < minScore) {
                minScore = unit->getAttackScore();
            }
        }
        for (auto &unit : unitList) {
            if (unit->getAttackScore() == minScore) {
                return {unit}; 
            }
        }
    }

    int n = unitList.size();
    int minScore = INT_MAX;
    for (int i = 0; i < (1 << n); i++) {
        vector<Unit*> currentCombination;
        int currentScore = 0;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                currentCombination.push_back(unitList[j]);
                currentScore += unitList[j]->getAttackScore();
            }
        }
        if (currentScore > targetScore && currentScore < minScore) {
            result = currentCombination;
            minScore = currentScore;
        }
    }
    return result;
}

vector<Unit*> UnitList::getInfantry() {
    vector<Unit*> infantryUnits;
    Node *p = sentinal->next;
    while (p != nullptr) {
        if (dynamic_cast<Infantry*>(p->unit)) {
            infantryUnits.push_back(p->unit);
        }
        p = p->next;
    }
    return infantryUnits;
}
vector<Unit*> UnitList::getVehicle() {
    vector<Unit*> vehicleUnits;
    Node *p = sentinal->next;
    while (p != nullptr) {
        if (dynamic_cast<Vehicle*>(p->unit)) {
            vehicleUnits.push_back(p->unit);
        }
        p = p->next;
    }
    return vehicleUnits;
}

void UnitList::removeARVN() {
    Node *prev = sentinal;
    Node *p = sentinal->next;
    while (p) {
        if (p->unit->quantity <= 1) {
            if (dynamic_cast<Infantry*>(p->unit)) {
                count_infantry--;
            } else if (dynamic_cast<Vehicle*>(p->unit)) {
                count_vehicle--;
            }
            if (p == last) {
                last = prev;
            }

            prev->next = p->next;
            delete p;
            p = prev->next;
        } else {
            prev = p;
            p = p->next;
        }
    }
}

string UnitList::str() const {
    return "UnitList[count_vehicle=" + to_string(count_vehicle) + ";count_infantry=" + to_string(count_infantry) + printList() + ']';
}

//====================== Army implementations ======================
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField) {
    LF = EXP = 0;
    for (int i = 0; i < size; i++) {
        if (dynamic_cast<Infantry*>(unitArray[i])) {
            unitArray[i]->setAttackScore(); 
            EXP += unitArray[i]->getAttackScore();
        }
        else
            LF += unitArray[i]->getAttackScore();
    }
    LF = max(0, min(LF, 1000));
    EXP = max(0, min(EXP, 500));
    int capa = (MathUtils::isSpecial(LF + EXP)) ? 12 : 8;
    unitList = new UnitList(capa);

    for (int i = 0; i < size; i++) {
        unitList->insert(unitArray[i]);
    }

}

void Army::modifyScore() {
    LF = max(0, min(LF, 1000));
    EXP = max(0, min(EXP, 500));
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
    modifyScore();
}

//====================== LiberationArmy implementations ======================
void LiberationArmy::fight(Army *enemy, bool defense) {
    if (!defense) {
        int newLF = MathUtils::safeCeil(LF * 1.5);
        int newEXP = MathUtils::safeCeil(EXP * 1.5);
        newLF = min(newLF, 1000);
        newEXP = min(newEXP, 500);
        int enemyLF = enemy->getLF();
        int enemyEXP = enemy->getEXP();
        
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
            resetScore();
            this->updateScore();
            enemy->updateScore();
        } 
        else if (hasInfantryCombo && newLF > enemyLF) {
            hadBattle = true;
            unitList->removeUnits(infantryCombination);
            unitList->removeUnits(false); 
            unitList->captureUnits(enemy);
            resetScore();
            this->updateScore();
            enemy->updateScore();
        } 
        else if (hasVehicleCombo && newEXP > enemyEXP) {
            hadBattle = true;
            unitList->removeUnits(vehicleCombination);
            unitList->removeUnits(true); 
            unitList->captureUnits(enemy);
            resetScore();
            this->updateScore();
            enemy->updateScore();
        } 
        else {
            unitList->updateWeights(0.9);
            updateScore();
        }
    }
    else {
        int enemyLF = enemy->getLF();
        int enemyEXP = enemy->getEXP();
        bool endFight = false;

        while (!endFight) {
            int newLF = MathUtils::safeCeil((double) LF * 1.3);
            int newEXP = MathUtils::safeCeil((double) EXP * 1.3);
            newLF = min(newLF, 1000);
            newEXP = min(newEXP, 500);

            if (newLF >= enemyLF && newEXP >= enemyEXP) {
                endFight = true;
                LF = newLF;
                EXP = newEXP;
                modifyScore();
                return;
            }
            else if (newLF >= enemyLF || newEXP >= enemyEXP) {
                unitList->updateQuantities(0.9);
                updateScore();

                endFight = true;
            } else {
                unitList->upToFibo();
                updateScore();
            }
        }
    }
    modifyScore();
}

string LiberationArmy::str() const {
    string res = "LiberationArmy[LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + ",unitList=" + unitList->str() + ",battleField=";
    if (battleField != nullptr) {
        res += battleField->str();
    }
    res += "]";
    return res;
}

//====================== ARVN implementations ======================
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}

void ARVN::fight(Army *enemy, bool defense) {
    if (!defense) {
        unitList->updateQuantities(0.8);
        unitList->removeARVN();
        updateScore();
    }
    else {
        LiberationArmy* liberationArmy = dynamic_cast<LiberationArmy*>(enemy);
        int newLF = MathUtils::safeCeil((double) liberationArmy->getLF() * 1.5);
        int newEXP = MathUtils::safeCeil((double) liberationArmy->getEXP() * 1.5);
        newLF = min(newLF, 1000);
        newEXP = min(newEXP, 500);

        vector<Unit*> infantryCombination = liberationArmy->getUnitList()->findCombination(true, this->EXP);
        vector<Unit*> vehicleCombination = liberationArmy->getUnitList()->findCombination(false, this->LF);
        bool hasVehicleCombo = !vehicleCombination.empty();
        bool hasInfantryCombo = !infantryCombination.empty();
        bool hadBattle = false;

        if (hasInfantryCombo && hasVehicleCombo) {
            unitList->updateWeights(0.8);
        } 
        else if (hasInfantryCombo && newLF > this->LF) {
            hadBattle = true;
            unitList->updateWeights(0.8);
        } 
        else if (hasVehicleCombo && newEXP > this->EXP) {
            hadBattle = true;
            unitList->updateWeights(0.8);
        } 
    }
}


string ARVN::str() const {
    string res = "ARVN[LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + ",unitList=" + unitList->str() + ",battleField=";
    if (battleField) {
        res += battleField->str();
    }
        res += "]";
        return res;
}

//====================== TerrainElement implementations ======================
double TerrainElement::calculateDistance(const Position& pos1, const Position& pos2) {
    int rowDiff = pos1.getRow() - pos2.getRow();
    int colDiff = pos1.getCol() - pos2.getCol();
    return (sqrt(double(rowDiff * rowDiff + colDiff * colDiff)));
}

TerrainElement::TerrainElement(Position pos) : pos(pos)
{}

TerrainElement::~TerrainElement() {
}

// Road: không có hiệu ứng
void Road::getEffect(Army *army) {
}

void Mountain::getEffect(Army *army) {
    if (army == nullptr) return; 
    vector<Unit *> Infantry = army->getUnitList()->getInfantry();
    vector<Unit *> Vehicle = army->getUnitList()->getVehicle();
    if (dynamic_cast<LiberationArmy*>(army)) {
        for (Unit *unit : Infantry) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 2) {
                int temp = unit->getAttackScore();
                army->setEXP(MathUtils::safeCeil(army->getEXP() + MathUtils::safeCeil(temp * 0.3)));
            }
        }
        for (Unit *unit : Vehicle) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 2) {
                int temp = unit->getAttackScore();
                army->setLF(MathUtils::safeCeil(army->getLF() - temp * 0.1));
            }
        }
    }
    else {
        for (Unit *unit : Infantry) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 4) {
                int temp = unit->getAttackScore();
                army->setEXP(MathUtils::safeCeil(army->getEXP() + MathUtils::safeCeil(temp * 0.2)));
            }
        }
        for (Unit *unit : Vehicle) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 4) {
                int temp = unit->getAttackScore();
                army->setLF(MathUtils::safeCeil(army->getLF() - temp * 0.05));
            }
        }
    }
    army->modifyScore();
}

void River::getEffect(Army *army) {
    if (army == nullptr) return;
  vector<Unit *> Infantry = army->getUnitList()->getInfantry();
  vector<Unit *> Vehicle = army->getUnitList()->getVehicle();
  for (Unit *unit : Infantry) {
    if (calculateDistance(unit->getCurrentPosition(), pos) <= 2) {
      unit->updateAttackScore(safeCeil(unit->getAttackScore() * 0.9));
      //unit->setAttackScore();
    }
  }
  army->modifyScore();
}

void Urban::getEffect(Army *army) {
    if (army == nullptr) return;
    vector<Unit *> Infantry = army->getUnitList()->getInfantry();
    vector<Unit *> Vehicle = army->getUnitList()->getVehicle();
    if (dynamic_cast<LiberationArmy*>(army)) {
        for (Unit *unit : Infantry) {
            double D = calculateDistance(unit->getCurrentPosition(), pos);
            if (D == 0) continue;
            if (D <= 5 && (unit->getType() == SPECIALFORCES || unit->getType() == REGULARINFANTRY)) {
                int temp = unit->getAttackScore();
                unit->updateAttackScore(unit->getAttackScore() + safeCeil((double)unit->getAttackScore() * 2 / D));
                //unit->setAttackScore();
            }
        }
        for (Unit *unit : Vehicle) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 2 && unit->getType() == ARTILLERY) {
                int temp = unit->getAttackScore();
                unit->updateAttackScore(safeCeil(unit->getAttackScore() * 0.5));
                //unit->setAttackScore();
            }
        }
    }
    else {
        for (Unit *unit : Infantry) {
            double D = calculateDistance(unit->getCurrentPosition(), pos);
            if (D <= 5 && unit->getType() == REGULARINFANTRY) {
                if (D == 0) continue;
                int temp = unit->getAttackScore();
                unit->updateAttackScore(unit->getAttackScore() + safeCeil((double)unit->getAttackScore() * 3 / (2 * D)));
                //unit->setAttackScore();
            }
        }
    }
    army->modifyScore();
}

void Fortification::getEffect(Army *army) {
    if (army == nullptr) return;
    vector<Unit *> Infantry = army->getUnitList()->getInfantry();
    vector<Unit *> Vehicle = army->getUnitList()->getVehicle();
    if (dynamic_cast<LiberationArmy*>(army)) {
        for (Unit *unit : Infantry) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 2) {
                int temp = unit->getAttackScore();
                unit->updateAttackScore(safeCeil(unit->getAttackScore() * 0.8));
                //unit->setAttackScore();
            }
        }
        for (Unit *unit : Vehicle) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 2) {
                int temp = unit->getAttackScore();
                unit->updateAttackScore(safeCeil(unit->getAttackScore() * 0.8));
                //unit->setAttackScore();
            }
        }
    }
    else {
        for (Unit *unit : Infantry) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 2) {
                int temp = unit->getAttackScore();
                unit->updateAttackScore(safeCeil(unit->getAttackScore() * 1.2));
                //unit->setAttackScore();
            }
        }
        for (Unit *unit : Vehicle) {
            if (calculateDistance(unit->getCurrentPosition(), pos) <= 2) {
                int temp = unit->getAttackScore();
                unit->updateAttackScore(safeCeil(unit->getAttackScore() * 1.2));
                //unit->setAttackScore();
            }
        }
    }
    army->modifyScore();
}

void SpecialZone::getEffect(Army *army) {
    if (army == nullptr) return;
    vector<Unit *> Infantry = army->getUnitList()->getInfantry();
    vector<Unit *> Vehicle = army->getUnitList()->getVehicle();
    for (Unit *unit : Infantry) {
        if (calculateDistance(unit->getCurrentPosition(), pos) <= 1) {
            //army->setEXP(army->getEXP() - unit->getAttackScore());
            unit->updateAttackScore(0);
        }
    }
    for (Unit *unit : Vehicle) {
        if (calculateDistance(unit->getCurrentPosition(), pos) <= 1) {
            unit->updateAttackScore(0);
        }
    }
    army->modifyScore();
}

//====================== Config implementations ======================
Configuration::Configuration(const std::string &filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filepath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        line = MathUtils::trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.find("NUM_ROWS=") == 0) {
            num_rows = stoi(MathUtils::trim(line.substr(line.find('=') + 1)));
        } else if (line.find("NUM_COLS=") == 0) {
            num_cols = stoi(MathUtils::trim(line.substr(line.find('=') + 1)));
        } else if (line.find("ARRAY_FOREST=") == 0) {
            parseTerrainArray(line, arrayForest);
        } else if (line.find("ARRAY_RIVER=") == 0) {
            parseTerrainArray(line, arrayRiver);
        } else if (line.find("ARRAY_FORTIFICATION=") == 0) {
            parseTerrainArray(line, arrayFortification);
        } else if (line.find("ARRAY_URBAN=") == 0) {
            parseTerrainArray(line, arrayUrban);
        } else if (line.find("ARRAY_SPECIAL_ZONE=") == 0) {
            parseTerrainArray(line, arraySpecialZone);
        } else if (line.find("UNIT_LIST=") == 0) {
            parseUnitList(line, liberationUnits, ARVNUnits);
        } else if (line.find("EVENT_CODE=") == 0) {
            eventCode = stoi(MathUtils::trim(line.substr(line.find('=') + 1)));
            if (eventCode < 0) {
                eventCode = 0;
            } else
                eventCode = eventCode % 100;
        }
    }

    file.close();
}

void Configuration::parseUnitList(const string &line, Unit** &liberationUnits, Unit** &ARVNUnits) {
    vector<Unit*> liberationUnitsTemp;
    vector<Unit*> ARVNUnitsTemp;
    size_t start = line.find('[');
    size_t end = line.find(']');
    if (start == string::npos || end == string::npos) return;
    string units = MathUtils::trim(line.substr(start + 1, end - start - 1));
    if (units.empty()) return;
    size_t pos = 0;
    while (pos < units.size()) {
        size_t unitStart = pos;
        int parenCount = 0;
        size_t unitEnd = pos;
        while (unitEnd < units.size()) {
            if (units[unitEnd] == '(') parenCount++;
            else if (units[unitEnd] == ')') parenCount--;
            if (parenCount == 0 && units[unitEnd] == ')' && (unitEnd + 1 == units.size() || units[unitEnd + 1] == ',')) {
                unitEnd++;
                break;
            }
            unitEnd++;
        }
        string unitStr = MathUtils::trim(units.substr(unitStart, unitEnd - unitStart));
        if (!unitStr.empty() && unitStr[unitStr.size() - 2] >= '0' && unitStr[unitStr.size() - 2] <= '1') {
            try {
                int armyBelong = unitStr[unitStr.size() - 2] - '0';
                Unit *newUnit = createUnit(unitStr);
                if (newUnit) {
                    if (armyBelong == 0) liberationUnitsTemp.push_back(newUnit);
                    else ARVNUnitsTemp.push_back(newUnit);
                }
            } catch (const std::exception& e) {
                cerr << "Unit error: " << unitStr << " (" << e.what() << ")" << endl;
            }
        }
        unitEnd++;
        pos = unitEnd;
    }
    liberationUnits = new Unit*[liberationUnitsTemp.size()];
    ARVNUnits = new Unit*[ARVNUnitsTemp.size()];
    for (size_t i = 0; i < liberationUnitsTemp.size(); i++) {
        liberationUnits[i] = liberationUnitsTemp[i];
    }
    for (size_t i = 0; i < ARVNUnitsTemp.size(); i++) {
        ARVNUnits[i] = ARVNUnitsTemp[i];
    }
    liberationSize = liberationUnitsTemp.size();
    ARVNSize = ARVNUnitsTemp.size();
    liberationUnitsTemp.clear();
    ARVNUnitsTemp.clear();
}

void Configuration::parseTerrainArray(const string &line, vector<Position*> &array) {
    size_t start = line.find('[');
    size_t end = line.find(']');
    if (start == string::npos || end == string::npos) return;
    string positions = MathUtils::trim(line.substr(start + 1, end - start - 1));
    if (positions.empty()) return;
    size_t pos = 0;
    while (pos < positions.size()) {
        size_t openParen = positions.find('(', pos);
        if (openParen == string::npos) break;
        size_t closeParen = positions.find(')', openParen);
        if (closeParen == string::npos) break;
        string positionStr = MathUtils::trim(positions.substr(openParen, closeParen - openParen + 1));
        if (!positionStr.empty()) {
            try {
                array.push_back(new Position(positionStr));
            } catch (const std::exception& e) {
                cerr << "Position error: " << positionStr << " (" << e.what() << ")" << endl;
            }
        }
        pos = closeParen + 1;
        if (pos < positions.size() && positions[pos] == ',') pos++;
    }
}

Unit* Configuration::createUnit(const string &unitStr) {
    string unitType = MathUtils::trim(unitStr.substr(0, unitStr.find('(')));
    size_t lastComma = unitStr.rfind(',');
    if (lastComma == string::npos) return nullptr;
    string params = MathUtils::trim(unitStr.substr(unitStr.find('(') + 1, lastComma - unitStr.find('(') - 1));
    stringstream ss(params);
    string quantityStr, weightStr, positionStr;
    getline(ss, quantityStr, ',');
    getline(ss, weightStr, ',');
    getline(ss, positionStr);
    try {
        int quantity = stoi(MathUtils::trim(quantityStr));
        int weight = stoi(MathUtils::trim(weightStr));
        Position pos(MathUtils::trim(positionStr));
        if (unitType == "TANK") return new Vehicle(quantity, weight, pos, TANK);
        else if (unitType == "MORTAR") return new Vehicle(quantity, weight, pos, MORTAR);
        else if (unitType == "ANTIAIRCRAFT") return new Vehicle(quantity, weight, pos, ANTIAIRCRAFT);
        else if (unitType == "ARMOREDCAR") return new Vehicle(quantity, weight, pos, ARMOREDCAR);
        else if (unitType == "APC") return new Vehicle(quantity, weight, pos, APC);
        else if (unitType == "ARTILLERY") return new Vehicle(quantity, weight, pos, ARTILLERY);
        else if (unitType == "TRUCK") return new Vehicle(quantity, weight, pos, TRUCK);
        else if (unitType == "SNIPER") return new Infantry(quantity, weight, pos, SNIPER);
        else if (unitType == "ANTIAIRCRAFTSQUAD") return new Infantry(quantity, weight, pos, ANTIAIRCRAFTSQUAD);
        else if (unitType == "MORTARSQUAD") return new Infantry(quantity, weight, pos, MORTARSQUAD);
        else if (unitType == "ENGINEER") return new Infantry(quantity, weight, pos, ENGINEER);
        else if (unitType == "SPECIALFORCES") return new Infantry(quantity, weight, pos, SPECIALFORCES);
        else if (unitType == "REGULARINFANTRY") return new Infantry(quantity, weight, pos, REGULARINFANTRY);
    } catch (const std::exception& e) {
        cerr << "createUnit error: " << unitStr << " (" << e.what() << ")" << endl;
    }
    return nullptr;
}

Configuration::~Configuration() {
    for (Position *pos : arrayForest) {
        delete pos;
    }
    for (Position *pos : arrayRiver) {
        delete pos;
    }
    for (Position *pos : arrayFortification) {
        delete pos;
    }
    for (Position *pos : arrayUrban) {
        delete pos;
    }
    for (Position *pos : arraySpecialZone) {
        delete pos;
    }
    if (liberationUnits != nullptr) {
        for (int i = 0; i < liberationSize; ++i) {
            delete liberationUnits[i];
        }
        delete[] liberationUnits;
    }

    if (ARVNUnits != nullptr) {
        for (int i = 0; i < ARVNSize; ++i) {
            delete ARVNUnits[i]; 
        }
        delete[] ARVNUnits;
    }
}

string Configuration::str() const {
    stringstream ss;
    ss << "[";
    ss << "num_rows=" << num_rows << ",";
    ss << "num_cols=" << num_cols << ",";
    ss << "arrayForest=[";
    for (size_t i = 0; i < arrayForest.size(); ++i) {
        ss << arrayForest[i]->str();
        if (i < arrayForest.size() - 1) {
            ss << ",";
        }
    }
    ss << "],";
    ss << "arrayRiver=[";
    for (size_t i = 0; i < arrayRiver.size(); ++i) {
        ss << arrayRiver[i]->str();
        if (i < arrayRiver.size() - 1) {
            ss << ",";
        }
    }
    ss << "],";
    ss << "arrayFortification=[";
    for (size_t i = 0; i < arrayFortification.size(); ++i) {
        ss << arrayFortification[i]->str();
        if (i < arrayFortification.size() - 1) {
            ss << ",";
        }
    }
    ss << "],";
    ss << "arrayUrban=[";
    for (size_t i = 0; i < arrayUrban.size(); ++i) {
        ss << arrayUrban[i]->str();
        if (i < arrayUrban.size() - 1) {
            ss << ",";
        }
    }
    ss << "],";
    ss << "arraySpecialZone=[";
    for (size_t i = 0; i < arraySpecialZone.size(); ++i) {
        ss << arraySpecialZone[i]->str();
        if (i < arraySpecialZone.size() - 1) {
            ss << ",";
        }
    }
    ss << "],";
    
    ss << "liberationUnits=[";
    for (size_t i = 0; i < liberationSize; ++i) {
        ss << liberationUnits[i]->str();
        if (i < liberationSize - 1) {
            ss << ",";
        }
    }
    ss << "],";

    ss << "ARVNUnits=[";
    for (size_t i = 0; i < ARVNSize; ++i) {
        ss << ARVNUnits[i]->str();
        if (
            i < ARVNSize - 1) {
            ss << ",";
        }
    }
    ss << "],";
    ss << "eventCode=" << eventCode;
    ss << "]";
    return ss.str();
}

//====================== HCMCampaign implementations ======================
HCMCampaign::HCMCampaign(const string &config_file_path) : 
    config(new Configuration(config_file_path)),
    battleField(new BattleField(config->getNumRows(), config->getNumCols(), config->getForestPositions(), config->getRiverPositions(), config->getFortificationPositions(), config->getUrbanPositions(), config->getSpecialZonePositions())),
    liberationArmy(nullptr),
    arvnArmy(nullptr) {
    liberationArmy = new LiberationArmy(config->getLiberationUnits(), config->getLiberationSize(), "Liberation Army", battleField);
    arvnArmy = new ARVN(config->getARVNUnits(), config->getARVNSize(), "ARVN Army", battleField);
}

void HCMCampaign::getTerrainEffect() {
    battleField->getForestEffect(liberationArmy);
    battleField->getRiverEffect(liberationArmy);
    battleField->getFortificationEffect(liberationArmy);
    battleField->getUrbanEffect(liberationArmy);
    battleField->getSpecialZoneEffect(liberationArmy);

    battleField->getForestEffect(arvnArmy);
    battleField->getRiverEffect(arvnArmy);
    battleField->getFortificationEffect(arvnArmy);
    battleField->getUrbanEffect(arvnArmy);
    battleField->getSpecialZoneEffect(arvnArmy);
}

void HCMCampaign::run() {
    getTerrainEffect();
    if (config->getEventCode() < 75) {
        liberationArmy->fight(arvnArmy, false);
        arvnArmy->fight(liberationArmy, true);
        removeUselessUnits();
    } else {
        arvnArmy->fight(liberationArmy, false);
        liberationArmy->fight(arvnArmy, true);
        
        liberationArmy->fight(arvnArmy, false);
        arvnArmy->fight(liberationArmy, true);
        removeUselessUnits();
    }
    liberationArmy->resetScore();
    arvnArmy->resetScore();
}

void HCMCampaign::removeUselessUnits() {
    liberationArmy->getUnitList()->removeUselessUnits(5);
    arvnArmy->getUnitList()->removeUselessUnits(5);
}

string HCMCampaign::printResult() {
    stringstream ss;
    ss << "LIBERATIONARMY[LF=" << liberationArmy->getLF() << ",EXP=" << liberationArmy->getEXP() << "]-";
    ss << "ARVN[LF=" << arvnArmy->getLF() << ",EXP=" << arvnArmy->getEXP() << "]";
    return ss.str();
}

HCMCampaign::~HCMCampaign() {
    delete liberationArmy;
    delete arvnArmy;
    delete battleField;
    delete config;
}


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////