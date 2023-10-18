package com.vpavlov.connection.request;

import com.vpavlov.connection.header.Header;
import com.vpavlov.connection.payload.Payload;
import com.vpavlov.connection.payload.mapping.PayloadMapper;

public class Request {

    private Header header;
    private Payload payload;

    public Request(Payload payload) {
       this(null, payload);
    }

    public Request(Header header) {
        this(header, null);
    }

    public Request(Header header, Payload payload) {
        this.header = header;
        this.payload = payload;
    }

    public void setHeader(Header header) {
        this.header = header;
    }

    public Header getHeader() {
        return header;
    }

    public Payload getPayload() {
        return payload;
    }

    public void setPayload(Payload payload) {
        this.payload = payload;
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
}
