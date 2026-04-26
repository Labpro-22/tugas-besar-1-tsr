#include "../../include/views/ViewGame.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

const std::string ANSI_RESET   = "\033[0m";
const std::string ANSI_RED     = "\033[1;31m";
const std::string ANSI_GREEN   = "\033[1;32m";
const std::string ANSI_YELLOW  = "\033[1;33m";
const std::string ANSI_BLUE    = "\033[1;34m";
const std::string ANSI_MAGENTA = "\033[1;35m";
const std::string ANSI_CYAN    = "\033[1;36m";
const std::string ANSI_WHITE   = "\033[1;37m";
const std::string ANSI_ORANGE  = "\033[38;5;208m";
const std::string ANSI_BROWN   = "\033[38;5;130m"; 
const std::string ANSI_GRAY    = "\033[1;30m";

void ViewGame::displayException(const GameException& e) {
    std::cout << ANSI_RED << e.what() << ANSI_RESET << "\n";
}

    std::string padTo(const std::string& str, const size_t num) {
        if (str.length() >= num) return str.substr(0, num);
        return str + std::string(num - str.length(), ' ');
    }

    std::string getColorCode(const std::string& colorName) {
        if (colorName == "MERAH") return ANSI_RED;
        if (colorName == "HIJAU") return ANSI_GREEN;
        if (colorName == "KUNING") return ANSI_YELLOW;
        if (colorName == "BIRU_MUDA") return ANSI_CYAN;
        if (colorName == "BIRU_TUA") return ANSI_BLUE;
        if (colorName == "MERAH_MUDA") return ANSI_MAGENTA;
        if (colorName == "ORANGE") return ANSI_ORANGE;
        if (colorName == "COKLAT") return ANSI_BROWN;
        if (colorName == "ABU_ABU") return ANSI_GRAY;
        return ANSI_WHITE;
    }

std::string ViewGame::getUserCommand() {
    std::vector<std::string> valid_commands = {
        "CETAK_PAPAN", "LEMPAR_DADU", "ATUR_DADU", "CETAK_AKTA", 
        "CETAK_PROPERTI", "GADAI", "TEBUS", "BANGUN", "SIMPAN", 
        "MUAT", "CETAK_LOG", "GUNAKAN_KEMAMPUAN"
    };

    std::string input;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin >> std::ws, input);

        std::stringstream ss(input);
        std::string base_cmd;
        ss >> base_cmd;

        bool is_valid = false;
        for (const std::string& cmd : valid_commands) {
            if (base_cmd == cmd) {
                is_valid = true;
                break;
            }
        }

        if (is_valid) {
            return input;
        } else {
            std::cout << ANSI_RED << "Perintah tidak dikenali! Silakan masukkan perintah yang valid." << ANSI_RESET << "\n";
        }
    }
}

std::string ViewGame::getPlayerName() {
    std::string input;
    std::cout << "Masukkan nama pemain: ";
    std::getline(std::cin >> std::ws, input);
    return input;
}

std::string ViewGame::getPropertyName() {
    std::string input;
    std::cout << "Masukkan nama properti: ";
    std::getline(std::cin >> std::ws, input);
    return input;
}

int ViewGame::getInt(int max_int) {
    int value;
    while (true) {
        if (std::cin >> value) {
            if (value >= 0 && value <= max_int) {
                std::cin.ignore(10000, '\n');
                return value;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cout << ANSI_RED << "Input tidak valid. Masukkan angka 0 - " << max_int << ": " << ANSI_RESET;
    }
}

bool ViewGame::getYesNo() {
    std::string input;
    while (true) {
        std::cin >> input;
        if (input == "y") {
            std::cin.ignore(10000, '\n');
            return true;
        }
        if (input == "n") {
            std::cin.ignore(10000, '\n');
            return false;
        }
        std::cout << ANSI_RED << "Input tidak valid, masukkan 'y' atau 'n': " << ANSI_RESET;
    }
}

void ViewGame::displayMessage(const std::string& message) {
    std::cout << message << "\n";
}

std::string ViewGame::getUserInput() {
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

void ViewGame::displayBoard() {
    const int CELL_WIDTH = 10;
    const int COLS       = 11;
    const int TOTAL      = COLS * (CELL_WIDTH + 1) + 1;
    const int MID_WIDTH  = TOTAL - 2 * (CELL_WIDTH + 1) - 2;

    auto pad = [](const std::string& s, int n) -> std::string {
        if ((int)s.size() >= n) return s.substr(0, n);
        return s + std::string(n - s.size(), ' ');
    };

    auto padA = [](const std::string& s, int visibleWidth, int ansiLen) -> std::string {
        int visible = (int)s.size() - ansiLen;
        if (visible >= visibleWidth) return s;
        return s + std::string(visibleWidth - visible, ' ');
    };

    auto centerStr = [&](const std::string& s) -> std::string {
        if ((int)s.size() >= MID_WIDTH) return s.substr(0, MID_WIDTH);
        int left  = (MID_WIDTH - (int)s.size()) / 2;
        int right = MID_WIDTH - left - (int)s.size();
        return std::string(left, ' ') + s + std::string(right, ' ');
    };

    auto fetchTileDisplayData = [&](int tileId) -> std::tuple<std::string, std::string, std::string> {
        auto& currentTile      = GameManager::property_manager->getTileAt(tileId);
        std::string code       = currentTile.getCode();
        std::string color      = currentTile.getColor();
        std::string propStatus = currentTile.getStatusString();

        std::string playersOnTile = "";
        for (size_t i = 0; i < GameManager::players.size(); ++i) {
            if (GameManager::players[i]->getPosition() == tileId)
                playersOnTile += "(" + std::to_string(i + 1) + ")";
        }

        std::string cc      = getColorCode(color);
        int         ansiLen = (int)cc.size() + (int)ANSI_RESET.size();

        std::string line1 = padA(cc + pad(code, CELL_WIDTH) + ANSI_RESET, CELL_WIDTH, ansiLen);
        std::string line2 = pad(propStatus, CELL_WIDTH);
        std::string line3 = pad(playersOnTile, CELL_WIDTH);

        return {line1, line2, line3};
    };

    auto fullBorder = [&]() {
        for (int i = 0; i < COLS; i++)
            std::cout << "+" << std::string(CELL_WIDTH, '-');
        std::cout << "+\n";
    };

    auto borderRow = [&]() {
        std::cout << "+" << std::string(CELL_WIDTH, '-')
                  << "+" << std::string(MID_WIDTH,  '-')
                  << "+" << std::string(CELL_WIDTH, '-')
                  << "+\n";
    };

    std::string playerCount = std::to_string(GameManager::players.size());
    std::string turnInfo    = "TURN " + std::to_string(GameManager::getCurrentTurn())
                            + " / "   + std::to_string(GameManager::getMaxTurns());

    std::cout << "\n";

    fullBorder();
    for (int i = 21; i <= 31; ++i) std::cout << "|" << std::get<0>(fetchTileDisplayData(i));
    std::cout << "|\n";
    for (int i = 21; i <= 31; ++i) std::cout << "|" << std::get<1>(fetchTileDisplayData(i));
    std::cout << "|\n";
    for (int i = 21; i <= 31; ++i) std::cout << "|" << std::get<2>(fetchTileDisplayData(i));
    std::cout << "|\n";
    fullBorder();

    for (int i = 0; i < 9; ++i) {
        auto [l1, l2, l3] = fetchTileDisplayData(20 - i);
        auto [r1, r2, r3] = fetchTileDisplayData(32 + i);

        std::string m1(MID_WIDTH, ' ');
        std::string m2(MID_WIDTH, ' ');
        std::string m3(MID_WIDTH, ' ');

        // Judul
        if (i == 0) m1 = centerStr("==================================");
        if (i == 1) m1 = centerStr("||         NIMONSPOLI          ||");
        if (i == 2) m1 = centerStr("==================================");
        // Turn
        if (i == 3) m1 = centerStr(turnInfo);
        // Legenda
        if (i == 4) m1 = centerStr("P1-P" + playerCount + " : Properti milik Pemain ke-N");
        if (i == 4) m2 = centerStr("^ = Lv1   ^^ = Lv2   ^^^ = Lv3   * = Hotel");
        if (i == 5) m1 = centerStr("(1)-(N) : Bidak Pemain ke-N");
        if (i == 6) m1 = centerStr("[CK]=Coklat  [MR]=Merah    [BM]=Biru Muda  [KN]=Kuning");
        if (i == 7) m1 = centerStr("[PK]=Pink    [HJ]=Hijau    [OR]=Orange     [BT]=Biru Tua");
        if (i == 8) m1 = centerStr("[DF]=Aksi    [AB]=Utilitas");

        std::cout << "|" << l1 << "|" << m1 << "|" << r1 << "|\n";
        std::cout << "|" << l2 << "|" << m2 << "|" << r2 << "|\n";
        std::cout << "|" << l3 << "|" << m3 << "|" << r3 << "|\n";
        borderRow();
    }

    for (int i = 11; i >= 1; --i) std::cout << "|" << std::get<0>(fetchTileDisplayData(i));
    std::cout << "|\n";
    for (int i = 11; i >= 1; --i) std::cout << "|" << std::get<1>(fetchTileDisplayData(i));
    std::cout << "|\n";
    for (int i = 11; i >= 1; --i) std::cout << "|" << std::get<2>(fetchTileDisplayData(i));
    std::cout << "|\n";
    fullBorder();

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

void ViewGame::displayPropertyDeed(const std::string& propertyCode) {
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
        std::cout << "Petak \"" + propertyCode + "\" tidak ditemukan.\n";
        return;
    }

    if (!target_tile->isProperty()) {
        std::cout << "Petak \"" + propertyCode + "\" bukan merupakan properti yang dapat dibeli.\n";
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
    std::cout << "| Nilai Gadai         : M" << property_tile->getMortgageValue() << "\n";
    std::cout << "+--------------------------------+\n";

    if (target_tile->isStreet()) {
        StreetTile* street = static_cast<StreetTile*>(target_tile);
        
        const std::vector<float>& rent_list = street->getRentList(); 
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

void ViewGame::displayPlayerProperties(const std::string& playerName) {
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
    std::cout << "2. Bayar 10\% dari total kekayaan\n";
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
        std::cout << "Nilai Gadai: M" << prop->getMortgageValue() << "\n";
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

void ViewGame::displayBuyPromptStreet(const std::string& prop_name, const std::string& prop_code, const std::string& color, int price, int rent, int current_money) {
    std::cout << "\nKamu mendarat di " << prop_name << " (" << prop_code << ")!\n";
    std::cout << "+================================+\n";
    std::cout << "| [" << color << "] " << prop_name << " (" << prop_code << ")\n";
    std::cout << "| Harga Beli    : M" << price << "\n";
    std::cout << "| Sewa dasar    : M" << rent << "\n";
    std::cout << "| ...                            |\n";
    std::cout << "+================================+\n";
    std::cout << "Uang kamu saat ini: M" << current_money << "\n";
}

void ViewGame::displayBuySuccess(const std::string& prop_name, int remaining_money) {
    std::cout << ANSI_GREEN << prop_name << " kini menjadi milikmu!" << ANSI_RESET << "\n";
    std::cout << "Uang tersisa: M" << remaining_money << "\n";
}

void ViewGame::displayBuyAuctionWarning() {
    std::cout << ANSI_YELLOW << "Properti ini akan masuk ke sistem lelang..." << ANSI_RESET << "\n";
}

void ViewGame::displayBuyAutoSuccess(const std::string& type, const std::string& prop_name) {
    std::cout << "\nKamu mendarat di " << prop_name << "!\n";
    std::cout << ANSI_GREEN << "Belum ada yang menginjaknya duluan, " << type << " ini kini menjadi milikmu!" << ANSI_RESET << "\n";
}

void ViewGame::displayBuildableGroups(const std::map<std::string, std::vector<StreetTile*>>& groups, int current_money) {
    std::cout << "\n=== Color Group yang Memenuhi Syarat ===\n";
    int index = 1;
    for (auto const& pair : groups) {
        std::cout << index++ << ". [" << pair.first << "]\n";
        for (const auto* street : pair.second) {
            int level = street->getBuildingLevel();
            std::string level_str = (level == 4) ? "Hotel" : std::to_string(level) + " rumah";
            int build_price = (level == 4) ? street->getHotelPrice() : street->getHousePrice();
            std::cout << "   - " << street->getName() << " (" << street->getCode() << ") : ";
            std::cout << level_str << " (Harga bangun: M" << build_price << ")\n";
        }
    }
    std::cout << "\nUang kamu saat ini : M" << current_money << "\n";
}

void ViewGame::displayBuildableTilesInGroup(const std::string& color_group, const std::vector<StreetTile*>& group_streets, const std::vector<StreetTile*>& eligible_streets) {
    std::cout << "\nColor group [" << color_group << "]:\n";
    for (const auto* street : group_streets) {
        int level = street->getBuildingLevel();
        std::string level_str = (level == 4) ? "Hotel" : std::to_string(level) + " rumah";
        
        bool is_eligible = false;
        for (const auto* eligible : eligible_streets) {
            if (eligible->getCode() == street->getCode()) {
                is_eligible = true; break;
            }
        }
        
        std::cout << "- " << street->getName() << " (" << street->getCode() << ") : " << level_str;
        if (level == 5) {
            std::cout << "  <- sudah maksimal, tidak dapat dibangun\n";
        } else if (level == 4 && is_eligible) {
            std::cout << "  <- siap upgrade ke hotel\n";
        } else if (is_eligible) {
            std::cout << "  <- dapat dibangun\n";
        } else {
            std::cout << "\n";
        }
    }
}

void ViewGame::displayBuildSuccess(const std::string& prop_name, int cost, int remaining_money) {
    std::cout << "Kamu membangun 1 rumah di " << prop_name << ". Biaya: M" << cost << "\n";
    std::cout << "Uang tersisa: M" << remaining_money << "\n";
}

void ViewGame::displayUpgradeHotelPrompt(const std::string& color_group, const std::vector<StreetTile*>& group_streets) {
    std::cout << "\nSeluruh color group [" << color_group << "] sudah memiliki 4 rumah. Siap di-upgrade ke hotel!\n";
}

void ViewGame::displayUpgradeHotelSuccess(const std::string& prop_name, int cost, int remaining_money) {
    std::cout << ANSI_GREEN << prop_name << " di-upgrade ke Hotel!" << ANSI_RESET << "\n";
    std::cout << "Uang tersisa: M" << remaining_money << "\n";
}

void ViewGame::displayHotelMaxError(const std::string& prop_name) {
    std::cout << ANSI_YELLOW << prop_name << " sudah memiliki Hotel dan tidak bisa dibangun lagi." << ANSI_RESET << "\n";
}

void ViewGame::displayNoBuildableError() {
    std::cout << "Tidak ada color group yang memenuhi syarat untuk dibangun.\n";
    std::cout << "Kamu harus memiliki seluruh petak dalam satu color group terlebih dahulu.\n";
}

void ViewGame::displayAuctionStart(const std::string& prop_name, const std::string& next_player_name) {
    std::cout << "\nProperti " << prop_name << " akan dilelang!\n";
    std::cout << "Urutan lelang dimulai dari pemain setelah " << next_player_name << ".\n";
}

void ViewGame::displayAuctionTurn(const std::string& player_name, int highest_bid, const std::string& highest_bidder) {
    std::cout << "\nGiliran: " << player_name << "\n";
    if (highest_bid > 0) {
        std::cout << "Penawaran tertinggi: M" << highest_bid << " (" << highest_bidder << ")\n";
    }
}

void ViewGame::displayAuctionEnd(const std::string& prop_name, const std::string& winner_name, int final_price) {
    std::cout << "\nLelang selesai!\n";
    std::cout << "Pemenang: " << winner_name << "\n";
    std::cout << "Harga akhir: M" << final_price << "\n\n";
    std::cout << "Properti " << prop_name << " kini dimiliki " << winner_name << ".\n";
}

void ViewGame::displayFestivalStart(const std::vector<PropertyTile*>& owned_props) {
    std::cout << "\nKamu mendarat di petak Festival!\n\n";
    std::cout << "Daftar properti milikmu:\n";
    for (const auto* prop : owned_props) {
        std::cout << "- " << prop->getCode() << " (" << prop->getName() << ")\n";
    }
}

void ViewGame::displayFestivalEffect(int old_rent, int new_rent, int duration) {
    std::cout << "\nEfek festival aktif!\n\n";
    std::cout << "Sewa awal: M" << old_rent << "\n";
    std::cout << "Sewa sekarang: M" << new_rent << "\n";
    std::cout << "Durasi: " << duration << " giliran\n";
}

void ViewGame::displayFestivalEnhanced(int old_rent, int new_rent, int duration) {
    std::cout << "\nEfek diperkuat!\n\n";
    std::cout << "Sewa sebelumnya: M" << old_rent << "\n";
    std::cout << "Sewa sekarang: M" << new_rent << "\n";
    std::cout << "Durasi di-reset menjadi: " << duration << " giliran\n";
}

void ViewGame::displayFestivalMaxed(int duration) {
    std::cout << "\nEfek sudah maksimum (harga sewa sudah digandakan tiga kali)\n\n";
    std::cout << "Durasi di-reset menjadi: " << duration << " giliran\n";
}

void ViewGame::displayFestivalErrorInvalidCode() {
    std::cout << ANSI_RED << "-> Kode properti tidak valid!" << ANSI_RESET << "\n";
}

void ViewGame::displayFestivalErrorNotOwned() {
    std::cout << ANSI_RED << "-> Properti bukan milikmu!" << ANSI_RESET << "\n";
}

void ViewGame::displayLiquidationWarning(int current_money, int total_debt, int deficit, int max_potential, const std::string& creditor_name) {
    std::cout << "\nKamu tidak dapat membayar kewajiban M" << total_debt << " kepada " << creditor_name << "!\n\n";
    std::cout << "Uang kamu       : M" << current_money << "\n";
    std::cout << "Total kewajiban : M" << total_debt << "\n";
    std::cout << "Kekurangan      : M" << deficit << "\n\n";
    std::cout << "Estimasi dana maksimum dari likuidasi: M" << max_potential << "\n";
}

void ViewGame::displayLiquidationPanel(int current_money, int debt, const std::vector<StreetTile*>& sellable_buildings, const std::vector<PropertyTile*>& mortgageables) {
    std::cout << "\n=== Panel Likuidasi ===\n";
    std::cout << "Uang kamu saat ini: M" << current_money << "  |  Kewajiban: M" << debt << "\n\n";
    
    int index = 1;
    
    if (!sellable_buildings.empty()) {
        std::cout << "[Jual Bangunan]\n";
        for (const auto* street : sellable_buildings) {
            int b_lvl = street->getBuildingLevel();
            std::string b_txt = (b_lvl == 4) ? "Hotel" : std::to_string(b_lvl) + " rumah";
            int sell_val = (b_lvl == 4) ? (street->getHotelPrice() / 2) : (b_lvl * (street->getHousePrice() / 2));
            
            std::cout << index++ << ". " << street->getName() << " (" << street->getCode() << ") [" << street->getColor() << "] ";
            std::cout << "Harga Jual Bangunan: M" << sell_val << " (" << b_txt << ")\n";
        }
        std::cout << "\n";
    }

    if (!mortgageables.empty()) {
        std::cout << "[Gadaikan Properti]\n";
        for (const auto* prop : mortgageables) {
            std::cout << index++ << ". " << prop->getName() << " (" << prop->getCode() << ") [" << prop->getColor() << "] ";
            std::cout << "Nilai Gadai: M" << prop->getMortgageValue() << "\n";
        }
    }
}

void ViewGame::displayLiquidationSellSuccess(const std::string& prop_name, int gain, int current_money) {
    std::cout << prop_name << " dilikuidasi. Kamu menerima M" << gain << ".\n";
    std::cout << "Uang kamu sekarang: M" << current_money << "\n";
}

void ViewGame::displayLiquidationDebtCleared(int debt, const std::string& creditor_name, int remaining_money, int creditor_before, int creditor_after) {
    std::cout << "\n" << ANSI_GREEN << "Kewajiban M" << debt << " terpenuhi. Membayar ke " << creditor_name << "..." << ANSI_RESET << "\n";
    std::cout << "Uang kamu : M" << remaining_money + debt << " -> M" << remaining_money << "\n";
    std::cout << "Uang " << creditor_name << ": M" << creditor_before << " -> M" << creditor_after << "\n";
}

void ViewGame::displayBankruptcyToPlayer(const std::string& bankrupt_player, const std::string& creditor, int remaining_money, int total_assets, int debt) {
    std::cout << "\nTotal aset + uang tunai : M" << total_assets << "\n";
    std::cout << "Tidak cukup untuk menutup kewajiban M" << debt << ".\n\n";
    std::cout << ANSI_RED << bankrupt_player << " dinyatakan BANGKRUT!" << ANSI_RESET << "\n";
    std::cout << "Kreditor: " << creditor << "\n\n";
    
    std::cout << "Pengalihan aset ke " << creditor << ":\n";
    std::cout << "  - Uang tunai sisa : M" << remaining_money << "\n";
    std::cout << "  - Seluruh properti beserta status bangunan dan gadainya.\n\n";
    
    std::cout << creditor << " menerima semua aset " << bankrupt_player << ".\n";
    std::cout << bankrupt_player << " telah keluar dari permainan.\n";
}

void ViewGame::displayBankruptcyToBank(const std::string& bankrupt_player, int remaining_money, int total_assets, int debt) {
    std::cout << "\nTotal aset + uang tunai : M" << total_assets << "\n";
    std::cout << "Tidak cukup untuk menutup kewajiban M" << debt << ".\n\n";
    std::cout << ANSI_RED << bankrupt_player << " dinyatakan BANGKRUT!" << ANSI_RESET << "\n";
    std::cout << "Kreditor: Bank\n\n";
    
    std::cout << "Uang sisa M" << remaining_money << " diserahkan ke Bank.\n";
    std::cout << "Seluruh properti dikembalikan ke status BANK.\n";
    std::cout << "Bangunan dihancurkan -- stok dikembalikan ke Bank.\n\n";
    
    std::cout << bankrupt_player << " telah keluar dari permainan.\n";
}

void ViewGame::displaySaveSuccess(const std::string& filename) {
    std::cout << "Menyimpan permainan...\n";
    std::cout << ANSI_GREEN << "Permainan berhasil disimpan ke: " << filename << ANSI_RESET << "\n";
}

void ViewGame::displaySaveOverwritePrompt(const std::string& filename) {
    std::cout << "File \"" << filename << "\" sudah ada.\n";
}

void ViewGame::displaySaveError() {
    std::cout << ANSI_RED << "Gagal menyimpan file! Pastikan direktori dapat ditulis." << ANSI_RESET << "\n";
}

void ViewGame::displayLoadSuccess(const std::string& current_player_turn) {
    std::cout << "Memuat permainan...\n";
    std::cout << ANSI_GREEN << "Permainan berhasil dimuat. Melanjutkan giliran " << current_player_turn << "..." << ANSI_RESET << "\n";
}

void ViewGame::displayLoadErrorNotFound(const std::string& filename) {
    std::cout << ANSI_RED << "File \"" << filename << "\" tidak ditemukan." << ANSI_RESET << "\n";
}

void ViewGame::displayLoadErrorCorrupted() {
    std::cout << ANSI_RED << "Gagal memuat file! File rusak atau format tidak dikenali." << ANSI_RESET << "\n";
}

void ViewGame::displayLogHeader(int count) {
    if (count == 0) {
        std::cout << "\n=== Log Transaksi Penuh ===\n\n";
    } else {
        std::cout << "\n=== Log Transaksi (" << count << " Terakhir) ===\n\n";
    }
}

void ViewGame::displayLogEntry(const std::vector<LogEntry>& logs) {
    for (const auto& log : logs) {
        std::cout << "[Turn " << log.getTurnNumber() << "] " << log.getUsername() << " | ";
        std::cout << std::left << std::setw(10) << log.getTextForEnum(log.getActionType()) << " | ";
        std::cout << log.getDesc() << "\n";
    }
}


void ViewGame::displayWinMaxTurn(const std::vector<Player*>& players, const std::vector<std::string>& winners) {
    std::cout << "\n" << ANSI_CYAN << "Permainan selesai! (Batas giliran tercapai)" << ANSI_RESET << "\n\n";
    std::cout << "Rekap pemain:\n\n";
    
    for (const auto* p : players) {
        std::cout << p->getName() << "\n";
        std::cout << "Uang      : M" << p->getBalance() << "\n";
    }

    std::cout << ANSI_GREEN << "Pemenang: ";
    for (size_t i = 0; i < winners.size(); ++i) {
        std::cout << winners[i] << (i < winners.size() - 1 ? ", " : "");
    }
    std::cout << ANSI_RESET << "\n";
}

void ViewGame::displayWinBankruptcy(const std::string& winner, const std::vector<std::string>& remaining_players) {
    std::cout << "\n" << ANSI_CYAN << "Permainan selesai! (Semua pemain kecuali satu bangkrut)" << ANSI_RESET << "\n\n";
    
    std::cout << "Pemain tersisa:\n";
    for (const auto& p_name : remaining_players) {
        std::cout << "- " << p_name << "\n";
    }
    
    std::cout << "\n" << ANSI_GREEN << "Pemenang: " << winner << ANSI_RESET << "\n";
}

void ViewGame::displayCardDraw(const std::string& petak_type, const std::string& card_text) {
    std::cout << "\nKamu mendarat di Petak " << petak_type << "!\n";
    std::cout << "Mengambil kartu...\n";
    std::cout << "Kartu: \"" << card_text << "\"\n";
}

void ViewGame::displayCardEffectMoney(int amount, int remaining_money, bool is_paying) {
    if (is_paying) {
        std::cout << "Kamu membayar M" << amount << " ke Bank. Sisa Uang = M" << remaining_money << ".\n";
    } else {
        std::cout << "Kamu menerima M" << amount << " dari Bank. Uang sekarang = M" << remaining_money << ".\n";
    }
}

void ViewGame::displayCardEffectMove(const std::string& target_petak) {
    std::cout << "Bidak dipindahkan ke " << target_petak << ".\n";
}

void ViewGame::displayCardBankruptcyWarning(int amount, int current_money) {
    std::cout << ANSI_RED << "Kamu tidak mampu membayar denda/biaya kartu! (M" << amount << ")" << ANSI_RESET << "\n";
    std::cout << "Uang kamu saat ini: M" << current_money << "\n";
}

void ViewGame::displaySkillCardList(const std::vector<std::unique_ptr<SkillCard>>& cards) {
    std::cout << "\nDaftar Kartu Kemampuan Spesial Anda:\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        std::string desc = "Efek kartu spesial";
        
        if (cards[i]->isMoveCard()) {
            MoveSkillCard* mc = static_cast<MoveSkillCard*>(cards[i].get());
            desc = "Maju " + std::to_string(mc->getStep()) + " Petak";
        } else if (cards[i]->isDiscountCard()) {
            DiscountSkillCard* dc = static_cast<DiscountSkillCard*>(cards[i].get());
            desc = "Diskon sewa sebesar " + std::to_string(static_cast<int>(dc->getPercentage())) + "%";
        } else if (cards[i]->isShieldCard()) {
            desc = "Kebal tagihan atau sanksi selama 1 giliran";
        } else if (cards[i]->isTeleportCard()) {
            desc = "Pindah ke petak manapun";
        } else if (cards[i]->isLassoCard()) {
            desc = "Menarik lawan ke petak Anda";
        } else if (cards[i]->isDemolitionCard()) {
            desc = "Menghancurkan bangunan lawan";
        }

        std::cout << (i + 1) << ". " << cards[i]->getName() << " - " << desc << "\n";
    }
    std::cout << "0. Batal\n";
}

void ViewGame::displaySkillCardActivated(const std::string& card_name, const std::string& effect_desc) {
    std::cout << ANSI_CYAN << "\n" << card_name << " diaktifkan!" << ANSI_RESET << " " << effect_desc << "\n";
}

void ViewGame::displaySkillCardShieldActive(int canceled_amount, int current_money) {
    std::cout << ANSI_CYAN << "[SHIELD ACTIVE]: Efek ShieldCard melindungi Anda!" << ANSI_RESET << "\n";
    std::cout << "Tagihan M" << canceled_amount << " dibatalkan. Uang Anda tetap: M" << current_money << ".\n";
}

void ViewGame::displaySkillCardErrorAlreadyUsed() {
    std::cout << ANSI_YELLOW << "\nKamu sudah menggunakan kartu kemampuan pada giliran ini! Penggunaan kartu dibatasi maksimal 1 kali dalam 1 giliran." << ANSI_RESET << "\n";
}

void ViewGame::displaySkillCardErrorAfterDice() {
    std::cout << ANSI_YELLOW << "\nKartu kemampuan hanya bisa digunakan SEBELUM melempar dadu." << ANSI_RESET << "\n";
}

void ViewGame::displayDropCardWarning(const std::string& new_card_name) {
    std::cout << "\nKamu mendapatkan 1 kartu acak baru!\n";
    std::cout << "Kartu yang didapat: " << new_card_name << ".\n";
    std::cout << ANSI_YELLOW << "PERINGATAN: Kamu sudah memiliki 3 kartu di tangan (Maksimal 3)! Kamu diwajibkan membuang 1 kartu." << ANSI_RESET << "\n";
}

void ViewGame::displayDropCardList(const std::vector<SkillCard*>& cards) {
    std::cout << "\nDaftar Kartu Kemampuan Anda:\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        std::string desc = "Efek kartu spesial";
        
        if (cards[i]->isMoveCard()) {
            MoveSkillCard* mc = static_cast<MoveSkillCard*>(cards[i]);
            desc = "Maju " + std::to_string(mc->getStep()) + " Petak";
        } else if (cards[i]->isDiscountCard()) {
            DiscountSkillCard* dc = static_cast<DiscountSkillCard*>(cards[i]);
            desc = "Diskon sewa sebesar " + std::to_string(static_cast<int>(dc->getPercentage())) + "%";
        } else if (cards[i]->isShieldCard()) {
            desc = "Kebal tagihan atau sanksi selama 1 giliran";
        } else if (cards[i]->isTeleportCard()) {
            desc = "Pindah ke petak manapun";
        } else if (cards[i]->isLassoCard()) {
            desc = "Menarik lawan ke petak Anda";
        } else if (cards[i]->isDemolitionCard()) {
            desc = "Menghancurkan bangunan lawan";
        }

        std::cout << (i + 1) << ". " << cards[i]->getName() << " - " << desc << "\n";
    }
}

void ViewGame::displayDropCardSuccess(const std::string& dropped_card_name) {
    std::cout << "\n" << dropped_card_name << " telah dibuang. Sekarang kamu memiliki 3 kartu di tangan.\n";
}