#include "ActionCard.hpp"
#include <sstream>
ActionCard::ActionCard(std::string name):Card(name){}
std::string ActionCard::toSaveFormat() const {
    std::ostringstream out;
    out << this->getName() << "\n";
    return out.str();
}