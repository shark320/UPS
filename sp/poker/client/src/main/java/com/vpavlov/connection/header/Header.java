package com.vpavlov.connection.header;

import com.vpavlov.connection.Status;
import com.vpavlov.connection.type.Subtype;
import com.vpavlov.connection.type.Type;

public class Header {

    private final String identifier = "POKR";

    private final int MAX_LENGTH = 9999;

    private  Type type;

    private  Subtype subtype;

    private Status status;

    private  Integer length;

    public Header(Type type, Subtype subtype) {
        this(type, subtype, null);
    }

    public Header(Type type, Subtype subtype, Status status) {
        this(null, type, subtype, status);
    }

    public Header(Integer length,Type type, Subtype subtype, Status status) {
        this.type = type;
        this.subtype = subtype;
        this.status = status;
        setLength(length);
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
        if (length != null && (length > MAX_LENGTH || length < 0)){
            throw new IllegalArgumentException("Length too large. Length must be: [0," + MAX_LENGTH+"]");
        }
        this.length = length;
    }

    public boolean checkValues(){
        if (length == null || length>MAX_LENGTH){
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
}
