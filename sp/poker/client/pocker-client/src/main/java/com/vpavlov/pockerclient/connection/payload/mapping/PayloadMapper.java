package com.vpavlov.pockerclient.connection.payload.mapping;


import com.vpavlov.pockerclient.connection.payload.Payload;

import java.util.List;
import java.util.Map;

public class PayloadMapper {


    public static String mapString(String string) {
        if (string == null){
            return null;
        }
        return '"' + string + '"';
    }

    public static String mapStringsList(List<String> stringsList) {
        if (stringsList == null || stringsList.isEmpty()) {
            return "null";
        }
        StringBuilder sb = new StringBuilder();
        for (String string : stringsList) {
            sb.append(mapString(string)).append(',');
        }
        //delete last comma
        sb.deleteCharAt(sb.length() - 1);
        return sb.toString();
    }

    public static String mapInt(Integer integer) {
        if (integer == null){
            return null;
        }
        return String.valueOf(integer);
    }

    public static String mapIntsList(List<Integer> integersList) {
        if (integersList == null || integersList.isEmpty()) {
            return "null";
        }
        StringBuilder sb = new StringBuilder();
        for (Integer integer : integersList) {
            sb.append(mapInt(integer)).append(',');
        }
        //delete last comma
        sb.deleteCharAt(sb.length() - 1);
        return sb.toString();
    }

    public static String mapBoolean(Boolean bool) {
        if (bool == null){
            return null;
        }
        return String.valueOf(bool);
    }

    public static String mapList(List<?> list) {
        if (list == null || list.isEmpty()){
            return null;
        }else{
            String result="";
            if (list.get(0) instanceof Integer){
                result = mapIntsList((List<Integer>)list);
            }
            if (list.get(0) instanceof String){
                result = mapStringsList((List<String>)list);
            }
            return "["+result+"]";
        }
    }


    public static String mapObject(Object o) {
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
            return mapList((List<?>)o);
        }
        if (o instanceof Boolean){
            return mapBoolean((Boolean) o);
        }
        return null;
    }

    public static String map(Payload payload) {
        Map<String, Object> data = payload.getData();
        if (data.isEmpty()) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        for (String key: data.keySet()){
            sb.append(key).append('=');
            sb.append(mapObject(data.get(key))).append(PayloadConstants.SEPARATOR);
        }
        sb.deleteCharAt(sb.length()-1);

        return sb.toString();
    }
}
