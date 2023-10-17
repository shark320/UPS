package com.vpavlov.connection.payload;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Payload {

    private static final char SEPARATOR = ';';

    private final Map<String, Object> data = new HashMap<String, Object>();

    public Payload() {
    }

    public void setData(String key, Object value) {
        data.put(key, value);
    }

    public Object getData(String key) {
        return data.get(key);
    }

    private String mapString(String string) {
        if (string == null){
            return null;
        }
        return '"' + string + '"';
    }

    private String mapStringsList(List<String> stringsList) {
        if (stringsList == null || stringsList.isEmpty()) {
            return "null";
        }
        StringBuilder sb = new StringBuilder();
        for (String string : stringsList) {
            sb.append(string).append(',');
        }
        //delete last comma
        sb.deleteCharAt(sb.length() - 1);
        return sb.toString();
    }

    private String mapInt(Integer integer) {
        if (integer == null){
            return null;
        }
        return String.valueOf(integer);
    }

    private String mapIntsList(List<Integer> integersList) {
        if (integersList == null || integersList.isEmpty()) {
            return "null";
        }
        StringBuilder sb = new StringBuilder();
        for (Integer string : integersList) {
            sb.append(string).append(',');
        }
        //delete last comma
        sb.deleteCharAt(sb.length() - 1);
        return sb.toString();
    }

    private String mapBoolean(Boolean bool) {
        if (bool == null){
            return null;
        }
        return String.valueOf(bool);
    }

//    private <T> String mapList(List<T>){
//        if () {}
//    }

    private String map(Object o) {
        if (o == null){
            return "null";
        }
        if (o instanceof String){
            return mapString((String)o);
        }
        if (o instanceof Integer){
            return mapInt((Integer) o);
        }
        if (o instanceof List<?>){

        }
    }

    public String construct() {
        if (data.isEmpty()) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        for (String key: data.keySet()){
            sb.append(key).append('=');

        }
    }


}
