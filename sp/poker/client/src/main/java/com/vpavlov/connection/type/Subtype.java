package com.vpavlov.connection.type;

import java.util.HashMap;
import java.util.Map;

public enum Subtype {
    PING(1)
    ;

    private final static Map<Integer, Subtype> values = new HashMap<Integer, Subtype>();

    static{
        for (Subtype type : Subtype.values()){
            values.put(type.getId(), type);
        }
    }

    private final int id;

    Subtype(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public static Subtype getSubtype(int id) {
        return values.get(id);
    }
}
