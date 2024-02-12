package com.vpavlov.pockerclient.connection;

import com.vpavlov.pockerclient.connection.header.Header;
import com.vpavlov.pockerclient.connection.payload.Payload;
import com.vpavlov.pockerclient.connection.payload.mapping.PayloadMapper;

public class Message {

    private Header header;
    private Payload payload;

    public Message(Header header, Payload payload) {
        this.header = header;
        this.payload = payload;
    }

    public Message(){}

    public void setHeader(Header header) {
        this.header = header;
    }

    public void setPayload(Payload payload) {
        this.payload = payload;
    }

    public Header getHeader() {
        return header;
    }

    public Payload getPayload() {
        return payload;
    }

    public String construct(){
        String payloadConstruct = PayloadMapper.map(this.payload);
        this.header.setLength(payloadConstruct.length());
        String headerConstruct = this.header.construct();
        return headerConstruct+payloadConstruct;
    }

    public static Message parse(String requestMsg){
        Header header = Header.extract(requestMsg);
        Payload payload = Payload.extract(requestMsg);

        return new Message(header, payload);
    }

    @Override
    public String toString() {
        return "Message{" +
                "header=" + header +
                ", payload=" + payload +
                '}';
    }
}
