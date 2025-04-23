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

int MathUtils::nextFibo(int n) {
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

//====================== Unit implementations ======================
Unit::~Unit() {}
Unit::Unit(int quantity, int weight, const Position pos) : quantity(quantity), weight(weight), pos(pos) { setAttackScore(); }
Position Unit::getCurrentPosition() const { return pos; }
void Unit::setAttackScore() {
    
}

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

vector<Unit *> Army::getUnit(int target) {
    // Default implementation
    vector<Unit *> result;
    return result;
}
vector<Unit *> UnitList::findCombination(bool isInfantry, int targetScore) {
    vector<Unit *> result;
    // Simple implementation - find units that match the criteria
    Node *p = sentinal->next;
    while (p != nullptr) {
        bool isUnitInfantry = dynamic_cast<Infantry*>(p->unit) != nullptr;
        if (isUnitInfantry == isInfantry && p->unit->getAttackScore() <= targetScore) {
            result.push_back(p->unit);
            targetScore -= p->unit->getAttackScore();
        }
        p = p->next;
    }
    return result;
}
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

void Army::terrainEffect(double factor, int radius, bool isEXP) {
    int row = battleField->n_rows;
    int col = battleField->n_cols;
    int r = 0, c = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
        }
    }
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

// Add with the ARVN implementations
vector<Unit *> ARVN::getUnit(int target) {
    vector<Unit *> result;
    // ARVN-specific implementation
    UnitList::Node *p = unitList->sentinal->next;
    int count = 0;
    while (p != nullptr && count < target) {
        result.push_back(p->unit);
        p = p->next;
        count++;
    }
    return result;
}


string ARVN::str() const {
    return "ARVN[name=" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + ",unitList=" + unitList->str() + "]";
}

//====================== BattleField implementations ======================
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
    vector<Position *> arrayRiver, vector<Position *> arrayFortification,
    vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
    : n_rows(n_rows), n_cols(n_cols) {

    terrain.resize(n_rows);
    for (int i = 0; i < n_rows; i++) {
        terrain[i].resize(n_cols);
        for (int j = 0; j < n_cols; j++) {
            terrain[i][j] = new Road();
        }
    }

    for (int i = 0; i < arrayForest.size(); i++) {
        Position *pos = arrayForest[i];
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];  // Delete default Road
            terrain[r][c] = new Mountain();
        }
    }
    
    for (int i = 0; i < arrayRiver.size(); i++) {
        Position *pos = arrayRiver[i];
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new River();
        }
    }
    
    for (int i = 0; i < arrayFortification.size(); i++) {
        Position *pos = arrayFortification[i];
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new Fortification();
        }
    }
    
    for (int i = 0; i < arrayUrban.size(); i++) {
        Position *pos = arrayUrban[i];
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new Urban();
        }
    }
    
    for (int i = 0; i < arraySpecialZone.size(); i++) {
        Position *pos = arraySpecialZone[i];
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new SpecialZone();
        }
    }
}

BattleField::~BattleField() {
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            delete terrain[i][j];
        }
    }
    terrain.clear();
}

TerrainElement* BattleField::getTerrainAt(int row, int col) const {
    if (row >= 0 && row < n_rows && col >= 0 && col < n_cols) {
        return terrain[row][col];
    }
    return nullptr;
}

// Add these before the implementations of specific terrain elements

TerrainElement::TerrainElement() {}
TerrainElement::~TerrainElement() {}

Road::Road() {}
Road::~Road() {}

Mountain::Mountain() {}
Mountain::~Mountain() {}

River::River() {}
River::~River() {}

Urban::Urban() {}
Urban::~Urban() {}

Fortification::Fortification() {}
Fortification::~Fortification() {}

SpecialZone::SpecialZone() {}
SpecialZone::~SpecialZone() {}

//====================== River implementations ======================

void River::getEffect(Army *army) {
    // For units at River positions
    for (UnitList::Node* node = army->unitList->sentinal->next; node != nullptr; node = node->next) {
        Position pos = node->unit->getCurrentPosition();
        int r = pos.getRow();
        int c = pos.getCol();
        
        // Calculate distance from unit to this River
        double distance = 0;  // In a real implementation, this would use coordinates
        
        // Direct effect at River position
        if (distance == 0) {
            // Units directly on river positions have EXP decreased by 10%
            army->modifyEXP(0.9);
        } 
        // Effect for units within 2 km radius
        else if (distance <= 2) {
            // Units within 2km have minor decrease in EXP
            army->modifyEXP(0.95);
        }
    }
}

//====================== Urban implementations ======================

void Urban::getEffect(Army *army) {
    // For units at Urban positions
    for (UnitList::Node* node = army->unitList->sentinal->next; node != nullptr; node = node->next) {
        Position pos = node->unit->getCurrentPosition();
        int r = pos.getRow();
        int c = pos.getCol();
        
        // Calculate distance from unit to Urban area
        double distance = 0;  // Will be calculated based on battlefield layout
        
        // Direct effect at Urban position
        if (distance == 0) {
            // Units in urban areas get 20% increase in LF
            army->modifyLF(1.2);
        } 
        // Effect for units within 1.5 km radius
        else if (distance <= 1.5) {
            // Units near urban areas get 10% increase in LF
            army->modifyLF(1.1);
        }
    }
}

//====================== Fortification implementations ======================

void Fortification::getEffect(Army *army) {
    // Only ARVN forces benefit from fortifications
    if (dynamic_cast<ARVN*>(army)) {
        for (UnitList::Node* node = army->unitList->sentinal->next; node != nullptr; node = node->next) {
            Position pos = node->unit->getCurrentPosition();
            int r = pos.getRow();
            int c = pos.getCol();
            
            // Calculate distance from unit to fortification
            double distance = 0;  // Will be calculated based on battlefield layout
            
            // Direct effect at fortification position
            if (distance == 0) {
                // ARVN units at fortifications get 30% boost in LF
                army->LF = round(army->LF * 1.3);
            } 
            // Effect for units within 3 km radius
            else if (distance <= 3) {
                // ARVN units near fortifications get 15% boost in LF
                army->LF = round(army->LF * 1.15);
            }
        }
    }
}

//====================== SpecialZone implementations ======================

void SpecialZone::getEffect(Army *army) {
    // Only Liberation Army forces benefit from special zones
    if (dynamic_cast<LiberationArmy*>(army)) {
        for (UnitList::Node* node = army->unitList->sentinal->next; node != nullptr; node = node->next) {
            Position pos = node->unit->getCurrentPosition();
            int r = pos.getRow();
            int c = pos.getCol();
            
            // Calculate distance from unit to special zone
            double distance = 0;  // Will be calculated based on battlefield layout
            
            // Direct effect at special zone position
            if (distance == 0) {
                // Units at special zones get 30% boost in EXP
                army->EXP = round(army->EXP * 1.3);
            } 
            // Effect for units within 2.5 km radius
            else if (distance <= 2.5) {
                // Units near special zones get 15% boost in EXP
                army->EXP = round(army->EXP * 1.15);
            }
        }
    }
}

//====================== Road implementations ======================
void Road::getEffect(Army *army) {
    // For units on or near road areas
    for (UnitList::Node* node = army->unitList->sentinal->next; node != nullptr; node = node->next) {
        Position pos = node->unit->getCurrentPosition();
        int r = pos.getRow();
        int c = pos.getCol();
        
        // Calculate distance from unit to road
        double distance = 0; // Will be calculated based on battlefield layout
        
        // Direct effect on Road
        if (distance == 0) {
            // Units on roads get mobility advantages (no penalty)
            // No direct stat changes
        }
        // Effect for units within 1 km of roads
        else if (distance <= 1) {
            // Slight movement advantage, but no numerical effect
        }
    }
}

//====================== Mountain implementations ======================
void Mountain::getEffect(Army *army) {
    // For units positioned in mountainous terrain
    for (UnitList::Node* node = army->unitList->sentinal->next; node != nullptr; node = node->next) {
        Position pos = node->unit->getCurrentPosition();
        int r = pos.getRow();
        int c = pos.getCol();
        
        // Calculate distance from unit to this Mountain
        double distance = 0;  // Will be calculated based on battlefield layout
        
        // Direct effect at Mountain position
        if (distance == 0) {
            // Units directly in mountains have LF decreased by 20%
            if (dynamic_cast<Vehicle*>(node->unit)) {
                node->unit->attackScore = round(node->unit->attackScore * 0.8);
            }
            army->LF = round(army->LF * 0.8);
        } 
        // Effect for units within 1.8 km radius
        else if (distance <= 1.8) {
            // Units near mountains have LF decreased by 10%
            if (dynamic_cast<Vehicle*>(node->unit)) {
                node->unit->attackScore = round(node->unit->attackScore * 0.9);
            }
            army->LF = round(army->LF * 0.9);
        }
    }
}

//====================== Configuration implementations ======================
Configuration::Configuration(const string &file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + file_path);
    }
    
    string line;
    
    getline(file, line);
    n_rows = stoi(line.substr(line.find('=') + 1));
    
    getline(file, line);
    n_cols = stoi(line.substr(line.find('=') + 1));
    
    getline(file, line);
    arrayForest = parsePositionArray(line.substr(line.find('=') + 1));
    
    getline(file, line);
    arrayRiver = parsePositionArray(line.substr(line.find('=') + 1));
    
    getline(file, line);
    arrayFortification = parsePositionArray(line.substr(line.find('=') + 1));
    
    getline(file, line);
    arrayUrban = parsePositionArray(line.substr(line.find('=') + 1));
    
    getline(file, line);
    arraySpecialZone = parsePositionArray(line.substr(line.find('=') + 1));
    
    getline(file, line);
    parseUnitList(line.substr(line.find('=') + 1));
    
    getline(file, line);
    eventCode = stoi(line.substr(line.find('=') + 1));
    
    file.close();
}

Configuration::~Configuration() {
    for (Position* pos : arrayForest) delete pos;
    for (Position* pos : arrayRiver) delete pos;
    for (Position* pos : arrayFortification) delete pos;
    for (Position* pos : arrayUrban) delete pos;
    for (Position* pos : arraySpecialZone) delete pos;
    
    for (Unit* unit : arrayLiberationArmy) delete unit;
    for (Unit* unit : arrayARVN) delete unit;
}

vector<Position*> Configuration::parsePositionArray(const string &value) {
    vector<Position*> positions;
    int start = value.find('[');
    int end = value.rfind(']');
    
    if (start == string::npos || end == string::npos) {
        return positions;
    }
    
    string posArray = value.substr(start + 1, end - start - 1);
    int index = 0;
    
    while ((index = posArray.find('(', index)) != string::npos) {
        int closeParen = posArray.find(')', index);
        if (closeParen == string::npos) break;
        
        string posStr = posArray.substr(index, closeParen - index + 1);
        positions.push_back(new Position(posStr));
        
        index = closeParen + 1;
    }
    
    return positions;
}

void Configuration::parseUnitList(const string &value) {
    size_t start = value.find('[');
    size_t end = value.rfind(']');
    
    if (start == string::npos || end == string::npos) {
        return;
    }
    
    string unitListStr = value.substr(start + 1, end - start - 1);
    size_t pos = 0;
    
    while (pos < unitListStr.size()) {
        size_t openParen = unitListStr.find('(', pos);
        if (openParen == string::npos) break;
        
        string unitType = unitListStr.substr(pos, openParen - pos);
        
        size_t closeParen = findMatchingParen(unitListStr, openParen);
        if (closeParen == string::npos) break;
        
        string unitData = unitListStr.substr(openParen + 1, closeParen - openParen - 1);
        
        vector<string> params = splitString(unitData, ',');
        if (params.size() >= 4) {
            int quantity = stoi(params[0]);
            int weight = stoi(params[1]);
            
            size_t posStart = params[2].find('(');
            size_t posEnd = params[2].find(')', posStart);
            string posStr = params[2].substr(posStart, posEnd - posStart + 1);
            Position pos(posStr);
            
            int armyId = stoi(params[3]);
            
            Unit* unit = nullptr;
            if (unitType == "TANK") {
                unit = new Vehicle(quantity, weight, pos, TANK);
            } 
            else if (unitType == "MORTAR") {
                unit = new Vehicle(quantity, weight, pos, MORTAR);
            }
            else if (unitType == "REGULARINFANTRY") {
                unit = new Infantry(quantity, weight, pos, REGULARINFANTRY);
            }
            
            if (unit != nullptr) {
                if (armyId == 0) {
                    arrayLiberationArmy.push_back(unit);
                } else {
                    arrayARVN.push_back(unit);
                }
            }
        }
        
        pos = closeParen + 1;
        if (pos < unitListStr.size() && unitListStr[pos] == ',') {
            pos++;
        }
    }
}

size_t Configuration::findMatchingParen(const string &str, size_t openPos) {
    int count = 1;
    for (size_t i = openPos + 1; i < str.size(); i++) {
        if (str[i] == '(') {
            count++;
        } else if (str[i] == ')') {
            count--;
            if (count == 0) {
                return i;
            }
        }
    }
    return string::npos;
}

vector<string> Configuration::splitString(const string &str, char delimiter) {
    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = str.find(delimiter, start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

string Configuration::str() const {
    return "Configuration[n_rows=" + to_string(n_rows) + ",n_cols=" + to_string(n_cols) + 
           ",event_code=" + to_string(eventCode) + "]";
}

//====================== HCMCampaign implementations ======================
HCMCampaign::HCMCampaign(const string &config_file_path) {
    config = new Configuration(config_file_path);
    
    // Create battlefield
    battleField = new BattleField(
        config->getNumRows(), 
        config->getNumCols(), 
        config->getForestPositions(), 
        config->getRiverPositions(), 
        config->getFortificationPositions(), 
        config->getUrbanPositions(), 
        config->getSpecialZonePositions()
    );
    
    // Create Liberation Army
    vector<Unit*> liberationUnits = config->getLiberationArmyUnits();
    Unit** liberationUnitArray = new Unit*[liberationUnits.size()];
    for (size_t i = 0; i < liberationUnits.size(); i++) {
        liberationUnitArray[i] = liberationUnits[i];
    }
    liberationArmy = new LiberationArmy(liberationUnitArray, liberationUnits.size(), "Liberation Army", battleField);
    delete[] liberationUnitArray;
    
    // Create ARVN
    vector<Unit*> arvnUnits = config->getARVNUnits();
    Unit** arvnUnitArray = new Unit*[arvnUnits.size()];
    for (size_t i = 0; i < arvnUnits.size(); i++) {
        arvnUnitArray[i] = arvnUnits[i];
    }
    arvn = new ARVN(arvnUnitArray, arvnUnits.size(), "ARVN", battleField);
    delete[] arvnUnitArray;
}

void HCMCampaign::run() {
    int eventCode = config->getEventCode();
    
    // Apply terrain effects for both armies
    for (int r = 0; r < battleField->n_rows; r++) {
        for (int c = 0; c < battleField->n_cols; c++) {
            TerrainElement* terrain = battleField->getTerrainAt(r, c);
            if (terrain) {
                terrain->getEffect(liberationArmy);
                terrain->getEffect(arvn);
            }
        }
    }
    
    // Execute the battle based on event code
    switch (eventCode) {
        case 0:
            // Liberation Army attacks ARVN
            liberationArmy->fight(arvn, false);
            break;
        case 1:
            // ARVN attacks Liberation Army
            arvn->fight(liberationArmy, false);
            break;
        case 2:
            // Liberation Army defends against ARVN
            liberationArmy->fight(arvn, true);
            break;
        case 3:
            // ARVN defends against Liberation Army
            arvn->fight(liberationArmy, true);
            break;
        default:
            // No action taken for unknown event codes
            break;
    }
}

string HCMCampaign::printResult() {
    string result = "";
    result += liberationArmy->str() + "\n";
    result += arvn->str();
    return result;
}