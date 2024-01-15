package com.vpavlov.pockerclient.connection;

import java.util.HashMap;
import java.util.Map;

public enum Status {
    NO_STATUS(0),
    OK(200),
    NOT_FOUND(404),
    CONFLICT(409),
    NOT_ALLOWED(405)
    ;

    private static final Map<Integer, Status> values = new HashMap<>();

    static{
        for (Status status : Status.values()){
            values.put(status.getCode(), status);
        }
    }

    private final int code;

    Status(int code) {
        this.code = code;
    }

    public int getCode() {
        return code;
    }

    public static Status getStatus(int code) {
        return values.get(code);
    }

}
