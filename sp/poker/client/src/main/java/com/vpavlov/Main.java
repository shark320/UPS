package com.vpavlov;

import com.vpavlov.connection.Connection;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;

public class Main {
    static {
        System.setProperty("log4j.configurationFile",
                "log4j-config.xml");
    }
    public static final Logger LOGGER = LogManager.getLogger();

    public static void main(String[] args) throws IOException {
        Connection connection = new Connection(10000);
        connection.connect();
    }
}