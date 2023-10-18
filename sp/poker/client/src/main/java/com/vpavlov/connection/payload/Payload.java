package com.vpavlov.connection.payload;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Payload {

    protected static final String SEPARATOR = ";";

    protected static final String KEY_SEPARATOR = "=";



    private final Map<String, Object> data = new HashMap<String, Object>();

    public void setValue(String key, Object value) {
        this.data.put(key, value);
    }

    public Object getValue(String key) {
        return data.get(key);
    }


    public Map<String, Object> getData() {
        return data;
    }

    @Override
    public String toString() {
        return "Payload{" +
                "data=" + data +
                '}';
    }
}
