package com.vpavlov.pockerclient.connection.header;

import com.vpavlov.pockerclient.connection.Constants;
import com.vpavlov.pockerclient.connection.Status;
import com.vpavlov.pockerclient.connection.type.Subtype;
import com.vpavlov.pockerclient.connection.type.Type;

public class Header {

    private String identifier;

    private Type type;

    private Subtype subtype;

    private Status status;

    private  Integer length;

    public Header() {
    }

    public Header(Type type, Subtype subtype) {
        this(type, subtype, null);
    }

    public Header(Type type, Subtype subtype, Status status) {
        this(null, type, subtype, status);
    }

    public Header(String identifier, Type type, Subtype subtype, Status status, Integer length) {
        this.identifier = identifier;
        this.type = type;
        this.subtype = subtype;
        this.status = status;
        setLength(length);
    }

    public Header(Integer length, Type type, Subtype subtype, Status status) {
        this(null, type, subtype, status, length);
    }

    public void setIdentifier(String identifier) {
        this.identifier = identifier;
    }

    public String getIdentifier() {
        return identifier;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }

    public Subtype getSubtype() {
        return subtype;
    }

    public void setSubtype(Subtype subtype) {
        this.subtype = subtype;
    }

    public Status getStatus() {
        return status;
    }

    public void setStatus(Status status) {
        this.status = status;
    }

    public Integer getLength() {
        return length;
    }

    public int getLengthValue(){
        return length == null ? 0 : length;
    }

    public void setLength(Integer length) {
        if (length != null && (length > Constants.MSG_MAX_LENGTH || length < 0)){
            throw new IllegalArgumentException("Length too large. Length must be: [0," + Constants.MSG_MAX_LENGTH+"]");
        }
        this.length = length;
    }

    public boolean checkValues(){
        if (length == null || length>Constants.MSG_MAX_LENGTH){
            return false;
        }
        return status != null && subtype != null && type != null;
    }

    public String construct(){
        if (!checkValues()){
            throw new IllegalStateException("Incomplete header.");
        }
        String length = String.format("%04d", this.getLength());
        String type = String.format("%1d", this.type.getId());
        String subType = String.format("%02d", this.subtype.getId());
        String status = String.format("%03d", this.status.getCode());
        return identifier + length + type + subType + status;
    }

    public static Header extract(String message) throws IllegalArgumentException{
        if (message.length() < Constants.MSG_HEADER_LENGTH){
            throw new IllegalArgumentException("Message header is too short");
        }

        String identifier = message.substring(Constants.MSG_IDENTIFIER_FIELD_POS, Constants.MSG_IDENTIFIER_FIELD_LENGTH);
        int msgLength = Integer.parseInt(message.substring(Constants.MSG_LENGTH_FIELD_POS, Constants.MSG_LENGTH_FIELD_POS+Constants.MSG_LENGTH_FIELD_LENGTH));
        int msgTypeInt = Integer.parseInt(message.substring(Constants.MSG_TYPE_FIELD_POS, Constants.MSG_TYPE_FIELD_POS+Constants.MSG_TYPE_FIELD_LENGTH));
        int msgSubtypeInt = Integer.parseInt(message.substring(Constants.MSG_SUBTYPE_FIELD_POS, Constants.MSG_SUBTYPE_FIELD_POS+Constants.MSG_SUBTYPE_FIELD_LENGTH));
        int msgStatusInt = Integer.parseInt(message.substring(Constants.MSG_STATUS_FIELD_POS, Constants.MSG_STATUS_FIELD_POS+Constants.MSG_STATUS_FIELD_LENGTH));

        Type msgType = Type.getType(msgTypeInt);
        Subtype msgSubtype = Subtype.getSubtype(msgSubtypeInt);
        Status msgStatus = Status.getStatus(msgStatusInt);
        return new Header(identifier, msgType, msgSubtype, msgStatus, msgLength);
    }

    @Override
    public String toString() {
        return "Header{" +
                "identifier='" + identifier + '\'' +
                ", type=" + type +
                ", subtype=" + subtype +
                ", status=" + status +
                ", length=" + length +
                '}';
    }
}
