package com.vpavlov;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.vpavlov.connection.Connection;
import com.vpavlov.connection.request.Request;
import com.vpavlov.connection.type.Subtype;
import com.vpavlov.connection.type.Type;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

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

    public static void sendMessage( Socket socket) throws IOException {
        LOGGER.debug("Connected: " + socket.isConnected());
        OutputStream outputStream = socket.getOutputStream();
        BufferedOutputStream bos = new BufferedOutputStream(outputStream);
        ObjectMapper objectMapper = new ObjectMapper();
        Request request = new Request(Type.GET, Subtype.PING);
        String json = objectMapper.writeValueAsString(request);
        LOGGER.debug("Request: " + json);
        bos.write(json.getBytes());
        bos.flush();
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        Connection connection = new Connection("127.0.0.1", 10000);
        Socket socket = connection.connect();

        // Get the output stream from the socket


        TimerTask timerTask = new TimerTask(){

            @Override
            public void run() {

            }
        };
        ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);

        // Schedule a task to run every 2 seconds
        scheduler.scheduleAtFixedRate(()-> {
            try {
                sendMessage(socket);
            } catch (IOException e) {
                LOGGER.error(e);
            }
        }, 0, 5, TimeUnit.SECONDS);

    }
}