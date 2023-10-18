package com.vpavlov.connection.payload.mapping;

import com.vpavlov.connection.payload.Payload;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PayloadParser {

    protected static final Pattern LIST_PATTERN = Pattern.compile("^\\[((\"[^\"]*(?:\",\"[^\"]*)*\")|\\d+(?:,\\d+)*)\\]$");

    protected static final Pattern LIST_INT_PATTERN = Pattern.compile("^\\[(\\d+(?:,\\d+)*)\\]$");

    protected static final Pattern LIST_STRING_PATTERN = Pattern.compile("^\\[(\"[^\"]*(?:\",\"[^\"]*)*\")\\]$");

    protected static final Pattern STRING_PATTERN = Pattern.compile("^\"([^\"]*)\"$");

    protected static final Pattern INT_PATTERN = Pattern.compile("^\\d+$");

    protected static final Pattern BOOL_PATTERN = Pattern.compile("^(true|false)$");

    public static List<Integer> parseIntList (String value){
        value = value.substring(1,value.length()-1);
        List<Integer> result = new ArrayList<Integer>();
        String[] tokens = value.split(PayloadConstants.LIST_SEPARATOR);
        for (String token : tokens){
            result.add(parseInt(token));
        }
        return result;
    }

    public static List<String> parseStringList (String value){
        value = value.substring(1,value.length()-1);
        List<String> result = new ArrayList<String>();
        String[] tokens = value.split(PayloadConstants.LIST_SEPARATOR);
        for (String token : tokens){
            result.add(parseString(token));
        }
        return result;
    }

    public static List<?> parseList(String value){
        Matcher stringListMatcher = LIST_STRING_PATTERN.matcher(value);
        if (stringListMatcher.matches()){
            return parseStringList(value);
        }
        Matcher intListMatcher = LIST_INT_PATTERN.matcher(value);
        if (intListMatcher.matches()){
            return parseIntList(value);
        }
        throw new IllegalArgumentException("Invalid list type: " + value);
    }

    public static String parseString(String value){
        return value.substring(1,value.length()-1);
    }

    public static Integer parseInt(String value){
        return Integer.parseInt(value);
    }

    public static Boolean parseBoolean(String value){
        return Boolean.parseBoolean(value);
    }

    public static Object parseSingleValue(String value){
        Matcher stringMatcher = STRING_PATTERN.matcher(value);
        boolean flag = stringMatcher.find();
        if (stringMatcher.matches()){
            return parseString(value);
        }
        Matcher intMatcher = INT_PATTERN.matcher(value);
        if (intMatcher.matches()){
            return parseInt(value);
        }
        Matcher boolMatcher = BOOL_PATTERN.matcher(value);
        if (boolMatcher.matches()){
            return parseBoolean(value);
        }
        throw new IllegalArgumentException("Invalid value: " + value);
    }

    public static Object parseValue(String value){
        value = value.trim();
        Matcher listMatcher = LIST_PATTERN.matcher(value);
        if (listMatcher.matches()){
            return parseList(value);
        }
        return parseSingleValue(value);
    }

    private static void parseToken(String token, Payload payload){
        String[] keyValueTokens = token.split(PayloadConstants.KEY_SEPARATOR);
        if (keyValueTokens.length != 2){
            throw new IllegalArgumentException("Unable to parse token: " + token);
        }
        String key = keyValueTokens[0];
        String value = keyValueTokens[1];
        Object parsedValue = parseValue(value);
        payload.setValue(key, parsedValue);
    }

    public static Payload parse(String payloadString){
        if (payloadString.isEmpty()){
            return null;
        }
        Payload payload = new Payload();
        String[] payloadTokens = payloadString.split(PayloadConstants.SEPARATOR);
        for (String token : payloadTokens){
            parseToken(token, payload);
        }
        return payload;
    }
}
