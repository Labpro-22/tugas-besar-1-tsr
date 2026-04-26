#pragma once

// ==================== CORE ====================
#include "core/CardManager.hpp"
#include "core/EconomyManager.hpp"
#include "core/GameException.hpp"
#include "core/GameManager.hpp"
#include "core/PropertyManager.hpp"
#include "core/SaveLoadManager.hpp"
#include "core/TransactionLog.hpp"

// ==================== MODELS ====================
#include "models/ActionCard.hpp"
#include "models/Board.hpp"
#include "models/Card.hpp"
#include "models/Deck.hpp"
#include "models/Effect.hpp"
#include "models/FinancialActionCard.hpp"
#include "models/JailActionCard.hpp"
#include "models/MoveActionCard.hpp"
#include "models/Player.hpp"
#include "models/PropertyTile.hpp"
#include "models/Saveable.hpp"
#include "models/SkillCard.hpp"
#include "models/StationActionCard.hpp"
#include "models/StreetTile.hpp"
#include "models/Tile.hpp"

// ==================== UTILS ====================
#include "utils/ConfigData.hpp"
#include "utils/IOManager.hpp"
#include "utils/SaveData.hpp"

// ==================== VIEWS ====================
#include "views/ViewGame.hpp"