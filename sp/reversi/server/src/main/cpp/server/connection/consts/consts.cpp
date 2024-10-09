#include "consts.hpp"

const std::string constants::IDENTIFIER = "RVSI";
const int constants::MSG_MAX_LENGTH = 9999;
const int constants::MSG_IDENTIFIER_FIELD_POS = 0;
const int constants::MSG_IDENTIFIER_FIELD_LENGTH = 4;
const int constants::MSG_LENGTH_FIELD_POS = constants::MSG_IDENTIFIER_FIELD_POS + constants::MSG_IDENTIFIER_FIELD_LENGTH;
const int constants::MSG_LENGTH_FIELD_LENGTH = 4;
const int constants::MSG_TYPE_FIELD_POS = constants::MSG_LENGTH_FIELD_POS + constants::MSG_LENGTH_FIELD_LENGTH;
const int constants::MSG_TYPE_FIELD_LENGTH = 1;
const int constants::MSG_SUBTYPE_FIELD_POS = constants::MSG_TYPE_FIELD_POS + constants::MSG_TYPE_FIELD_LENGTH;
const int constants::MSG_SUBTYPE_FIELD_LENGTH = 2;
const int constants::MSG_STATUS_FIELD_POS = constants::MSG_SUBTYPE_FIELD_POS + constants::MSG_SUBTYPE_FIELD_LENGTH;
const int constants::MSG_STATUS_FIELD_LENGTH = 3;
const int constants::MSG_HEADER_LENGTH = constants::MSG_IDENTIFIER_FIELD_LENGTH + constants::MSG_LENGTH_FIELD_LENGTH +
        constants::MSG_TYPE_FIELD_LENGTH + constants::MSG_SUBTYPE_FIELD_LENGTH +
        constants::MSG_STATUS_FIELD_LENGTH;

