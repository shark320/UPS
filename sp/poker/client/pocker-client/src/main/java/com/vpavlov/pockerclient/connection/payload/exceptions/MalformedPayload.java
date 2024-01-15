package com.vpavlov.pockerclient.connection.payload.exceptions;

public class MalformedPayload extends Exception{

    public MalformedPayload(String message) {
        this(message, null);
    }

    public MalformedPayload(String message, Throwable cause) {
        super(message, cause);
    }
}
