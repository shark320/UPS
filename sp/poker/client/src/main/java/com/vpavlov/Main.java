package com.vpavlov;


import com.vpavlov.connection.payload.mapping.PayloadMapper;
import com.vpavlov.connection.payload.mapping.PayloadParser;
import com.vpavlov.connection.Status;
import com.vpavlov.connection.header.Header;
import com.vpavlov.connection.payload.Payload;
import com.vpavlov.connection.request.Request;
import com.vpavlov.connection.type.Subtype;
import com.vpavlov.connection.type.Type;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.util.Arrays;

public class Main {
    static {
        System.setProperty("log4j.configurationFile",
                "log4j-config.xml");
    }

    public static final Logger LOGGER = LogManager.getLogger();

//    public static void sendMessage( Socket socket) throws IOException {
//        LOGGER.debug("Connected: " + socket.isConnected());
//        OutputStream outputStream = socket.getOutputStream();
//        BufferedOutputStream bos = new BufferedOutputStream(outputStream);
//        ObjectMapper objectMapper = new ObjectMapper();
//        Request request = new Request(Type.GET, Subtype.PING);
//        String json = objectMapper.writeValueAsString(request);
//        LOGGER.debug("Request: " + json);
//        bos.write(json.getBytes());
//        bos.flush();
//    }

    public static void main(String[] args) throws IOException, InterruptedException {
        Header header = new Header(10 , Type.GET, Subtype.PING, Status.NO_STATUS);
        Payload payload = new Payload();
        payload.setValue("name", "Shark32");
        payload.setValue("number", 10);
        payload.setValue("list-number", Arrays.asList(1,2,3));
        payload.setValue("string-list", Arrays.asList("s1","s2","s3"));
        payload.setValue("bool", true);
        Request request = new Request(header, payload);
        System.out.println(request.construct());

        String constructedPayload = PayloadMapper.map(payload);
        System.out.println("Constructed payload: " + constructedPayload);
        Payload parsedPayload = PayloadParser.parse(constructedPayload);
        System.out.println("Parsed payload: " + parsedPayload);

    }
}