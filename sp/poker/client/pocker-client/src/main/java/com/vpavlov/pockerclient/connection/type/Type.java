package com.vpavlov.pockerclient.connection.type;

import java.util.HashMap;
import java.util.Map;

public enum Type{
    GET(1),
    POST(2)
    ;

    private static final Map<Integer, Type> values = new HashMap<Integer, Type>();

    static {
        for (Type type : Type.values()){
            values.put(type.getId(), type);
        }
    }

    private final int id;

    Type(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public static Type getType(int id) {
        return values.get(id);
    }
}
