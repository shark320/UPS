package com.vpavlov.connection.request;

import com.vpavlov.connection.payload.Payload;
import com.vpavlov.connection.type.Subtype;
import com.vpavlov.connection.type.Type;

public class Request {

    private Type type;

    private Subtype subtype;

    private Payload payload;

    public Request(Type type, Subtype subtype) {
        this.type = type;
        this.subtype = subtype;
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

    public Payload getPayload() {
        return payload;
    }

    public void setPayload(Payload payload) {
        this.payload = payload;
    }
}
