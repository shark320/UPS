package com.vpavlov.pockerclient.connection.payload;

import com.vpavlov.pockerclient.connection.Constants;
import com.vpavlov.pockerclient.connection.payload.mapping.PayloadMapper;
import com.vpavlov.pockerclient.connection.payload.mapping.PayloadParser;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Payload {
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

    public String construct(){
        return PayloadMapper.map(this);
    }

    public static Payload extract(String message){
        if (message.length() <= Constants.MSG_HEADER_LENGTH){
            //empty payload
            return new Payload();
        }
        String payloadStr = message.substring(Constants.MSG_HEADER_LENGTH);
        return PayloadParser.parse(payloadStr);
    }

    @Override
    public String toString() {
        return "Payload{" +
                "data=" + data +
                '}';
    }
}
