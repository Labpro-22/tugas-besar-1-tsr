#include "ViewGame.hpp"
#include "GameManager.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>

const std::string ANSI_RESET   = "\033[0m";
const std::string ANSI_RED     = "\033[1;31m";
const std::string ANSI_GREEN   = "\033[1;32m";
const std::string ANSI_YELLOW  = "\033[1;33m";
const std::string ANSI_BLUE    = "\033[1;34m";
const std::string ANSI_MAGENTA = "\033[1;35m";
const std::string ANSI_CYAN    = "\033[1;36m";
const std::string ANSI_WHITE   = "\033[1;37m";
const std::string ANSI_ORANGE  = "\033[38;5;208m";

std::string padTo(const std::string& str, const size_t num) {
    if (str.length() >= num) return str.substr(0, num);
    return str + std::string(num - str.length(), ' ');
}

std::string getColorCode(const std::string& colorName) {
    if (colorName == "MERAH") return ANSI_RED;
    if (colorName == "HIJAU") return ANSI_GREEN;
    if (colorName == "KUNING") return ANSI_YELLOW;
    if (colorName == "BIRU MUDA") return ANSI_CYAN;
    if (colorName == "BIRU TUA") return ANSI_BLUE;
    if (colorName == "MERAH MUDA" || colorName == "PINK") return ANSI_MAGENTA;
    if (colorName == "ORANGE") return ANSI_ORANGE;
    return ANSI_WHITE;
}

std::string ViewGame::getUserCommand() {
    // Implementation
}

std::string ViewGame::getPlayerName() {
    // Implementation
}

std::string ViewGame::getPropertyName() {
    // Implementation
}

int ViewGame::gettInt(int max_int) {
    // Implementation
}

bool ViewGame::getYesNo() {
    // Implementation
}

void ViewGame::displayMessage(const std::string& message) {
    // Implementation
}

std::string ViewGame::getUserInput() {
    // Implementation
}

void ViewGame::displayBoard(const GameManager& gm) {
    const int CELL_WIDTH = 10;

    auto fetchTileDisplayData = [&](int tileId) -> std::pair<std::string, std::string> {
        auto& currentTile = GameManager::property_manager->getTileAt(tileId);

        std::string code = currentTile.getCode();  
        std::string color = currentTile.getColor();
        std::string propertyStatus = currentTile.getStatusString(); 

        std::string playersOnTile = "";
        
        for (size_t i = 0; i < GameManager::players.size(); ++i) {
            if (GameManager::players[i]->getPosition() == tileId) {
                playersOnTile += "(" + std::to_string(i + 1) + ")"; 
            }
        }

        std::string secondLineText = propertyStatus;
        if (!playersOnTile.empty()) {
            if (!secondLineText.empty()) {
                secondLineText += " ";
            }
            secondLineText += playersOnTile;
        }

        std::string line1 = getColorCode(color) + padTo(code, CELL_WIDTH) + ANSI_RESET;
        std::string line2 = padTo(secondLineText, CELL_WIDTH);

        return {line1, line2};
    };

    auto printBorder = [&](int count) {
        for (int i = 0; i < count; ++i) std::cout << "+----------";
        std::cout << "+\n";
    };

    std::cout << "\n";

    printBorder(11);
    for (int i = 21; i <= 31; ++i) std::cout << "|" << fetchTileDisplayData(i).first;
    std::cout << "|\n";
    for (int i = 21; i <= 31; ++i) std::cout << "|" << fetchTileDisplayData(i).second;
    std::cout << "|\n";
    printBorder(11);

    for (int i = 0; i < 9; ++i) {
        int leftId = 20 - i;
        int rightId = 32 + i;
        
        auto leftTile = fetchTileDisplayData(leftId);
        auto rightTile = fetchTileDisplayData(rightId);

        std::cout << "|" << leftTile.first << "|";
        
        if (i == 1) {
            std::cout << "   ==================================   " << std::string(40, ' ');
        } else if (i == 2) {
            std::cout << "   ||           NIMONSPOLI         ||   " << std::string(40, ' ');
        } else if (i == 3) {
            std::cout << "   ==================================   " << std::string(40, ' ');
        } else if (i == 5) {
            std::string turnInfo = "TURN " + std::to_string(gm.getCurrentTurn()) + " / " + std::to_string(gm.getMaxTurns());
            std::cout << "           " << padTo(turnInfo, 23) << "        " << std::string(40, ' ');
        } else {
            std::cout << std::string(80, ' '); 
        }

        std::cout << "|" << rightTile.first << "|\n";

        std::cout << "|" << leftTile.second << "|";
        std::cout << std::string(80, ' ');
        std::cout << "|" << rightTile.second << "|\n";

        std::cout << "+----------+" << std::string(80, ' ') << "+----------+\n";
    }

    for (int i = 11; i >= 1; --i) std::cout << "|" << fetchTileDisplayData(i).first;
    std::cout << "|\n";
    for (int i = 11; i >= 1; --i) std::cout << "|" << fetchTileDisplayData(i).second;
    std::cout << "|\n";
    printBorder(11);
    std::cout << "\n";
}

void ViewGame::displayDiceRollResult(const std::string& playerName, int dice1, int dice2, const std::string& destTile) {
    int total_dice = dice1 + dice2;
    
    std::cout << "Mengocok dadu...\n";
    std::cout << "Hasil: " << dice1 << " + " << dice2 << " = " << total_dice << "\n";
    std::cout << "Memajukan Bidak " << playerName << " sebanyak " << total_dice << " petak...\n";
    std::cout << "Bidak mendarat di: " << destTile << ".\n";
}

void ViewGame::displayManualDiceRollResult(const std::string& playerName, int dice1, int dice2, const std::string& destTile) {
    int total_dice = dice1 + dice2;
    
    std::cout << "Dadu diatur secara manual.\n";
    std::cout << "Hasil: " << dice1 << " + " << dice2 << " = " << total_dice << "\n";
    std::cout << "Memajukan Bidak " << playerName << " sebanyak " << total_dice << " petak...\n";
    std::cout << "Bidak mendarat di: " << destTile << ".\n";
}

void ViewGame::displayPropertyDeed(const GameManager& gm, const std::string& propertyCode) {
    Tile* target_tile = nullptr;
    Board& board = GameManager::property_manager->getBoard();
    
    for (int i = 0; i < board.getSize(); ++i) {
        Tile& current_tile = GameManager::property_manager->getTileAt(i);
        
        if (current_tile.getCode() == propertyCode) {
            target_tile = &current_tile;
            break; 
        }
    }

    if (target_tile == nullptr) {
        std::cout << "Petak \"" + propertyCode + "\" tidak ditemukan." << std::endl;
        return;
    }

    if (!target_tile->isProperty()) {
        std::cout << "Petak \"" + propertyCode + "\" bukan merupakan properti yang dapat dibeli." << std::endl;
        return;
    }

    PropertyTile* property_tile = static_cast<PropertyTile*>(target_tile);
    
    std::string tile_color = property_tile->getColor();
    std::string tile_name = property_tile->getName();
    std::string tile_code = property_tile->getCode();
    
    std::cout << "\n+================================+\n";
    std::cout << "|        AKTA KEPEMILIKAN        |\n";
    std::cout << "| " << getColorCode(tile_color) << "[" << tile_color << "] " << tile_name << " (" << tile_code << ")" << ANSI_RESET << " |\n";
    std::cout << "+================================+\n";
    std::cout << "| Harga Beli          : M" << property_tile->getBuyPrice() << "\n";
    std::cout << "| Nilai Gadai         : M" << property_tile->getMortgagePrice() << "\n";
    std::cout << "+--------------------------------+\n";

    if (target_tile->isStreet()) {
        StreetTile* street = static_cast<StreetTile*>(target_tile);
        
        const std::vector<int>& rent_list = street->getRentList(); 
        if (rent_list.size() >= 6) {
            std::cout << "| Sewa (unimproved)   : M" << rent_list[0] << "\n";
            std::cout << "| Sewa (1 rumah)      : M" << rent_list[1] << "\n";
            std::cout << "| Sewa (2 rumah)      : M" << rent_list[2] << "\n";
            std::cout << "| Sewa (3 rumah)      : M" << rent_list[3] << "\n";
            std::cout << "| Sewa (4 rumah)      : M" << rent_list[4] << "\n";
            std::cout << "| Sewa (hotel)        : M" << rent_list[5] << "\n";
        }
        std::cout << "+--------------------------------+\n";
        std::cout << "| Harga Rumah         : M" << street->getHousePrice() << "\n";
        std::cout << "| Harga Hotel         : M" << street->getHotelPrice() << "\n";
        std::cout << "+================================+\n";
    }

    std::shared_ptr<Player> owner = property_tile->getPropertyOwner().lock();
    
    if (owner != nullptr) {
        if (property_tile->getPropertyStatus() == PropertyStatus::MORTGAGED) {
            std::cout << "| Status : MORTGAGED (" << owner->getName() << ") |\n";
        } else {
            std::cout << "| Status : OWNED (" << owner->getName() << ")     |\n";
        }
    } else {
        std::cout << "| Status : BANK                  |\n";
    }
    
    std::cout << "+================================+\n";
}

void ViewGame::displayPlayerProperties(const GameManager& gm, const std::string& playerName) {
    std::cout << "\n=== Properti Milik: " << playerName << " ===\n";

    std::map<std::string, std::vector<PropertyTile*>> grouped_properties;
    bool has_property = false;

    for (int i = 0; i < 40; ++i) {
        Tile& current_tile = GameManager::property_manager->getTileAt(i);
        
        if (current_tile.isProperty()) {
            PropertyTile* prop = static_cast<PropertyTile*>(&current_tile);
            std::shared_ptr<Player> owner = prop->getPropertyOwner().lock();
            
            if (owner != nullptr && owner->getName() == playerName) {
                grouped_properties[prop->getColor()].push_back(prop);
                has_property = true;
            }
        }
    }

    if (!has_property) {
        std::cout << playerName << " belum memiliki properti apa pun.\n";
        std::cout << "====================================\n";
        return;
    }

    for (const auto& pair : grouped_properties) {
        const std::string& color = pair.first;
        const std::vector<PropertyTile*>& properties = pair.second;

        std::cout << "\n[" << getColorCode(color) << color << ANSI_RESET << "]\n";
        
        for (PropertyTile* prop : properties) {
            std::cout << " - " << prop->getName() << " (" << prop->getCode() << ")";
            
            if (prop->isStreet()) {
                StreetTile* street = static_cast<StreetTile*>(prop);
                
                int b_level = street->getBuildingLevel(); 
                
                if (b_level == 4) {
                    std::cout << " [1 Hotel]";
                } else if (b_level > 0 && b_level < 4) {
                    std::cout << " [" << b_level << " Rumah]";
                }
            }
            
            if (prop->getPropertyStatus() == PropertyStatus::MORTGAGED) {
                std::cout << " " << ANSI_RED << "(DIGADAIKAN)" << ANSI_RESET;
            }
            
            std::cout << "\n";
        }
    }
    std::cout << "====================================\n";
}

void ViewGame::displayBuyPrompt(const GameManager& gm) {
    const PropertyTile& currentProperty = gm.getCurrentProperty();
    const Player& currentPlayer = gm.getCurrentPlayer();
    std::string prop_name = currentProperty.getName();
    int prop_price = currentProperty.getBuyPrice();
    int player_money = currentPlayer.getBalance();

    std::cout << "\n====================================\n";
    std::cout << "Kamu mendarat di " << prop_name << " yang belum dimiliki siapa pun!\n";
    std::cout << "+================================+\n";
    std::cout << "| Harga Beli : M" << prop_price << "\n";
    std::cout << "+================================+\n";
    std::cout << "Uang kamu saat ini: M" << player_money << "\n";
}

void ViewGame::displayBuyResult(bool success, int remaining_money) {
    if (success) {
        std::cout << ANSI_GREEN << "Selamat! Properti berhasil dibeli." << ANSI_RESET << "\n";
    } else {
        std::cout << ANSI_RED << "Maaf, uang kamu tidak cukup untuk membeli properti ini." << ANSI_RESET << "\n";
    }
    std::cout << "Sisa uang kamu: M" << remaining_money << "\n";
}

void ViewGame::displayRentPayment(const PropertyTile& property, const Player& victim, const Player& owner, int rent_amount) {
    int victim_after = victim.getBalance();
    int victim_before = victim_after - rent_amount;
    int owner_after = owner.getBalance();
    int owner_before = owner_after + rent_amount;

    std::string condition = "Tanah/Fasilitas";
    if (property.isStreet()) {
        const StreetTile* street = static_cast<const StreetTile*>(&property);
        int b_level = street->getBuildingLevel();
        if (b_level == 4) condition = "1 Hotel";
        else if (b_level > 0) condition = std::to_string(b_level) + " rumah";
    }

    std::cout << "Kamu mendarat di " << property.getName() << " (" << property.getCode() << "), milik " << owner.getName() << "!\n\n";
    std::cout << "Kondisi      : " << condition << "\n";
    std::cout << "Sewa         : M" << rent_amount << "\n\n";
    std::cout << "Uang kamu    : M" << victim_before << " -> M" << victim_after << "\n";
    std::cout << "Uang " << owner.getName() << " : M" << owner_before << " -> M" << owner_after << "\n";
}

void ViewGame::displayMortgagedRent(const PropertyTile& property, const Player& owner) {
    std::cout << "Kamu mendarat di " << property.getName() << " (" << property.getCode() << "), milik " << owner.getName() << ".\n";
    std::cout << "Properti ini sedang digadaikan [M]. Tidak ada sewa yang dikenakan.\n";
}

void ViewGame::displayRentBankruptcyWarning(int rent_amount, int current_money) {
    std::cout << ANSI_RED << "Kamu tidak mampu membayar sewa penuh! (M" << rent_amount << ")" << ANSI_RESET << "\n";
    std::cout << "Uang kamu saat ini: M" << current_money << "\n";
}

void ViewGame::displayPphPrompt() {
    std::cout << "Kamu mendarat di Pajak Penghasilan (PPH)!\n";
    std::cout << "Pilih opsi pembayaran pajak:\n";
    std::cout << "1. Bayar flat M150\n";
    std::cout << "2. Bayar 10% dari total kekayaan\n";
    std::cout << "(Pilih sebelum menghitung kekayaan!)\n";
}

void ViewGame::displayPphPercentageResult(const Player& player, int prop_value, int build_value, int tax_amount) {
    int money_after = player.getBalance();
    int money_before = money_after + tax_amount;
    int total_wealth = money_before + prop_value + build_value;
    
    std::cout << "\nTotal kekayaan kamu:\n";
    std::cout << "- Uang tunai          : M" << money_before << "\n";
    std::cout << "- Harga beli properti : M" << prop_value << " (termasuk yang digadaikan)\n";
    std::cout << "- Harga beli bangunan : M" << build_value << "\n";
    std::cout << "Total                 : M" << total_wealth << "\n";
    std::cout << "Pajak 10%             : M" << tax_amount << "\n";
    std::cout << "Uang kamu: M" << money_before << " -> M" << money_after << "\n";
}

void ViewGame::displayTaxSuccess(const std::string& tax_name, const Player& player, int tax_amount) {
    int money_after = player.getBalance();
    int money_before = money_after + tax_amount;

    if (tax_name == "PBM") {
        std::cout << "Kamu mendarat di Pajak Barang Mewah (PBM)!\n";
        std::cout << "Pajak sebesar M" << tax_amount << " langsung dipotong.\n";
    }
    std::cout << "Uang kamu: M" << money_before << " -> M" << money_after << "\n";
}

void ViewGame::displayTaxBankruptcyWarning(const std::string& tax_name, int tax_amount, int current_money) {
    if (tax_name == "PBM") {
        std::cout << "Kamu mendarat di Pajak Barang Mewah (PBM)!\n";
        std::cout << "Pajak sebesar M" << tax_amount << " langsung dipotong.\n";
    }
    std::string flat_text = (tax_name == "PPH") ? " flat M" + std::to_string(tax_amount) : "";
    std::cout << ANSI_RED << "Kamu tidak mampu membayar pajak" << flat_text << "!" << ANSI_RESET << "\n";
    std::cout << "Uang kamu saat ini: M" << current_money << "\n";
}

void ViewGame::displayMortgageList(const std::vector<PropertyTile*>& properties) {
    if (properties.empty()) {
        displayNoMortgageableError();
        return;
    }

    std::cout << "=== Properti yang Dapat Digadaikan ===\n";
    for (size_t i = 0; i < properties.size(); ++i) {
        PropertyTile* prop = properties[i];
        std::cout << i + 1 << ". " << prop->getName() << " (" << prop->getCode() << ") [" << prop->getColor() << "] ";
        std::cout << "Nilai Gadai: M" << prop->getMortgagePrice() << "\n";
    }
    std::cout << "\n";
}

void ViewGame::displayBuildingSellList(const std::string& color_group, const std::vector<StreetTile*>& streets) {
    std::cout << ANSI_YELLOW << "Properti tidak dapat digadaikan!" << ANSI_RESET << "\n";
    std::cout << "Masih terdapat bangunan di color group [" << color_group << "].\n";
    std::cout << "Bangunan harus dijual terlebih dahulu.\n\n";
    std::cout << "Daftar bangunan di color group [" << color_group << "]:\n";

    for (size_t i = 0; i < streets.size(); ++i) {
        StreetTile* street = streets[i];
        int b_level = street->getBuildingLevel();
        std::string b_text = (b_level == 4) ? "1 hotel" : std::to_string(b_level) + " rumah";

        int sell_value = (b_level == 4) ? (street->getHotelPrice() / 2) : (b_level * (street->getHousePrice() / 2));
        
        std::cout << i + 1 << ". " << street->getName() << " (" << street->getCode() << ") - " << b_text;
        std::cout << " -> Nilai jual bangunan: M" << sell_value << "\n";
    }
    std::cout << "\n";
}

void ViewGame::displayBuildingSellSuccess(const std::string& prop_name, int sell_gain) {
    std::cout << "Bangunan " << prop_name << " terjual. Kamu menerima M" << sell_gain << ".\n";
}

void ViewGame::displayMortgageSuccess(const std::string& prop_name, int gain_amount, int current_balance) {
    std::cout << ANSI_GREEN << prop_name << " berhasil digadaikan." << ANSI_RESET << "\n";
    std::cout << "Kamu menerima M" << gain_amount << " dari Bank.\n";
    std::cout << "Uang kamu sekarang: M" << current_balance << "\n";
    std::cout << "Catatan: Sewa tidak dapat dipungut dari properti yang digadaikan.\n";
}

void ViewGame::displayNoMortgageableError() {
    std::cout << "Tidak ada properti yang dapat digadaikan saat ini.\n";
}

void ViewGame::displayUnmortgageList(const std::vector<PropertyTile*>& properties, int currentBalance) {
    if (properties.empty()) {
        displayNoMortgagedProperties();
        return;
    }

    std::cout << "\n=== Properti yang Sedang Digadaikan ===\n";
    for (size_t i = 0; i < properties.size(); ++i) {
        PropertyTile* prop = properties[i];
        std::cout << i + 1 << ". " << prop->getName() << " (" << prop->getCode() << ") ";
        std::cout << "[" << prop->getColor() << "] [M] ";
        std::cout << "Harga Tebus: M" << prop->getBuyPrice() << "\n";
    }

    std::cout << "\nUang kamu saat ini: M" << currentBalance << "\n";
}

void ViewGame::displayUnmortgageSuccess(const std::string& prop_name, int cost, int current_balance) {
    std::cout << "\n" << ANSI_GREEN << prop_name << " berhasil ditebus!" << ANSI_RESET << "\n";
    std::cout << "Kamu membayar M" << cost << " ke Bank.\n";
    std::cout << "Uang kamu sekarang: M" << current_balance << "\n";
}

void ViewGame::displayUnmortgageFailure(const std::string& prop_name, int cost, int current_balance) {
    std::cout << "\n" << ANSI_RED << "Uang kamu tidak cukup untuk menebus " << prop_name << "." << ANSI_RESET << "\n";
    std::cout << "Harga tebus: M" << cost << " | Uang kamu: M" << current_balance << "\n";
}

void ViewGame::displayNoMortgagedProperties() {
    std::cout << "\nTidak ada properti yang sedang digadaikan.\n";
}