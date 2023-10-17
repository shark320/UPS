package com.vpavlov;


import com.vpavlov.connection.Status;
import com.vpavlov.connection.header.Header;
import com.vpavlov.connection.type.Subtype;
import com.vpavlov.connection.type.Type;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.util.JsonUtils;

import java.io.BufferedOutputStream;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

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
        System.out.println(header.construct());
    }
}