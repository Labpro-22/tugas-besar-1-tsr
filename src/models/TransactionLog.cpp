#include "../../include/core/TransactionLog.hpp"
#include "../../include/utils/SaveData.hpp"
#include <sstream>


const char* actionStrings[] = {
    "DADU", 
    "DOUBLE", 
    "SEWA", 
    "BELI", 
    "RAILROAD", 
    "UTILITY", 
    "KARTU", 
    "FESTIVAL", 
    "GOJAIL", 
    "LELANG", 
    "GADAI",  
    "PAJAK",      
    "UNMORTGAGE",     
    "BANKRUPT",   
    "KESEMPATAN",     
    "DANA_UMUM",  
    "WIN",    
    "BUILD_HOUSE", 
    "BUILD_HOTEL", 
    "SAVE", 
    "LOAD"
};

void LogEntry::setDesc(std::string desc){
    description=desc;
}
int LogEntry::getTurnNumber()const{
    return turn_number;
}
std::string LogEntry::getUsername()const{
    return username;
}
std::string LogEntry::getDesc()const{
    return description;
}
actions LogEntry::getActionType()const{
    return action_type;
}
std::string LogEntry::printFormat()const{
    std::string temp;
    std::string enumstr=getTextForEnum(getActionType());
    for(int i=0;i<11-enumstr.length();i++){
        temp+=" ";
    }
    return "[Turn "+std::to_string(getTurnNumber())+"] "+getUsername()+" | "+enumstr+temp+" | "+getDesc();
}

void TransactionLog::recordEvent(LogEntry entry){
    logs.push_back(entry);
}
std::string TransactionLog::getAllLogs()const{
    if(logs.empty()) return "";
    std::string temp;
    for(auto it=logs.begin();it<logs.end();it++){
        temp+=it->printFormat();
        if(it+1!=logs.end()){
            temp+="\n";
        }
    }
    return temp;
}
std::string TransactionLog::getRecentLogs(int n)const{
    if(n<=0) return "";
    std::string temp;
    int size = logs.size();
    int start_idx = std::max(0, size - n);
    for(int i=start_idx;i<size;i++){
        temp+=logs.at(i).printFormat();
        if(i+1!=logs.size()){
            temp+="\n";
        }
    }
    return temp;
}
void TransactionLog::clearLogs(){
    logs.clear();
}

static actions parseActionType(const std::string& actionType) {
    if (actionType == "DADU") return DADU;
    if (actionType == "DOUBLE") return DOUBLE;
    if (actionType == "SEWA") return SEWA;
    if (actionType == "BELI") return BELI;
    if (actionType == "RAILROAD") return RAILROAD;
    if (actionType == "UTILITY") return UTILITY;
    if (actionType == "KARTU") return KARTU;
    if (actionType == "FESTIVAL") return FESTIVAL;
    if (actionType == "GOJAIL") return GOJAIL;
    if (actionType == "LELANG") return LELANG;
    if (actionType == "GADAI") return GADAI;
    if (actionType == "PAJAK") return PAJAK;
    if (actionType == "UNMORTGAGE") return UNMORTGAGE;
    if (actionType == "BANKRUPT") return BANKRUPT;
    if (actionType == "KESEMPATAN") return KESEMPATAN;
    if (actionType == "DANA_UMUM") return DANA_UMUM;
    if (actionType == "WIN") return WIN;
    if (actionType == "BUILD_HOUSE") return BUILD_HOUSE;
    if (actionType == "BUILD_HOTEL") return BUILD_HOTEL;
    if (actionType == "SAVE") return SAVE;
    return LOAD;
}

void TransactionLog::loadLogState(const std::vector<LogSaveData>& data){
    clearLogs();
    for (const auto& savedLog : data) {
        logs.emplace_back(savedLog.turn, savedLog.username, parseActionType(savedLog.action_type), savedLog.detail);
    }
}
std::string LogEntry::toSaveFormat() const {
    std::ostringstream out;
    // <TURN> <USERNAME> <JENIS_AKSI> <DETAIL>
    out << turn_number << " " 
        << username << " " 
        << getTextForEnum(getActionType()) << " " 
        << description << "\n";
        
    return out.str();
}
std::string TransactionLog::toSaveFormat() const {
    std::ostringstream out;
    
    // <JUMLAH_ENTRI_LOG>
    out << logs.size() << "\n";
    
    for (const auto& log : logs) {
        out << log.toSaveFormat();
    }
    
    return out.str();
}