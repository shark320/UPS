package com.vpavlov.pockerclient.connection;

public final class Constants {

    public static final String IDENTIFIER = "POKR";

    public static final int MSG_MAX_LENGTH = 9999;

    public static final int MSG_IDENTIFIER_FIELD_POS = 0;

    public static final int MSG_IDENTIFIER_FIELD_LENGTH = 4;

    public static final int MSG_LENGTH_FIELD_POS = MSG_IDENTIFIER_FIELD_POS + MSG_IDENTIFIER_FIELD_LENGTH;

    public static final int MSG_LENGTH_FIELD_LENGTH = 4;

    public static final int MSG_TYPE_FIELD_POS = MSG_LENGTH_FIELD_POS + MSG_LENGTH_FIELD_LENGTH;

    public static final int MSG_TYPE_FIELD_LENGTH = 1;

    public static final int MSG_SUBTYPE_FIELD_POS = MSG_TYPE_FIELD_POS + MSG_TYPE_FIELD_LENGTH;

    public static final int MSG_SUBTYPE_FIELD_LENGTH = 2;

    public static final int MSG_STATUS_FIELD_POS = MSG_SUBTYPE_FIELD_POS + MSG_SUBTYPE_FIELD_LENGTH;

    public static final int MSG_STATUS_FIELD_LENGTH = 3;

    public static final int MSG_HEADER_LENGTH = MSG_IDENTIFIER_FIELD_LENGTH + MSG_LENGTH_FIELD_LENGTH + MSG_TYPE_FIELD_LENGTH + MSG_SUBTYPE_FIELD_LENGTH + MSG_STATUS_FIELD_LENGTH;

    private Constants() {
    }
}
