package com.vpavlov.pockerclient.connection.request;

import com.vpavlov.pockerclient.connection.header.Header;
import com.vpavlov.pockerclient.connection.payload.Payload;
import com.vpavlov.pockerclient.connection.payload.mapping.PayloadMapper;

public class Request {

    private final Header header;
    private final Payload payload;

    public Request(Header header, Payload payload) {
        this.header = header;
        this.payload = payload;
    }

    public Header getHeader() {
        return header;
    }

    public Payload getPayload() {
        return payload;
    }

    public String construct(){
        if (this.payload == null){
            throw new IllegalStateException("Payload is null.");
        }
        String payloadConstruct = PayloadMapper.map(this.payload);
        this.header.setLength(payloadConstruct.length());
        String headerConstruct = this.header.construct();
        return headerConstruct+payloadConstruct;
    }

    public static Request parse(String requestMsg){
        Header header = Header.extract(requestMsg);
        Payload payload = Payload.extract(requestMsg);

        return new Request(header, payload);
    }

    @Override
    public String toString() {
        return "Request{" +
                "header=" + header +
                ", payload=" + payload +
                '}';
    }
}
